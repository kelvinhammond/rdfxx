
/* RDF C++ API 
 *
 * 			stream.cpp
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
#include <rdfxx/stream.hpp>
#include <rdfxx/parser.hpp>

using namespace rdf;
using namespace std;

// -----------------------------------------------------------------------------
//	Stream
// -----------------------------------------------------------------------------

Stream::Stream()
	: std::shared_ptr< Stream_ >( new _Stream )
{}

// -----------------------------------------------------------------------------

Stream::Stream( Parser parser, URI uri, URI base )
	: std::shared_ptr< Stream_ >( new _Stream( parser, uri, base ))
{}

// -----------------------------------------------------------------------------

Stream::Stream( Stream_ * _stream )
	: std::shared_ptr< Stream_ >( _stream )
{}

// -----------------------------------------------------------------------------
//	_Stream
// -----------------------------------------------------------------------------

_Stream::_Stream()
	 : stream(0), currStatement(nullptr)
{
    _World & world = _World::instance();

    stream = librdf_new_empty_stream(world);
    if(!stream)
	throw VX(Error) << "Failed to allocate stream";
}

// -----------------------------------------------------------------------------

_Stream::_Stream(Parser _parser, URI _uri, URI _base)
	 : stream(0), currStatement(nullptr)
{
    librdf_parser *p = DEREF( Parser, librdf_parser, _parser );
    librdf_uri *u = DEREF( URI, librdf_uri, _uri );
    librdf_uri *bu = DEREF( URI, librdf_uri, _base );

    stream = librdf_parser_parse_as_stream(p, u, bu );
    if(!stream)
	throw VX(Error) << "Failed to allocate stream";
}

// -----------------------------------------------------------------------------

_Stream::_Stream(librdf_stream* _stream) :
    stream(_stream),
    currStatement(nullptr)
{}

// -----------------------------------------------------------------------------

_Stream::~_Stream()
{
    if(stream)
        librdf_free_stream(stream);
}

// -----------------------------------------------------------------------------
bool
_Stream::end()
{
    int status = librdf_stream_end(stream);

    return (status != 0) ? true : false;
}

// -----------------------------------------------------------------------------
bool
_Stream::next()
{
    currStatement.reset();
    int status = librdf_stream_next(stream);

    return (status == 0) ? true : false;
}

// -----------------------------------------------------------------------------

StatementRef
_Stream::current()
{
	// gets a shared pointer into librdf structures.
	// only valid until next() or stream closed.
        currStatement = Statement(new _Statement(librdf_stream_get_object(stream),false));
        return currStatement;
}

// -----------------------------------------------------------------------------

_Stream::operator librdf_stream*()
{
    return stream;
}

// -------------------------------- end ----------------------------------------
