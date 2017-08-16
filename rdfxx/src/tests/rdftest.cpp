/*
 * 			rdftest.cpp
 *
 *		Copyright 2012 - 2017 Brenton Ross
 *
 *	This file is part of SASSY.
 *
 *	SASSY is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	SASSY is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with SASSY. If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include "rdftest.h"
#include <cfi/xini.h>
#include "rdfxx/rdfxx.h"
#include <iostream>
#include <map>
#include <vector>

using namespace std;
using namespace SASSY;
using namespace SASSY::cdi;
using namespace SASSY::cfi;
using namespace rdf;

const string SASSY::cfi::XINI::config =
		"PECHD:"
		"-i:SASSY_CONF:"
		"sassy.xml:"
		".local/share/sassy/sassy.xml:"
		"$SASSY/projects/rdfxx/sassy.xml:"
		"$SASSY/share/sassy/sassy.xml:"
		"$SASSY/sassy.xml";


// ------------------------------------------------------------
//	URITestCase
// ------------------------------------------------------------

URITestCase::URITestCase( csr nm)
	: TestCaseT<URITestCase>(nm)
{}

// ------------------------------------------------------------

URITestCase::~URITestCase()
{}

// ------------------------------------------------------------

bool
URITestCase::runTest()
{
	bool rc = true;

	try {
		URI uri("http://purl.org/dc/0.1/title");
		rc = rc && test( true, "uri 1");

		rc = rc && test( (uri->toString() == "http://purl.org/dc/0.1/title"), "uri 2");

		URI uri2 = uri->copy();

		rc = rc && test( (uri2->toString() == "http://purl.org/dc/0.1/title"), "uri 2");
		rc = rc && test((uri == uri2), "uri 3");
		
		// confirm that URIs can go into standard containers
		vector< URI > uris;
		uris.push_back( uri2 );

		map< int, URI > uri_map;
		uri_map[1] = uri;

	}
	catch( vx & e )
	{
		rc = test( false, e.what());
	}

	return rc;
}

// ------------------------------------------------------------
//	NodeTestCase
// ------------------------------------------------------------

NodeTestCase::NodeTestCase( csr nm)
	: TestCaseT<NodeTestCase>(nm)
{}

// ------------------------------------------------------------

NodeTestCase::~NodeTestCase()
{}

// ------------------------------------------------------------

bool
NodeTestCase::runTest()
{
	bool rc = true;

	try {
		Node n1("http://purl.org/dc/0.1/title");
		rc = rc && test( true, "node 1");
		rc = rc && test( n1->isResource(), "node 2");
		rc = rc && test( ! n1->isBlank(), "node 3");
		rc = rc && test( ! n1->isLiteral(), "node 4");
		Node n2;
		rc = rc && test( n2->isBlank(), "node 5");
		Node n3("fred", false);
		rc = rc && test( n3->isLiteral(), "node 6");
		Node n4("<dc:name>TestLog</dc:name>", true);
		rc = rc && test( n4->isLiteral(), "node 7");
		Node n5("<dc:name>TestLog</dc:name>", false, "en");
		rc = rc && test( n5->isLiteral(), "node 8");
		Node n6("fred", false, "en");
		rc = rc && test( n6->isLiteral(), "node 9");
		Node n7("http://purl.org/dc/0.1/title", false);
		rc = rc && test( !n7->isResource(), "node 10");
		rc = rc && test( n7->isLiteral(), "node 12");

		URI uri("http://organise.org/ofw/0.4/categories/documents");
		Node n8( uri );
		rc = rc && test( n8->isResource(), "node 11");

		Node n9 = n8->copy();
		rc = rc && test( n9->isResource(), "node 13");

		URI uri2 = n9->toURI();
		rc = rc && test( uri == uri2, "node 14");
		rc = rc && test( n3->toString() == "\"fred\"", "node 15");
		cout << "n3 = \"" << n3->toString() << "\"" << endl;

		// confirm that Nodes can go into standard containers
		vector< Node > node_list;
		node_list.push_back(n8);
		map< int, Node > node_map;
		node_map[2] = n2;
	}
	catch( vx & e )
	{
		rc = test(false, e.what());
	}
	return rc;
}

// ------------------------------------------------------------
//	StmntTestCase
// ------------------------------------------------------------

StmntTestCase::StmntTestCase(csr nm )
	: TestCaseT<StmntTestCase>(nm)
{}

// ------------------------------------------------------------

StmntTestCase::~StmntTestCase()
{}

// ------------------------------------------------------------

bool
StmntTestCase::runTest()
{
	bool rc = true;

	try {
		Node n1;
		Statement s1( n1, n1, n1 );
		rc = rc && test( true, "stmnt 1");
		rc = rc && test( ! s1->isComplete(), "stmnt 2");
		Node n2("fred", false);
		Node n3("isA", false);
		Node n4("moron", false );
		s1->subject(n2);
		rc = rc && test( ! s1->isComplete(), "stmnt 3");
		NodeRef nr2 = s1->subject();
		Node n5(nr2);
		rc = rc && test( n5->toString() == "\"fred\"", "stmnt 4");
		s1->predicate(n3);
		n5 = Node(s1->predicate());
		rc = rc && test( n5->toString() == "\"isA\"", "stmnt 5");
		s1->object(n4);
		n5 = Node(s1->object());
		rc = rc && test( n5->toString() == "\"moron\"", "stmnt 6");

		// this test is false because all the nodes are literals
		rc = rc && test( ! s1->isComplete(), "stmnt 7");

		URI uri("http://organise.org/ofw/0.4/categories/documents");
		s1->subject(Node(uri));
		rc = rc && test( Node(s1->subject())->toURI() == uri, "stmnt 8");

		n3 = Node("http://purl.org/dc/0.1/title");
		s1->predicate(n3);
		rc = rc && test( s1->isComplete(), "stmnt 9");
		
		Statement s2( n1, n1, n1 );
		s2->clear();
		s2->predicate(n3);
		rc = rc && test( s1->match(s2), "stmnt 10");

		Statement s3 = s1->copy();
		rc = rc && test( s3 == s1, "stmnt 11");
		
		rc = rc && test( s1->toString() == 
			"<http://organise.org/ofw/0.4/categories/documents> <http://purl.org/dc/0.1/title> \"moron\"", 
			"stmnt 12");

		// confirm that Statements can go into standard containers
		vector< Statement > stmnt_list;
		map< int, Statement > stmnt_map;
		stmnt_list.push_back(s2);
		stmnt_map[3] = s3;
	}
	catch( vx & e )
	{
		rc = test( false, e.what() );
	}
	return rc;
}

// ------------------------------------------------------------
//	ModelTestCase
// ------------------------------------------------------------

ModelTestCase::ModelTestCase( csr nm)
	: TestCaseT< ModelTestCase >(nm)
{}

// ------------------------------------------------------------

ModelTestCase::~ModelTestCase()
{}

// ------------------------------------------------------------

bool
ModelTestCase::runTest()
{
	bool rc = true;

	try {
		Model m1("memory");
		rc = rc && test( m1->size() == 0, "model 1");
		cout << "model size is " << m1->size() << endl;
		URI uri("http://organise.org/ofw/0.4/categories/documents");
		Node n1( uri );
		Node n2("http://purl.org/dc/0.1/title");
		Node n3("some literal value", false);
		Node n4("a different literal value", false );

		Statement s1( n1, n2, n3 );
		m1->add( n1, n2, n3 );
		rc = rc && test( m1->size() == 1, "model 2");
		rc = rc && test( m1->contains( s1 ), "model 3");

		Statement s2( n1, n2, n4);
		rc = rc && test( ! m1->contains( s2 ), "model 4");
		bool res = m1->add( s1 );
		rc = rc && test( res, "model 5"); // odd
		rc = rc && test( m1->size() == 1, "model 6");

		res = m1->add( s2 );
		rc = rc && test( res && m1->size() == 2, "model 7");

		res = m1->remove( s2 );
		rc = rc && test( res && m1->size() == 1, "model 8");
		res = m1->remove( s2 );
		rc = rc && test( (!res) && m1->size() == 1, "model 9");
		rc = rc && test( ! m1->contains(s2), "model 10");

		res = m1->update( s1, s2 );
		rc = rc && test( res && m1->size() == 1, "model 11");
		rc = rc && test( ! m1->contains( s1 ), "model 12");
		rc = rc && test( m1->contains( s2 ), "model 13");
		m1->add( s1 );

		Stream x = m1->toStream();
		while ( ! x->end() )
		{
			rc = rc && test( m1->contains( x->current() ), "model 14");
			x->next();
		}


	}
	catch( vx & e )
	{
		rc = test( false, e.what());
	}
	return rc;
}

// ------------------------------------------------------------
//	StoreTestCase
// ------------------------------------------------------------

StoreTestCase::StoreTestCase(csr nm)
	: TestCaseT< StoreTestCase >(nm)
{
	XINI & xini = XINI::instance();
	string xpath("/SASSY-CONFIG/Test/librdfxx/TestData");
	int n = xini.getVals( xpath, rdfFiles );
	if ( n == 0 )
		throw cdi::test_exception() << "No RDF test data\n";
}

// ------------------------------------------------------------

StoreTestCase::~StoreTestCase()
{}

// ------------------------------------------------------------

bool
StoreTestCase::runTest()
{
	bool rc = true;
	
	// need to prevent stores being deleted before the models
	// TODO - this needs to be handled better
	// std::map< string, Storage > stores;
	std::map< string, Model > models;

	for ( auto &f : rdfFiles )
	{
		Path p(f);
		string s(p.base());
		// cout << "Parsing " << s << endl;
		// Storage store("file", f);
		Model m("file", f);
		models[s] = m;
		// stores[s] = store;
	}
	rc = rc && test( models.size() > 0, "store 1");

	int count = 0;
	for ( auto &m : models )
	{
		// cout << string( 20, '+') << m.first << endl;
		Stream x = m.second->toStream();
		while ( ! x->end() )
		{
			// cout << Statement(x->current())->toString() << endl;
			count++;
			x->next();
		}
	}
	cout << "Found " << count << " statements" << endl;
	rc = rc && test( count = 188, "store 2");

	
	return rc;
}

// ------------------------------------------------------------
//	IOTestCase
// ------------------------------------------------------------

IOTestCase::IOTestCase( csr nm )
	: TestCaseT<IOTestCase>(nm)
{
	XINI & xini = XINI::instance();
	string xpath("/SASSY-CONFIG/Test/librdfxx/TestData");
	int n = xini.getVals( xpath, rdfFiles );
	if ( n == 0 )
		throw cdi::test_exception() << "No RDF test data\n";
}

// ------------------------------------------------------------

IOTestCase::~IOTestCase()
{}

// ------------------------------------------------------------

bool
IOTestCase::runTest()
{
	bool rc = true;

	try {
		string fn( rdfFiles.front() );
		Model m1("file", fn );
		rc = rc && test( true, "io 1");
		Serializer ser;
		rc = rc && test( true, "io 2");
		URI uri( string("file://") + fn + "#" );
		ser->setNamespace( uri, "bross");
		bool res = ser->toFile( "/tmp/iotest.rdf", m1, uri );
		rc = rc && test( res, "io 3");

		vector< string> parsers;
		Parser_::listParsers( parsers );
		for( auto & x : parsers )
			cout << x << endl;
		rc = rc && test( parsers.size() > 0, "io 4" );

		Model m2("memory" );
		Parser p( "rdfxml" );
		URI base( "file://tmp/");
		URI local( "file:///tmp/iotest.rdf");
		p->parseIntoModel(m2, local, base );
		rc = rc && test( true, "io 5");

		rc = rc && test( m1->size() == m2->size(), "io 6");
		cout << fn << ": " << m1->size() << "," << m2->size() << endl;
		
		Stream x = m1->toStream();
		while ( ! x->end() )
		{
			rc = rc && test( m2->contains( x->current()), "io 7");
			x->next();
		}
	}
	catch( vx & e )
	{
		rc = test( false, e.what());
	}

	return rc;
}

// ------------------------------------------------------------
//	QueryTestCase
// ------------------------------------------------------------

QueryTestCase::QueryTestCase( csr nm)
	: TestCaseT<QueryTestCase>(nm)
{
	XINI & xini = XINI::instance();
	string xpath("/SASSY-CONFIG/Test/librdfxx/TestData");
	int n = xini.getVals( xpath, rdfFiles );
	if ( n == 0 )
		throw cdi::test_exception() << "No RDF test data\n";
}

// ------------------------------------------------------------

QueryTestCase::~QueryTestCase()
{}

// ------------------------------------------------------------

bool
QueryTestCase::runTest()
{
	bool rc = true;

	try {
		string fn( rdfFiles.front() );
		Model m1("file", fn );

		/*
		Stream x = m1->toStream();
		while ( ! x->end() )
		{
			cout << x->current()->toString() << endl;
			x->next();
		}
		*/
		QueryString qs;
		qs.addPrefix("rdf", "http://www.w3.org/1999/02/22-rdf-syntax-ns#");
		qs.addPrefix("rdfs", "http://www.w3.org/2000/01/rdf-schema#");
		qs.setVariables("?label");
		qs.addCondition("?x rdfs:label ?label");

		// cout << (string)qs << endl;
		string ts = 
		"PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> \n"
		"PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#> \n"
		"SELECT ?label\n"
		"WHERE\n"
		"{\n"
		"?x rdfs:label ?label.\n"
		"}\n";
		rc = rc && test( ts == (string)qs, "query 1");

		Query q(qs);
		QueryResults qr = q->execute(m1);

		int count = 0;
		for( auto &x : *qr )
		{
			count++;
			/*
			for ( int i=0; i<x.count(); i++ )
			{
				cout << x.getBoundName(i) << ": ";
				cout << x.getBoundValue(i)->toString() << endl;
			}
			*/
		}
		rc = rc && test( count == 48, "query 2");

	}
	catch( vx & e )
	{
		rc = test( false, e.what());
	}

	return rc;
}



