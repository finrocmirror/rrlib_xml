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
/*!\file    tCleanupHandler.h
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
#ifndef _rrlib_xml2_wrapper_tCleanupHandler_h_
#define _rrlib_xml2_wrapper_tCleanupHandler_h_

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
extern "C"
{
#include <libxml/tree.h>
}

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/util/patterns/singleton.h"

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
//!
/*!
 *
 */
class tCleanupHandlerImplementation
{

//----------------------------------------------------------------------
// Public methods
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

typedef rrlib::util::tSingletonHolder<tCleanupHandlerImplementation, rrlib::util::CreateStatic, rrlib::util::PhoenixSingleton> tCleanupHandler;

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
