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
/*!\file    rrlib/xml/tCleanupHandler.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-12-24
 *
 * \brief   Contains tCleanupHandler
 *
 * \b tCleanupHandler
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__xml__tCleanupHandler_h__
#define __rrlib__xml__tCleanupHandler_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
extern "C"
{
#include <libxml/tree.h>
}

#include "rrlib/design_patterns/singleton.h"

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
//!
/*!
 *
 */
class tCleanupHandlerImplementation
{

//----------------------------------------------------------------------
// Public methods and typedefs
//----------------------------------------------------------------------
public:

  /*! The ctor of tCleanupHandler
   *
   */
  tCleanupHandlerImplementation()
  {}

  ~tCleanupHandlerImplementation()
  {
    xmlCleanupParser();
  }

};

typedef design_patterns::tSingletonHolder<tCleanupHandlerImplementation, design_patterns::singleton::PhoenixSingleton> tCleanupHandler;

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
