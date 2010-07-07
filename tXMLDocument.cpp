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
/*!\file    tXMLDocument.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-24
 *
 * \brief
 *
 * \b
 *
 * A few words for tXMLDocument.cpp
 *
 */
//----------------------------------------------------------------------
#include "tXMLDocument.h"

//----------------------------------------------------------------------
// External includes with <>
//----------------------------------------------------------------------
#include <cassert>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/xml2_wrapper/tXML2WrapperException.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Namespace usage
//----------------------------------------------------------------------
using namespace rrlib::xml2;

//----------------------------------------------------------------------
// Forward declarations
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Const values
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// class tXMLDocument constructors
//----------------------------------------------------------------------
tXMLDocument::tXMLDocument(const std::string &file_name, bool validate)
    : document(xmlReadFile(file_name.c_str(), 0, validate ? XML_PARSE_DTDVALID : 0)),
    root_node(0)
{
  assert(this->document);
  if (!this->document)
  {
    throw tXML2WrapperException("Could not parse XML file `" + file_name + "'!");
  }
}

//----------------------------------------------------------------------
// class tXMLDocument destructor
//----------------------------------------------------------------------
tXMLDocument::~tXMLDocument()
{
  delete this->root_node;
  xmlFreeDoc(this->document);
}

//----------------------------------------------------------------------
// class tXMLDocument GetRootNode
//----------------------------------------------------------------------
const tXMLNode &tXMLDocument::GetRootNode() const
{
  if (!this->root_node)
  {
    this->root_node = new tXMLNode(xmlDocGetRootElement(this->document));
  }
  return *this->root_node;
}
