//
// You received this file as part of RRLib
// Robotics Research Library
//
// Copyright (C) AG Robotersysteme TU Kaiserslautern
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//----------------------------------------------------------------------
/*!\file    tXML2WrapperException.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-26
 *
 * \brief   Contains tXML2WrapperException
 *
 * \b tXML2WrapperException
 *
 * Runtime errors that come along with invalid XML input are handled
 * by throwing an instance of tXML2WrapperException, which is the only
 * proper way to cope with construction errors.
 * tXML2WrapperException is derived from std::runtime_error and thus
 * can be caught either especially for this library or in a general
 * runtime_error block.
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__xml2_wrapper__tXML2WrapperException_h__
#define __rrlib__xml2_wrapper__tXML2WrapperException_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <stdexcept>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Namespace declaration
//----------------------------------------------------------------------
namespace rrlib
{
namespace xml2
{

//----------------------------------------------------------------------
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Class declaration
//----------------------------------------------------------------------
//! Exceptions thrown in RRLib XML2 Wrapper are of this type.
/*! This exception class is a std::runtime_error and used when invalid
 *  XML input is encountered that can not be handled automatically.
 *  Thus, catching exceptions of this type distinguishes from other runtime
 *  errors.
 *
 */
class tXML2WrapperException : public std::runtime_error
{

//----------------------------------------------------------------------
// Public methods
//----------------------------------------------------------------------
public:

  /*! The ctor of tXML2WrapperException
   *
   * This ctor forwards instantiation of an exception object to
   * std::runtime_error with the given message as error description.
   *
   * \param message   A message that describes the error
   */
  tXML2WrapperException(const std::string &message)
      : std::runtime_error(message)
  {}

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
