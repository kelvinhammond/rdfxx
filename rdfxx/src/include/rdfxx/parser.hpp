
/* RDF C++ API 
 *
 * 			parser.hpp
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


#ifndef RDFXX_PARSER_H
#define RDFXX_PARSER_H

#include <iostream>
#include <librdf.h>

#include <rdfxx/uri.hpp>
#include <rdfxx/stream.hpp>
#include <rdfxx/rdfxx.h>

namespace rdf
{

//! RDF C++ Parser.
class _Parser : public Parser_
{
 private:
    librdf_parser* parser;
 
 public:
    //! RDF C++ Parser constructor.
    /*! Initializes a new RDF Parser.
     *  Throws AllocationError if allocation/initialization failed.
     *
     *  @param _name Parsing engine name. See Redland documentation for details.
     *  @param _syntax_mime MIME type of syntax, defaults to RDF/XML.
     */
    _Parser(const std::string & _name, const std::string & _syntax_mime="");

    //! RDF C++ Parser constructor.
    /*! Initializes a new RDF Parser.
     *  Throws AllocationError if allocation/initialization failed.
     *
     *  @param _name Parsing engine name. See Redland documentation for details.
     *  @param _syntax_uri URI to be parsed, can be ommitted.
     */
    _Parser(const std::string & _name, URI _syntax_uri);

	bool parseIntoModel(Model model, URI uri, URI base_uri);

    //! RDF C++ Statement destructor.
	/*! Deletes the internally stored librdf_parser object.
     */
    ~_Parser();

    // This is used internally for the C API.
    operator librdf_parser*();
};
} // namespace rdf
#endif
