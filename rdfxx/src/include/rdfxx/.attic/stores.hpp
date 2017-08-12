
/* RDF C++ API 
 *
 * 			stores.hpp
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


#ifndef RDFXX_STORES_HPP
#define RDFXX_STORES_HPP

#include <rdfxx/storage.hpp>
#include <rdfxx/model.hpp>

namespace rdf
{

// ****************************************************************************
//! RDF C++ Store
// ****************************************************************************

class _Store
{
    protected:
    // ========================================================================
    Storage storage;
    URI base_uri;

    public:
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    _Store(const char* _type, std::string _name, std::string _options="");

    Storage getStorage();

    void baseURI(URI);

    URI baseURI();
};

// ****************************************************************************
//! RDF C++ FileStore
// ****************************************************************************
/*! The FileStore class is an interface for working with RDF/XML files. It 
 *  automatically initialises a file based storage and associates a model with
 *  it.
 */

class FileStore : public _Store, public _Model
{
    public:
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //! FileStore constructor
    FileStore(std::string _path);
};

// ****************************************************************************
//! RDF C++ MemoryStore
// ****************************************************************************
/*! The MemoryStore class is an interface for working with in-memory databases. 
 */

class MemoryStore : public _Store, public _Model
{
    public:
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //! MemoryStore constructor
    MemoryStore(const char* _name="");
};

// ****************************************************************************
//! RDF C++ BdbStore
// ****************************************************************************
/*! The BdbStore class is an interface for working with BDB hash databases. It 
 *  automatically initialises a hash based storage and associates a model with
 *  it.
 */

class BdbStore : public _Store, public _Model
{
    public:
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //! BdbStore constructor
    BdbStore(std::string _name, std::string _path);
};

// ****************************************************************************
//! RDF C++ SqliteStore
// ****************************************************************************
/*! The SqliteStore class is an interface for working with Sqlite databases. It 
 *  automatically initialises a sqlite based storage and associates a model 
 *  with it.
 */

class SqliteStore : public _Store, public _Model
{
    public:
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //! SqliteStore constructor
    SqliteStore(std::string _name);
};
} // namespace rdf
#endif
