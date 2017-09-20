
/* RDF C++ API 
 *
 * 			world.cpp
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
#include <rdfxx/world.hpp>
#include <rdfxx/serializer.hpp>
#include <iostream>

using namespace rdf;
using namespace std;

// ----------------------------------------------------------------------------
//	Universe
// ----------------------------------------------------------------------------

Universe &
Universe::instance()
{
	static Universe universe;
	return universe;
}

// ----------------------------------------------------------------------------

World
Universe::world( const std::string & name )
{
	auto wi = worlds.find( name );
	if ( wi == worlds.end() )
	{
		World w( new _World(name) );
		worlds[name] = w;
		return w;
	}
	else
		return wi->second;
}

// ----------------------------------------------------------------------------
//	Prefixes
// ----------------------------------------------------------------------------

int Prefixes::anonCounter = 1;

// ----------------------------------------------------------------------------

Prefixes::Prefixes( World _w)
	: world(_w)
{
	librdf_world*w = DEREF( World, librdf_world, _w );
	
	URI rdf_uri( new _URI( librdf_get_concept_ms_namespace( w )));
	insert( "rdf", rdf_uri );

	URI rdfs_uri( new _URI( librdf_get_concept_schema_namespace(w)));
	insert( "rdfs", rdfs_uri );

	URI xsd_uri( new _URI( World(world), "http://www.w3.org/2001/XMLSchema#"));
	insert( "xsd", xsd_uri );

	URI dc_uri( new _URI( World(world), "http://purl.org/dc/elements/1.1/"));
	insert( "dc", dc_uri );

	URI owl_uri( new _URI( World(world), "http://www.w3.org/2002/07/owl#"));
	insert( "owl", owl_uri );

	URI rdfxx_uri( new _URI( World(world), "https://sourceforge.net/p/ocratato-sassy/rdfxx#"));
	insert( "rdfxx", rdfxx_uri );
}

// ----------------------------------------------------------------------------

void
Prefixes::base( const std::string & uri )
{
	char *abs_path = realpath( uri.c_str(), NULL );
	if ( abs_path )
	{
		string absPath(abs_path);
		free(abs_path);
		string::size_type p = absPath.find('#');
		if ( p == string::npos )
		{
			absPath += "#";
		}
		else
		{
			absPath = absPath.substr(0,p+1);
		}
		base( URI( absPath, World(world)));
	}
	else
	{
		throw VX(Error) << "Failed to get real path for " << uri 
			<< ": " << strerror(errno);
	}
}

// ----------------------------------------------------------------------------

bool
Prefixes::isBase( URI uri ) const
{
	string s( uri->toString() );
	string::size_type p = s.find_last_of('#');
	if ( p == string::npos )
	{
		return false;
	}
	else
	{
		URI u(World(world), s.substr(0,p+1));
		return (u == base_uri);
	}
}

// ----------------------------------------------------------------------------

std::string
Prefixes::removeBase( URI uri ) const
{
	string s( uri->toString() );
	string::size_type p = s.find_last_of('#');
	if ( p == string::npos )
	{
		return s;
	}
	else
	{
		return s.substr(p+1);
	}
}

// ----------------------------------------------------------------------------

void
Prefixes::anonymous( URI _uri )
{
	string pfx = find( _uri );
	if ( pfx.empty())
	{
		pfx = "pfx";
		pfx += std::to_string( anonCounter++ );
		insert( pfx, _uri );
	}
}

// ----------------------------------------------------------------------------

void
Prefixes::insert( const std::string &prefix, URI _uri )
{
	
	//
	// ns0 is used by librdf for local namespaces within a definition
	// so they need to be ignored.
	//
	if ( prefix == "ns0" )
	{
		return;
	}
	
	uriForPrefix[ prefix ] = _uri;
	prefixForURI[ _uri->toString() ] = prefix;
}

// ----------------------------------------------------------------------------

void
Prefixes::update( const std::string &oldPrefix, const std::string &newPrefix, URI _uri )
{
	remove( oldPrefix );
	insert( newPrefix, _uri );
}

// ----------------------------------------------------------------------------

void
Prefixes::remove( const std::string &prefix )
{
	URI uri = uriForPrefix[ prefix ];
	uriForPrefix.erase( prefix );
	prefixForURI.erase( uri->toString() );
}

// ----------------------------------------------------------------------------

std::string
Prefixes::find( URI _uri )
{
	URI u = _uri->trim( World(world));
	auto I = prefixForURI.find( u->toString() );
	if ( I == prefixForURI.end() )
	{
		return "";
	}
	else
		return I->second;
}

// ----------------------------------------------------------------------------

URI
Prefixes::find( const std::string &prefix )
{
	return uriForPrefix[ prefix ];
}

// ----------------------------------------------------------------------------

URI
Prefixes::uriForm( const std::string &s )
{
	if ( s.empty() ) return nullptr;
	if ( s[0] == '<' && s[s.length()-1] == '>' )
		return URI(World(world), s.substr(1, s.length()-2));
	string::size_type p = s.find(':');
	if ( p == string::npos )
	{
		return URI( s, base() );
	}
	if ( s.length() > p+2 )
	{
		if ( s[p+1] == '/' && s[p+2] == '/' )
			return URI(World(world), s );
	}
	URI uri = uriForPrefix[ s.substr(0,p)];
	if ( uri )
		return URI( s, URI(World(world), s.substr(0,p+1)), uri );
	else
		return nullptr;
}

// ----------------------------------------------------------------------------

std::string
Prefixes::prefixForm( URI uri )
{
	string s( uri->toString());
	if ( isBase( uri ))
	{
		s = removeBase( uri );
	}
	else
	{
		string prefix = find( uri );
		if (! prefix.empty())
		{
			URI src = find( prefix );
			if ( !( src == uri ) )
			{
				prefix.append(":");
				URI res( uri->toString(), src, URI(World(world), prefix));
				s = res->toString();
			}
		}
	}
	return s;
}

// ----------------------------------------------------------------------------
//	_World
// ----------------------------------------------------------------------------

_World::~_World()
{
    if(world)
    {
        librdf_free_world(world);
    }
}

// ----------------------------------------------------------------------------

_World::_World( const std::string &nm)
	: world_name(nm), world_prefixes(nullptr), 
	  forErrors(false), forWarnings(true)
{
	world = librdf_new_world();
        if(!world)
		throw VX(Error) << "Failed to allocate the World";
	
	librdf_world_set_warning( world, &forWarnings, errorHandler );
	librdf_world_set_error( world, &forErrors, errorHandler);
}

// ----------------------------------------------------------------------------

void 
_World::registerErrorClient( ErrorClient *client, bool warnings, bool errors )
{
	if ( warnings )
	{
		forWarnings.registerClient( client );
	}

	if ( errors )
	{
		forErrors.registerClient( client );
	}
}

// ----------------------------------------------------------------------------

void
_World::deregisterErrorClient( ErrorClient *client )
{
	forWarnings.deregisterClient( client );
	forErrors.deregisterClient( client );
}

// ----------------------------------------------------------------------------

Prefixes &
_World::prefixes()
{
	if ( world_prefixes == nullptr )
	{
		world_prefixes = new Prefixes( shared_from_this());
	}
	return *world_prefixes;
}

// ----------------------------------------------------------------------------

Serializer
_World::defaultSerializer()
{
	if ( defSerializer == nullptr )
	{
		World w = shared_from_this();
		defSerializer = Serializer( new _Serializer(w) );
	}
	return defSerializer;
}

// ----------------------------------------------------------------------------

// static
int 
_World::errorHandler( void *user_data, const char *message, va_list arguments)
{
	// printf the string
	string msg;
	const size_t buffSz = 1024;
	char buffer[buffSz];
	int n = vsnprintf( buffer, buffSz, message, arguments );
	if ( n < 0 )
	{
		throw VX(Error) << "Failed to process error message: " << message;
	}
	else if ( n >= (int)buffSz )
	{
		// need more space
		if ( n > 100000 ) throw VX(Error) << "Failed to process error message";
		char *bigBuffer = new char[n+1];
		n = vsnprintf( bigBuffer, n, message, arguments );
		msg.assign( bigBuffer );
		delete [] bigBuffer;
	}
	else
	{
		msg.assign( buffer );
	}
	
	// pass it the handler
	ErrorHandler *eh = static_cast< ErrorHandler *>( user_data );
	if ( eh )
	{
		eh->processMessage( msg );
	}
	
	// return success flag as per librdf_log_level_func
	return 1;
}

// ----------------------------------------------------------------------------


_World::operator librdf_world*()
{
	return world;
}


// ----------------------------------------------------------------------------
//	ErrorHandler
// ----------------------------------------------------------------------------

void
ErrorHandler::registerClient( ErrorClient *client )
{
	clients.push_back(client);
}

// ----------------------------------------------------------------------------

void
ErrorHandler::deregisterClient( ErrorClient *client )
{
	clients.remove( client );
}

// ----------------------------------------------------------------------------

void
ErrorHandler::processMessage( const std::string & message )
{
	if ( clients.empty() )
	{
		if ( warnings )
		{
			cerr << "RDF Warning: " << message << endl;
		}
		else
		{
			throw VX(Error) << message;
		}
	}
	else
	{
		for( auto c : clients )
		{
			if ( warnings )
			{
				c->handleWarning( message );
			}
			else
			{
				c->handleError( message );
			}
		}
	}

}

// ------------------------------- end --------------------------------------

