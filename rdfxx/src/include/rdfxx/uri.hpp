
/* RDF C++ API 
 *
 * 			uri.hpp
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


#ifndef RDFXX_URI_HPP
#define RDFXX_URI_HPP

#include <iostream>
#include <memory>
#include <librdf.h>
#include <rdfxx/rdfxx.h>

// #include <rdfxx/world.hpp>

namespace rdf
{

// ============================================================================
//! RDF C++ _URI
// ============================================================================

class _URI : public URI_
{
    protected:
    librdf_uri* uri;

    public:
    //! RDF C++ URI constructor.
    /*! Initializes a empty URI object.
     */
    _URI();

    //! RDF C++ URI constructor.
    /*! Initializes a new URI object with a given URI string.
     *  Throws AllocationError if allocation/initialization failed. 
     *
     *  @param uri A URI string.
     */
    _URI(const char* _uri);

    _URI( librdf_uri * );

    //! RDF C++ URI destructor.
    /*! Deletes the internally stored librdf_URI object.
     */
    ~_URI();

	URI copy() const;

    //! Set the URI string.
    /*! Note: This allocates a new librdf_uri object.
     */ 
    void set_string(const char* _uri_string);

    //! Get the URI as string.
    /*! 
     *  @return URI as const char* string.
     */ 
    // const char* get_string();
    std::string toString() const;

    //! Type conversion operator to const char*.
    /*! 
     *  @return URI as const char* string.
     */
    operator const char*() const;

    //! Equality operator.
    /*! Returns true if both URI objects are equal.
     */ 
    bool operator ==(_URI& _uri) const;
    bool operator ==(URI _uri) const;

    // This is used internally for the C API.
    operator librdf_uri*() const;
};


} // namespace rdf
#endif
