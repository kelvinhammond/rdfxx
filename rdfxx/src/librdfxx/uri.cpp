
/* RDF C++ API 
 *
 * 			uri.cpp
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
#include <rdfxx/uri.hpp>
#include <rdfxx/world.hpp>

using namespace rdf;
using namespace std;

// -----------------------------------------------------------------------------
//	URI
// -----------------------------------------------------------------------------

URI::URI()
	: std::shared_ptr< URI_ >( new _URI )
{}

// -----------------------------------------------------------------------------

URI::URI(URI_* _uri)
	: std::shared_ptr< URI_ >( _uri )
{}

// -----------------------------------------------------------------------------

URI::URI( World w, const std::string & _uri )
	: std::shared_ptr< URI_ >( new _URI( w, _uri.c_str() ))
{}

// -----------------------------------------------------------------------------
//	_URI
// -----------------------------------------------------------------------------

_URI::_URI()
	: uri(0)
{}

// -----------------------------------------------------------------------------

_URI::_URI(World w, const char* _uri_string)
	 : uri(0)
{
	set_string(w, _uri_string );
}

// -----------------------------------------------------------------------------

_URI::_URI( librdf_uri * _uri )
	 : uri(0)
{
	uri = librdf_new_uri_from_uri( _uri );
    if(!uri)
	throw VX(Error) << "Failed to allocate URI";
}

// -----------------------------------------------------------------------------
_URI::~_URI()
{
    if(uri)
    {
        librdf_free_uri(uri);
        uri = 0;
    } 
}

// -----------------------------------------------------------------------------

URI
_URI::copy() const
{
	return URI( new _URI( (librdf_uri*)*this ));
}

// -----------------------------------------------------------------------------
void
_URI::set_string(World _w, const char* _uri_string)
{
    if(uri)
    {
        // this->~URI();
	librdf_free_uri(uri);
	uri = 0;
    }
    
    // _World & world = _World::instance();

    librdf_world* w = DEREF( World, librdf_world, _w);

    uri = librdf_new_uri(w, (const unsigned char*) _uri_string);
    if(!uri)
	throw VX(Error) << "Failed to allocate URI";
}

// -----------------------------------------------------------------------------

std::string
_URI::toString() const
{
    string s( (const char*) librdf_uri_to_string(uri));

    return s;
}

// -----------------------------------------------------------------------------

_URI::operator const char*() const
{
    return toString().c_str();
}

// -----------------------------------------------------------------------------

bool
_URI::operator ==(_URI& _uri) const
{
    return (librdf_uri_equals(uri, _uri) == 0) ? false : true;
}

// -----------------------------------------------------------------------------

bool
_URI::operator ==( URI _uri ) const
{
	librdf_uri *u = DEREF( URI, librdf_uri, _uri );
    return (librdf_uri_equals(uri, u) == 0) ? false : true;
}

// -----------------------------------------------------------------------------

bool
rdf::operator == ( URI _a, URI _b )
{
	_URI *a = static_cast< _URI * >( _a.get());
	if ( a )
	{
		return ((*a) == _b);
	}
	else
		return false;
}

// -----------------------------------------------------------------------------

_URI::operator librdf_uri*() const
{
    return uri;
}

// ---------------------------- end --------------------------------------------
