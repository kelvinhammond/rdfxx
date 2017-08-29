
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
#if USE_NODE
class _Node : public Node_
{
    // ------------------------------------------------------------------------
    protected:
    World world;
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
    _Node( World );

    //! RDF C++ Node constructor.
    /*! Initializes a new Node object from a given URI string.
     *  Throws AllocationError if allocation/initialization failed. 
     */
    _Node( World, const std::string & _uri);

    //! RDF C++ Node constructor.
    /*! Initializes a new Node object from a given URI string.
     *  Throws AllocationError if allocation/initialization failed. 
     *
     *  @param uri A URI string.
     */
    _Node( World, const char* _uri);

    //! RDF C++ Node constructor.
    /*! Initializes a new Node object with a given URI object.
     *  Throws AllocationError if allocation/initialization failed. 
     *
     *  @param uri A URI object.
     */
    _Node( World,  URI _uri);

    _Node( World, const Literal & );

    //! RDF C++ Node constructor.
    /*! Initializes a new Node object with a given URI.
     *  Throws AllocationError if allocation/initialization failed. 
     *
     *  @param uri A URI string.
     */
    _Node( World, const char* _literal, bool _is_wf_xml, const char* xml_language=0);

    //! RDF C++ Node copy-constructor.
    /*! Copies a RDF C++ Node object.
     *  Throws AllocationError if allocation/initialization failed. 
     * 
     *  @param node A RDF C++ Node object reference.
     */
    _Node( const _Node& _node);

    //  RDF C++ Node constructor.
    /*  Initializes a new RDF Node from a given librdf_node* pointer.
     *
     *  NOTE:
     *  Intentionally not documented, only used internally in QueryResult Object.
     *  This constructor prevents freeing the stored librdf_node*.
     *
     *  Throws ArgumentError if given a 0-pointer.
     */ 
    _Node(World, librdf_node* _node, bool freeOnDelete);

    //! RDF C++ Node destructor.
    /*  Deletes the internally stored librdf_node object.
     */
    ~_Node();

    // Node copy() const;

    //! Get the node as string.
    /*! 
     *  @return Node as const char* string.
     */
    std::string toString() const;
    std::string toString( const Format & ) const;

	URI toURI() const;

	bool isResource() const;
	bool isBlank() const;
	bool isLiteral() const;

    // This is used internally for the C API.
    operator librdf_node*() const;
};

#else	// USE_NODE
// ------------------------------------------------------------------------

class _NodeBase
{
protected:
	World world;
	librdf_node* node;		// owned except when free is false
	bool free;

	_NodeBase( World w, librdf_node *n, bool f)
		: world(w), node(n), free(f) {}
public:
	// Node copy() const;
	std::string toString() const;

	// This is used internally for the C API.
	operator librdf_node*() const;

	static librdf_node* derefNode( Node );

	// construct _ResourceNode, _LiteralNode, or _BlankNode
	// from librdf_node
	static Node make( World, librdf_node*, bool freeOnDelete );
};

using _Node = _NodeBase;

// ------------------------------------------------------------------------

class _ResourceNode : public ResourceNode_, public _NodeBase
{
    	static std::map< Concept, librdf_concepts_index > concepts;
public:
	_ResourceNode( World, URI );
    	_ResourceNode( World, Concept );

	_ResourceNode( World w, librdf_node *n, bool f) : _NodeBase( w, n, f) {}

	// Node copy() const { return _NodeBase::copy(); }

	virtual std::string toString() const { return _NodeBase::toString(); }
	virtual std::string toString(const Format &) const;
	virtual URI toURI() const;

	// this will be removed when _Node is removed.
	virtual Literal toLiteral() const;

	virtual bool isLiteral() const { return false; }
	virtual bool isBlank() const { return false; }
	virtual bool isResource() const { return true; }
};

// ------------------------------------------------------------------------

class _LiteralNode : public LiteralNode_, public _NodeBase
{
public:
	_LiteralNode( World, const Literal & );
	_LiteralNode( World w, librdf_node *n, bool f) : _NodeBase( w, n, f) {}

	// Node copy() const { return _NodeBase::copy(); }
	
	virtual std::string toString() const { return _NodeBase::toString(); }
	virtual std::string toString(const Format &) const;

	// this will be removed when _Node is removed.
	virtual URI toURI() const;
	virtual Literal toLiteral() const;

	virtual bool isLiteral() const { return true; }
	virtual bool isBlank() const { return false; }
	virtual bool isResource() const { return false; }
};

// ------------------------------------------------------------------------

class _BlankNode : public BlankNode_, public _NodeBase
{
public:
	_BlankNode( World, const std::string &id = "" );
	_BlankNode( World w, librdf_node *n, bool f) : _NodeBase( w, n, f) {}

	// Node copy() const { return _NodeBase::copy(); }

	virtual std::string toString() const { return _NodeBase::toString(); }
	virtual std::string toString(const Format &) const;

	// these will be removed when _Node is removed.
	virtual URI toURI() const;
	virtual Literal toLiteral() const;

	virtual bool isLiteral() const { return false; }
	virtual bool isBlank() const { return true; }
	virtual bool isResource() const { return false; }
};

#endif
// ------------------------------------------------------------------------


} // namespace rdf
#endif
