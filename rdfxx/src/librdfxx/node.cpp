
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
std::string
Literal::toTypeName( DataType dt )
{
	if ( names.empty()) initXSDtypes();
	return names[ dt ];
}

// -----------------------------------------------------------------------------

// static
DataType
Literal::toDataType( const std::string & xsd_type )
{
	if ( xsd_types.empty()) initXSDtypes();
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

// static
DataType
Literal::asDataType( const std::string & type_name )
{
	if ( names.empty()) initXSDtypes();
	//
	// converting string to datatype is probably something
	// for the UI. Hence we can afford to use a slower reverse lookup
	//
	for ( auto &x : names )
	{
		if ( x.second == type_name ) return x.first;
	}
	return DataType::UNDEF;
}

// -----------------------------------------------------------------------------

// static
std::vector< std::string >
Literal::getDataTypeNames()
{
	if ( names.empty()) initXSDtypes();
	vector< string > res;
	for ( auto &x : names )
	{
		res.push_back( x.second );
	}
	return res;
}

// -----------------------------------------------------------------------------

std::map< DataType, std::string > Literal::xsd_types;
std::map< DataType, std::string > Literal::names;

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

	names =
	{
        	{ DataType::PlainLiteral, "PlainLiteral" },
        	{ DataType::XMLLiteral, "XMLLiteral" },
        	{ DataType::XHTML, "HTML" },

        	{ DataType::String, "string" },
        	{ DataType::Boolean, "boolean" },
        	{ DataType::Decimal, "decimal" },
        	{ DataType::Integer, "integer" },

        	{ DataType::Double, "double" },
        	{ DataType::Float, "float" },

        	{ DataType::Data, "date" },
        	{ DataType::Time, "time" },
        	{ DataType::DateTime, "date time" },
        	{ DataType::DateTimeStamp, "date time stamp" },

        	{ DataType::Year, "Year" },
        	{ DataType::Month, "Month" },
        	{ DataType::Day, "Day" },
        	{ DataType::YearMonth, "Year Month" },
        	{ DataType::MonthDay, "Month Day" },
        	{ DataType::Duration, "duration" },
        	{ DataType::YearMonthDuration, "year month duration" },
        	{ DataType::DayTimeDuration, "day time duration" },

        	{ DataType::Byte, "byte" },
        	{ DataType::Short, "short" },
        	{ DataType::Int, "int" },
        	{ DataType::Long, "long" },
        	{ DataType::UnsignedByte, "unsigned byte" },
        	{ DataType::UnsignedShort, "unsigned short" },
        	{ DataType::UnsignedLong, "unsigned long" },
        	{ DataType::PositiveInteger, "positive integer" },
        	{ DataType::NonNegativeInteger, "non-negative integer" },
        	{ DataType::NegativeInteger, "negative integer" },
        	{ DataType::NonPositiveInteger, "non-positive integer" },

        	{ DataType::HexBinary, "hex binary" },
        	{ DataType::Base64Binary, "base 64 binary" },

        	{ DataType::AnyURI, "any URI" },
        	{ DataType::Language, "language" },
        	{ DataType::NormalizedString, "normalized string" },
        	{ DataType::Token, "token" },
        	{ DataType::NMTOKEN, "NMTOKEN" },
        	{ DataType::Name, "Name" },
        	{ DataType::NCName, "NCName" }
	};
}

// -----------------------------------------------------------------------------
//	Node
// -----------------------------------------------------------------------------

std::map< Concept, librdf_concepts_index > _ResourceNode::concepts =
	{
        	{ Concept::Container, 		LIBRDF_CONCEPT_S_Container },
		{ Concept::Bag, 		LIBRDF_CONCEPT_MS_Bag },
		{ Concept::Sequence, 		LIBRDF_CONCEPT_MS_Seq },
		{ Concept::Alternative, 	LIBRDF_CONCEPT_MS_Alt },
		{ Concept::aboutEach, 		LIBRDF_CONCEPT_MS_aboutEach },
        	{ Concept::List, 		LIBRDF_CONCEPT_RS_List },
		{ Concept::first, 		LIBRDF_CONCEPT_RS_first },
		{ Concept::rest, 		LIBRDF_CONCEPT_RS_rest },
		{ Concept::nil, 		LIBRDF_CONCEPT_RS_nil },
		{ Concept::Statement, 		LIBRDF_CONCEPT_MS_Statement },
		{ Concept::object, 		LIBRDF_CONCEPT_MS_object },
		{ Concept::predicate, 		LIBRDF_CONCEPT_MS_predicate },
		{ Concept::subject, 		LIBRDF_CONCEPT_MS_subject },
        	{ Concept::Resource, 		LIBRDF_CONCEPT_S_Resource },
		{ Concept::Class, 		LIBRDF_CONCEPT_S_Class },
		{ Concept::subClassOf, 		LIBRDF_CONCEPT_S_subClassOf },
		{ Concept::type, 		LIBRDF_CONCEPT_MS_type },
		{ Concept::Property, 		LIBRDF_CONCEPT_MS_Property },
		{ Concept::subPropertyOf,	LIBRDF_CONCEPT_S_subPropertyOf },
        	{ Concept::domain, 		LIBRDF_CONCEPT_S_domain },
		{ Concept::range, 		LIBRDF_CONCEPT_S_range },
		{ Concept::ConstraintProperty, 	LIBRDF_CONCEPT_S_ConstraintProperty },
		{ Concept::ConstraintResource, 	LIBRDF_CONCEPT_S_ConstraintResource },
        	{ Concept::Description, 	LIBRDF_CONCEPT_MS_Description },
		{ Concept::label, 		LIBRDF_CONCEPT_S_label },
		{ Concept::seeAlso, 		LIBRDF_CONCEPT_S_seeAlso },
		{ Concept::comment, 		LIBRDF_CONCEPT_S_comment },
		{ Concept::isDefinedBy, 	LIBRDF_CONCEPT_S_isDefinedBy }
	};



