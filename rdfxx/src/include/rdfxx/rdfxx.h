
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

//! @file
//! \brief A C++ wrapper for Redland librdf
//! 
//! This file contains classes that provide a C++ interface to RDF
//! models. 
//!

#ifndef RDFXX_H
#define RDFXX_H

#include <memory>
#include <string>
#include <map>

#include <rdfxx/except.h>

//! The namespace for the Resource Description Framework interface.
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
class ResourceNode_;
class LiteralNode_;
class BlankNode_;
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
//! A shared pointer to a World object.
using World = std::shared_ptr< World_ >;

//! A weak shared pointer to a World object.
using WorldRef = std::weak_ptr< World_ >;

//! A weak shared pointer to a Node object.
using NodeRef = std::weak_ptr< Node_ >;

//! A shared pointer to a set of query results.
using QueryResults = std::shared_ptr< QueryResults_ >;

//! A weak shared pointer to a statement.
using StatementRef = std::weak_ptr< Statement_ >;

// ---------------------------------------------------------------

//! An enumeration of RDF and RDFS concepts.
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

//! \class URI rdfxx.h rdfxx/rdfxx.h
//! \brief A shared pointer with constructors for the URI_ class.

class URI : public std::shared_ptr< URI_ >
{
public:
	//! Default constructor.
	URI() : std::shared_ptr< URI_ >( nullptr ) {}

	//! Create shared pointer to new URI_ object.
	URI( URI_* );

	//! Create URI using a string.
	URI( World,  const std::string &uri_string );

	//! Create URI using a Concept
	URI( World, Concept );

	//! Convert a file name into a URI
	URI( const std::string & filename, World );

	//! Create URI the has source_uri replaced by base_uri in uri_string
	// useful for conversion between prefix and namespace forms.
	URI( const std::string &uri_string, URI source_uri, URI base_uri );

	//! Create URI where the uri_string replaces anything after the final '/' in base_uri
	URI( URI base_uri, const std::string &uri_string );

	//! Create a URI from base URI and local name
	URI( const std::string &local_name, URI basr_uri );
};

// ---------------------------------------------------------------

//! \class Model rdfxx.h rdfxx/rdfxx.h
//! \brief A shared pointer with constructors for the Model_ class.

class Model : public std::shared_ptr< Model_ >
{
public:
	//! Create an empty model.
	Model() : std::shared_ptr< Model_ >( nullptr ) {}

	//! Replicate shared_ptr<> constructor
	Model( Model_* );

	//! New model attached to storage
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

//! A shared pointer to a Node object.
using Node = std::shared_ptr< Node_ >;

//! \class ResourceNode rdfxx.h rdfxx/rdfxx.h
//! \brief A shared pointer with constructors for the ResourceNode_ class.

class ResourceNode : public std::shared_ptr< ResourceNode_ >
{
public:
	//! Create resource node based on a URI.
	ResourceNode( World, URI );

	//! Create resource node for a concept.
	ResourceNode( World, Concept );

	//! Create a numbered resource node for containers
	ResourceNode( World, int );

	//! Convert a Node into a ResourceNode
	ResourceNode( Node resourceNode );
};

//! \class LiteralNode rdfxx.h rdfxx/rdfxx.h
//! \brief A shared pointer with constructors for the LiteralNode_ class.

class LiteralNode : public std::shared_ptr< LiteralNode_ >
{
public:
	//! Create a literal node using a literal value.
	LiteralNode( World, const Literal & );

	//! Convert a Node to a LiteralNode.
	LiteralNode( Node literalNode );
};

//! \class BlankNode rdfxx.h rdfxx/rdfxx.h
//! \brief A shared pointer with constructors for the BlankNode_ class.


class BlankNode : public std::shared_ptr< BlankNode_ >
{
public:
	//! Create a blank node using a supplied identifier.
	BlankNode( World, const std::string &id = "" );

	//! Create a BlankNode from a Node
	BlankNode( Node blankNode );
};

// ---------------------------------------------------------------

//! \class Statement rdfxx.h rdfxx/rdfxx.h
//! \brief A shared pointer with constructors for the Statement_ class.


class Statement : public std::shared_ptr< Statement_ >
{
public:
	//! Create a nullptr based default statement
	Statement();

	//! Create an empty statement
	Statement(World);

	//! Create a statement with subject, predicate and object nodes.
	Statement(World, Node subject, Node predicate, Node object);

