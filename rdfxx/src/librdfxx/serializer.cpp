
/* RDF C++ API 
 *
 * 			serializer.cpp
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
#include <rdfxx/serializer.hpp>

using namespace rdf;
using namespace std;

// -----------------------------------------------------------------------------
//	Serializer
// -----------------------------------------------------------------------------

Serializer::Serializer( const std::string &name, const std::string & syntax_mime )
	: std::shared_ptr< Serializer_ >( new _Serializer( name, syntax_mime ))
{}

// -----------------------------------------------------------------------------

Serializer::Serializer( const std::string &name, URI syntax_uri )
	: std::shared_ptr< Serializer_ >( new _Serializer( name, syntax_uri ))
{}

// -----------------------------------------------------------------------------
//	_Serializer
// -----------------------------------------------------------------------------

_Serializer::_Serializer(const std::string& _name, const std::string& _syntax_mime)
	: serializer(0)
{
    if(_name.empty())
    {
	throw VX(Error) << "Factory name parameter was blank";
    }

    _World& world = _World::instance();

    serializer = librdf_new_serializer(world, _name.c_str(), _syntax_mime.c_str(), 0);
    if(!serializer)
    {
	throw VX(Error) << "Failed to allocate serializer: " << _name 
		<< ", " << _syntax_mime;
    }
}

// -----------------------------------------------------------------------------

_Serializer::_Serializer(const std::string& _name, URI _syntax_uri)
	: serializer(0)
{
    if(_name.empty())
    {
	throw VX(Error) << "Factory name parameter was blank";
    }

    librdf_uri * su = DEREF( URI, librdf_uri, _syntax_uri );
    _World& world = _World::instance();

    serializer = librdf_new_serializer(world, _name.c_str(), 0, su );
    if(!serializer)
    {
	throw VX(Error) << "Failed to allocate serializer: " << _name 
		<< ", " << _syntax_uri->toString();
    }
}

// -----------------------------------------------------------------------------

_Serializer::~_Serializer()
{
    if(serializer)
    {
        librdf_free_serializer(serializer);
        serializer = 0;
    } 
}

// -----------------------------------------------------------------------------

bool
_Serializer::setNamespace(URI _uri, const std::string & _prefix)
{
    librdf_uri *u = DEREF( URI, librdf_uri, _uri );
    int status = librdf_serializer_set_namespace(serializer, u, _prefix.c_str());

    return (status == 0) ? true : false;
}

// -----------------------------------------------------------------------------

bool
_Serializer::toFile(const std::string & _file, Model _model)
{
    if(_file.empty())
    {
	throw VX(Error) << "File name parameter was not specified";
    }

	librdf_model *m = DEREF( Model, librdf_model, _model );
    int status =
    librdf_serializer_serialize_model_to_file(serializer, _file.c_str(), 0, m );

    return (status == 0) ? true : false;
};

// -----------------------------------------------------------------------------

bool
_Serializer::toFile(const std::string & _file, Model _model, URI _base_uri)
{
    if(_file.empty())
    {
	throw VX(Error) << "File name parameter was not specified";
    }
	librdf_model *m = DEREF( Model, librdf_model, _model );
	librdf_uri  *bu = DEREF( URI, librdf_uri, _base_uri );

    int status =
    librdf_serializer_serialize_model_to_file(serializer, _file.c_str(), bu, m);

    return (status == 0) ? true : false;
};

// -------------------------------- end ------------------------------------

