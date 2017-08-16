
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
		World w( new _World );
		worlds[name] = w;
		return w;
	}
	else
		return wi->second;
}

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

_World::_World()
	: forErrors(false), forWarnings(true)
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

