
/* RDF C++ API 
 *
 * 			parser.cpp
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
#include <rdfxx/parser.hpp>
#include <rdfxx/model.hpp>
#include <rdfxx/world.hpp>

using namespace rdf;
using namespace std;

// -----------------------------------------------------------------------------
//	Parser
// -----------------------------------------------------------------------------

Parser::Parser( World w, const std::string & name, const std::string & syntax_mime )
	: std::shared_ptr< Parser_ >( new _Parser(w, name, syntax_mime))
{}

// -----------------------------------------------------------------------------

Parser::Parser( World w, const std::string & name, URI syntax_uri )
	: std::shared_ptr< Parser_ >( new _Parser( w, name, syntax_uri ))
{}

// -----------------------------------------------------------------------------
//	_Parser
// -----------------------------------------------------------------------------

_Parser::_Parser( World _w, const std::string& _name, const std::string& _syntax_mime)
	 : parser(0)
{
    // _World& world = _World::instance();
    	librdf_world* world = DEREF( World, librdf_world, _w);

    parser = librdf_new_parser(world, _name.c_str(), _syntax_mime.c_str(), 0);
    if(!parser)
	throw VX(Error) << "Failed to allocate parser";
}

// -----------------------------------------------------------------------------

_Parser::_Parser( World _w, const std::string& _name, URI _syntax_uri)
	 : parser(0)
{
    // _World& world = _World::instance();
    	librdf_world* world = DEREF( World, librdf_world, _w);
    librdf_uri *uri = DEREF( URI, librdf_uri, _syntax_uri );

    parser = librdf_new_parser(world, _name.c_str(), 0, uri);
    if(!parser)
	throw VX(Error) << "Failed to allocate parser";
}

// -----------------------------------------------------------------------------

bool
_Parser::parseIntoModel(Model _model, URI _file, URI _base_uri)
{
	_Model* m = static_cast< _Model * >( _model.get() );
	_URI * u  = static_cast< _URI * >( _file.get());
	_URI * bu = static_cast< _URI * >( _base_uri.get());
	return (librdf_parser_parse_into_model(parser, *u, *bu, *m) == 0) ? true : false;
}

// -----------------------------------------------------------------------------

_Parser::~_Parser()
{
    if(parser)
    {
        librdf_free_parser(parser);
        parser = 0;
    } 
}

// -----------------------------------------------------------------------------

_Parser::operator librdf_parser*()
{
    return parser;
}

// -----------------------------------------------------------------------------

//static
int Parser_::listParsers( World _w, std::vector< std::string > & parsers )
{
    	librdf_world* world = DEREF( World, librdf_world, _w);
	int counter = 0;  // guess it starts at zero
	while( true )
	{
		const raptor_syntax_description * desc 
			= librdf_parser_get_description( world, counter );
		if ( desc )
		{
			string s;
			const char *n = desc->names[0];
			if ( n )
				s += n;
			s += ":";
			const char *u = nullptr;
			for ( unsigned int i=0; i< desc->uri_strings_count; i++ )
			{
				s += " ";
				u = desc->uri_strings[i];
				if ( u )
				{
					s += "<";
					s += u;
					s += ">";
				}
			}
			parsers.push_back( s );
			counter++;
		}
		else
		{
			break;
		}
	}
	return counter;
}

// -------------------------------- end ----------------------------------------
