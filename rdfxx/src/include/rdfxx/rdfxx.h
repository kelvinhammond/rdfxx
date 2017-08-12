
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
class Parser_;
class Query_;
class QueryResults_;
class Serializer_;
class Statement_;
class Storage_;
class Stream_;
class URI_;

class QueryString;

//
// Shared pointers for use by the client applications
//
using World = std::shared_ptr< World_ >;
using NodeRef = std::weak_ptr< Node_ >;
using QueryResults = std::shared_ptr< QueryResults_ >;
using StatementRef = std::shared_ptr< Statement_ >;

// ---------------------------------------------------------------
//
// The following declarations allow the user to create an object
// and its associated shared pointer as if it were an ordinary object
//
// ---------------------------------------------------------------

class URI : public std::shared_ptr< URI_ >
{
public:
	URI();
	URI( URI_* );
	URI(  const std::string & );
};

// ---------------------------------------------------------------

class Storage : public std::shared_ptr< Storage_ >
{
public:
	// storage_name - name of storage factory
	// 		"file", "memory", "hashes", "sqlite"
	// name - an identifier, eg filename or database name
	// options - whatever needed for initialisation
	Storage( const std::string & storage_name,
		  const std::string & name = "",
		  const std::string & options = "" );
};

// ---------------------------------------------------------------

class Model : public std::shared_ptr< Model_ >
{
public:
	// replicate shared_ptr<> constructor
	Model( Model_* );

	//
	// New model attached to storage
	//
	// Options: Apparently in the format of XML attributes.
	// 	    It is not documented as to what they are or how they are used.
	//
	Model( Storage _storage, const std::string &options = "");
	
	// New empty model
	Model();
};

// ---------------------------------------------------------------

class Node : public std::shared_ptr< Node_ >
{
public:
	Node();
	Node( Node_* );
	Node( NodeRef );
	Node( URI );
	Node( const std::string & uri );
	Node( const std::string & literal, bool _is_wf_xml,
			const std::string & xml_language = "" );
};

// ---------------------------------------------------------------

class Statement : public std::shared_ptr< Statement_ >
{
public:
	Statement(Node subject, Node predicate, Node object);
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
	Parser( const std::string & name,
	        const std::string & syntax_mime = std::string() );
	
	Parser( const std::string & name, URI syntax_uri );
};

// ---------------------------------------------------------------

class Serializer : public std::shared_ptr< Serializer_ >
{
public:
	Serializer( const std::string & name = "rdfxml",
			const std::string & syntax_mime = "" );
	Serializer( const std::string & name, URI syntax_uri );
};

// ---------------------------------------------------------------

class Stream : public std::shared_ptr< Stream_ >
{
public:
	Stream();
	Stream( Stream_* );
	Stream( Parser, URI, URI base );
};

// ---------------------------------------------------------------

class Query : public std::shared_ptr< Query_ >
{
public:
	Query( const std::string & query, const std::string & lang = "sparql" );

	Query( const std::string & query, URI base_uri, const std::string & lang = "sparql" );
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
// 
// The following abstract classes provide the functional interface
// to the underlying librdf C library.
//
// ---------------------------------------------------------------

class World_
{
public:
	virtual ~World_(){}
	static World instance();
	virtual void registerErrorClient( ErrorClient *, bool warnings, bool errors ) = 0;
};

// ---------------------------------------------------------------


class Model_
{
public:
	virtual ~Model_(){}

	// create an independent copy of the model
	virtual Model copy() = 0;

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

	// TODO - lots of useful functions for navigating the graph.
};

// ---------------------------------------------------------------

class Node_
{
public:
	virtual ~Node_() {}
	virtual Node copy() const = 0;

	virtual std::string toString() const = 0;
	virtual URI toURI() const = 0;

	virtual bool isLiteral() const = 0;
	virtual bool isBlank() const = 0;
	virtual bool isResource() const = 0;
};

// ---------------------------------------------------------------

class Parser_
{
public:
	virtual ~Parser_() {}

	virtual bool parseIntoModel( Model, URI uri, URI base_uri ) = 0;

	// Get a list of parser names with their syntax URIs
	static int listParsers( std::vector< std::string > & );
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
};

// ---------------------------------------------------------------

class Statement_
{
public:
	virtual ~Statement_() {}
	
	virtual Statement copy() const = 0;

	virtual void subject( Node n ) = 0;
	virtual NodeRef subject() = 0;
	virtual void predicate( Node n ) = 0;
	virtual NodeRef predicate() = 0;
	virtual void object( Node n ) = 0;
	virtual NodeRef object() = 0;

	virtual bool isComplete() const = 0;
	virtual bool match( Statement ) const = 0;
	virtual void clear() = 0;

	virtual std::string toString() const = 0;
	virtual bool operator == ( Statement ) const = 0;
};

bool operator == ( Statement, Statement );

// ---------------------------------------------------------------

class Storage_
{
public:
	virtual ~Storage_() {}

	// create association with a model
	virtual bool open( Model ) = 0;
	virtual bool close() = 0;

	virtual int size() = 0;  // <0 if cannot be determined

	// Add statements from a stream 
	// (does not include duplicates of existing statements)
	virtual bool add( Stream ) = 0;
	virtual bool add( Statement ) = 0;
	virtual bool remove( Statement ) = 0;
	virtual bool update( Statement old, Statement _new ) = 0;
	virtual bool contains( Statement ) = 0;
};

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
	virtual std::string toString() const = 0;
	virtual bool operator == (URI)const = 0;
};

bool operator == ( URI, URI );

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
