//
// You received this file as part of RRLib
// Robotics Research Library
//
// Copyright (C) Finroc GbR (finroc.org)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//----------------------------------------------------------------------
/*!\file    rrlib/xml/tException.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-26
 *
 * \brief   Contains tException
 *
 * \b tException
 *
 * Runtime errors that come along with invalid XML input are handled
 * by throwing an instance of tException, which is the only
 * proper way to cope with construction errors.
 * tException is derived from std::runtime_error and thus
 * can be caught either especially for this library or in a general
 * runtime_error block.
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__xml__tException_h__
#define __rrlib__xml__tException_h__

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
namespace xml
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
class tException : public std::runtime_error
{

//----------------------------------------------------------------------
// Public methods
//----------------------------------------------------------------------
public:

  /*! The ctor of tException
   *
   * This ctor forwards instantiation of an exception object to
   * std::runtime_error with the given message as error description.
   *
   * \param message   A message that describes the error
   */
  tException(const std::string &message)
    : std::runtime_error(message)
  {}

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