// ------------------------------------------------------------
//	ExampleTestCase
// ------------------------------------------------------------

ExampleTestCase::ExampleTestCase( csr nm)
	: TestCaseT<ExampleTestCase>(nm), testValue(100)
{}

// ------------------------------------------------------------

ExampleTestCase::~ExampleTestCase()
{}

// ------------------------------------------------------------

bool
ExampleTestCase::runTest()
{
	bool rc = true;
	rc = rc && test( testValue == 100, "success test");
	rc = rc && test( testValue == 99, "failure test", EXPECTED );

	return rc;

}

// ------------------------------------------------------------

int main( int argc, char * argv[])
{
	bool rc = true;
	try {
		XINI::configure(argc, argv);
		XINI &xini = XINI::instance();
		cout << "Using config: " << xini.getConfigFilename() << endl;

		Tester::instance().configure("librdfxx");

		// ExampleTestCase::install("Tester test", "Tester scenario");
		  URITestCase::install("A URI test",       "RDF scenario");
		 NodeTestCase::install("B Node test",      "RDF scenario");
		StmntTestCase::install("C Statement test", "RDF scenario");
		ModelTestCase::install("D Model test",     "RDF scenario");
		StoreTestCase::install("E Store test",     "RDF scenario");
		   IOTestCase::install("F IO test",        "RDF scenario");
		QueryTestCase::install("G Query test",     "RDF scenario");

		Tester::instance().runTests();
		rc = Tester::instance().summary();
		cout << "rdftest completed " << (rc ? "without errors" : "with errors") << endl;
	}
	catch( sx & xx )
	{
		cerr << "Exception: " << xx.what() << endl;
		rc = false;
	}
	return (rc ? 0 : 1);
}


// --------------------- end ---------------------------------------

