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
#include "rdfxx/except.h"
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
{
	world = Universe::instance().world("test");
	Serializer ser = world->defaultSerializer();
	ser->setNamespace( URI(world, "http://www.w3.org/1999/02/22-rdf-syntax-ns#"), "rdf" );
}

// ------------------------------------------------------------

URITestCase::~URITestCase()
{}

// ------------------------------------------------------------

bool
URITestCase::runTest()
{
	bool rc = true;

	try {
		URI uri(world,"http://purl.org/dc/0.1/title#");
		rc = rc && test( true, "uri 1");

		rc = rc && test( (uri->toString() == "http://purl.org/dc/0.1/title#"), "uri 2");

		URI uri2 = uri->copy();

		rc = rc && test( (uri2->toString() == "http://purl.org/dc/0.1/title#"), "uri 3");
		rc = rc && test((uri == uri2), "uri 4");
		
		// confirm that URIs can go into standard containers
		vector< URI > uris;
		uris.push_back( uri2 );

		map< int, URI > uri_map;
		uri_map[1] = uri;
		map< URI, int > rev_uri_map;
		rev_uri_map[uri] = 4;
		rc = rc && test( uri_map[1] == uri, "uri 3.0.1");
		rc = rc && test( rev_uri_map[uri] == 4, "uri 3.0.2");

		// removal of fragment
		URI uri_frag(world,"http://purl.org/dc/0.1/title#stuff");
		rc = rc && test( (uri_frag->toString() == "http://purl.org/dc/0.1/title#stuff"), "uri 3.1");
		URI uri_defrag = uri_frag->trim(world);
		rc = rc && test( uri_defrag == uri, "uri 3.2");
		cout << "3.2: " << uri_defrag->toString() << endl;

		// uri <--> filename
		URI uri_fn( "/tmp/filename.rdf", world );
		cout << "URI filename: " << uri_fn->toString() << endl;
		cout << "URI filename as filename: " << uri_fn->toFileName() << endl;
		rc = rc && test( uri_fn->isFileName(), "uri 5");

		URI uri3( uri, "fruit" );
		cout << "relative to base: " << uri3->toString() << endl;
		rc = rc && test( uri3->toString() == "http://purl.org/dc/0.1/fruit", "uri 6");

		URI src_uri(world, "http://purl.org/dc/0.1/title#");
		URI base_uri(world, "dc:");
		URI uri4( "http://purl.org/dc/0.1/title#fred", src_uri, base_uri );

		cout << "prefix form: " << uri4->toString() << endl;
		rc = rc && test( uri4->toString() == "dc:fred", "uri 7");

		URI uri5( uri4->toString(), base_uri, src_uri );
		cout << "normal form: " << uri5->toString() << endl;
		rc = rc && test( uri5->toString() == "http://purl.org/dc/0.1/title#fred", "uri 8");

		Prefixes &prefixes = world->prefixes();
		prefixes.insert( "dc", uri );

		rc = rc && test( prefixes.find( uri ) == "dc", "uri 9");
		cout << "9: \"" << prefixes.find( uri ) << "\"" << endl;
		rc = rc && test( prefixes.find( "dc" ) == uri, "uri 10");


		cout << "---------- end URI tests -------------" << endl;
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
{
	world = Universe::instance().world("test");
}

// ------------------------------------------------------------

NodeTestCase::~NodeTestCase()
{}

// ------------------------------------------------------------

bool
NodeTestCase::runTest()
{
	bool rc = true;

	try {
		cout << "------------------ nodes --------------" << endl;
		Node n1(world,"http://purl.org/dc/0.1/title");
		rc = rc && test( true, "node 1");
		rc = rc && test( n1->isResource(), "node 2");
		rc = rc && test( ! n1->isBlank(), "node 3");
		rc = rc && test( ! n1->isLiteral(), "node 4");
		Node n2(world);
		rc = rc && test( n2->isBlank(), "node 5");
		Node n3(world,"fred", false);
		rc = rc && test( n3->isLiteral(), "node 6");
		Node n4(world,"<dc:name>TestLog</dc:name>", true);
		rc = rc && test( n4->isLiteral(), "node 7");
		Node n5(world,"<dc:name>TestLog</dc:name>", false, "en");
		rc = rc && test( n5->isLiteral(), "node 8");
		Node n6(world,"fred", false, "en");
		rc = rc && test( n6->isLiteral(), "node 9");
		Node n7(world,"http://purl.org/dc/0.1/title", false);
		rc = rc && test( !n7->isResource(), "node 10");
		rc = rc && test( n7->isLiteral(), "node 12");

		URI uri(world,"http://organise.org/ofw/0.4/categories/documents");
		Node n8(world, uri );
		rc = rc && test( n8->isResource(), "node 11");

		Node n9 = n8->copy();
		rc = rc && test( n9->isResource(), "node 13");

		URI uri2 = n9->toURI();
		rc = rc && test( uri == uri2, "node 14");
		rc = rc && test( n3->toString() == "\"fred\"", "node 15");
		// cout << "n3 = \"" << n3->toString() << "\"" << endl;

		// confirm that Nodes can go into standard containers
		vector< Node > node_list;
		node_list.push_back(n8);
		map< int, Node > node_map;
		node_map[2] = n2;

		// formatting
		Format format;
		format.usePrefixes = false;
		format.angleBrackets = true;
		rc = rc && test( n8->toString(format) == "<http://organise.org/ofw/0.4/categories/documents>",
					"node 16" );
		format.angleBrackets = false;
		rc = rc && test( n8->toString(format) == "http://organise.org/ofw/0.4/categories/documents",
					"node 17" );

		string s3("http://organise.org/ofw/0.4/categories/documents#");
		URI uri3(world, s3);
		world->prefixes().insert( "ofw", uri3 );
		URI uri4(world, s3 + "stuff");
		Node nf1(world, uri4);
		format.usePrefixes = true;
		rc = rc && test( nf1->toString(format) == "ofw:stuff", "node 18");
		if ( ! rc ) cout << "\"" << nf1->toString(format) << "\"" << endl;

		cout << "------------------ end nodes --------------" << endl;
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
{
	world = Universe::instance().world("test");
	world->registerErrorClient( this, true, true );
}

// ------------------------------------------------------------

StmntTestCase::~StmntTestCase()
{
	world->deregisterErrorClient( this );
}

// ------------------------------------------------------------

void 
StmntTestCase::handleError( const std::string & message )
{
	throw VX(rdf::Error) << message;
}

// ------------------------------------------------------------

void 
StmntTestCase::handleWarning( const std::string & message )
{
	throw VX(rdf::Warning) << message;
}


// ------------------------------------------------------------
bool
StmntTestCase::runTest()
{
	bool rc = true;

	try {
		Node n1(world);
		Statement s1(world, n1, n1, n1 );
		rc = rc && test( true, "stmnt 1");
		rc = rc && test( ! s1->isComplete(), "stmnt 2");
		Node n2(world,"fred", false);
		Node n3(world,"isA", false);
		Node n4(world,"moron", false );
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

		URI uri(world,"http://organise.org/ofw/0.4/categories/documents");
		s1->subject(Node(world, uri));
		rc = rc && test( Node(s1->subject())->toURI() == uri, "stmnt 8");

		n3 = Node(world,"http://purl.org/dc/0.1/title");
		s1->predicate(n3);
		rc = rc && test( s1->isComplete(), "stmnt 9");
		
		Statement s2(world, n1, n1, n1 );
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
{
	world = Universe::instance().world("test");
}

// ------------------------------------------------------------

ModelTestCase::~ModelTestCase()
{}

// ------------------------------------------------------------

bool
ModelTestCase::runTest()
{
	bool rc = true;

	try {
		Model m1(world,"memory");
		rc = rc && test( m1->size() == 0, "model 1");
		// cout << "model size is " << m1->size() << endl;
		URI uri(world,"http://organise.org/ofw/0.4/categories/documents");
		Node n1(world, uri );
		Node n2(world,"http://purl.org/dc/0.1/title");
		Node n3(world,"some literal value", false);
		Node n4(world,"a different literal value", false );

		Statement s1(world, n1, n2, n3 );
		m1->add( n1, n2, n3 );
		rc = rc && test( m1->size() == 1, "model 2");
		rc = rc && test( m1->contains( s1 ), "model 3");

		Statement s2(world, n1, n2, n4);
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
	world = Universe::instance().world("test");
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
	
	std::map< string, Model > models;

	for ( auto &f : rdfFiles )
	{
		Path p(f);
		string s(p.base());
		// cout << "Parsing " << s << endl;
		Model m(world,"file", f);
		models[s] = m;
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
	// cout << "Found " << count << " statements" << endl;
	rc = rc && test( count = 188, "store 2");

	
	return rc;
}

// ------------------------------------------------------------
//	IOTestCase
// ------------------------------------------------------------

IOTestCase::IOTestCase( csr nm )
	: TestCaseT<IOTestCase>(nm)
{
	world = Universe::instance().world("test");
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
		Model m1(world,"file", fn );
		rc = rc && test( true, "io 1");
		Serializer ser(world);
		rc = rc && test( true, "io 2");
		URI uri(world, string("file://") + fn + "#" );
		ser->setNamespace( uri, "bross");
		bool res = ser->toFile( "/tmp/iotest.rdf", m1, uri );
		rc = rc && test( res, "io 3");

		cout << "Parser list" << endl;
		vector< string> parsers = Parser_::listParsers(world );
		for( auto & x : parsers )
			cout << x << endl;
		rc = rc && test( parsers.size() > 0, "io 4" );
		cout << string( 30, '-' ) << endl;

		Model m2(world,"memory" );
		Parser p(world, "rdfxml" );
		URI base(world, "file://tmp/");
		URI local(world, "file:///tmp/iotest.rdf");
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
	world = Universe::instance().world("test");
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
		Model m1(world,"file", fn );
		world->prefixes().base( URI( fn + "#", world));

		Format format = { true, true, "blank", true, true, "en", true };
		Stream x = m1->toStream();
		while ( ! x->end() )
		{
			cout << "stmnt: >>> " << x->current()->toString(format) << " <<< " << endl;
			x->next();
		}
		
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

		Query q(world,qs);
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
//	LiteralTestCase
// ------------------------------------------------------------

LiteralTestCase::LiteralTestCase( csr nm )
	: TestCaseT< LiteralTestCase >(nm)
{
	world = Universe::instance().world("test");
}

// ------------------------------------------------------------

LiteralTestCase::~LiteralTestCase()
{
}

// ------------------------------------------------------------

bool
LiteralTestCase::runTest()
{
	bool rc = true;

	try {
		cout << "---------- begin literal tests ---------- " << endl;
		Literal L1;
		rc = rc && test( L1.dataType() == DataType::PlainLiteral, "literal 1");

		Literal L2(string("hello world"));
		rc = rc && test( L2.dataType() == DataType::PlainLiteral, "literal 2");
		rc = rc && test( L2.language() == "en", "literal 3");
		rc = rc && test( L2.asString() == "hello world", "literal 4");

		Literal L3("hello la world", "fr");
		rc = rc && test( L3.dataType() == DataType::PlainLiteral, "literal 5");
		rc = rc && test( L3.language() == "fr", "literal 6");
		rc = rc && test( L3.asString() == "hello la world", "literal 7");

		Literal L4("hello to the world", DataType::String, "hu");
		rc = rc && test( L4.dataType() == DataType::String, "literal 8");
		rc = rc && test( L4.language() == "hu", "literal 9");
		rc = rc && test( L4.asString() == "hello to the world", "literal 10");

		Literal L5( 23, DataType::Integer );
		rc = rc && test( L5.dataType() == DataType::Integer, "literal 11");
		rc = rc && test( L5.language() == "", "literal 12");
		rc = rc && test( L5.asString() == "23", "literal 13");

		Literal L6( 23.7, DataType::Float );
		rc = rc && test( L6.dataType() == DataType::Float, "literal 14");
		rc = rc && test( L6.language() == "", "literal 15");
		rc = rc && test( L6.asString() == "23.7", "literal 16");

		Literal L7( true );
		rc = rc && test( L7.dataType() == DataType::Boolean, "literal 17");
		rc = rc && test( L7.language() == "", "literal 18");
		rc = rc && test( L7.asString() == "true", "literal 19");

		rc = rc && test( L7.toString() == "\"true\"^^xsd:boolean", "literal 20");
		rc = rc && test( L6.toString() == "\"23.7\"^^xsd:float", "literal 21");
		rc = rc && test( L4.toString() == "\"hello to the world\"^^xsd:string", "literal 22");

		cout << "---------- end literal tests ---------- " << endl;
	}
	catch( vx & e )
	{
		rc = test( false, e.what());
	}
	return rc;
}


// ------------------------------------------------------------


// ------------------------------------------------------------
//	ExampleTestCase
// ------------------------------------------------------------

ExampleTestCase::ExampleTestCase( csr nm)
	: TestCaseT<ExampleTestCase>(nm), testValue(100)
{
	world = Universe::instance().world("test");
}

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
		LiteralTestCase::install("H Literal test", "RDF scenario");
		  /*
		*/

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

