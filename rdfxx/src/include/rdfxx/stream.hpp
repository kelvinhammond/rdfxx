
/* RDF C++ API 
 *
 * 			stream.hpp
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


#ifndef RDFXX_STREAM_H
#define RDFXX_STREAM_H

#include <iostream>
#include <librdf.h>

#include <rdfxx/statement.hpp>
#include <rdfxx/uri.hpp>

namespace rdf
{

class _Parser;

//! RDF C++ Stream
class _Stream : public Stream_
{
 private:
    librdf_stream* stream;
    Statement currStatement;
 
 public:
    //! RDF C++ Stream constructor.
    /*! Initializes a empty Stream object.
     *  Throws AllocationError if allocation/initialization failed.
     */
    _Stream();

    //! RDF C++ Stream constructor.
    /*! Initializes a Stream object from a given librdf_stream* object.
     *  Throws AllocationError if allocation/initialization failed.
     *
     *  @param _parser The parser object to use for parsing the URI.
     *  @param _uri The URI to be parsed.
     *  @param _base_uri The base URI to use.
     */
    _Stream(Parser _parser, URI _uri, URI _base_uri);

    //! RDF C++ Stream constructor.
    /*! Initializes a a Stream object from a given librdf_stream* object.
     */
    _Stream(librdf_stream* _stream); 

    //! RDF C++ Stream destructor.
	/*! Deletes the internally stored librdf_stream object.
     */
    ~_Stream();

    //! Indicates the end of statement stream.
    /*! 
     *  @return True if end of stream has been reached. 
     */
    bool end();

    //! Selects next statement in stream.
    /*! 
     *  @return False if no more statements in stream. 
     */
    bool next();

    //! Returns current Statement in stream.
    /*! 
     *  @return RDF C++ Statement object reference.
     */
    StatementRef current();

	// This is used internally for the C API.
    operator librdf_stream*();
};
} // namespace rdf
#endif
