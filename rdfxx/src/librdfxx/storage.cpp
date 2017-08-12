
/* RDF C++ API 
 *
 * 			storage.cpp
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
#include <rdfxx/storage.hpp>
#include <rdfxx/model.hpp>

using namespace rdf;
using namespace std;

// -----------------------------------------------------------------------------
//	Storage
// -----------------------------------------------------------------------------

Storage::Storage(const std::string& _storage_name, const std::string& _name,
			const std::string& _options)
	: std::shared_ptr< Storage_ >( new _Storage( _storage_name, _name, _options ))
{}

// -----------------------------------------------------------------------------
//	_Storage
// -----------------------------------------------------------------------------

_Storage::_Storage(const std::string& _storage_name, const std::string& _name, 
			const std::string& _options)
	 : storage(0), model(nullptr)
{
    cout << "Creating storage in " << _storage_name << endl;
    _World& world = _World::instance();

    storage = librdf_new_storage(world,  _storage_name.c_str(), 
    			 _name.c_str(),  _options.c_str());
    if(!storage)
	throw VX(Error) << "Failed to allocate storage";
    cout << "Created storage " << _storage_name << endl;
}

// ----------------------------------------------------------------------------

_Storage::~_Storage()
{
    // Destroy the associated model first.
    if(model)
    {
        //model->~Model(); //BR WTF!?
	model.reset();
    }

    if(storage)
    {
        librdf_free_storage(storage);
        storage = 0;
    }
}

// -----------------------------------------------------------------------------

bool
_Storage::open(Model _model)
{
    // If there is already a model assigned, destroy it.
    if(model)
    {
        // model->~Model();
	model.reset();
    }

    librdf_model* m = DEREF( Model, librdf_model, _model );
    bool status = (librdf_storage_open(storage, m) == 0) ? true : false;

    if(status)
    {
        model = _model;
    }
    return status;
}

// -----------------------------------------------------------------------------

bool
_Storage::close()
{
    bool status = (librdf_storage_close(storage) == 0) ? true : false;

    if(status)
        model.reset();

    return status;
}

// -----------------------------------------------------------------------------

int
_Storage::size()
{
    return librdf_storage_size(storage);
}

// -----------------------------------------------------------------------------

bool
_Storage::add(Statement _statement)
{
    librdf_statement *st = DEREF( Statement, librdf_statement, _statement );

    // Makes a copy of the statement
    int status = librdf_storage_add_statement(storage, st);

    return (status == 0) ? true : false;
}

// -----------------------------------------------------------------------------

bool
_Storage::add(Stream _statement_stream)
{
    librdf_stream* strm = DEREF( Stream, librdf_stream, _statement_stream );
    int status = librdf_storage_add_statements(storage, strm );

    return (status == 0) ? true : false;
}

// -----------------------------------------------------------------------------

bool
_Storage::remove(Statement _statement)
{
    librdf_statement *st = DEREF( Statement, librdf_statement, _statement );
    int status = librdf_storage_remove_statement(storage, st);

    return (status == 0) ? true : false;
}

// -----------------------------------------------------------------------------

bool
_Storage::update(Statement _old, Statement _new)
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
_Storage::contains(Statement _statement)
{
    librdf_statement *st = DEREF( Statement, librdf_statement, _statement );
    int status = librdf_storage_contains_statement(storage, st);

    if(status > 0)
    {
    	throw VX(Error) << "Illegal statement";
    }

    return (status == 0) ? true : false;
}

// -----------------------------------------------------------------------------

_Storage::operator librdf_storage*()
{
    return storage;
}

// --------------------------------- end ---------------------------------------
