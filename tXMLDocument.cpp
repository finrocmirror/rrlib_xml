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
 */
//----------------------------------------------------------------------
#include "rrlib/xml2_wrapper/tXMLDocument.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <iostream>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/xml2_wrapper/tXML2WrapperException.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------
#include <cassert>

//----------------------------------------------------------------------
// Namespace usage
//----------------------------------------------------------------------
using namespace rrlib::xml2;

//----------------------------------------------------------------------
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Const values
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// tXMLDocument constructors
//----------------------------------------------------------------------
tXMLDocument::tXMLDocument()
    : document(xmlNewDoc(reinterpret_cast<const xmlChar *>("1.0"))),
    root_node(0)
{
  assert(this->document);
}

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
// tXMLDocument destructor
//----------------------------------------------------------------------
tXMLDocument::~tXMLDocument()
{
  delete this->root_node;
  xmlFreeDoc(this->document);
}

//----------------------------------------------------------------------
// tXMLDocument GetRootNode
//----------------------------------------------------------------------
tXMLNode &tXMLDocument::GetRootNode() const
{
  if (!this->root_node)
  {
    xmlNodePtr node = xmlDocGetRootElement(this->document);
    if (!node)
    {
      throw tXML2WrapperException("No root node defined for this document!");
    }
    this->root_node = new tXMLNode(xmlDocGetRootElement(this->document));
  }
  return *this->root_node;
}

//----------------------------------------------------------------------
// tXMLDocument AddRootNode
//----------------------------------------------------------------------
tXMLNode &tXMLDocument::AddRootNode(const std::string &name)
{
  if (this->root_node)
  {
    throw tXML2WrapperException("Root node already exists with name `" + name + "'!");
  }
  this->root_node = new tXMLNode(xmlNewNode(0, reinterpret_cast<const xmlChar *>(name.c_str())));
  xmlDocSetRootElement(this->document, this->root_node->data->node);
  return *this->root_node;
}

//----------------------------------------------------------------------
// tXMLDocument WriteToFile
//----------------------------------------------------------------------
void tXMLDocument::WriteToFile(const std::string &file_name, int compression) const
{
  if (compression)
  {
    xmlSetDocCompressMode(this->document, compression);
  }
  xmlSaveFormatFileEnc(file_name.c_str(), this->document, "UTF-8", 1);
}