	//! Replicate the shared_ptr<> constructor
	Statement( Statement_* );

	//! Convert a weak pointer to a shared pointer.
	Statement( StatementRef );
};

// ---------------------------------------------------------------

//! \class Parser rdfxx.h rdfxx/rdfxx.h
//! \brief A shared pointer with constructors for the Parser_ class.


class Parser : public std::shared_ptr< Parser_ >
{
public:
	
	// name is the name of a parsing engine:
	// Use listParsers() to get names.
	// Empty string for default.
	//
	
	//! Create an RDF parser using the specified parsing engine.
	Parser( World, const std::string & name,
	        const std::string & syntax_mime = std::string() );
	
	//! Create an RDF parser using the specified parsing engine.
	Parser( World, const std::string & name, URI syntax_uri );
};

// ---------------------------------------------------------------

//! \class Serializer rdfxx.h rdfxx/rdfxx.h
//! \brief A shared pointer with constructors for the Serializer_ class.


class Serializer : public std::shared_ptr< Serializer_ >
{
public:
	//! Create a nullptr shared pointer
	Serializer();

	//! replicate the shared pointer constructor
	Serializer( Serializer_* );

	//! Create a serialiser using the specified syntax.
	Serializer( World, const std::string & name = "rdfxml",
			const std::string & syntax_mime = "" );
	
	//! Create a serialiser using the specified syntax.
	Serializer( World, const std::string & name, URI syntax_uri );
};

// ---------------------------------------------------------------

//! \class Stream rdfxx.h rdfxx/rdfxx.h
//! \brief A shared pointer with constructors for the Stream_ class.


class Stream : public std::shared_ptr< Stream_ >
{
public:
	//! Create a nullptr shared pointer.
	Stream();

	//! Create a stream object.
	Stream( World );

	//! Replicate the shared pointer constructot
	Stream( Stream_* );

	//! Create a stream from the specified parser.
	Stream( World, Parser, URI, URI base );
};

// ---------------------------------------------------------------

//! \class Query rdfxx.h rdfxx/rdfxx.h
//! \brief A shared pointer with constructors for the Query_ class.


class Query : public std::shared_ptr< Query_ >
{
public:
	//! Create a query.
	Query( World, const std::string & query, const std::string & lang = "sparql" );

	//! Create a query.
	Query( World, const std::string & query, URI base_uri, const std::string & lang = "sparql" );
};

// ---------------------------------------------------------------
// 
// Callback abstract classes
//
// ---------------------------------------------------------------

//! \class ErrorClient rdfxx.h rdfxx/rdfxx.h
//! \brief Client that is notified of errors and/or warnings.

class ErrorClient
{
public:
	//! Virtual destructor
        virtual ~ErrorClient(){}

	//! The function that will be called when an error is detected.
        virtual void handleError( const std::string & message ) = 0;

	//! The function that will be called when a warning is detected.
        virtual void handleWarning( const std::string & message ) = 0;
};

// ---------------------------------------------------------------

//! \class Universe rdfxx.h rdfxx/rdfxx.h
//! \brief A singleton class responsible for managing the World objects.

//!
//! The Universe object allows the using program to access a World
//! object by name. Access will be protected by a mutex so that different
//! threads can each use RDF processing.
//!
class Universe
{
	// see world.cpp for the implementation
private:
	Universe(){}

	// TODO - protect with mutex
	std::map< std::string, World > worlds;
public:
	//! Get a reference to the universe object.
	static Universe& instance();

	//! Create, or return an existing world object.
	World world( const std::string & name );
};

// ---------------------------------------------------------------

//! \struct Format rdfxx.h rdfxx/rdfxx.h
//! \brief Instructions for converting a node to a string.

//!
//! These objects are passed to the toString() method of Nodes and Statements
//! to control how they will be presented.
//!

struct Format
{
	// resources
	bool usePrefixes;	//!< Replace namespaces with prefixes
	bool angleBrackets;	//!< Enclose URI in '<' ... '>'
	
	// blank
	std::string blank;	//!< Name to give blank nodes

