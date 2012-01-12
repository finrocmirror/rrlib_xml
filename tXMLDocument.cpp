//
// You received this file as part of RRLib
// Robotics Research Library
//
// Copyright (C) Finroc GbR (finroc.org)
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
#include "rrlib/xml2_wrapper/tCleanupHandler.h"

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
  tCleanupHandler::Instance();
}

tXMLDocument::tXMLDocument(const std::string &file_name, bool validate)
    : document(xmlReadFile(file_name.c_str(), 0, validate ? XML_PARSE_DTDVALID : 0)),
    root_node(reinterpret_cast<tXMLNode *>(xmlDocGetRootElement(this->document)))
{
  assert(this->document);
  if (!this->document)
  {
    throw tXML2WrapperException("Could not parse XML file `" + file_name + "'!");
  }
  tCleanupHandler::Instance();
}

tXMLDocument::tXMLDocument(const void *buffer, size_t size, bool validate)
    : document(xmlReadMemory(reinterpret_cast<const char *>(buffer), size, "noname.xml", 0, validate ? XML_PARSE_DTDVALID : 0)),
    root_node(reinterpret_cast<tXMLNode *>(xmlDocGetRootElement(this->document)))
{
  if (!this->document)
  {
    throw tXML2WrapperException("Could not parse XML from memory buffer `" + std::string(reinterpret_cast<const char *>(buffer)) + "'!");
  }
  tCleanupHandler::Instance();
}

tXMLDocument::tXMLDocument(tXMLDocument && other)
    : document(0),
    root_node(0)
{
  std::swap(document, other.document);
  std::swap(root_node, other.root_node);
}

//----------------------------------------------------------------------
// tXMLDocument destructor
//----------------------------------------------------------------------
tXMLDocument::~tXMLDocument()
{
  if (this->document)
  {
    xmlFreeDoc(this->document);
  }
}

//----------------------------------------------------------------------
// tXMLDocument operator =
//----------------------------------------------------------------------
tXMLDocument &tXMLDocument::operator = (const tXMLDocument & other)
{
  if (this == &other)
  {
    return *this;
  }
  xmlFreeDoc(this->document);
  this->document = xmlCopyDoc(other.document, true);
  this->root_node = reinterpret_cast<tXMLNode *>(xmlDocGetRootElement(this->document));
  return *this;
}

//----------------------------------------------------------------------
// tXMLDocument RootNode
//----------------------------------------------------------------------
tXMLNode &tXMLDocument::RootNode()
{
  if (!this->root_node)
  {
    throw tXML2WrapperException("No root node defined for this document!");
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
  this->root_node = reinterpret_cast<tXMLNode *>(xmlNewNode(0, reinterpret_cast<const xmlChar *>(name.c_str())));
  xmlDocSetRootElement(this->document, this->root_node);
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
