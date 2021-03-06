/*
 * 			rdftest.h
 *
 *		Copyright 2012 - 2017 Brenton Ross
 *
 *		This file is part of SASSY.
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

#include <cfi/testmgr.h>
#include <cfi/stringy.h>
#include "rdfxx/rdfxx.h"

#ifndef SASSY_RDFTEST_H
#define SASSY_RDFTEST_H

// -------------------------------------------------------------------

class URITestCase : public SASSY::cdi::TestCaseT<URITestCase>
{
protected:
	rdf::World world;
	bool runTest();

public:
	URITestCase(csr nm);
	~URITestCase();
};

// -------------------------------------------------------------------

class PrefixTestCase : public SASSY::cdi::TestCaseT<PrefixTestCase>
{
protected:
	rdf::World world;
	std::vector< std::string > rdfFiles;
	bool runTest();

public:
	PrefixTestCase(csr nm);
	~PrefixTestCase();
};


// -------------------------------------------------------------------

class NodeTestCase : public SASSY::cdi::TestCaseT<NodeTestCase>
{
protected:
	rdf::World world;
	bool runTest();

public:
	NodeTestCase(csr nm);
	~NodeTestCase();
};

// -------------------------------------------------------------------

class StmntTestCase : public SASSY::cdi::TestCaseT<StmntTestCase>, 
			public rdf::ErrorClient
{
protected:
	rdf::World world;
	bool runTest();

public:
	StmntTestCase(csr nm);
	~StmntTestCase();

	virtual void handleError( const std::string & message );
	virtual void handleWarning( const std::string & message );
};

// -------------------------------------------------------------------

class ModelTestCase : public SASSY::cdi::TestCaseT<ModelTestCase>
{
protected:
	rdf::World world;
	bool runTest();

public:
	ModelTestCase(csr nm);
	~ModelTestCase();
};

// -------------------------------------------------------------------

class StoreTestCase :  public SASSY::cdi::TestCaseT<StoreTestCase>
{
protected:
	rdf::World world;
	bool runTest();
	std::vector< std::string > rdfFiles;
public:
	StoreTestCase(csr nm);
	~StoreTestCase();
};


// -------------------------------------------------------------------
// A test case for parsers and serialisers

class IOTestCase : public SASSY::cdi::TestCaseT<IOTestCase>
{
protected:
	rdf::World world;
	bool runTest();
	std::vector< std::string > rdfFiles;
public:
	IOTestCase(csr nm);
	~IOTestCase();
};


// -------------------------------------------------------------------

class QueryTestCase : public SASSY::cdi::TestCaseT<QueryTestCase>
{
protected:
	rdf::World world;
	bool runTest();
	std::vector< std::string > rdfFiles;
public:
	QueryTestCase(csr nm);
	~QueryTestCase();
};

// -------------------------------------------------------------------

class LiteralTestCase : public SASSY::cdi::TestCaseT< LiteralTestCase >
{
protected:
	rdf::World world;
	bool runTest();
public:
	LiteralTestCase(csr nm);
	~LiteralTestCase();
};

// -------------------------------------------------------------------

namespace graph
{

struct Node
{
	enum NodeType { Resource, Literal, Blank };
	
	NodeType nt;
	std::string value;
};
using NodePtr = std::shared_ptr< Node >;
using NodeWPtr = std::weak_ptr< Node >;

struct Edge
{
	NodeWPtr fromNode, toNode;
	std::string value;
};
using EdgePtr = std::shared_ptr< Edge >;

class Graph
{
private:
	std::map< std::string, NodePtr > nodes;
	std::vector< EdgePtr > edges;
public:
	Graph(){}

	void insertEdge( const Node &from, csr val, const Node & to ); 
	std::vector< EdgePtr >::iterator begin() { return edges.begin(); }
	std::vector< EdgePtr >::iterator end() { return edges.end(); }
};

}

class ConstructTestCase : public SASSY::cdi::TestCaseT< ConstructTestCase >
{
protected:
	rdf::World world;
	SASSY::Path testFile;
	bool runTest();
public:
	ConstructTestCase(csr nm);
	~ConstructTestCase();
};


// -------------------------------------------------------------------

class ExampleTestCase : public SASSY::cdi::TestCaseT<ExampleTestCase>
{
protected:
	rdf::World world;
	bool runTest();
	int testValue;
public:
	ExampleTestCase(csr nm);
	~ExampleTestCase();
};

#endif

// ------------------------------ end ------------------------------------

