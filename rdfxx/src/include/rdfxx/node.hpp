
/* RDF C++ API 
 *
 * 			node.hpp
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


#ifndef RDFXX_NODE_HPP
#define RDFXX_NODE_HPP

#include <new>
#include <iostream>
#include <librdf.h>
#include <rdfxx/uri.hpp>
#include <rdfxx/world.hpp>

namespace rdf
{


// ============================================================================
//! RDF C++ _Node
// ============================================================================

class _Node : public Node_
{
    // ------------------------------------------------------------------------
    protected:
    librdf_node* node;		// owned except when free is false
 
    // Indicate whether to free node in destructor. 
    // Used to prevent freeing shared pointers ie pointers that are references into
    // librdf structures.
    //
    bool free;

    // Hold the dynamically allocated node string, returned by the
    // librdf_node_to_string() function. Used to free the memory 
    // when the object is destructed.
    // BR - replace with a string - may not even be required
    // const char* nstring;

    // ------------------------------------------------------------------------
    public:
    //! RDF C++ Node constructor.
    /*! Initializes a empty Node object.
     *  Throws AllocationError if allocation/initialization failed. 
     */
    _Node();

    //! RDF C++ Node constructor.
    /*! Initializes a new Node object from a given URI string.
     *  Throws AllocationError if allocation/initialization failed. 
     */
    _Node(const std::string & _uri);

    //! RDF C++ Node constructor.
    /*! Initializes a new Node object from a given URI string.
     *  Throws AllocationError if allocation/initialization failed. 
     *
     *  @param uri A URI string.
     */
    _Node(const char* _uri);

    //! RDF C++ Node constructor.
    /*! Initializes a new Node object with a given URI object.
     *  Throws AllocationError if allocation/initialization failed. 
     *
     *  @param uri A URI object.
     */
    _Node(const _URI& _uri);

    //! RDF C++ Node constructor.
    /*! Initializes a new Node object with a given URI.
     *  Throws AllocationError if allocation/initialization failed. 
     *
     *  @param uri A URI string.
     */
    _Node(const char* _literal, bool _is_wf_xml, const char* xml_language=0);

    //! RDF C++ Node copy-constructor.
    /*! Copies a RDF C++ Node object.
     *  Throws AllocationError if allocation/initialization failed. 
     * 
     *  @param node A RDF C++ Node object reference.
     */
    _Node(const _Node& _node);

    //  RDF C++ Node constructor.
    /*  Initializes a new RDF Node from a given librdf_node* pointer.
     *
     *  NOTE:
     *  Intentionally not documented, only used internally in QueryResult Object.
     *  This constructor prevents freeing the stored librdf_node*.
     *
     *  Throws ArgumentError if given a 0-pointer.
     */ 
    _Node(librdf_node* _node, bool freeOnDelete);

    //! RDF C++ Node destructor.
    /*  Deletes the internally stored librdf_node object.
     */
    ~_Node();

    Node copy() const;

    //! Get the node as string.
    /*! 
     *  @return Node as const char* string.
     */
    std::string toString() const;

	URI toURI() const;

	bool isResource() const;
	bool isBlank() const;
	bool isLiteral() const;

    // This is used internally for the C API.
    operator librdf_node*() const;
};

} // namespace rdf
#endif
