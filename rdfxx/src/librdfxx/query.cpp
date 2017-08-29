
/* RDF C++ API 
 *
 * 			query.cpp
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
#include <rdfxx/query.hpp>
#include <rdfxx/uri.hpp>

using namespace rdf;
using namespace std;

// -----------------------------------------------------------------------------
//	Query
// -----------------------------------------------------------------------------

Query::Query( World w, const std::string & query, const std::string & lang )
	: std::shared_ptr< Query_ >( new _Query(w, query, lang))
{}

// -----------------------------------------------------------------------------

Query::Query( World w, const std::string & query, URI base_uri, const std::string & lang )
	: std::shared_ptr< Query_ >( new _Query(w, query, base_uri, lang))
{}

// -----------------------------------------------------------------------------
//	_Query
// -----------------------------------------------------------------------------

_Query::_Query(World _w, const string & _query_string, const std::string& _lang)
	 : world(_w), query(0)
{
    librdf_world* w = DEREF( World, librdf_world, _w );
    
    query = librdf_new_query(w, _lang.c_str(), 0, (unsigned char*) _query_string.c_str(), NULL);
    if(!query)
	throw VX(Error) << "Failed to allocate query";
}

// -----------------------------------------------------------------------------

_Query::_Query(World _w, const string & _query_string, URI _base_uri, const std::string& _lang)
	 : world(_w), query(0)
{
	librdf_uri *bu = DEREF( URI, librdf_uri, _base_uri );
    librdf_world* w = DEREF( World, librdf_world, _w );

    query = librdf_new_query(w, _lang.c_str(), 0, (unsigned char*) _query_string.c_str(), bu );
    if(!query)
	throw VX(Error) << "Failed to allocate query";
}

// -----------------------------------------------------------------------------

_Query::~_Query()
{
    if(query)
    {
        librdf_free_query(query);
        query = 0;
    } 
}

// -----------------------------------------------------------------------------

bool
_Query::setLimit(int _limit)
{
    int status = librdf_query_set_limit(query, _limit);
    return (status == 0) ? true : false;
}

// -----------------------------------------------------------------------------

int
_Query::getLimit() const
{
    return librdf_query_get_limit(query);
}

// -----------------------------------------------------------------------------

QueryResults
_Query::execute( Model _model )
{
	if ( _model )
	{
		_Model *m = static_cast< _Model * >( _model.get());
		return QueryResults( new _QueryResults(world, *this, *m ));
	}
	else
	{
		throw VX(Code) << "Model is null";
	}
	// not reached
	return nullptr;
}

// -----------------------------------------------------------------------------

QueryResults
_Query::execute( librdf_model* _model )
{
	if ( _model )
	{
		return QueryResults( new _QueryResults(world, *this, _model ));
	}
	else
	{
		throw VX(Code) << "Model is null";
	}
	// not reached
	return nullptr;
}


// -----------------------------------------------------------------------------

_Query::operator librdf_query*()
{
    return query;
}

// --------------------------------- end ---------------------------------------
