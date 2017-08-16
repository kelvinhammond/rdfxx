
/* RDF C++ API 
 *
 * 			statement.cpp
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
#include <rdfxx/statement.hpp>
#include <rdfxx/stream.hpp>
#include <rdfxx/serializer.hpp>

using namespace rdf;
using namespace std;

// -----------------------------------------------------------------------------
//	Statement
// -----------------------------------------------------------------------------

Statement::Statement()
	: std::shared_ptr< Statement_ >( new _Statement )
{}

// -----------------------------------------------------------------------------

Statement::Statement( Node subject, Node predicate, Node object)
	: std::shared_ptr< Statement_ >( new _Statement( subject, predicate, object ))
{}

// -----------------------------------------------------------------------------

Statement::Statement( Statement_* _statement )
	: std::shared_ptr< Statement_ >( _statement )
{}

// -----------------------------------------------------------------------------

Statement::Statement( StatementRef _ref )
	: std::shared_ptr< Statement_ >( _ref )
{}

// -----------------------------------------------------------------------------
//	_Statement
// -----------------------------------------------------------------------------

_Statement::_Statement()
	: statement(0), free(true)
{
	_World & world = _World::instance();
    	statement = librdf_new_statement(world); 
    	if(!statement)
		throw VX(Error) << "Failed to allocate statement";

}

// -----------------------------------------------------------------------------

_Statement::_Statement(Node _subject, Node _predicate, Node _object)
	 : statement(0), free(true)
{
    _World & world = _World::instance();

	librdf_node *s = DEREF( Node, librdf_node, Node(_subject) );
	librdf_node *p = DEREF( Node, librdf_node, Node(_predicate) );
	librdf_node *o = DEREF( Node, librdf_node, Node(_object) );

	//
	// librdf takes ownership of the nodes which may be confusing
	// for the users of this library, so we make copies.
	//
	// Later we may provide a move interface to save making copies.
	//

    statement = librdf_new_statement_from_nodes(world, 
    			librdf_new_node_from_node(s), 
			librdf_new_node_from_node(p), 
			librdf_new_node_from_node(o) );
    if(!statement)
	throw VX(Error) << "Failed to allocate statement";
}

// -----------------------------------------------------------------------------

_Statement::_Statement( Statement _statement)
	 : statement(0), free(true)
{
    librdf_statement *s = DEREF( Statement, librdf_statement, _statement );
    statement = librdf_new_statement_from_statement( s );
    if(!statement)
	throw VX(Error) << "Failed to allocate statement";
}

// -----------------------------------------------------------------------------

_Statement::_Statement(librdf_statement* _statement, bool freeOnDelete )
	 : statement(0), free(freeOnDelete)
{
    if(_statement == 0)
    {
    	throw VX(Error) << "Parameter _statement is NULL pointer";
    }

    statement = librdf_new_statement_from_statement( _statement );
    if(!statement)
	throw VX(Error) << "Failed to allocate statement";
}

// -----------------------------------------------------------------------------

_Statement::~_Statement()
{
    if(statement && free ) 
        librdf_free_statement(statement);
}

// -----------------------------------------------------------------------------

Statement
_Statement::copy() const
{
	return Statement( new _Statement( statement, true ));
}

// -----------------------------------------------------------------------------

NodeRef
_Statement::subject()
{
    librdf_node* subject = librdf_statement_get_subject(statement);

    if(subject)
    {
	// subject is a shared pointer within librdf, so we won't delete it
    	subject_holder.reset( new _Node(subject, false));
    }
    else
    {	
    	subject_holder.reset();
    }
    return subject_holder;
}

// -----------------------------------------------------------------------------

void
_Statement::subject(Node _node)
{
    librdf_node* n = DEREF( Node, librdf_node, Node(_node) );
    //
    // this call takes ownership, so we make a copy
    librdf_statement_set_subject(statement, 
    		librdf_new_node_from_node(n) );
}

// -----------------------------------------------------------------------------

NodeRef
_Statement::predicate()
{
    librdf_node* predicate = librdf_statement_get_predicate(statement);

    if(predicate)
    {
	// predicate is a shared pointer within librdf, so we won't delete it
        predicate_holder.reset( new _Node( predicate, false ));
    }
    else
    {
    	predicate_holder.reset();
    }
	return predicate_holder;
}

// -----------------------------------------------------------------------------

void
_Statement::predicate(Node _node)
{
    librdf_node* n = DEREF( Node, librdf_node, Node(_node) );
    // this call takes ownership, so we make a copy
    librdf_statement_set_predicate(statement,
    		librdf_new_node_from_node(n) );
}

// -----------------------------------------------------------------------------

NodeRef
_Statement::object()
{
    librdf_node* object = librdf_statement_get_object(statement);

    if(object)
    {
	// object is a shared pointer within librdf, so we won't delete it
        object_holder.reset( new _Node( object, false ));
    }
    else
    {
    	object_holder.reset();
    }
	return object_holder;
}

// -----------------------------------------------------------------------------

void
_Statement::object(Node _node)
{
    librdf_node* n = DEREF( Node, librdf_node, Node(_node) );
    // this call takes ownership, so we make a copy
    librdf_statement_set_object(statement,
    		librdf_new_node_from_node(n) );
}

// -----------------------------------------------------------------------------

bool
_Statement::isComplete() const
{
	int status = librdf_statement_is_complete(statement);
 
    return (status == 0) ? false : true;
}

// -----------------------------------------------------------------------------

bool
_Statement::match(Statement _statement) const
{
    librdf_statement * s = DEREF( Statement, librdf_statement, _statement );
    bool match = librdf_statement_match(statement, s );

    return (match != 0) ? true : false;
}

// -----------------------------------------------------------------------------

void
_Statement::clear()
{
    subject_holder.reset();
    predicate_holder.reset();
    object_holder.reset();

    librdf_statement_clear(statement);
}

// -----------------------------------------------------------------------------

std::string
_Statement::toString() const
{
	// TODO - add a serialiser to handle namespace prefixes
	
	string s;
	char *str = NULL;
	size_t len = 2;
	raptor_world * rw = librdf_world_get_raptor(_World::instance());
	if ( rw )
	{
		raptor_iostream *stream = raptor_new_iostream_to_string(rw, (void**)& str, &len, malloc );
		librdf_statement_write( statement, stream );
		raptor_free_iostream(stream);
	}
	else
	{
		cerr << "Failed to get raptor world" << endl;
	}
	if ( str ) s = str;
	::free(str);

    // deprecated
    // sstring = (const char*) librdf_statement_to_string(statement);

    return s;
}

// -----------------------------------------------------------------------------

bool
_Statement::operator ==(Statement _statement) const
{
    librdf_statement * s = DEREF( Statement, librdf_statement, _statement );
    int equal = librdf_statement_equals(statement, s);

    return (equal != 0) ? true : false;
}

// -----------------------------------------------------------------------------

bool
rdf::operator == ( Statement _a, Statement _b )
{
	_Statement *s = static_cast< _Statement *>( _a.get());
	if ( s )
	{
		return (*s) == _b;
	}
	else
		return false;
}

// -----------------------------------------------------------------------------

_Statement::operator librdf_statement*() const
{
    return statement;
}

// ------------------------------- end -----------------------------------------