	// literal
	bool quotes;		//!< Put values in quotes
	bool showLanguage;	//!< Include language identifier as for example: \c "@en"
	std::string prefLang;	//!< Restrict to this language when there are alternatives
	bool showDataType;	//!< Include data type as for example: ^^xsd:integer
};

// ---------------------------------------------------------------

//! \class Prefixes rdfxx.h rdfxx/rdfxx.h
//! \brief Manages the prefixes and namespaces for a World.


class Prefixes
{
	// see world.cpp for the implementation
private:
	WorldRef world;
	URI base_uri;
	std::map< std::string, URI > uriForPrefix;
	std::map< std::string, std::string > prefixForURI;
	static int anonCounter;
public:
	//! Constructor
	Prefixes( World );

	// methods for the base URI which usually corresponds to the 
	// file.

	//! Set the base URI
	void base( URI uri ) { base_uri = uri; }
	
	//! insert an anonymous namespace
	void anonymous( URI uri );

	//! Set the base URI.
	void base( const std::string & filename );  // appends # to normalised file name

	//! Get the base URI
	URI base() const { return base_uri; }

	//! Check if a URI is the base URI
	bool isBase( URI ) const;

	//! Strip the base URI from a URI
	std::string removeBase( URI ) const;

	// methods for accessing the internal maps
	
	//! Save a prefix and its corresponding namespace URI
	void insert( const std::string &prefix, URI );

	//! Find the namespace URI with the supplied prefix.
	URI find( const std::string & );

	//! Find the prefix for the supplied namespace URI.
	std::string find( URI );

	//! Update a prefix and namespace
	void update( const std::string &oldPrefix, const std::string &newPrefix, URI );

	//! Remove a prefix and namespace
	void remove( const std::string & );

	// methods for converting between the prefix and normal forms

	//! Convert a prefix and fragment to a URI and fragment.
	URI uriForm( const std::string & );

	//! Convert a URI and fragment to its prefix and fragment.
	std::string prefixForm( URI );

	//! Get an iterator for the saved prefixes.
	std::map< std::string, URI >::iterator begin() { return uriForPrefix.begin(); }

	//! get the end iterator for the saves prefixes.
	std::map< std::string, URI >::iterator end() { return uriForPrefix.end(); }
};

// ---------------------------------------------------------------

//! A enumeration of the data types available for RDF literals.

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

//! \class Literal rdfxx.h rdfxx/rdfxx.h
//! \brief Hold the value, language and data type for an RDF literal.


class Literal
{
	// see node.cpp for the implementation
	static std::map< DataType, std::string > xsd_types;
	static std::map< DataType, std::string > names;
	static void initXSDtypes();
private:

	std::string mLanguage;		// eg "en" for English
	DataType    mDataType;

	std::string mValue;

public:
	//! Default constructor.
	Literal();	// empty

	//! Create a PlainLiteral with English language.
	Literal( const std::string &val );	// plain, English

	//! Create a PlainLiteral with English language.
	Literal( const char *val ) : Literal(std::string(val)) {}

	//! Create a PlainLiteral with the specified language
	Literal( const std::string &val, const std::string &lan );	// plain

	//! Create a literal with the specified content, data type and/or language.
	Literal( const std::string &val, DataType, const std::string &lan = "en" );

	//! Create a literal with some type of integral value
	Literal( int, DataType );

	//! Create a literal with some type of real number value.
	Literal( double, DataType );

	//! Create a literal with a boolean value
	explicit Literal( bool );

	//! Set the language.
	void language( const std::string & lang ) { mLanguage = lang; }

	//! Set the data type
	void dataType( DataType t ) { mDataType = t; }

	//! Set the value
	void value( const std::string & v ) { mValue = v; }

	//! Default conversion to a string.
	std::string toString() const;

	//! Convert to astringusing the specified format.
	std::string toString( const Format & ) const;

	//! Get the data type
	DataType dataType() const { return mDataType; }

	//! Get the data type as a URI
	URI dataTypeURI( World ) const;

	//! Get the language.
	std::string language() const { return mLanguage; }

	//! Get the value as a string.
	std::string asString() const { return mValue; }

	//! Get the value as an integer.
	int asInteger() const;

	//! Get the value as a double.
	double asDouble() const;

	//! Get the value as a boolean.
	bool asBoolean() const;
	// TODO - more conversions as required

	//! Convert a data type into an xsd form,
	static std::string toXSD( DataType );
	static std::string toTypeName( DataType );

	//! Convert an xsd form into a data type
	static DataType toDataType( const std::string & xsd_type );
	static DataType asDataType( const std::string & type_name );

