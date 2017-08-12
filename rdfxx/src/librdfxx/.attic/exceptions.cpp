
/* RDF C++ API 
 *
 * 			exceptions.cpp
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


#include <rdfxx/exceptions.hpp>

using namespace rdf;
using namespace std;

// -----------------------------------------------------------------------------
void
Exception::print()
{
    cerr << "!! " << type << " in " << where() << endl;
    cerr << "!! " << what() << endl;
}

// -----------------------------------------------------------------------------
LibraryError::LibraryError(string& _where, string& _what)
{
    type = "LibraryError";
    Where = _where; What = _what;
}

// -----------------------------------------------------------------------------
ArgumentError::ArgumentError(string& _where, string& _what)
{
    type = "ArgumentError";
    Where = _where; What = _what;
}

// -----------------------------------------------------------------------------
AllocationError::AllocationError(const char* _where)
{
    type = "AllocationError";
    Where = _where;
    What = "Memory allocation with 'new' failed";
}

// -----------------------------------------------------------------------------
StatementError::StatementError(string& _where, string& _what)
{
    type = "StatementError";
    Where = _where; What = _what;
}
