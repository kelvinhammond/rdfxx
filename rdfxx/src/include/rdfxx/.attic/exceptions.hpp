/* RDF C++ API 
 *
 * 			exceptions.hpp
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

#ifndef RDFXX_EXCEPTIONS_HPP
#define RDFXX_EXCEPTIONS_HPP

#include <iostream>

namespace rdf
{
    // using namespace std;

    //! RDF C++ base exception class.
    class Exception
    {
        protected:
        std::string type;
        std::string Where;
        std::string What;
    
        public:
        Exception() {}

        //! Exception constructor.
        /*! Initializes a new Exception.
         *
         *  @param _where Description in which method the error occured. 
         *  @param _what Description of what happened.
         */
        Exception(std::string& _where, std::string& _what)
        : Where(_where), What(_what) {}

        virtual ~Exception() {}
    
        //! Get error location description.
        /*! 
         * @return String describing in which method the error occured.
         */
        virtual std::string& where() { return Where; } 
    
        //! Get error description.
        /*! 
         * @return String describing what happened.
         */
        virtual std::string& what() { return What; }

        //! Print formatted description of exception to std::cerr.
        /*!
         *  @param _type Type of exception.
         */
        virtual void print();
    };
    
    //! Library exception.
    /*! This exception is thrown when one of the library functions
     *  used in RDF C++ returns a NULL pointer. This happens 
     *  when faulty/invalid parameters were given or there is
     *  not enough memory.
     */
    class LibraryError : public Exception
    {
        public:
        LibraryError(std::string& _where, std::string& _what);
    };

    //! Argument exception.
    /*! This indicates an invalid parameter to a object method.
     */
    class ArgumentError : public Exception
    {
        public:
        ArgumentError(std::string& _where, std::string& _what);
    };
    
    //! Memory allocation exception.
    /*! This exception is thrown when allocating objects
     *  with 'new' have failed. This normally means
     *  there is not enough memory left.
     */
    class AllocationError : public Exception 
    {
        public:
        AllocationError(const char* _where);
    };

    //! Statement error exception.
    /*! This exception is thrown when a invalid statement is
     *  encountered.  
     */
    class StatementError : public Exception 
    {
        public:
        StatementError(std::string& _where, std::string& _what);
    };
}
#endif
