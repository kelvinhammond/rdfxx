
/* RDF C++ API 
 *
 * 			stores.cpp
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


#include <rdfxx/stores.hpp>
#include <rdfxx/model.hpp>

using namespace rdf;
using namespace std;

// -----------------------------------------------------------------------------

_Store::_Store(const char* _type, string _name, string _options) :
    storage( Storage_::make(_type, _name.c_str(), _options.c_str()))
{}

// -----------------------------------------------------------------------------
Storage
_Store::getStorage()
{
    return storage;
}

// -----------------------------------------------------------------------------
void
_Store::baseURI( URI uri )
{
    //base_uri.set_string(_base_uri);
    base_uri = URI_::make( uri );
}

// -----------------------------------------------------------------------------
URI
_Store::baseURI()
{
    return base_uri;
}

// -----------------------------------------------------------------------------
FileStore::FileStore(string _file) :
    _Store("file", _file.c_str()),
    _Model(getStorage())
{

}

// -----------------------------------------------------------------------------
MemoryStore::MemoryStore(const char* _name) :
    _Store("memory", _name),
    _Model(getStorage())
{}

// -----------------------------------------------------------------------------
BdbStore::BdbStore(string _name, string _path) :
    _Store("hashes", _name, string("hash-type='bdb',dir='") + _path + "'"),
    _Model(getStorage())
{}

// -----------------------------------------------------------------------------
SqliteStore::SqliteStore(string _name) :
    _Store("sqlite", _name),
    _Model(getStorage())
{}

// --------------------------------- end ---------------------------------------

