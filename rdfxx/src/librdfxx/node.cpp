
/* RDF C++ API 
 *
 * 			node.cpp
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


#include <rdfxx/except.h>
#include <rdfxx/node.hpp>

using namespace rdf;
using namespace std;

// -----------------------------------------------------------------------------
//	Node
// -----------------------------------------------------------------------------

Node::Node()
	: std::shared_ptr< Node_ >( nullptr )
{}

// -----------------------------------------------------------------------------

Node::Node( World w )
	: std::shared_ptr< Node_ >( new _Node( w ) )
{}

// -----------------------------------------------------------------------------

Node::Node( Node_* _node )
	: std::shared_ptr< Node_ >( _node )
{}

// -----------------------------------------------------------------------------

Node::Node( World w, URI _uri )
	: std::shared_ptr< Node_ >( new _Node( w, _uri ) )
{}

// -----------------------------------------------------------------------------

Node::Node( World w, const std::string & uri )
	: std::shared_ptr< Node_ >( new _Node(w, uri) )
{}

// -----------------------------------------------------------------------------

Node::Node(World w, const std::string & literal, bool _is_wf_xml,
                        const std::string & xml_language )
	: std::shared_ptr< Node_ >( new _Node( w, literal.c_str(), _is_wf_xml, 
			xml_language.c_str() ))
{}

// -----------------------------------------------------------------------------

Node::Node( NodeRef _ref )
	: std::shared_ptr< Node_ >( _ref )
{}

// -----------------------------------------------------------------------------
//	_Node
// -----------------------------------------------------------------------------

_Node::_Node( World _w)
	 : world(_w), node(0), free(true)
{
    	// _World& world = _World::instance();
	librdf_world *w = DEREF( World, librdf_world, _w );

    	// Create a node. User controls its lifetime.
    	node = librdf_new_node(w);
    	if(!node)
		throw VX(Error) << "Failed to allocate node";
}

// -----------------------------------------------------------------------------

_Node::_Node( const _Node & _node)
	 : world(_node.world), free(true)
{
    	// Create a new node as a copy. User controls its lifetime.
    	node = librdf_new_node_from_node(_node.node);
    	if(!node)
		throw VX(Error) << "Failed to allocate node";
}

// -----------------------------------------------------------------------------

_Node::_Node(World _w, const string & _uri)
	 : world(_w), node(0), free(true)
{
    	//_World& world = _World::instance();
	librdf_world *w = DEREF( World, librdf_world, _w );

    	// Create a new node. User controls lifetime.
    	node = librdf_new_node_from_uri_string(w, (const unsigned char*) _uri.c_str());
    	if(!node)
		throw VX(Error) << "Failed to allocate node";
}

// -----------------------------------------------------------------------------

_Node::_Node(World _w, const char* _uri)
	 : world(_w), node(0), free(true)
{
    	//_World& world = _World::instance();
	librdf_world *w = DEREF( World, librdf_world, _w );

    	// Create a new node. User controls lifetime.
    	node = librdf_new_node_from_uri_string(w, (const unsigned char*) _uri);
    	if(!node)
		throw VX(Error) << "Failed to allocate node";
}

// -----------------------------------------------------------------------------

_Node::_Node( World _w, URI _uri)
	 : world(_w), node(0), free(true)
{
    	// _World& world = _World::instance();
	librdf_world *w = DEREF( World, librdf_world, _w );
    	librdf_uri *uri = DEREF( URI, librdf_uri, _uri );

    	// Create a new node. User controls lifetime.
    	node = librdf_new_node_from_uri(w, uri);
    	if(!node)
		throw VX(Error) << "Failed to allocate node";
}

// -----------------------------------------------------------------------------

_Node::_Node( World _w, const char* _literal, bool _is_wf_xml, const char* _xml_language)
	 : world(_w), node(0), free(true)
{
    	// _World& world = _World::instance();

	librdf_world *w = DEREF( World, librdf_world, _w );
	int is_wf_xml = (_is_wf_xml) ? 1 : 0;

    	// Create a new node. User controls lifetime.
    	node = librdf_new_node_from_literal(w, (const unsigned char*) _literal, 
    			_xml_language, is_wf_xml);
    	if(!node)
		throw VX(Error) << "Failed to allocate node";
}

// -----------------------------------------------------------------------------

_Node::_Node(World w, librdf_node* _node, bool freeOnDelete)
	 : world(w), node(0), free(freeOnDelete)
{
    if(!_node)
    {
	throw VX(Error) << "Unexpected NULL pointer";
    }
	// We are wrapping a librdf pointer that is a reference into
	// librdf structures.
	// Hence we don't delete it, and should give the user a weark pointer.
    node = _node;
}

// -----------------------------------------------------------------------------

_Node::~_Node()
{
    if(node && free)
        librdf_free_node(node);
}

// -----------------------------------------------------------------------------

Node
_Node::copy() const
{
	return Node( new _Node( *this ));
}

// -----------------------------------------------------------------------------

std::string
_Node::toString() const
{
	string s;
	char *str = NULL;
	size_t len = 2;
	librdf_world *w = DEREF( World, librdf_world, world );
	raptor_world * rw = librdf_world_get_raptor(w);
	if ( rw )
	{
		raptor_iostream *stream = raptor_new_iostream_to_string(rw, (void**)& str, &len, malloc );
		librdf_node_write( node, stream );
		raptor_free_iostream(stream);
	}
	else
	{
		cerr << "Failed to get raptor world" << endl;
	}
	if ( str ) s = str;
	::free(str);
    
    // deprecated
    // nstring = (const char*) librdf_node_to_string(node);

    return s;
}

// -----------------------------------------------------------------------------

URI
_Node::toURI() const
{
	// Get a reference pointer.
	librdf_uri* u = librdf_node_get_uri(node);

	// URI constructor makes a copy so user controls lifetime.
	return URI( new _URI( u ));
}

// -----------------------------------------------------------------------------

bool
_Node::isResource() const
{
	return (librdf_node_is_resource(node) != 0) ? true : false;
}

// -----------------------------------------------------------------------------

bool
_Node::isBlank() const
{
	return (librdf_node_is_blank(node) != 0) ? true : false;
}

// -----------------------------------------------------------------------------

bool
_Node::isLiteral() const
{
	return (librdf_node_is_literal(node) != 0) ? true : false;
}

// -----------------------------------------------------------------------------

_Node::operator librdf_node*() const
{
    return node;
}

// -------------------------------------- end ------------------------------------

