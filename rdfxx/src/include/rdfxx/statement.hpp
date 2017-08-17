
/* RDF C++ API 
 *
 * 			statement.hpp
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


#ifndef RDFXX_STATEMENT_HPP
#define RDFXX_STATEMENT_HPP

#include <iostream>
#include <librdf.h>

#include <rdfxx/world.hpp>
#include <rdfxx/node.hpp>

namespace rdf
{

// ============================================================================
//! RDF C++ _Statement
// ============================================================================

class _Statement : public Statement_
{
 private:
    World world;
    librdf_statement* statement;
    
    // keep shared pointers so that returned weak pointer remains valid
    Node subject_holder;
    Node predicate_holder;
    Node object_holder;

    // control clean up
    bool free;
    
 public:
    // Create new empty statement
    _Statement( World );

    //! RDF C++ Statement constructor. Initializes a new RDF Statement.
    /*! Throws AllocationError if allocation/initialization failed.
     *
     *  @param subject RDF C++ Node object reference.
     *  @param predicate RDF C++ Node object reference.
     *  @param object RDF C++ Node object reference.
     */
    _Statement(World, Node subject, Node predicate, Node object);

    //! RDF C++ Statement copy-constructor.
    /*! Makes a completecopy of a RDF C++ Statement object.
     * 
     *  @param statement A RDF C++ Statement object.
     */
    _Statement( Statement statement);

    //  RDF C++ Statement constructor.
    /*  Initializes a new RDF Statement from a given librdf_statement* pointer.
     *
     *  NOTE:
     *  Intentionally not documentet, only used internally in Stream Object.
     *  XX This constructor prevents freeing the stored librdf_statement*. 
     *  It now makes a copy, so the statement can be freed as normal
     *
     *  Throws ArgumentError if given a 0-pointer.
     */ 
    _Statement(World, librdf_statement* _statement, bool freeOnDelete );

    //! RDF C++ Statement destructor.
    /*! Deletes the internally stored librdf_statement object.
     */
    ~_Statement(); 

	Statement copy() const;

    //! Get the subject Node of the Statement object.
    /*!
     *  @return A RDF C++ Node object.
     */ 
    NodeRef subject();

    //! Set the subject Node of the Statement object.
    /*!
     *  @param _node A RDF C++ Node object reference.
     */ 
    void subject(Node node);

    //! Get the predicate Node of the Statement.
    /*!
     *  @return A RDF C++ Node object.
     */ 
    NodeRef predicate();

    //! Set the predicate Node of the Statement object.
    /*!
     *  @param _node A RDF C++ Node object reference.
     */
    void predicate(Node _node);

    //! Get the object Node of the Statement object.
    /*!
     *  @return A RDF C++ Node object.
     */ 
    NodeRef object();

    //! Set the predicate Node of the Statement object.
    /*!
     *  @param _node A RDF C++ Node object reference.
     */
    void object(Node node);

    //! Indicates if the current statement has a valid subject, 
    //! predicate and object.
    /*! 
     *  @return True if statement is complete.
     */ 
    bool isComplete() const;

    //! Check if two Statements match.
    /*! Match a Statement agains a 'partial' Statement. A partial
     *  Statement can contain empty parts (subject, predicate, object).
     *  Empty parts match against any value, parts with values must match 
     *  exactly.
     *
     *  @return True if statements match.
     */
    bool match(Statement statement) const;

    //! Empty a RDF C++ Statements of nodes.
    /*! After calling this method, all the Statements' nodes
     *  are freed. Note that this invalidates the Statement. 
     */ 
    void clear();

    //! Get the statement as string.
    /*! 
     *  @return Statement as const char* string.
     */ 
    // const char* get_string();
    std::string toString() const;

    //! Equality operator.
    /*!
     *  @return True if statements are equal.
     */
    bool operator ==( Statement  _statement) const;

    // This is used internally for the C API.
    operator librdf_statement*() const;
};
} // namespace rdf
#endif
