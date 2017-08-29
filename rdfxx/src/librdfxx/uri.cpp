
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
#include <rdf_concepts.h>

using namespace rdf;
using namespace std;

// -----------------------------------------------------------------------------
//	URI
// -----------------------------------------------------------------------------

std::map< Concept, librdf_concepts_index > _URI::concepts =
	{
        	{ Concept::Container, 		LIBRDF_CONCEPT_S_Container },
		{ Concept::Bag, 		LIBRDF_CONCEPT_MS_Bag },
		{ Concept::Sequence, 		LIBRDF_CONCEPT_MS_Seq },
		{ Concept::Alternative, 	LIBRDF_CONCEPT_MS_Alt },
		{ Concept::aboutEach, 		LIBRDF_CONCEPT_MS_aboutEach },
        	{ Concept::List, 		LIBRDF_CONCEPT_RS_List },
		{ Concept::first, 		LIBRDF_CONCEPT_RS_first },
		{ Concept::rest, 		LIBRDF_CONCEPT_RS_rest },
		{ Concept::nil, 		LIBRDF_CONCEPT_RS_nil },
		{ Concept::Statement, 		LIBRDF_CONCEPT_MS_Statement },
		{ Concept::object, 		LIBRDF_CONCEPT_MS_object },
		{ Concept::predicate, 		LIBRDF_CONCEPT_MS_predicate },
		{ Concept::subject, 		LIBRDF_CONCEPT_MS_subject },
        	{ Concept::Resource, 		LIBRDF_CONCEPT_S_Resource },
		{ Concept::Class, 		LIBRDF_CONCEPT_S_Class },
		{ Concept::subClassOf, 		LIBRDF_CONCEPT_S_subClassOf },
		{ Concept::type, 		LIBRDF_CONCEPT_MS_type },
		{ Concept::Property, 		LIBRDF_CONCEPT_MS_Property },
		{ Concept::subPropertyOf,	LIBRDF_CONCEPT_S_subPropertyOf },
        	{ Concept::domain, 		LIBRDF_CONCEPT_S_domain },
		{ Concept::range, 		LIBRDF_CONCEPT_S_range },
		{ Concept::ConstraintProperty, 	LIBRDF_CONCEPT_S_ConstraintProperty },
		{ Concept::ConstraintResource, 	LIBRDF_CONCEPT_S_ConstraintResource },
        	{ Concept::Description, 	LIBRDF_CONCEPT_MS_Description },
		{ Concept::label, 		LIBRDF_CONCEPT_S_label },
		{ Concept::seeAlso, 		LIBRDF_CONCEPT_S_seeAlso },
		{ Concept::comment, 		LIBRDF_CONCEPT_S_comment },
		{ Concept::isDefinedBy, 	LIBRDF_CONCEPT_S_isDefinedBy }
	};

// -----------------------------------------------------------------------------

/*
URI::URI()
	: std::shared_ptr< URI_ >( new _URI )
{}
*/

// -----------------------------------------------------------------------------

URI::URI(URI_* _uri)
	: std::shared_ptr< URI_ >( _uri )
{}

// -----------------------------------------------------------------------------

URI::URI( World w, const std::string & _uri )
	: std::shared_ptr< URI_ >( new _URI( w, _uri.c_str() ))
{}

// -----------------------------------------------------------------------------

URI::URI( World w, Concept c )
	: std::shared_ptr< URI_ >( new _URI( w, c ))
{}

// -----------------------------------------------------------------------------

URI::URI( const std::string & filename, World _w )
	: std::shared_ptr< URI_ >( new _URI( filename.c_str(), _w ))
{}

// -----------------------------------------------------------------------------

URI::URI( const std::string &uri_string, URI source_uri, URI base_uri )
	: std::shared_ptr< URI_ >( new _URI( uri_string.c_str(), source_uri, base_uri ))
{}

// -----------------------------------------------------------------------------

URI::URI( URI base_uri, const std::string &uri_string )
	: std::shared_ptr< URI_ >( new _URI( base_uri, uri_string.c_str() ))
{}

// -----------------------------------------------------------------------------
//	_URI
// -----------------------------------------------------------------------------

/*
_URI::_URI()
	: uri(0)
{}
*/

// -----------------------------------------------------------------------------

_URI::_URI(World w, const char* _uri_string)
	 : uri(0)
{
	set_string(w, _uri_string );
}

// -----------------------------------------------------------------------------

_URI::_URI( const char* _filename, World _w )
{
    	librdf_world* w = DEREF( World, librdf_world, _w);
	uri = librdf_new_uri_from_filename( w, _filename );
    	if(!uri)
		throw VX(Error) << "Failed to allocate URI";
}

// -----------------------------------------------------------------------------

_URI::_URI( World _w, Concept concept )
{
    	librdf_world* w = DEREF( World, librdf_world, _w);
	librdf_uri* _uri = librdf_get_concept_uri_by_index( w, concepts[concept] );
	uri = librdf_new_uri_from_uri( _uri );
    	if(!uri)
		throw VX(Error) << "Failed to allocate URI";
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

_URI::_URI( const char *uri_string, URI source_uri, URI base_uri )
{
	librdf_uri *base = DEREF( URI, librdf_uri, base_uri );
	librdf_uri *src = DEREF( URI, librdf_uri, source_uri );

	uri = librdf_new_uri_normalised_to_base( (const unsigned char *)uri_string,
				src, base );
    	if(!uri)
		throw VX(Error) << "Failed to allocate URI";
}


// -----------------------------------------------------------------------------

_URI::_URI( URI _base, const char *_uri_string )
{
	librdf_uri *base = DEREF( URI, librdf_uri, _base );
	uri = librdf_new_uri_relative_to_base( base, (const unsigned char *)_uri_string );
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

// trim everything after last / or #
URI
_URI::trim( World w ) const
{
	string s( toString() );
	string::size_type p = s.find_last_of('#');
	if ( p == string::npos )
	{
		p = s.find_last_of('/');
		if ( p == string::npos )
			return copy();
		else
			return URI( w, s.substr(0,p+1));
	}
	else
		return URI( w, s.substr(0,p+1));
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
    string s( (const char*) librdf_uri_as_string(uri));

    return s;
}

// -----------------------------------------------------------------------------

_URI::operator const char*() const
{
    return toString().c_str();
}

// -----------------------------------------------------------------------------

bool
_URI::isFileName() const
{
	int res = librdf_uri_is_file_uri( uri );
	return ( res != 0 );
}

// -----------------------------------------------------------------------------

std::string
_URI::toFileName() const
{
	string s( librdf_uri_to_filename( uri ));
	return s;
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
	if ( _uri == std::shared_ptr<URI_>(nullptr) ) return false;
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
