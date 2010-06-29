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
 * \brief
 *
 * \b
 *
 * A few words for tXML2WrapperException.h
 *
 */
//----------------------------------------------------------------------
#ifndef rrlib_xml2_wrapper_tXML2WrapperException_h_
#define rrlib_xml2_wrapper_tXML2WrapperException_h_

//----------------------------------------------------------------------
// External includes with <>
//----------------------------------------------------------------------
#include <exception>
#include <string>

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
//! Short description of tXML2WrapperException
/*! A more detailed description of tXML2WrapperException, which
 *  Tobias Foehst hasn't done yet!
 *
 */
class tXML2WrapperException : public std::exception
{
  std::string message;

//----------------------------------------------------------------------
// Public methods
//----------------------------------------------------------------------
public:

  tXML2WrapperException(const std::string &message)
      : message(message)
  {}

  virtual ~tXML2WrapperException() throw()
  {}

  virtual const char *what() const throw()
  {
    return this->message.c_str();
  }
};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
