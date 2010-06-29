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
/*!\file    tXMLDocument.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-24
 *
 * \brief
 *
 * \b
 *
 * A few words for tXMLDocument.h
 *
 */
//----------------------------------------------------------------------
#ifndef rrlib_xml2_wrapper_tXMLDocument_h_
#define rrlib_xml2_wrapper_tXMLDocument_h_

//----------------------------------------------------------------------
// External includes with <>
//----------------------------------------------------------------------
#include <string>

extern "C"
{
#include <libxml/tree.h>
}
//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/xml2_wrapper/tXMLNode.h"

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
//! Short description of tXMLDocument
/*! A more detailed description of tXMLDocument, which
 *  Tobias Foehst hasn't done yet!
 *
 */
class tXMLDocument
{
  xmlDocPtr document;

//----------------------------------------------------------------------
// Public methods
//----------------------------------------------------------------------
public:

  tXMLDocument(const std::string &file_name, bool validate = true);

  /*! The dtor of tXMLDocument
   */
  ~tXMLDocument();

  inline const tXMLNode GetRootElement() const
  {
    return tXMLNode(xmlDocGetRootElement(this->document));
  }

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
