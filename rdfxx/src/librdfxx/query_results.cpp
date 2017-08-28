
/* RDF C++ API 
 *
 * 			query_results.cpp
 *
 * 	Copyright 2007 - 2008	Sebastian Faubel
 * 	Copyright 2017		Brenton Ross
 *
 * -----------------------------------------------------------------------------
 * LICENSE
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 * 
 * -----------------------------------------------------------------------------
 */


#include <rdfxx/except.h>
#include <rdfxx/query_results.hpp>
#include <rdfxx/node.hpp>
#include <rdfxx/query.hpp>
#include <rdfxx/rdfxx.h>

using namespace rdf;
using namespace std;

// -----------------------------------------------------------------------------
//	QueryResults_::iterator
// -----------------------------------------------------------------------------

QueryResults_::iterator::iterator( QueryResult_* qr )
	: query_result( qr )
{
}

// -----------------------------------------------------------------------------

QueryResults_::iterator::iterator()
	: query_result( new _QueryResult)
{
}

// -----------------------------------------------------------------------------

QueryResult_ &
QueryResults_::iterator::operator *() const
{
	if ( query_result )
		return *query_result;
	else
		throw VX(Code) << "Attempt to dereference null pointer";
}

// -----------------------------------------------------------------------------

QueryResults_::iterator &
QueryResults_::iterator::operator ++ ()
{
	_QueryResult *qe = static_cast< _QueryResult * >( query_result );
	librdf_query_results *qr = qe->ptr();
	if ( qr )
	{
    		librdf_query_results_next(qr);
		// ignore result - rely on call to finished
	}
	qe->reset();
	return *this;
}

// -----------------------------------------------------------------------------

QueryResults_::iterator &
QueryResults_::iterator::operator ++ (int)
{
	return operator++();
}

// -----------------------------------------------------------------------------

bool
QueryResults_::iterator::operator == ( const QueryResults_::iterator &x ) const
{
	_QueryResult *qr  = static_cast< _QueryResult * >( query_result );
	_QueryResult *qrx = static_cast< _QueryResult * >( x.query_result );

	return qr->ptr() == qrx->ptr();
}

// -----------------------------------------------------------------------------

bool
QueryResults_::iterator::operator != ( const QueryResults_::iterator &x ) const
{
	return !(operator==(x));
}

// -----------------------------------------------------------------------------
//	_QueryResult
// -----------------------------------------------------------------------------

int
_QueryResult::count() const
{
    if ( numberBound < 0 )
    {
    	numberBound = librdf_query_results_get_bindings_count(query_results);
	if ( numberBound < 0 )
	{
		throw VX(Code) << "query failed or results exhausted";
	}
    }
    // cout << "Number bound = " << numberBound << endl;
    return numberBound;
}

// -----------------------------------------------------------------------------

Node
_QueryResult::getBoundValue(int _offset) const
{
    if(_offset >= count())
    {
	throw VX(Error) << "Parameter out of range";
    }

    // The library returns a new node pointer so it will need to be freed
#if USE_NODE
    Node value( new _Node(world, librdf_query_results_get_binding_value(query_results, _offset), true));
#else
	librdf_node *n = librdf_query_results_get_binding_value(query_results, _offset);
	Node value( _NodeBase::make( world, n, true ));
#endif
    if(!value)
	throw VX(Error) << "Failed to allocate node";

    return value;
}

// -----------------------------------------------------------------------------

Node
_QueryResult::getBoundValue(const std::string & _name ) const
{
    if(_name.empty())
    {
	throw VX(Error) << "No binding name supplied";
    }

    // The library returns a new node pointer so it will need to be freed
    librdf_node *n = librdf_query_results_get_binding_value_by_name(
    			query_results, _name.c_str());
    if ( ! n )
    {
    	// cout << toString() << endl;
    	throw VX(Error) << "\"" << _name << "\" is not bound.";
    }

    // The library returns a new node pointer so it will need to be freed
#if USE_NODE
    Node value( new _Node( world, n, true ));
#else
	Node value( _NodeBase::make( world, n, true ));
#endif
    if(!value)
	throw VX(Error) << "Failed to allocate node";


    return value;
}