	static std::vector< std::string > getDataTypeNames();
};

// ---------------------------------------------------------------
// 
// The following abstract classes provide the functional interface
// to the underlying librdf C library.
//

// ---------------------------------------------------------------

//! \class World_ rdfxx.h rdfxx/rdfxx.h
//! \brief An abstract class defining the methods for an RDF World.


class World_
{
public:
	//! Virtual destructor
	virtual ~World_(){}

	//! Register a client object to receive error and/or error messages.
	virtual void registerErrorClient( ErrorClient *, bool warnings, bool errors ) = 0;

	//! Remove a client that was to get error messages.
	virtual void deregisterErrorClient( ErrorClient * ) = 0;

	//! Get a default serialiser - depreicated
	virtual Serializer defaultSerializer() = 0;

	//! Get a reference to the saved prefixes.
	virtual Prefixes & prefixes() = 0;
};

// ---------------------------------------------------------------


//! \class Model_ rdfxx.h rdfxx/rdfxx.h
//! \brief An abstract class defining the methods for an RDF Model.


class Model_
{
public:
	//! Virtual destructor
	virtual ~Model_(){}

	//! Return a reference to the model's world
	virtual World getWorld() = 0;

	//! Get number of statements if possible. May return <0 if not known.
	virtual int size() const = 0;

	//! Save the model to its storage
	virtual bool sync() = 0;

	//! Get a pointer to a stream. The user controls its lifetime.
	virtual Stream toStream() = 0;

	// Methods for modifying the model.
	
	//! Add the nodes of a statement to the model.
	virtual bool add( Node subject, Node predicate, Node object ) = 0;

	//! Add a statement to the model
	virtual bool add( Statement ) = 0;

	//! Remove a statement from the model
	virtual bool remove( Statement ) = 0;

	//! Update a statement in the model
	virtual bool update( Statement old, Statement _new ) = 0;

	//! Check if a statement is in the model.
	virtual bool contains( Statement )const = 0;

	// lots of useful functions for navigating the graph.

	//! Get a list of predicates that link a subject and object.
	virtual std::vector< Node > predicates( Node subject, Node object ) = 0;

	//! Get a list of objects that are linked to a subject by a particular predicate
	virtual std::vector< Node > objects( Node subject, Node predicate ) = 0;

	//! Get a list of subjects that are linked to an object by a particular predicate
	virtual std::vector< Node > subjects( Node predicate, Node object ) = 0;

	//! Get a list of predicates connected to an object.
	virtual std::vector< Node > arcsIn( Node object ) = 0;

	//! Get a list of predicates connected to a ssubject.
	virtual std::vector< Node > arcsOut( Node subject ) = 0;
	
	// TODO - add and remove sub-models
};

// ---------------------------------------------------------------

//! \class Node_ rdfxx.h rdfxx/rdfxx.h
//! \brief An abstract class defining the methods for an RDF Node.


class Node_
{
public:
	//! Virtual destructor
	virtual ~Node_() {}

	//! Get a string representation of the node
	virtual std::string toString() const = 0;

	//! Get a string representation of the node using the supplied formatting instructions.
	virtual std::string toString(const Format &) const = 0;

	//! Get the URI for a Node
	virtual URI toURI() const = 0;

	//! Check if the node is a literal
	virtual bool isLiteral() const = 0;

	//! Check if the node is blank
	virtual bool isBlank() const = 0;

	//! Check if the node is a resource.
	virtual bool isResource() const = 0;

	// TODO operator ==
};
// TODO operator == (Node, Node)
// TODO operator << ( ostream &, Node )

//! \class ResourceNode_ rdfxx.h rdfxx/rdfxx.h
//! \brief An abstract class defining the methods for an RDF Resource Node


class ResourceNode_ : public Node_
{
public:
	//! Get a string representation of the node
	virtual std::string toString() const = 0;

	//! Get a string representation of the node using the supplied formatting instructions.
	virtual std::string toString(const Format &) const = 0;

	//! Get the URI for a Node
	virtual URI toURI() const = 0;

	//! Get the ordinal value for a list item.
	virtual int listItemOrdinal() const = 0;
};

//! \class LiteralNode_ rdfxx.h rdfxx/rdfxx.h
//! \brief An abstract class defining the methods for an RDF Literal Node


class LiteralNode_ : public Node_
{
public:
	//! Get a string representation of the node
	virtual std::string toString() const = 0;