// -----------------------------------------------------------------------------
//	ResourceNode
// -----------------------------------------------------------------------------

ResourceNode::ResourceNode( World w, URI uri )
	: std::shared_ptr< ResourceNode_ >( new _ResourceNode( w, uri ))
{}

// -----------------------------------------------------------------------------

ResourceNode::ResourceNode( World w, Concept c)
	: std::shared_ptr< ResourceNode_ >( new _ResourceNode( w, c ))
{}

// -----------------------------------------------------------------------------

ResourceNode::ResourceNode( World w, int i)
	: std::shared_ptr< ResourceNode_ >( new _ResourceNode( w, i ))
{}

// -----------------------------------------------------------------------------

ResourceNode::ResourceNode( Node n )
	: std::shared_ptr< ResourceNode_ >( static_pointer_cast< ResourceNode_ >( n ))
{}

// -----------------------------------------------------------------------------
//	LiteralNode
// -----------------------------------------------------------------------------

LiteralNode::LiteralNode( World w, const Literal & L)
	: std::shared_ptr< LiteralNode_ >( new _LiteralNode( w, L ))
{}

// -----------------------------------------------------------------------------

LiteralNode::LiteralNode( Node n )
	: std::shared_ptr< LiteralNode_ >( static_pointer_cast< LiteralNode_ >( n ))
{}

// -----------------------------------------------------------------------------
//	BlankNode
// -----------------------------------------------------------------------------

BlankNode::BlankNode( World w, const std::string &id )
	: std::shared_ptr< BlankNode_ >( new _BlankNode( w, id ))
{}

// -----------------------------------------------------------------------------

BlankNode::BlankNode( Node n )
	: std::shared_ptr< BlankNode_ >( static_pointer_cast< BlankNode_ >( n ))
{}

// -----------------------------------------------------------------------------
//	_NodeBase
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
	librdf_world *w = DEREF( World, librdf_world, _w );
    	librdf_uri *uri = DEREF( URI, librdf_uri, _uri );

    	// Create a new node. User controls lifetime.
    	node = librdf_new_node_from_uri(w, uri);
    	if(!node)
		throw VX(Error) << "Failed to allocate node";
}

// -----------------------------------------------------------------------------

_ResourceNode::_ResourceNode( World _w, Concept concept )
	: _NodeBase( _w, 0, true )
{
	librdf_world *w = DEREF( World, librdf_world, _w );

    	// Create a new node. User controls lifetime.
    	librdf_node * _node = librdf_get_concept_resource_by_index( w, concepts[concept] );
    	node = librdf_new_node_from_node(_node);
    	if(!node)
		throw VX(Error) << "Failed to allocate node";
}

// -----------------------------------------------------------------------------

_ResourceNode::_ResourceNode( World _w, int i )
	: _NodeBase( _w, 0, true )
{
	Prefixes &prefixes = world->prefixes();
	librdf_world *w = DEREF( World, librdf_world, _w );

    	// Create a new node. User controls lifetime.
	string s("rdf:_");
	s += to_string( i );
	URI _uri = prefixes.uriForm(s);
    	librdf_uri *uri = DEREF( URI, librdf_uri, _uri );
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

int
_ResourceNode::listItemOrdinal() const
{
	return librdf_node_get_li_ordinal( node );
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
	// get a reference pointer - don't free it
	char *val = (char *)librdf_node_get_literal_value(node);
	if ( val == NULL )
		return s;
	s.assign( val );
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
			URI uri( new _URI( dturi ));
			s += world->prefixes().prefixForm( uri );
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
	Literal L;
	char *val = (char *)librdf_node_get_literal_value(node);
	if ( val )
	{
		L.value( val );
	}

	char *lang = librdf_node_get_literal_value_language( node );
	if ( lang )
	{
		L.language( lang );
	}

	librdf_uri *dturi = librdf_node_get_literal_value_datatype_uri( node );
	if ( dturi )
	{
		URI uri( new _URI( dturi ));
		string s( world->prefixes().prefixForm( uri ));
		L.dataType( Literal::toDataType( s ));
	}
	else
	{
		L.dataType( DataType::PlainLiteral );
	}
	
	return L;
}

// -----------------------------------------------------------------------------
//	_BlankNode
// -----------------------------------------------------------------------------

_BlankNode::_BlankNode( World _w, const std::string &id )
	: _NodeBase( _w, 0, true )
{
	librdf_world *w = DEREF( World, librdf_world, _w );

    	// Create a new node. User controls lifetime.
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

// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------

// -------------------------------------- end ------------------------------------

