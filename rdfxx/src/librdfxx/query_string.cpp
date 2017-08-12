
/* RDF C++ API 
 *
 * 			query_string.cpp
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


#include <rdfxx/rdfxx.h>

using namespace rdf;
using namespace std;

// -----------------------------------------------------------------------------
//	QueryString
// -----------------------------------------------------------------------------

void
QueryString::addPrefix(const std::string & _id, const std::string & _uri)
{
    prefixes[_id] = _uri;
}

// -----------------------------------------------------------------------------

void
QueryString::setVariables( const std::string & _variables)
{
    variables = _variables;
}

// -----------------------------------------------------------------------------

void
QueryString::addCondition(const std::string & _condition)
{
    conditions.push_back(_condition);
}

// -----------------------------------------------------------------------------

void
QueryString::orderBy( const std::string & _order)
{
    order = _order; 
}

// -----------------------------------------------------------------------------

// TODO: Improve..
// * Probably new line escapes '\n' cause portability issues..

std::string
QueryString::get_string()
{
    if(variables == "" || conditions.size() == 0 )
    {
        throw VX(Code) << "Must have variables and conditions in a query";
    }

    string query_string;

    map<string, string>::iterator it_pfx = prefixes.begin();
    while(it_pfx != prefixes.end())
    {
        query_string += "PREFIX ";
        query_string += it_pfx->first;
        query_string += ": <";
        query_string += it_pfx->second;
        query_string += "> \n";

        it_pfx++;
    }

    query_string += "SELECT ";
    query_string += variables;
    query_string += "\nWHERE\n{\n";

    vector<string>::iterator it_cnd = conditions.begin();
    while(it_cnd != conditions.end())
    {
        query_string += *it_cnd;
        query_string += ".\n";

        it_cnd++;
    }
    
    query_string += "}\n";

    if(order != "")
    {
        query_string += "ORDER BY " + order + '\n';
    }

    return query_string;
}

// -----------------------------------------------------------------------------

QueryString::operator std::string()
{
    return get_string();
}

// --------------------------- end -----------------------------------------

