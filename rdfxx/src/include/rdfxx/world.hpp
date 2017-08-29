
/* RDF C++ API 
 *
 * 			world.hpp
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


#ifndef RDFXX_WORLD_H
#define RDFXX_WORLD_H

#include <librdf.h>
#include <vector>
#include <list>
#include <string>
#include <memory>
#include <rdfxx/rdfxx.h>

namespace rdf
{

// ---------------------------------------------------------------

// Error handler

class ErrorHandler
{
private:
	std::list< ErrorClient * > clients;  // references - do not delete
	bool warnings;
public:
	ErrorHandler(bool _warnings ) : warnings(_warnings){}
	void registerClient( ErrorClient * );
	void deregisterClient( ErrorClient * );
	void processMessage( const std::string & message );
};

// ---------------------------------------------------------------

//! RDF C++ World.
//
// TODO - limit a world to a single process thread
//
class _World : public World_, public std::enable_shared_from_this< _World >
{
	friend class Universe;
private:
	librdf_world* world;
	std::string world_name;
	Prefixes *world_prefixes;

	// method to handle warnings and errors
	// ASSUMPTION: This works identically to printf.
	static int errorHandler( void *user_data, const char *message, va_list arguments);
	ErrorHandler forErrors;
	ErrorHandler forWarnings;

	Serializer defSerializer;
 
	//! RDF C++ World constructor.
	_World( const std::string &name );

public:
	//! RDF C++ World copy-constructor.
	_World(const _World&) = delete;
	_World& operator = (const _World&) = delete;

	//! RDF C++ World destructor.
	/*! Deletes the internally stored librdf_world object.
	*/
	~_World();

	std::string name() { return world_name; }
	Prefixes &prefixes();

	//! Get the instance pointer of the singleton object.
	/*! Instances new RDF C++ World object if none is associated.
	*/
	// static _World& instance();

	virtual void registerErrorClient( ErrorClient *, bool warnings, bool errors );
	virtual void deregisterErrorClient( ErrorClient * );

	virtual Serializer defaultSerializer();

	// This is used internally for the C API.
	operator librdf_world*();

};
} // namespace rdf
#endif