// -----------------------------------------------------------------------------

std::string
_QueryResult::getBoundName(int _offset) const
{
    if(_offset >= count())
    {
	throw VX(Error) << "Parameter out of range";
    }

    const char* name =
    	librdf_query_results_get_binding_name(query_results, _offset);

	string res;
	if ( name ) res.assign(name);
    return res;
}

// -----------------------------------------------------------------------------

void
_QueryResult::reset()
{
	// cout << "_QueryResult::reset() ";
	numberBound = -1;
    	int status = librdf_query_results_finished(query_results);
	if ( status )
	{
		// cout << "query finished";
		query_results = nullptr;
	}
	// cout << endl;
}

// -----------------------------------------------------------------------------

std::string
_QueryResult::toString() const
{
	string s;

	if ( count() <= 0 )
		s += "No values bound";

	bool first = true;
	for ( int i=0; i<count(); i++ )
	{
		if ( first )
			first = false;
		else
			s += "; ";
		s += getBoundName(i);
		s += " = ";
		s += getBoundValue(i)->toString();
	}

	return s;
}

// -----------------------------------------------------------------------------
//	_QueryResults
// -----------------------------------------------------------------------------

_QueryResults::_QueryResults(World w, _Query& _query, _Model& _model)
	 : world(w), query_results(0)
{
    query_results = librdf_query_execute(_query, _model);
    if(!query_results)
	throw VX(Error) << "Failed to allocate query results";

    int status = librdf_query_results_finished(query_results);
    if ( status )
    	currIter = QueryResult( new _QueryResult );
    else
    	currIter = QueryResult( new _QueryResult( world, query_results ));
}

// -----------------------------------------------------------------------------

_QueryResults::_QueryResults(World w, _Query& _query, librdf_model* _model)
	 : world(w), query_results(0)
{
    query_results = librdf_query_execute(_query, _model);
    if(!query_results)
	throw VX(Error) << "Failed to allocate query results";

    int status = librdf_query_results_finished(query_results);
    if ( status )
    	currIter = QueryResult( new _QueryResult );
    else
    	currIter = QueryResult( new _QueryResult( world, query_results ));
}

// -----------------------------------------------------------------------------

_QueryResults::~_QueryResults()
{
    if(query_results)
    {
        librdf_free_query_results(query_results);
        query_results = 0;
    } 
}

// -----------------------------------------------------------------------------

std::string
_QueryResults::toString()
{
    URI syntax_uri(world, "http://www.w3.org/TR/2004/WD-rdf-sparql-XMLres-20041221/");
    URI base_uri;

    return toString(syntax_uri, base_uri);
}

// -----------------------------------------------------------------------------

std::string
_QueryResults::toString(URI _syntax_uri, URI _base_uri)
{
    	if(!query_results)
        	return "";

	librdf_uri* su = DEREF( URI, librdf_uri, _syntax_uri );
	librdf_uri* bu = DEREF( URI, librdf_uri, _base_uri );

    	const char*qr = (const char*) librdf_query_results_to_string2( query_results, 
    				NULL, NULL, su, bu );
	if ( qr )
		return string(qr);
	else
		return string("query returned no results");
	
	// this function has consumed the results, so we cannot iterate over them
    	currIter = QueryResult(nullptr);
}

// -----------------------------------------------------------------------------

QueryResults_::iterator
_QueryResults::begin() const
{
	QueryResults_::iterator I( currIter.get() );
	return I;
}

// -----------------------------------------------------------------------------

QueryResults_::iterator
_QueryResults::end() const
{
	QueryResults_::iterator I;
	return I;
}

// -----------------------------------------------------------------------------

bool
_QueryResults::success() const
{
    int status = librdf_query_results_finished(query_results);

    return (status != 0) ? false : true;
}

// ----------------------------- end -------------------------------

