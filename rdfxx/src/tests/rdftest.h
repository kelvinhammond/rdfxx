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

#ifndef SASSY_RDFTEST_H
#define SASSY_RDFTEST_H

// -------------------------------------------------------------------

class URITestCase : public SASSY::cdi::TestCaseT<URITestCase>
{
protected:
	bool runTest();

public:
	URITestCase(csr nm);
	~URITestCase();
};

// -------------------------------------------------------------------

class NodeTestCase : public SASSY::cdi::TestCaseT<NodeTestCase>
{
protected:
	bool runTest();

public:
	NodeTestCase(csr nm);
	~NodeTestCase();
};

// -------------------------------------------------------------------

class StmntTestCase : public SASSY::cdi::TestCaseT<StmntTestCase>
{
protected:
	bool runTest();

public:
	StmntTestCase(csr nm);
	~StmntTestCase();
};

// -------------------------------------------------------------------

class ModelTestCase : public SASSY::cdi::TestCaseT<ModelTestCase>
{
protected:
	bool runTest();

public:
	ModelTestCase(csr nm);
	~ModelTestCase();
};

// -------------------------------------------------------------------

class StoreTestCase :  public SASSY::cdi::TestCaseT<StoreTestCase>
{
protected:
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
	bool runTest();
	std::vector< std::string > rdfFiles;
public:
	QueryTestCase(csr nm);
	~QueryTestCase();
};

// -------------------------------------------------------------------

class ExampleTestCase : public SASSY::cdi::TestCaseT<ExampleTestCase>
{
protected:
	bool runTest();
	int testValue;
public:
	ExampleTestCase(csr nm);
	~ExampleTestCase();
};

#endif

// ------------------------------ end ------------------------------------

