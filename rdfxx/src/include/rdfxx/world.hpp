
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
	std::vector< ErrorClient * > clients;  // references - do not delete
	bool warnings;
public:
	ErrorHandler(bool _warnings ) : warnings(_warnings){}
	void registerClient( ErrorClient * );
	void processMessage( const std::string & message );
};

// ---------------------------------------------------------------

//! RDF C++ World.
/*!
 *  Singleton class, internally used for librdf statements.
 */
class _World : public World_
{
 private:
    librdf_world* world;

    // method to handle warnings and errors
    // ASSUMPTION: This works identically to printf.
    static int errorHandler( void *user_data, const char *message, va_list arguments);
    ErrorHandler forErrors;
    ErrorHandler forWarnings;
 
 protected:
    //! RDF C++ World constructor.
    /*! Protected due to singleton state.
     */
    _World();

    //! RDF C++ World copy-constructor.
    /*! Protected due to singleton state.
     */
    _World(const _World&) = delete;
    _World& operator = (const _World&) = delete;

 public:
    //! RDF C++ World destructor.
    /*! Deletes the internally stored librdf_world object.
     */
    ~_World();

    //! Get the instance pointer of the singleton object.
    /*! Instances new RDF C++ World object if none is associated.
     */
    static _World& instance();

	void registerErrorClient( ErrorClient *, bool warnings, bool errors );

    // This is used internally for the C API.
    operator librdf_world*();

};
} // namespace rdf
#endif

