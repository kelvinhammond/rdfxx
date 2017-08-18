
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

Prefixes::Prefixes( World _w)
	: world(_w)
{
	librdf_world*w = DEREF( World, librdf_world, _w );
	URI rdf_uri( new _URI( librdf_get_concept_ms_namespace( w )));
	insert( "rdf", rdf_uri );
	URI rdfs_uri( new _URI( librdf_get_concept_schema_namespace(w)));
	insert( "rdfs", rdfs_uri );
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
Prefixes::insert( const std::string &prefix, URI _uri )
{
	uriForPrefix[ prefix ] = _uri;
	prefixForURI[ _uri->toString() ] = prefix;
}

// ----------------------------------------------------------------------------

std::string
Prefixes::find( URI _uri )
{
	// cout << "Prefixes::find" << endl;
	URI u = _uri->trim( World(world));
	// for ( auto &x : prefixForURI )
	// {
		// cout << x.first << " --> " << x.second;
		// if ( u->toString() == x.first ) cout << " found";
		// cout << endl;
	// }
	// cout << "end Prefixes::find \"" << u->toString() << "\"" << endl;
	auto I = prefixForURI.find( u->toString() );
	if ( I == prefixForURI.end() )
	{
		// cout << "Failed to find \"" << u->toString() << "\"" << endl;
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

// ----------------------------------------------------------------------------

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
/*
_World &
//_World::instance()
{
	//
	// Thread safe instance creation.
	// C++ guarantees static initialisation only done once.
	//
	static _World theWorld;

	return theWorld;
}
*/
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
	// cout << "Error handler: " << msg << endl;
	
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

