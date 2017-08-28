
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
//	Literal
// -----------------------------------------------------------------------------

Literal::Literal()
	: mDataType( DataType::UNDEF )
{}

// -----------------------------------------------------------------------------

Literal::Literal( const std::string &val )
	: mLanguage("en"), mDataType( DataType::PlainLiteral ),
	  mValue(val)
{}

// -----------------------------------------------------------------------------

Literal::Literal( const std::string &val, const std::string &lang )
	: mLanguage(lang), mDataType( DataType::PlainLiteral ),
	  mValue(val)
{}

// -----------------------------------------------------------------------------

Literal::Literal( const std::string &val, DataType dt, const std::string &lang )
	: mLanguage(lang), mDataType( dt ),
	  mValue(val)
{}

// -----------------------------------------------------------------------------

Literal::Literal( int x, DataType dt )
	: mDataType(dt)
{
	mValue = std::to_string( x );
}

// -----------------------------------------------------------------------------

Literal::Literal( double x, DataType dt )
	: mDataType(dt)
{
	mValue = std::to_string(x);
	string::size_type p = mValue.find('.');
	if ( p != string::npos )
	{
		p = mValue.find_last_not_of("0");
		if ( p != string::npos )
		{
			mValue = mValue.substr(0,p+1);
		}
	}
}

// -----------------------------------------------------------------------------

Literal::Literal( bool x)
	: mDataType(DataType::Boolean)
{
	mValue = (x ? "true" : "false");
}

// -----------------------------------------------------------------------------

std::string
Literal::toString() const
{
	Format format = { false, false, "", true, true, "en", true };
	return toString( format );
}

// -----------------------------------------------------------------------------

std::string
Literal::toString( const Format &format ) const
{
	string s( mValue );
	if ( format.quotes )
	{
		s = "\"" + s + "\"";
	}
	if ( mDataType == DataType::PlainLiteral
		&& format.showLanguage )
	{
		s += "@" + mLanguage;
	}
	if ( mDataType != DataType::PlainLiteral )
	{
		s += "^^" + toXSD( mDataType );
	}
	return s;
}

// -----------------------------------------------------------------------------

URI
Literal::dataTypeURI( World w ) const
{
	if ( mDataType == DataType::UNDEF )
		throw VX(Code) << "Literal not initialised";
	string s = toXSD( mDataType );
	string::size_type p = s.find(':');
	URI s2(w, s.substr(0,p+1));
	URI base_uri = w->prefixes().find( s.substr(0,p ));
	return URI( s, s2, base_uri );
}

// -----------------------------------------------------------------------------

int
Literal::asInteger() const
{
	return std::stoi( mValue );
}

// -----------------------------------------------------------------------------

double
Literal::asDouble() const
{
	return std::stod( mValue );
}

// -----------------------------------------------------------------------------

bool
Literal::asBoolean() const
{
	return ( mValue == "true" );
}

// -----------------------------------------------------------------------------

// static
std::string
Literal::toXSD( DataType dt )
{
	if ( xsd_types.empty()) initXSDtypes();
	return xsd_types[ dt ];
}

// -----------------------------------------------------------------------------

// static
DataType
Literal::toDataType( const std::string & xsd_type )
{
	//
	// converting string to datatype is probably something
	// for the UI. Hence we can afford to use a slower reverse lookup
	//
	for ( auto &x : xsd_types )
	{
		if ( x.second == xsd_type ) return x.first;
	}
	return DataType::UNDEF;
}

// -----------------------------------------------------------------------------

std::map< DataType, std::string > Literal::xsd_types;

