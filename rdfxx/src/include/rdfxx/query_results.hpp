
/* RDF C++ API 
 *
 * 			query_results.hpp
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


#ifndef RDFXX_QUERYRESULTS_HPP
#define RDFXX_QUERYRESULTS_HPP

#include <iostream>
#include <queue>
#include <librdf.h>

#include <rdfxx/world.hpp>
#include <rdfxx/uri.hpp>
#include <rdfxx/rdfxx.h>

namespace rdf
{

class _Query;
class _Model;
class _Node;
class _QueryResults;

class _QueryResult : public QueryResult_
{
private:
	World world;
	librdf_query_results* query_results;
	mutable int numberBound;
public:
	_QueryResult() : query_results(nullptr), numberBound(-1) {}
	_QueryResult( World w, librdf_query_results *qr ) 
		: world(w), query_results(qr), numberBound(-1) {}

	virtual int  count() const;
	virtual std::string getBoundName(int offset) const;
	virtual Node getBoundValue(int offset) const;
	virtual Node getBoundValue( const std::string & name ) const;
	virtual std::string toString() const;

	librdf_query_results* ptr()const { return query_results; }
	void reset();
};

using QueryResult = std::shared_ptr< _QueryResult >;

// ============================================================================
//! RDF C++ _QueryResults
// ============================================================================

class _QueryResults : public QueryResults_
{
    QueryResult currIter;
    World world;

    // ------------------------------------------------------------------------
    private:
    librdf_query_results* query_results;

    // ------------------------------------------------------------------------
    public:
    //! RDF C++ QueryResults constructor.
    /*! Stores results of executing a query
     *  
     *  @param _query The query to execute.
     *  @param _model The model to query. 
     */
    _QueryResults(World, _Query& _query, _Model& _model);

    //! RDF C++ QueryResults destructor.
    /*! Deletes the internally stored librdf_query_results object.
     */
    ~_QueryResults();

    //! Get the query results as string.
    /*! NOTE:
     *  This method empties results.
     *
     *  @return QueryResults as const char* string.
     */ 
    //const char* get_string();
    std::string toString();

    //! Get the query results as string.
    /*! NOTE:
     *  This method empties results.
     *
     *  @param _syntax_uri URI of Syntax.
     *  @param _base_uri Base URI for Syntax.
     *  @return QueryResults as const char* string.
     */ 
    // const char* get_string(URI& _syntax_uri, URI& _base_uri);
    std::string toString( URI synatx, URI base_uri );

	//
	// iterator to get the results
	//
	QueryResults_::iterator begin() const;
	QueryResults_::iterator end() const;

    //! Indicates if there are bindings in the result.
    /*! 
     *  @return True if there are bindings. 
     */
    bool success() const;
};

} // nmamespace rdf

#endif
