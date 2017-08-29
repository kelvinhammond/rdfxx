
/* RDF C++ API 
 *
 * 			rdfxx.h
 *
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


#ifndef RDFXX_H
#define RDFXX_H

#include <memory>
#include <string>
#include <map>

#include <rdfxx/except.h>

// remove once Node no longer required
#define USE_NODE 0

namespace rdf
{

//
// NOTE: Class names use the following convention:
//	Name_  is an abstract class
//	_Name  is its implementation  (private to the implementation)
//	Name   is a shared pointer to a child of Name_ or a simple class.
//	NameRef is a weak pointer to a child of Name_
//

// Forward declarations of abstract base classes
class World_;
class Model_;
class Node_;
#if ! USE_NODE
class ResourceNode_;
class LiteralNode_;
class BlankNode_;
#endif // USE_NODE
class Parser_;
class Query_;
class QueryResults_;
class Serializer_;
class Statement_;
class Stream_;
class URI_;

class QueryString;
class Literal;

//
// Shared pointers for use by the client applications
//
using World = std::shared_ptr< World_ >;
using WorldRef = std::weak_ptr< World_ >;
using NodeRef = std::weak_ptr< Node_ >;
using QueryResults = std::shared_ptr< QueryResults_ >;
using StatementRef = std::shared_ptr< Statement_ >;

// ---------------------------------------------------------------

enum class Concept
{
	// Containers
	Container, Bag, Sequence, Alternative, aboutEach, 

	// Collections
	List, first, rest, nil, 

	// Reification
	Statement, object, predicate, subject, 
	
	// Classes
	Resource, Class, subClassOf, type, Property, subPropertyOf,
	domain, range, ConstraintProperty, ConstraintResource, 

	// Descriptive stuff
	Description, label, seeAlso, comment, isDefinedBy

	// probably not useful in our models
	// RDF, value, Literal, XMLLiteral, NodeID, li, 
	// aboutEachPrefix, // depricated
};

// ---------------------------------------------------------------
//
// The following declarations allow the user to create an object
// and its associated shared pointer as if it were an ordinary object
//
// ---------------------------------------------------------------

class URI : public std::shared_ptr< URI_ >
{
public:
	URI() : std::shared_ptr< URI_ >( nullptr ) {}
	URI( URI_* );
	URI( World,  const std::string &uri_string );
	URI( World, Concept );

	// convert a file name into a URI
	URI( const std::string & filename, World );

	// result has source_uri replaced by base_uri in uri_string
	// useful for conversion between prefix and namespace forms.
	URI( const std::string &uri_string, URI source_uri, URI base_uri );

	// result is that uri_string replaces anything after the final '/' in base_uri
	URI( URI base_uri, const std::string &uri_string );

};

// ---------------------------------------------------------------

class Model : public std::shared_ptr< Model_ >
{
public:
	// create an empty model
	Model() : std::shared_ptr< Model_ >( nullptr ) {}

	// replicate shared_ptr<> constructor
	Model( Model_* );

	//
	// New model attached to storage
	//
	// Options: Apparently in the format of XML attributes.
	// 	    It is not documented as to what they are or how they are used.
	//
	// storage_type - name of storage factory
	// 		"file", "memory", "hashes", "sqlite"
	// storage_name - an identifier, eg filename or database name
	// options - whatever needed for initialisation
	Model( World, const std::string & storage_type,
		  const std::string & storage_name = "",
		  const std::string & storage_options = "",
		  const std::string & model_options = "" );
};

// ---------------------------------------------------------------

#if USE_NODE
class Node : public std::shared_ptr< Node_ >
{
public:
	Node();		// nullptr
	Node( World );	// blank
	Node( Node_* );
	Node( NodeRef );
	Node( World, URI );
	Node( World, const Literal & );

	// Node( World, const std::string & uri );

	// _is_wf_xml - true if its well formed XML
	// don't set _is_wf_xml true and have xml_language defined
	//Node( World, const std::string & literal, bool _is_wf_xml,
	//		const std::string & xml_language = "" );
};
#else
using Node = std::shared_ptr< Node_ >;

class ResourceNode : public std::shared_ptr< ResourceNode_ >
{
public:
	ResourceNode( World, URI );
	ResourceNode( World, Concept );
};

class LiteralNode : public std::shared_ptr< LiteralNode_ >
{
public:
	LiteralNode( World, const Literal & );
};

class BlankNode : public std::shared_ptr< BlankNode_ >
{
public:
	BlankNode( World, const std::string &id = "" );
};
#endif

// ---------------------------------------------------------------

class Statement : public std::shared_ptr< Statement_ >
{
public:
	Statement();
	Statement(World);
	Statement(World, Node subject, Node predicate, Node object);
	Statement( Statement_* );
	Statement( StatementRef );
};

// ---------------------------------------------------------------

class Parser : public std::shared_ptr< Parser_ >
{
public:
	//
	// name is the name of a parsing engine:
	// Use listParsers() to get names.
	// Empty string for default.
	//
	Parser( World, const std::string & name,
	        const std::string & syntax_mime = std::string() );
	
	Parser( World, const std::string & name, URI syntax_uri );
};

// ---------------------------------------------------------------

class Serializer : public std::shared_ptr< Serializer_ >
{
public:
	Serializer();
	Serializer( Serializer_* );
	Serializer( World, const std::string & name = "rdfxml",
			const std::string & syntax_mime = "" );
	Serializer( World, const std::string & name, URI syntax_uri );
};

// ---------------------------------------------------------------

class Stream : public std::shared_ptr< Stream_ >
{
public:
	Stream();
	Stream( World );
	Stream( Stream_* );
	Stream( World, Parser, URI, URI base );
};

// ---------------------------------------------------------------

class Query : public std::shared_ptr< Query_ >
{
public:
	Query( World, const std::string & query, const std::string & lang = "sparql" );

	Query( World, const std::string & query, URI base_uri, const std::string & lang = "sparql" );
};

// ---------------------------------------------------------------
// 
// Callback abstract classes
//
// ---------------------------------------------------------------

// Client that is notified of errors and/or warnings
class ErrorClient
{
public:
        virtual ~ErrorClient(){}
        virtual void handleError( const std::string & message ) = 0;
        virtual void handleWarning( const std::string & message ) = 0;
};

// ---------------------------------------------------------------

class Universe
{
	// see world.cpp for the implementation
private:
	Universe(){}

	// TODO - protect with mutex
	std::map< std::string, World > worlds;
public:
	static Universe& instance();
	World world( const std::string & name );
};

// ---------------------------------------------------------------

// Instructions for converting a node to a string.
struct Format
{
	// resources
	bool usePrefixes;	// replace namespaces with prefixes
	bool angleBrackets;	// enclose URI in '<' ... '>'
	
	// blank
	std::string blank;	// name to give blank nodes

	// literal
	bool quotes;		// put values in quotes
	bool showLanguage;	// include language identifier as for example: @en
	std::string prefLang;	// restrict to this language when there are alternatives
	bool showDataType;	// include data type as for example: ^^xsd:integer
};

// ---------------------------------------------------------------

class Prefixes
{
	// see world.cpp for the implementation
private:
	WorldRef world;
	URI base_uri;
	std::map< std::string, URI > uriForPrefix;
	std::map< std::string, std::string > prefixForURI;
public:
	Prefixes( World );

	// methods for the base URI which usually corresponds to the 
	// file.
	void base( URI uri ) { base_uri = uri; }
	void base( const std::string & filename );  // appends # to normalised file name
	URI base() const { return base_uri; }
	bool isBase( URI ) const;
	std::string removeBase( URI ) const;

	// methods for accessing the internal maps
	void insert( const std::string &prefix, URI );
	URI find( const std::string & );
	std::string find( URI );

	// methods for converting between the prefix and normal forms
	URI uriForm( const std::string & );
	std::string prefixForm( URI );

	std::map< std::string, URI >::iterator begin() { return uriForPrefix.begin(); }
	std::map< std::string, URI >::iterator end() { return uriForPrefix.end(); }
};

// ---------------------------------------------------------------

enum class DataType
{
	UNDEF,		// use when cannot find a match
	PlainLiteral,	// default
	XMLLiteral,
	XHTML,		// must be valid well formed XML fragment

	// Core Types
	String, Boolean, Decimal, Integer,

	// IEEE floating point
	Double, Float,

	// Time and Data
	Data, Time, DateTime, DateTimeStamp,

	// Recurring and partial dates
	Year, Month, Day, YearMonth, MonthDay, Duration, YearMonthDuration,
	DayTimeDuration,

	// Limited range integers
	Byte, Short, Int, Long, UnsignedByte, UnsignedShort, UnsignedLong,
	PositiveInteger, NonNegativeInteger, NegativeInteger, NonPositiveInteger,

	// Encoded binary data
	HexBinary, Base64Binary,

	// Miscellaneous
	AnyURI, Language, NormalizedString, Token, NMTOKEN, Name, NCName
};

// ---------------------------------------------------------------

// 
// This object holds the various components of an RDF or XML literal
// value. This includes the value, the language, and the data type.
//

class Literal
{
	// see node.cpp for the implementation
	static std::map< DataType, std::string > xsd_types;
	static void initXSDtypes();
private:

	std::string mLanguage;		// eg "en" for English
	DataType    mDataType;

	std::string mValue;

public:
	Literal();	// empty
	Literal( const std::string &val );	// plain, English
	Literal( const char *val ) : Literal(std::string(val)) {}
	Literal( const std::string &val, const std::string &lan );	// plain
	Literal( const std::string &val, DataType, const std::string &lan = "en" );
	Literal( int, DataType );
	Literal( double, DataType );
	explicit Literal( bool );

	void language( const std::string & lang ) { mLanguage = lang; }
	void dataType( DataType t ) { mDataType = t; }
	void value( const std::string & v ) { mValue = v; }

	std::string toString() const;
	std::string toString( const Format & ) const;

	DataType dataType() const { return mDataType; }
	URI dataTypeURI( World ) const;
	std::string language() const { return mLanguage; }

	std::string asString() const { return mValue; }
	int asInteger() const;
	double asDouble() const;
	bool asBoolean() const;
	// TODO - more conversions as required

	static std::string toXSD( DataType );
	static DataType toDataType( const std::string & xsd_type );
};

// ---------------------------------------------------------------
// 
// The following abstract classes provide the functional interface
// to the underlying librdf C library.
//

// ---------------------------------------------------------------

class World_
{
public:
	virtual ~World_(){}
	// static World instance();
	virtual void registerErrorClient( ErrorClient *, bool warnings, bool errors ) = 0;
	virtual void deregisterErrorClient( ErrorClient * ) = 0;
	virtual Serializer defaultSerializer() = 0;
	virtual Prefixes & prefixes() = 0;
};

// ---------------------------------------------------------------


class Model_
{
public:
	virtual ~Model_(){}

	// Get number of statements if possible. May return <0 if not known.
	virtual int size() const = 0;
	virtual bool sync() = 0;

	// Get a pointer to a stream. The user controls its lifetime.
	virtual Stream toStream() = 0;

	// Methods for modifying the model.
	virtual bool add( Node subject, Node predicate, Node object ) = 0;
	virtual bool add( Statement ) = 0;
	virtual bool remove( Statement ) = 0;
	virtual bool update( Statement old, Statement _new ) = 0;
	virtual bool contains( Statement )const = 0;

	// lots of useful functions for navigating the graph.
	virtual std::vector< Node > predicates( Node subject, Node object ) = 0;
	virtual std::vector< Node > objects( Node subject, Node predicate ) = 0;
	virtual std::vector< Node > subjects( Node predicate, Node object ) = 0;

	virtual std::vector< Node > arcsIn( Node object ) = 0;
	virtual std::vector< Node > arcsOut( Node subject ) = 0;
	
	// TODO - add and remove sub-models
};

// ---------------------------------------------------------------

class Node_
{
public:
	virtual ~Node_() {}
	// virtual Node copy() const = 0;

	virtual std::string toString() const = 0;
	virtual std::string toString(const Format &) const = 0;
	virtual URI toURI() const = 0;

	virtual bool isLiteral() const = 0;
	virtual bool isBlank() const = 0;
	virtual bool isResource() const = 0;

	// TODO operator ==
};
// TODO operator == (Node, Node)
// TODO operator << ( ostream &, Node )

#if ! USE_NODE

class ResourceNode_ : public Node_
{
public:
	virtual std::string toString() const = 0;
	virtual std::string toString(const Format &) const = 0;
	virtual URI toURI() const = 0;
};

class LiteralNode_ : public Node_
{
public:
	virtual std::string toString() const = 0;
	virtual std::string toString(const Format &) const = 0;
	virtual Literal toLiteral() const = 0;
};

class BlankNode_ : public Node_
{
	virtual std::string toString() const = 0;
	virtual std::string toString(const Format &) const = 0;
};
#endif // USE_NODE

// ---------------------------------------------------------------

class Parser_
{
public:
	virtual ~Parser_() {}

	virtual bool parseIntoModel( Model, URI uri, URI base_uri ) = 0;

	// Get a list of parser names with their syntax URIs
	static std::vector< std::string > listParsers( World );
};

// ---------------------------------------------------------------

class Query_
{
public:
	virtual ~Query_() {}

	// set the max number of results returned
	virtual bool setLimit(int) = 0;
	virtual int getLimit() const = 0;

	virtual QueryResults execute( Model ) = 0;
};

// ---------------------------------------------------------------

class QueryResult_
{
public:
	virtual int  count() const = 0;
	virtual std::string getBoundName(int offset) const = 0;
	virtual Node getBoundValue(int offset) const = 0;
	virtual Node getBoundValue( const std::string & name ) const = 0;
	virtual std::string toString() const = 0;
};

// ---------------------------------------------------------------

class QueryResults_
{
public:
	virtual ~QueryResults_() {}

	virtual bool success() const = 0;

	// Convert all results to a string. This consumes the results.
	// Default syntax URI is SPARQL XML
	virtual std::string toString() = 0;
	virtual std::string toString( URI syntax, URI base ) = 0;

	class iterator : public std::iterator< std::forward_iterator_tag, QueryResult_ >
	{
	private:
		QueryResult_ *query_result;
	public:
		explicit iterator( QueryResult_* qr );
		iterator();

		iterator & operator = ( const QueryResult_ & ) = delete;
		virtual QueryResult_ & operator *() const;
		virtual iterator & operator ++ ();
		virtual iterator & operator ++ ( int );
		virtual bool operator == ( const iterator & ) const;
		virtual bool operator != ( const iterator & ) const;
	};

	virtual iterator begin() const = 0;
	virtual iterator end() const = 0;
};

// ---------------------------------------------------------------

class QueryString
{
private:
	std::map<std::string, std::string> prefixes;
	std::string variables;
	std::vector<std::string> conditions;
	std::string order;

	std::string get_string();

public:
	QueryString(){}

	// TODO - use Prefixes to set prefixes for the query
	void addPrefix( const std::string & id, const std::string & uri);
	void setVariables( const std::string & variables);
	void addCondition( const std::string & condition);
	void orderBy( const std::string & order);

	operator std::string();
};

// ---------------------------------------------------------------

class Serializer_
{
public:
	virtual ~Serializer_() {}
	virtual bool setNamespace( URI, const std::string & prefix ) = 0;
	virtual bool toFile( const std::string &filename, Model ) = 0;
	virtual bool toFile( const std::string &filename, Model, URI base_uri ) = 0;
	// TODO static int listSerializers( std::vector< std::string > & );
};

// ---------------------------------------------------------------

class Statement_
{
public:
	virtual ~Statement_() {}
	
	virtual Statement copy() const = 0;

	virtual void subject( Node n ) = 0;
	virtual NodeRef subject() const = 0;
	virtual void predicate( Node n ) = 0;
	virtual NodeRef predicate() const = 0;
	virtual void object( Node n ) = 0;
	virtual NodeRef object() const = 0;

	virtual bool isComplete() const = 0;
	virtual bool match( Statement ) const = 0;
	virtual void clear() = 0;

	virtual std::string toString() const = 0;
	virtual std::string toString( const Format &) const = 0;
	virtual bool operator == ( Statement ) const = 0;
};

bool operator == ( Statement, Statement );
// TODO operator << ( ostream &, Node )

// ---------------------------------------------------------------

class Stream_
{
public:
	virtual ~Stream_() {}

	virtual bool end() = 0;
	virtual bool next() = 0;

	// TODO - statement only valid until next() or closed.
	virtual StatementRef current() = 0;

	// TODO - iterator interface
};

// ---------------------------------------------------------------

class URI_
{
public:
	virtual ~URI_() {}
	
	virtual URI copy() const = 0;

	// strip off fragment
	virtual URI trim( World ) const = 0;

	virtual std::string toString() const = 0;
	virtual bool operator == (URI)const = 0;

	virtual bool isFileName() const = 0;
	virtual std::string toFileName() const = 0;
};

bool operator == ( URI, URI );
// TODO operator << ( ostream &, Node )

// ---------------------------------------------------------------

//
// A template function that converts a shared pointer
// into the corresponding librdf pointer.
// Class C (child) is the implementation of the abstract class P (parent)
// T is the type for the librdf pointer.
// Requires C to have a type conversion operator that returns the 
// appropriate pointer.
//
template < class C, class P, typename T >
T* deref( std::shared_ptr<C> a )
{
        T* t = nullptr;
        P* c = static_cast< P* >( a.get() );
        if ( c ) t = *c;
        return t;
}

// 
// Simplify the template calls.
// Relies on the naming convention for classes described above.
// A is the shared pointer type, b is the librdf type
// and c is the shared pointer object.
//
#define DEREF( A, b, c ) deref< A##_, _##A, b >( c ) 

} // namespace rdf

#endif
