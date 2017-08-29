
/* RDF C++ API 
 *
 * 			model.cpp
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
#include <rdfxx/rdfxx.h>
#include <rdfxx/model.hpp>
#include <rdfxx/query.hpp>
#include <rdfxx/query_results.hpp>

using namespace rdf;
using namespace std;

// -----------------------------------------------------------------------------
//	Model
// -----------------------------------------------------------------------------

Model::Model( Model_* _model )
	: std::shared_ptr< Model_ >(_model)
{}

// -----------------------------------------------------------------------------

Model::Model( World w, const std::string & _storage_type, const std::string & _storage_name,
                const std::string & _storage_options, const std::string & _model_options )
	: std::shared_ptr< Model_ >( new _Model( w, _storage_type, _storage_name, 
			_storage_options, _model_options ))
{
}

// -----------------------------------------------------------------------------
//	_Model
// -----------------------------------------------------------------------------

_Model::_Model( World _w, const std::string & _storage_type, const std::string & _storage_name,
                const std::string & _storage_options, const std::string & _model_options )
	: world(_w)
{
	librdf_world*w = DEREF( World, librdf_world, _w);

	storage = librdf_new_storage(w,  _storage_type.c_str(), 
    			 _storage_name.c_str(),  _storage_options.c_str());
	if(!storage)
		throw VX(Error) << "Failed to allocate storage";

	// Get a pointer to a model. We control its lifetime. 
	model = librdf_new_model(w, storage, _model_options.c_str());
	if(!model)
		throw VX(Error) << "Failed to allocate model";

	updatePrefixes();
}

// -----------------------------------------------------------------------------

_Model::~_Model()
{
    if(model)
    {
        librdf_free_model(model);
    }

    if(storage)
    {
        librdf_free_storage(storage);
    }
}

// -----------------------------------------------------------------------------

int
_Model::size() const
{
	// may return <0 if size not discernable
	return librdf_model_size(model);
}

// -----------------------------------------------------------------------------

bool
_Model::sync()
{
	//
	// save the prefixes
	//
	savePrefixes();

	int status = librdf_model_sync(model);

	return (status == 0) ? true : false;
}

// -----------------------------------------------------------------------------

Stream
_Model::toStream()
{
	return Stream( new _Stream(world, librdf_model_as_stream(model)));
}

// -----------------------------------------------------------------------------

bool
_Model::add(Node _subject, Node _predicate, Node _object)
{
    librdf_node *s = _NodeBase::derefNode( _subject );
    librdf_node *p = _NodeBase::derefNode( _predicate );
    librdf_node *o = _NodeBase::derefNode( _object );
    int status = librdf_model_add(model, s, p, o );

    return (status == 0) ? true : false;
}

// -----------------------------------------------------------------------------

bool
_Model::add(Statement _statement)
{
    librdf_statement *s = DEREF( Statement, librdf_statement, _statement );
    int status = librdf_model_add_statement(model, s);

    return (status == 0) ? true : false;
}

// -----------------------------------------------------------------------------

bool
_Model::remove(Statement _statement)
{
    librdf_statement *s = DEREF( Statement, librdf_statement, _statement );
    int status = librdf_model_remove_statement(model, s);

    return (status == 0) ? true : false;
}

// -----------------------------------------------------------------------------

bool
_Model::update(Statement _old, Statement _new)
{
    if(contains(_old))
    {
        remove(_old);
        add(_new);

        return true;
    }
    
    return false;
}

// -----------------------------------------------------------------------------

bool
_Model::contains(Statement _statement) const
{
    librdf_statement *s = DEREF( Statement, librdf_statement, _statement );
    int status = librdf_model_contains_statement(model, s);

    if(status > 0)
    {
	throw VX(Error) << "Illegal RDF Statement";
    }

    return (status == 0) ? false : true;
}

// -----------------------------------------------------------------------------

std::vector< Node >
_Model::predicates( Node subject, Node object )
{
	librdf_node *s = _NodeBase::derefNode( subject );
	librdf_node *o = _NodeBase::derefNode( object );
	std::vector< Node > nodes;
	librdf_iterator *iter = librdf_model_get_arcs( model, s, o );
	if ( iter )
	{
		nodeIteratorToVector( iter, nodes );
		return nodes;
	}
	else
		throw VX(Error) << "Failed to get predicates";
}

// -----------------------------------------------------------------------------

std::vector< Node >
_Model::objects( Node subject, Node predicate )
{
	librdf_node *s = _NodeBase::derefNode( subject );
	librdf_node *p = _NodeBase::derefNode( predicate );
	std::vector< Node > nodes;
	librdf_iterator *iter = librdf_model_get_targets( model, s, p );
	if ( iter )
	{
		nodeIteratorToVector( iter, nodes );
		return nodes;
	}
	else
		throw VX(Error) << "Failed to get predicates";
}

// -----------------------------------------------------------------------------

std::vector< Node >
_Model::subjects( Node predicate, Node object )
{
	librdf_node *p = _NodeBase::derefNode( predicate );
	librdf_node *o = _NodeBase::derefNode( object );
	std::vector< Node > nodes;
	librdf_iterator *iter = librdf_model_get_sources( model, p, o );
	if ( iter )
	{
		nodeIteratorToVector( iter, nodes );
		return nodes;
	}
	else
		throw VX(Error) << "Failed to get predicates";
	throw VX(Code) << "not implemented";
}

// -----------------------------------------------------------------------------

std::vector< Node >
_Model::arcsIn( Node object )
{
	librdf_node *o = _NodeBase::derefNode( object );
	std::vector< Node > nodes;
	librdf_iterator *iter = librdf_model_get_arcs_in( model, o );
	if ( iter )
	{
		nodeIteratorToVector( iter, nodes );
		return nodes;
	}
	else
		throw VX(Error) << "Failed to get predicates";
}

// -----------------------------------------------------------------------------

std::vector< Node >
_Model::arcsOut( Node subject )
{
	librdf_node *s = _NodeBase::derefNode( subject );
	std::vector< Node > nodes;
	librdf_iterator *iter = librdf_model_get_arcs_out( model, s );
	if ( iter )
	{
		nodeIteratorToVector( iter, nodes );
		return nodes;
	}
	else
		throw VX(Error) << "Failed to get predicates";
}

// -----------------------------------------------------------------------------

void 
_Model::nodeIteratorToVector( librdf_iterator *iter, std::vector< Node > & nodes )
{
	while( ! librdf_iterator_end( iter ))
	{
		librdf_node *x = (librdf_node*)librdf_iterator_get_object( iter );
		librdf_node *xn = librdf_new_node_from_node(x);
		Node n( _NodeBase::make( world, xn, true ));
		nodes.push_back(n);
		librdf_iterator_next( iter );
	}
}

// -----------------------------------------------------------------------------

void
_Model::savePrefixes()
{
	Prefixes &prefs = world->prefixes();
	ResourceNode pred( world, URI( prefs.uriForm("rdfxx:hasPrefix")));

	for ( auto & I : world->prefixes() )
	{
		ResourceNode subj( world, I.second );
		LiteralNode  obj( world, Literal(I.first));
		Statement st( world, subj, pred, obj );
		if ( ! contains( st ))
			add( st );	
	}
}

// -----------------------------------------------------------------------------

void
_Model::updatePrefixes()
{
	Prefixes &prefs = world->prefixes();
	QueryString qs;
	qs.addPrefix("rdfxx", "https://sourceforge.net/p/ocratato-sassy/rdfxx#");
	qs.setVariables("DISTINCT ?uri ?prefix");
	qs.addCondition("?uri rdfxx:hasPrefix ?prefix");

	_Query q( world, qs );
	QueryResults qr( q.execute( model ));

	Format format = {false, false, "_", false, false, "en", false };
	for( auto &P : *qr )
	{
		Node prefix = P.getBoundValue("prefix");
		Node uri = P.getBoundValue("uri");
		if ( prefix->isLiteral() && uri->isResource() )
		{
			prefs.insert( prefix->toString(format), uri->toURI() );
		}
		else
			throw VX(Alert) << "Unexpected node types";
	}
}

// -----------------------------------------------------------------------------

_Model::operator librdf_model*() const
{
    return model;
}

// ---------------------------------- end --------------------------------------
