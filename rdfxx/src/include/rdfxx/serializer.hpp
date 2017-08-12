
/* RDF C++ API 
 *
 * 			serializer.hpp
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


#ifndef RDFXX_SERIALIZER_H
#define RDFXX_SERIALIZER_H

#include <iostream>
#include <librdf.h>

#include <rdfxx/world.hpp>
#include <rdfxx/uri.hpp>
#include <rdfxx/model.hpp>
#include <rdfxx/rdfxx.h>

namespace rdf
{

//! RDF C++ Serializer.
class _Serializer : public Serializer_
{
 private:
    librdf_serializer* serializer;
 
 public:
    //! RDF C++ Serializer constructor.
    /*! Initializes a new RDF Serializer.
     *  Throws AllocationError if allocation/initialization failed.
     *
     *  @param _name Factory name. See Redland documentation for details.
     *  @param _syntax_mime MIME type of syntax.
     */
    _Serializer(const std::string& _name="rdfxml", const std::string& _syntax_mime="");

    //! RDF C++ Serializer constructor.
    /*! Initializes a new RDF Serializer.
     *  Throws AllocationError if allocation/initialization failed.
     *
     *  @param _name Factory name. See Redland documentation for details.
     *  @param _syntax_uri URI of syntax.
     */
    _Serializer(const std::string& _name, URI _syntax_uri);

    //! RDF C++ Serializer destructor.
	/*! Deletes the internally stored librdf_serializer object.
     */
    ~_Serializer();

    //! Set the namespace to be used for a URI.
    /*!
     *  @param _uri RDF C++ URI object reference.
     *  @param _prefix Namespace prefix string.
     */
    bool setNamespace(URI _uri, const std::string & _prefix);

    //! Serializer a given model to a file.
    /*! Throws ArgumentError no file name is given.
     *
     *  @param _file Path to a file.
     *  @param _model RDF C++ Model.
     */
    bool toFile(const std::string & _file, Model _model);

    //! Serializer a given model to a file.
    /*! Throws ArgumentError no file name is given.
     *
     *  @param _file Path to a file.
     *  @param _model RDF C++ Model.
     *  @param _base_uri RDF C++ URI - Base URI to use for serialization.
     */
    bool toFile(const std::string & _file, Model _model, URI _base_uri);
};
} // namespace rdf
#endif