// static
void
Literal::initXSDtypes()
{
	xsd_types =
	{
        	{ DataType::PlainLiteral, "rdf:PlainLiteral" },
        	{ DataType::XMLLiteral, "rdf:XMLLiteral" },
        	{ DataType::XHTML, "rdf:HTML" },

        	{ DataType::String, "xsd:string" },
        	{ DataType::Boolean, "xsd:boolean" },
        	{ DataType::Decimal, "xsd:decimal" },
        	{ DataType::Integer, "xsd:integer" },

        	{ DataType::Double, "xsd:double" },
        	{ DataType::Float, "xsd:float" },

        	{ DataType::Data, "xsd:date" },
        	{ DataType::Time, "xsd:time" },
        	{ DataType::DateTime, "xsd:dateTime" },
        	{ DataType::DateTimeStamp, "xsd:dateTimeStamp" },

        	{ DataType::Year, "xsd:gYear" },
        	{ DataType::Month, "xsd:gMonth" },
        	{ DataType::Day, "xsd:gDay" },
        	{ DataType::YearMonth, "xsd:gYearMonth" },
        	{ DataType::MonthDay, "xsd:gMonthDay" },
        	{ DataType::Duration, "xsd:duration" },
        	{ DataType::YearMonthDuration, "xsd:yearMonthDuration" },
        	{ DataType::DayTimeDuration, "xsd:dayTimeDuration" },

        	{ DataType::Byte, "xsd:byte" },
        	{ DataType::Short, "xsd:short" },
        	{ DataType::Int, "xsd:int" },
        	{ DataType::Long, "xsd:long" },
        	{ DataType::UnsignedByte, "xsd:unsignedByte" },
        	{ DataType::UnsignedShort, "xsd:unsignedShort" },
        	{ DataType::UnsignedLong, "xsd:unsignedLong" },
        	{ DataType::PositiveInteger, "xsd:positiveInteger" },
        	{ DataType::NonNegativeInteger, "xsd:nonNegativeInteger" },
        	{ DataType::NegativeInteger, "xsd:negativeInteger" },
        	{ DataType::NonPositiveInteger, "xsd:nonPositiveInteger" },

        	{ DataType::HexBinary, "xsd:hexBinary" },
        	{ DataType::Base64Binary, "xsd:base64Binary" },

        	{ DataType::AnyURI, "xsd:anyURI" },
        	{ DataType::Language, "xsd:language" },
        	{ DataType::NormalizedString, "xsd:normalizedString" },
        	{ DataType::Token, "xsd:token" },
        	{ DataType::NMTOKEN, "xsd:NMTOKEN" },
        	{ DataType::Name, "xsd:Name" },
        	{ DataType::NCName, "xsd:NCName" }
	};
}

// -----------------------------------------------------------------------------
//	Node
// -----------------------------------------------------------------------------
#if USE_NODE
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

Node::Node( World w, const Literal &L )
	: std::shared_ptr< Node_ >( new _Node( w, L ))
{}

// -----------------------------------------------------------------------------

/*
Node::Node( World w, const std::string & uri )
	: std::shared_ptr< Node_ >( new _Node(w, uri) )
{}
*/

// -----------------------------------------------------------------------------
/*
Node::Node(World w, const std::string & literal, bool _is_wf_xml,
                        const std::string & xml_language )
	: std::shared_ptr< Node_ >( new _Node( w, literal.c_str(), _is_wf_xml, 
			xml_language.c_str() ))
{}
*/
// -----------------------------------------------------------------------------

Node::Node( NodeRef _ref )
	: std::shared_ptr< Node_ >( _ref )
{}

#else	// USE_NODE

// -----------------------------------------------------------------------------
//	ResourceNode
// -----------------------------------------------------------------------------

ResourceNode::ResourceNode( World w, URI uri )
	: std::shared_ptr< ResourceNode_ >( new _ResourceNode( w, uri ))
{}

// -----------------------------------------------------------------------------
//	LiteralNode
// -----------------------------------------------------------------------------

LiteralNode::LiteralNode( World w, const Literal & L)
	: std::shared_ptr< LiteralNode_ >( new _LiteralNode( w, L ))
{}

// -----------------------------------------------------------------------------
//	BlankNode
// -----------------------------------------------------------------------------

BlankNode::BlankNode( World w, const std::string &id )
	: std::shared_ptr< BlankNode_ >( new _BlankNode( w, id ))
{}

#endif	// USE_NODE

#if USE_NODE
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

