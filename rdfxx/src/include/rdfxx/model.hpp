/* RDF C++ API 
 *
 * 			model.hpp
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

#ifndef RDFXX_MODEL_HPP
#define RDFXX_MODEL_HPP

#include <iostream>
#include <librdf.h>

#include <rdfxx/rdfxx.h>
#include <rdfxx/stream.hpp>
#include <rdfxx/statement.hpp>

namespace rdf
{

// =============================================================================
//! RDF C++ Model.
// =============================================================================

class _Model : public Model_
{
    // -------------------------------------------------------------------------
    protected:
    World world;
    librdf_model* model;	// owned
    librdf_storage *storage;	// owned

	void nodeIteratorToVector( librdf_iterator *, std::vector< Node > & );
	void savePrefixes();	// ensure all prefixes are recorded as statements in the model
 
    // -------------------------------------------------------------------------
    public:

    //! RDF C++ Model constructor.
    /*! Initializes a new RDF Graph/Model.
     *  Throws AllocationError if allocation/initialization failed.
     *
     *  @param storage RDF C++ Storage object reference.
     *  @param options See Redland documentation for valid options.
     */
    //_Model(Storage _storage, const std::string & _options="");
    _Model( World, const std::string & _storage_type, const std::string & _storage_name = "",
    		const std::string & _storage_options = "", const std::string & _model_options = "" );

    //! RDF C++ Model copy constructor.
    /*! Initializes a new RDF Graph/Model from a existing one.
     *  Also copies the librdf_model object.
     */
    _Model( const _Model & _model) = delete;

    //! RDF C++ Model destructor.
    /*! Deletes the internally stored 
     *  librdf_model object.
     */
    ~_Model();

    // Disallow assignment operator
    _Model & operator = (const _Model &) = delete;

	World getWorld() { return world; }

    //! Get the number of statements int the model. 
    /*! 
     *  @return A positive integer or 0.
     */
    int size() const;

    //! Synchronize the model with its associated storage.
    /*!
     *  @return True if synchronization was successful.
     */
    bool sync();
	
	
	void updatePrefixes();	// update Prefixes from statements in the model

    //! Serialise the model to a Stream.
    /*!
     *  @return A RDF C++ Stream object.
     */
    Stream toStream();
   
    //! Add a new statement to the model.
    /*!
     *  @param subject RDF C++ Node object reference.
     *  @param predicate RDF C++ Node object reference.
     *  @param object RDF C++ Node object reference.
     *  @return False on failure.
     */
    bool add(Node subject, Node predicate, Node object);

    //! Add a statement to the storage.
    /*! Expects a statically allocated Statement object, which is 
     *  automatically initialised.
     * 
     *  @param statement A RDF C++ Statement object reference.
     *  @return False on failure.
     */
    bool add(Statement statement);

    //! Add a statement* to the storage.
    /*! Expects a pointer to a dynamically allocated object.
     * 
     *  @param statement A RDF C++ Statement object pointer.
     *  @return False on failure.
     */
    // bool add_statement(Statement* _statement);

    //! Delete a RDF C++ Statement from the model.
    /*! 
     *  @param statement RDF C++ Statement object reference.
     *  @return False on failure.
     */
    // bool delete_statement(Statement& _statement);
    bool remove( Statement );

    //! Updates a existing RDF C++ Statement.
    /*!
     *  @param old RDF C++ Statement object reference.
     *  @param new RDF C++ Statement object reference.
     *  @return False on failure.
     */
    bool update(Statement _old, Statement _new);

    //! Check if a given statement is in the storage. 
    /*!
     *  @param statement A RDF C++ Statement object reference.
     *  @return False on failure.
     */
    bool contains(Statement statement) const;

	virtual std::vector< Node > predicates( Node subject, Node object );
	virtual std::vector< Node > objects( Node subject, Node predicate );
	virtual std::vector< Node > subjects( Node predicate, Node object );

	virtual std::vector< Node > arcsIn( Node object );
	virtual std::vector< Node > arcsOut( Node subject );
 
    // This is used internally for the C API.
    operator librdf_model*() const;
};

} // namespace rdf
#endif
