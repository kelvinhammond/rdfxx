
/* RDF C++ API 
 *
 * 			except.h
 *
 * 	Copyright 2001 - 2017		Brenton Ross
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



/**
 * @file
 * \brief An exception object with stream semantics.
 */

#ifndef RDFXX_VX_H
#define RDFXX_VX_H

#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

// these included here since its normal to include the reason for the failure
// by calling strerror(errno)
#include <errno.h>
#include <string.h>

// ------------------------------------------------------------------

namespace rdf
{

// ------------------------------------------------------------------

/// Severity levels for log messages.
enum Severity {
	Emergency, 		///< A fault has been detected which may compromise the computer.
	Alert, 			///< A configuration error has been detected
	Critical,		///< The program cannot continue and may have corrupted its data
	Error, 			///< The program cannot continue
	Code,			///< A programming error has been detected.
	Warning, 		///< There is a problem but the program can continue.
	Notice, 		///< Something is odd.
	Info, 			///< Information messages
	Debug 			///< Debugging messages
};

// ------------------------------------------------------------------

/// An exception object with severity levels.

/** \class vxt except.h rdfxx/except.h
 *
 * The vxt class is an exception object that includes
 * severity levels and has stream syntax for creating messages.
 *
 * Typical usage: throw vxt(VICI::Error) << "something went wrong: " << strerror(errno);
 *
 * A macro version that automatically records the file name and line number
 * is also defined. It would be used like:
 *
 * throw VX(Warning) << "there is a problem";
 *
 */

template< typename N >
class vxt : public std::runtime_error
{
protected:
	Severity mSeverity;		///< severity level of the exception
	std::string buff;		///< message built into this
	std::string file;		///< source file from which it was thrown
	int line;			///< line number in the source file
	int ptr;			///< insertion point for message text

public:
	/// constructor
	/**
	 * \param s The severity of the problem.
	 */
	explicit vxt( Severity s )
		: runtime_error("vici error"), mSeverity(s), line(0)
		{
			buff = sevToString(s) + ": ";
			ptr = 0;
		}

	/// constructor
	/**
	 * \param s the severity of the exception
	 * \param f the file name of the source file
	 * \param ln the line number of the source file
	 */
	vxt( Severity s, const std::string &f, int ln )
		: runtime_error("vici error"), mSeverity(s), file(f), line(ln)
	{
		buff = sevToString(s) + ": ";
		ptr = buff.length();
		if ( ! file.empty() )
		{
			std::ostringstream ss;
			ss << " [" << file << ":" << line << "]";
			buff.append( ss.str() );
			file.clear();
		}
	}


	//
	// Use this one like this:
	// vx x; throw x << "Message" << value << etc;
	// or
	// throw vx( VICI::Alert ) << "message " << strerror( errno );
	//

	/// Provide stream syntax for the exception object.
	template < class T >
	vxt & operator << ( T x )
	{
		std::ostringstream ss;
		ss << x;
		if ( ptr )
		{
			buff.insert( ptr, ss.str() );
			ptr += ss.str().length();
		}
		else
		{
			buff.append( ss.str() );
		}

		return *this;
	}

	/// get the severity of the exception
	Severity severity() { return mSeverity; }

	/// get the message from the exception
	virtual const char *what() const throw()
	{
		return buff.c_str();
	}

	/// Convert a severity into a string
	static const std::string & sevToString( Severity s)
	{
		static std::vector< std::string >snames;
		if ( snames.size() == 0)
		{
			snames.push_back( "Emergency" );
			snames.push_back( "Alert" );
			snames.push_back( "Critical" );
			snames.push_back( "Error" );
			snames.push_back( "Code" );
			snames.push_back( "Warning" );
			snames.push_back( "Notice" );
			snames.push_back( "Info" );
			snames.push_back( "Debug" );
		}
		return snames[(int)s];
	}

};

// ----------------------------------------------------

class rdf_class;	// matches the namespace

} // namespace rdf

/// A project specific exception class.
using vx = rdf::vxt< rdf::rdf_class >;

/// Macro to include file name and line number in exception messages
// defined method so that file and line number are passed correctly.
// typically used like: throw VX(Error) << "message: " << strerror(errno);
#define VX(x) vx(x, __FILE__, __LINE__ )

#endif  // VX_H

// ----------------------------- end --------------------------------

