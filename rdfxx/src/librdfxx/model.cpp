
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
#include <rdfxx/model.hpp>

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
/*
_Model::_Model(Storage _storage, const string & _options)
{
    //_World& world = _World::instance();
    librdf_storage* st = DEREF( Storage, librdf_storage, _storage );

    // Get a pointer to a model. We control its lifetime. 
    model = librdf_new_model(world, st, _options.c_str());
    if(!model)
	throw VX(Error) << "Failed to allocate model";
}
*/
// -----------------------------------------------------------------------------

_Model::_Model( World _w, const std::string & _storage_type, const std::string & _storage_name,
                const std::string & _storage_options, const std::string & _model_options )
	: world(_w)
{
    //_World& world = _World::instance();
    librdf_world*w = DEREF( World, librdf_world, _w);

    storage = librdf_new_storage(w,  _storage_type.c_str(), 
    			 _storage_name.c_str(),  _storage_options.c_str());
    if(!storage)
	throw VX(Error) << "Failed to allocate storage";

    // Get a pointer to a model. We control its lifetime. 
    model = librdf_new_model(w, storage, _model_options.c_str());
    if(!model)
	throw VX(Error) << "Failed to allocate model";
}

// -----------------------------------------------------------------------------
/*
_Model::_Model(const _Model & _model)
{
    cout << "creating copy of model" << endl;
    //SF  BUGGY!!! <-> SEGFAULT
    // Get a pointer to a model. We control its lifetime.
    model = librdf_new_model_from_model(_model);
    if(!model)
	throw VX(Error) << "Failed to allocate model";
}
*/

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
/*
Model
_Model::copy()
{
	return Model( new _Model( *this ));
}
*/
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
    librdf_node *s = DEREF( Node, librdf_node, _subject );
    librdf_node *p = DEREF( Node, librdf_node, _predicate );
    librdf_node *o = DEREF( Node, librdf_node, _object );

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

    //if(status < 0)
    if(status > 0)
    {
	throw VX(Error) << "Illegal RDF Statement";
    }

    return (status == 0) ? false : true;
}

// -----------------------------------------------------------------------------

_Model::operator librdf_model*() const
{
    return model;
}

// ---------------------------------- end --------------------------------------
