
/* RDF C++ API 
 *
 * 			query_string.hpp
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


#ifndef RDFXX_QUERY_STRING_HPP
#define RDFXX_QUERY_STRING_HPP

#include <iostream>
#include <map>
#include <vector>
#include <librdf.h>

#include <rdfxx/uri.hpp>

namespace rdf
{

// ============================================================================
//! RDF C++ _QueryString
// ============================================================================

/*
class _QueryString
{
    // ------------------------------------------------------------------------
    private:
    std::map<std::string, std::string> prefixes;
    std::string variables;
    std::vector<std::string> conditions;
    std::string order;

    // ------------------------------------------------------------------------
    public:
    _QueryString() {}

    void addPrefix( const std::string & _id,  const std::string & _uri);
    void setVariables( const std::string & _variables);
    void addCondition( const std::string & _condition);
    void orderBy( const std::string & _order);

    std::string get_string();

    operator std::string();
};
*/

} // namespace rdf

#endif