_Node::_Node( World _w, const Literal & L )
	: world(_w), node(0), free(true)
{
	librdf_world *w = DEREF( World, librdf_world, _w );

	if ( L.dataType() == DataType::PlainLiteral )
	{
    		node = librdf_new_node_from_literal(w, (const unsigned char*) L.asString().c_str(),
    			L.language().c_str(), 0);
    		if(!node)
			throw VX(Error) << "Failed to allocate node";
	}
	else if ( L.dataType() == DataType::XMLLiteral
			|| L.dataType() == DataType::XHTML )
	{
    		node = librdf_new_node_from_literal(w, (const unsigned char*) L.asString().c_str(),
    			"", 1);
    		if(!node)
			throw VX(Error) << "Failed to allocate node";
	}
	else
	{
		librdf_uri *type_uri = DEREF( URI, librdf_uri, L.dataTypeURI(_w));

		node = librdf_new_node_from_typed_literal( w, (const unsigned char*) L.asString().c_str(),
				"", type_uri );
    		if(!node)
			throw VX(Error) << "Failed to allocate node";
	}

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
/*
Node
_Node::copy() const
{
	return Node( new _Node( *this ));
}
*/
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

std::string
_Node::toString( const Format & format ) const
{
	string s;
	Prefixes &prefixes = world->prefixes();
	if ( librdf_node_is_resource(node) )
	{
		URI uri = toURI();
		if ( format.usePrefixes )
		{
			s = prefixes.prefixForm( uri );
		}
		if ( s.empty())
		{
			if ( format.angleBrackets )
			{
				s += "<";
				s += uri->toString();
				s += ">";
			}
			else
			{
				s += uri->toString();
			}

		}
	}
	else if ( librdf_node_is_literal(node))
	{
		s.assign((char *) librdf_node_get_literal_value(node));
		if ( format.quotes )
		{
			s = "\"" + s + "\"";
		}
		if ( format.showLanguage )
		{
			char *lang = librdf_node_get_literal_value_language( node );
			if ( lang )
			{
				s += "@";
				s += lang;
			}
		}
		if ( format.showDataType )
		{
			librdf_uri *dturi = librdf_node_get_literal_value_datatype_uri( node );
			if ( dturi )
			{
				s += "^^";
			}
		}
	}
	if ( s.empty() ) s = toString();
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
	return (librdf_node_is_resource(node) != 0);
}

// -----------------------------------------------------------------------------

bool
_Node::isBlank() const
{
	return (librdf_node_is_blank(node) != 0);
}

// -----------------------------------------------------------------------------

bool
_Node::isLiteral() const
{
	return (librdf_node_is_literal(node) != 0);
}

// -----------------------------------------------------------------------------

_Node::operator librdf_node*() const
{
    return node;
}
#else

// -----------------------------------------------------------------------------
//	_NodeBase
// -----------------------------------------------------------------------------
/*
Node
_NodeBase::copy() const
{
	return Node( new _NodeBase( *this ));
}
*/
// -----------------------------------------------------------------------------

// static
librdf_node *
_NodeBase::derefNode( Node a )
{
	librdf_node *t = nullptr;
	_NodeBase *c = nullptr;
	if ( a->isResource() )
		c = static_cast< _ResourceNode * >( a.get());
	else if ( a->isLiteral())
		c = static_cast< _LiteralNode * >( a.get());
	else
		c = static_cast< _BlankNode * >( a.get());
	if ( c ) t = *c;
	return t;
}

// -----------------------------------------------------------------------------

_NodeBase::operator librdf_node*() const
{
    return node;
}

// -----------------------------------------------------------------------------

// static
Node
_NodeBase::make( World world, librdf_node *xn, bool freeOnDelete )
{
	Node n;
	if ( librdf_node_is_resource(xn))
	{
		n = Node( new _ResourceNode( world, xn, freeOnDelete ));
	}
	else if ( librdf_node_is_literal(xn))
	{
		n = Node( new _LiteralNode( world, xn, freeOnDelete ));
	}
	else if ( librdf_node_is_blank(xn))
	{
		n = Node( new _BlankNode( world, xn, freeOnDelete ));
	}
	else
		throw VX(Code) << "Unknown node type";
	return n;
}

// -----------------------------------------------------------------------------

std::string
_NodeBase::toString() const
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
//	_ResourceNode
// -----------------------------------------------------------------------------

_ResourceNode::_ResourceNode( World _w, URI _uri )
	: _NodeBase( _w, 0, true )
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

std::string
_ResourceNode::toString(const Format &format) const
{
	string s;
	Prefixes &prefixes = world->prefixes();
	if ( librdf_node_is_resource(node) )
	{
		URI uri = toURI();
		if ( format.usePrefixes )
		{
			/*
			if ( prefixes.isBase( uri ))
			{
				s = prefixes.removeBase( uri );
			}
			else
			{
				string prefix = prefixes.find( uri );
				if (! prefix.empty())
				{
					URI src = prefixes.find( prefix );
					prefix.append(":");
					URI res( uri->toString(), src, URI(world, prefix));
					s = res->toString();
				}
			}
			*/
			s = prefixes.prefixForm( uri );
		}
		if ( s.empty())
		{
			if ( format.angleBrackets )
			{
				s += "<";
				s += uri->toString();
				s += ">";
			}
			else
			{
				s += uri->toString();
			}

		}
	}
	if ( s.empty() )
		s = _NodeBase::toString();
	return s;
}

// -----------------------------------------------------------------------------

URI
_ResourceNode::toURI() const
{
	// Get a reference pointer.
	librdf_uri* u = librdf_node_get_uri(node);

	// URI constructor makes a copy so user controls lifetime.
	return URI( new _URI( u ));
}

// -----------------------------------------------------------------------------

Literal
_ResourceNode::toLiteral() const
{
	// TODO - remove this function
	throw VX(Code) << "not implemented";
}

// -----------------------------------------------------------------------------
//	_LiteralNode
// -----------------------------------------------------------------------------

_LiteralNode::_LiteralNode( World _w, const Literal & L )
	: _NodeBase( _w, 0, true )
{
	librdf_world *w = DEREF( World, librdf_world, _w );

	if ( L.dataType() == DataType::PlainLiteral )
	{
    		node = librdf_new_node_from_literal(w, (const unsigned char*) L.asString().c_str(),
    			L.language().c_str(), 0);
    		if(!node)
			throw VX(Error) << "Failed to allocate node";
	}
	else if ( L.dataType() == DataType::XMLLiteral
			|| L.dataType() == DataType::XHTML )
	{
    		node = librdf_new_node_from_literal(w, (const unsigned char*) L.asString().c_str(),
    			"", 1);
    		if(!node)
			throw VX(Error) << "Failed to allocate node";
	}
	else
	{
		librdf_uri *type_uri = DEREF( URI, librdf_uri, L.dataTypeURI(_w));

		node = librdf_new_node_from_typed_literal( w, (const unsigned char*) L.asString().c_str(),
				"", type_uri );
    		if(!node)
			throw VX(Error) << "Failed to allocate node";
	}
}

// -----------------------------------------------------------------------------

std::string
_LiteralNode::toString(const Format & format) const
{
	string s;
	s.assign((char *) librdf_node_get_literal_value(node));
	if ( format.quotes )
	{
		s = "\"" + s + "\"";
	}
	if ( format.showLanguage )
	{
		char *lang = librdf_node_get_literal_value_language( node );
		if ( lang )
		{
			s += "@";
			s += lang;
		}
	}
	if ( format.showDataType )
	{
		librdf_uri *dturi = librdf_node_get_literal_value_datatype_uri( node );
		if ( dturi )
		{
			s += "^^";
		}
	}
	if ( s.empty() ) s = _NodeBase::toString();
	return s;
}

// -----------------------------------------------------------------------------

URI
_LiteralNode:: toURI() const
{
	throw VX(Code) << "wrong type of node";
}

// -----------------------------------------------------------------------------

Literal
_LiteralNode::toLiteral() const
{
	throw VX(Code) << "not implemented";
}

// -----------------------------------------------------------------------------
//	_BlankNode
// -----------------------------------------------------------------------------

_BlankNode::_BlankNode( World _w, const std::string &id )
	: _NodeBase( _w, 0, true )
{
    	// _World& world = _World::instance();
	librdf_world *w = DEREF( World, librdf_world, _w );

    	// Create a new node. User controls lifetime.
	// TODO - specify blank identifier
    	node = librdf_new_node_from_blank_identifier(w, (const unsigned char*)id.c_str() );
    	if(!node)
		throw VX(Error) << "Failed to allocate node";
}

// -----------------------------------------------------------------------------

std::string
_BlankNode::toString(const Format &) const
{
	return _NodeBase::toString();
}

// -----------------------------------------------------------------------------

Literal
_BlankNode::toLiteral() const
{
	throw VX(Code) << "not implemented";
}

// -----------------------------------------------------------------------------

URI
_BlankNode:: toURI() const
{
	throw VX(Code) << "wrong type of node";
}
#endif	// USE_NODE

// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------

// -------------------------------------- end ------------------------------------