	//! Get a string representation of the node using the supplied formatting instructions.
	virtual std::string toString(const Format &) const = 0;

	//! Get the Literal data for the node.
	virtual Literal toLiteral() const = 0;
};

//! \class BlankNode_ rdfxx.h rdfxx/rdfxx.h
//! \brief An abstract class defining the methods for an RDF blank Node


class BlankNode_ : public Node_
{
public:
	//! Get a string representation of the node
	virtual std::string toString() const = 0;

	//! Get a string representation of the node using the supplied formatting instructions.
	virtual std::string toString(const Format &) const = 0;
};

// ---------------------------------------------------------------

//! \class Parser_ rdfxx.h rdfxx/rdfxx.h
//! \brief An abstract class defining the methods for an RDF Parser.


class Parser_
{
public:
	//! Virtual destructor
	virtual ~Parser_() {}

	//! Parse a data source into a model.
	virtual bool parseIntoModel( Model, URI uri, URI base_uri ) = 0;

	//! Get a list of parser names with their syntax URIs
	static std::vector< std::string > listParsers( World );
};

// ---------------------------------------------------------------

//! \class Query_ rdfxx.h rdfxx/rdfxx.h
//! \brief An abstract class defining the methods for an RDF Query.


class Query_
{
public:
	//! Virtual destructor
	virtual ~Query_() {}

	//! Set the max number of results returned
	virtual bool setLimit(int) = 0;

	//! Get the max number of results to be returned.
	virtual int getLimit() const = 0;

	//! Run the query on a model.
	virtual QueryResults execute( Model ) = 0;
};

// ---------------------------------------------------------------

//! \class QueryResult_ rdfxx.h rdfxx/rdfxx.h
//! \brief An abstract class defining the methods for an RDF Query Result.


class QueryResult_
{
public:
	//! Get the number of bound values in the result.
	virtual int  count() const = 0;

	//! Get the variable name at a position.
	virtual std::string getBoundName(int offset) const = 0;

	//! Get a value at a position.
	virtual Node getBoundValue(int offset) const = 0;

	//! Get a value for a variable name.
	virtual Node getBoundValue( const std::string & name ) const = 0;

	//! Convert all the names and values to a string.
	virtual std::string toString() const = 0;
};

// ---------------------------------------------------------------

//! \class QueryResults_ rdfxx.h rdfxx/rdfxx.h
//! \brief An abstract class defining the methods for a set of Query Results.


class QueryResults_
{
public:
	//! Virtual destructor
	virtual ~QueryResults_() {}

	//! Check if the query got any results.
	virtual bool success() const = 0;

	//! Convert all results to a string. This consumes the results.
	// Default syntax URI is SPARQL XML
	virtual std::string toString() = 0;

	//! Convert the results to a string in the specified syntax.
	virtual std::string toString( URI syntax, URI base ) = 0;

	//! \class iterator rdfxx.h rdfxx/rdfxx.h
	//! \brief Provide a C++ iterator over the results of a RDF query
	
	class iterator : public std::iterator< std::forward_iterator_tag, QueryResult_ >
	{
	private:
		QueryResult_ *query_result;
	public:
		//! Construct an iterator over the results.
		explicit iterator( QueryResult_* qr );
		
		//! Default constructor.
		iterator();

		//! No assignment allowed for these iterators.
		iterator & operator = ( const QueryResult_ & ) = delete;

		//! Dereference the iterator to get a result.
		virtual QueryResult_ & operator *() const;

		//! Move to next result
		virtual iterator & operator ++ ();

		//! Move to next result
		virtual iterator & operator ++ ( int );

		//! Check for equality
		virtual bool operator == ( const iterator & ) const;

		//! Check for inequality
		virtual bool operator != ( const iterator & ) const;
	};

	//! Get iterator at start of result set
	virtual iterator begin() const = 0;

	//! Get iterator past the end of result set.
	virtual iterator end() const = 0;
};

// ---------------------------------------------------------------

//! \class QueryString rdfxx.h rdfxx/rdfxx.h
//! \brief A class for assistingin the preparation of a SPARQL query.


class QueryString
{
private:
	std::map<std::string, std::string> prefixes;
	std::string variables;
	std::vector<std::string> conditions;
	std::string order;

	std::string get_string();

public:
	//! Create empty query.
	QueryString(){}

