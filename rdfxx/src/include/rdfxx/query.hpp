
/* RDF C++ API 
 *
 * 			query.hpp
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


#ifndef RDFXX_QUERY_HPP
#define RDFXX_QUERY_HPP

#include <iostream>
#include <librdf.h>

#include <rdfxx/world.hpp>
#include <rdfxx/uri.hpp>
#include <rdfxx/model.hpp>
#include <rdfxx/query_results.hpp>

namespace rdf
{

// ============================================================================
//! RDF C++ _Query
// ============================================================================

class _Query : public Query_
{
    // ------------------------------------------------------------------------
    private:
    World world;
    librdf_query* query;

    // ------------------------------------------------------------------------
    public:
    //! RDF C++ Query constructor.
    /*! Initializes a new Query.
     *  Throws AllocationError if allocation/initialization failed.
     *
     *  @param query_string The all mighty query string.
     *  @param uri The URI identifying the query language.
     *  @param lang The query language, defaults to "sparql".
     */
    _Query( World, const std::string & _query_string, const std::string& _lang="sparql");

    //! RDF C++ Query constructor.
    /*! Initializes a new Query.
     *  Throws AllocationError if allocation/initialization failed.
     *
     *  @param query_string The all mighty query string.
     *  @param uri The URI identifying the query language.
     *  @param base_uri A optional base URI.
     *  @param lang The query language, defaults to "sparql".
     */
    _Query( World, const std::string & _query_string, URI _base_uri, const std::string& _lang="sparql");

    //! RDF C++ Query destructor.
    /*! Deletes the internally stored librdf_query object.
     */
    ~_Query();

    //! Limit the number of results.
    /*!
     *  @param limit The maximum number of results.
     *  @return False on failure.
     */
    bool setLimit(int _limit);

    //! Get the limit for number of results.
    /*!
     *  @return The maximum number of results.
     */
    int getLimit() const;

    // Execute the query on a model
    QueryResults execute( Model );
    QueryResults execute( librdf_model* );

    // This is used internally for the C API.
    operator librdf_query*();
};

} // namespace rdf

#endif
