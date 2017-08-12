
/* RDF C++ API 
 *
 * 			storage.hpp
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


#ifndef RDFXX_STORAGE_HPP
#define RDFXX_STORAGE_HPP

#include <iostream>
#include <librdf.h>

#include <rdfxx/world.hpp>
#include <rdfxx/stream.hpp>
#include <rdfxx/statement.hpp>

namespace rdf
{

class _Model;

// ============================================================================
//! RDF C++ _Storage
// ============================================================================

class _Storage : public Storage_
{
    // ------------------------------------------------------------------------
    protected:
    librdf_storage* storage;
    Model model;
 
    // ------------------------------------------------------------------------
    public:
    //! RDF C++ Storage constructor.
    /*! Initializes a new RDF Storage.
     *  There is no Model assigned to the storage by default.
     *  Throws AllocationError exception if allocation/initialization failed.
     *
     *  @param storage_name See Redland documentation for valid storage names.
     *  @param name Freely choosable name for the storage.
     *  @param options See Redland documentation for valid options.
     */
    _Storage(const std::string& _storage_name, const std::string& _name="", 
    			const std::string& _options="");

    //! RDF C++ Storage destructor.
    /*! Tries to close opened stores and deletes the internally stored 
     *  librdf_storage object.
     */
    ~_Storage();

    //! Open a storage/model association.
    /*! 
     *  @param model A RDF C++ Model object.
     *  @return False on failure.
     */ 
    bool open(Model _model);

    //! Close a storage/model association.
    /*!
     *  @return False on failure.
     */ 
    bool close();

    //! Get the number of statements in the storage.
    /*! 
     *  @return A positive integer or 0.
     */ 
    int size();

    //! Add a statement to the storage.
	/*! Expects a statically allocated Statement reference, which is
     *  automatically initialised. Throws a StatementError exception 
     *  if the statement is invalid. 
     * 
     *  @param statement A RDF C++ Statement object reference.
     *  @return False on failure.
     */
    bool add(Statement _statement);

    //! Add a stream of statements to the storage.
    /*!
     *  @param statement_stream A RDF C++ Stream object reference.
     *  @return False on failure.
     */
    bool add(Stream _statement_stream);

    //! Delete a statement from the storage.
    /*!
     *  @param statement A RDF C++ Statement object reference.
     *  @return False on failure.
     */ 
    bool remove(Statement _statement);

    //! Delete a statement from the storage.
    /*!
     *  @param old A RDF C++ Statement object reference.
     *  @param new A RDF C++ Statement object reference.
     *  @return False if old statement doesn't exist.
     */
    bool update(Statement _old, Statement _new);

    //! Check if a given statement is in the storage.
	/*! Throws StatementError exception if the statement is invalid.
     *
     *  @param statement A RDF C++ Statement object reference.
     *  @return False on failure.
     */
    bool contains(Statement _statement);
    
    //! Type conversion operator to librdf_storage*.
	/*  This is used internally for the C API.
     *
     *  @return A RDF C librdf_storage* object.
     */
    operator librdf_storage*();
};
} // namespace rdf
#endif