	// TODO - use Prefixes to set prefixes for the query
	//! Add a prefix and namespace to the query.
	void addPrefix( const std::string & id, const std::string & uri);

	//! Set the variables to search for.
	void setVariables( const std::string & variables);

	//! Add a condition to the query.
	void addCondition( const std::string & condition);

	//! Add an ordering clause to the query.
	void orderBy( const std::string & order);

	//! Get the query as a string.
	operator std::string();
};

// ---------------------------------------------------------------

//! \class Serializer_ rdfxx.h rdfxx/rdfxx.h
//! \brief An abstract class defining the methods for an RDF Serializer.


class Serializer_
{
public:
	//! Virtual destructor
	virtual ~Serializer_() {}

	//! Add a namespace and prefix to the output
	virtual bool setNamespace( URI, const std::string & prefix ) = 0;

	//! Write the model to a file.
	virtual bool toFile( const std::string &filename, Model ) = 0;

	//! Write the model to a file.
	virtual bool toFile( const std::string &filename, Model, URI base_uri ) = 0;

	// TODO static int listSerializers( std::vector< std::string > & );
};

// ---------------------------------------------------------------

//! \class Statement_ rdfxx.h rdfxx/rdfxx.h
//! \brief An abstract class defining the methods for an RDF Statement.


class Statement_
{
public:
	//! Virtual destructor
	virtual ~Statement_() {}
	
	//! Clone the statement
	virtual Statement copy() const = 0;

	//! Set the subject node
	virtual void subject( Node n ) = 0;

	//! Get a reference to the subject node.
	virtual NodeRef subject() const = 0;

	//! Set the predicate node.
	virtual void predicate( Node n ) = 0;

	//! Get a reference to the predicate node.
	virtual NodeRef predicate() const = 0;

	//! Get the object node.
	virtual void object( Node n ) = 0;

	//! Get a reference to the object node.
	virtual NodeRef object() const = 0;

	//! Check if all three nodes are defined.
	virtual bool isComplete() const = 0;

	//! Compare if the non-null nodes arethe same.
	virtual bool match( Statement ) const = 0;

	//! Remove the nodes.
	virtual void clear() = 0;

	//! Get a string representaion of the statement.
	virtual std::string toString() const = 0;

	//! Get a string representaion of the statement using the specified formatting.
	virtual std::string toString( const Format &) const = 0;

	//! Compare with another statement.
	virtual bool operator == ( Statement ) const = 0;
};

//! Check for equality of two statements.
bool operator == ( Statement, Statement );

// TODO operator << ( ostream &, Node )

// ---------------------------------------------------------------

//! \class Stream_ rdfxx.h rdfxx/rdfxx.h
//! \brief An abstract class defining the methods for an RDF Stream.


class Stream_
{
public:
	//! Virtual destructor
	virtual ~Stream_() {}

	//! Check if at the end of the stream.
	virtual bool end() = 0;

	//! Move to the next statement in the stream.
	virtual bool next() = 0;

	//! Get a reference to the current statement
	// statement only valid until next() or closed.
	virtual StatementRef current() = 0;

	// TODO - iterator interface
};

// ---------------------------------------------------------------

//! \class URI_ rdfxx.h rdfxx/rdfxx.h
//! \brief An abstract class defining the methods for an RDF URI.


class URI_
{
public:
	//! Virtual destructor
	virtual ~URI_() {}
	
	//! Clonethe URI
	virtual URI copy() const = 0;

	//! Strip off fragment
	virtual URI trim( World ) const = 0;

	//! Get a string representation.
	virtual std::string toString() const = 0;

	//! Compare with another URI.
	virtual bool operator == (URI)const = 0;

	//! Check if URI represents a file path.
	virtual bool isFileName() const = 0;

	//! Convert the URI to a file path name
	virtual std::string toFileName() const = 0;
};

//! Check for equality of two URIs
bool operator == ( URI, URI );

// TODO operator << ( ostream &, Node )

// ---------------------------------------------------------------

//
//! A template function that converts a shared pointer into the corresponding librdf pointer.

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
//! \brief Simplify the template calls for dereferencing a shared pointer.

//! Relies on the naming convention for classes described above.
//! A is the shared pointer type, b is the librdf type
//! and c is the shared pointer object.
//
#define DEREF( A, b, c ) deref< A##_, _##A, b >( c ) 

} // namespace rdf

#endif
