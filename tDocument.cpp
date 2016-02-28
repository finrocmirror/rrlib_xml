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
/*!\file    rrlib/xml/tDocument.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-24
 *
 */
//----------------------------------------------------------------------
#include "rrlib/xml/tDocument.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <iostream>
#include <memory>

extern "C"
{
#include <libxml/xpath.h>
}

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/xml/tException.h"
#include "rrlib/xml/tCleanupHandler.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------
#include <cassert>

//----------------------------------------------------------------------
// Namespace usage
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
// Const values
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// tDocument constructors
//----------------------------------------------------------------------
tDocument::tDocument()
  : document(xmlNewDoc(reinterpret_cast<const xmlChar *>("1.0"))),
    root_node(0)
{
  assert(this->document);
  tCleanupHandler::Instance();
}

tDocument::tDocument(const std::string &file_name, bool validate)
  : document(xmlReadFile(file_name.c_str(), 0, validate ? XML_PARSE_DTDVALID : 0)),
    root_node(reinterpret_cast<tNode *>(xmlDocGetRootElement(this->document)))
{
  this->CheckIfDocumentIsValid("Could not parse XML file `" + file_name + "'!");
  tCleanupHandler::Instance();
}

tDocument::tDocument(const std::string &file_name, const std::string &encoding, bool validate)
  : document(xmlReadFile(file_name.c_str(), encoding.c_str(), validate ? XML_PARSE_DTDVALID : 0)),
    root_node(reinterpret_cast<tNode *>(xmlDocGetRootElement(this->document)))
{
  this->CheckIfDocumentIsValid("Could not parse XML file `" + file_name + "'!");
  tCleanupHandler::Instance();
}

tDocument::tDocument(const void *buffer, size_t size, bool validate)
  : document(xmlReadMemory(reinterpret_cast<const char *>(buffer), size, "noname.xml", 0, validate ? XML_PARSE_DTDVALID : 0)),
    root_node(reinterpret_cast<tNode *>(xmlDocGetRootElement(this->document)))
{
  this->CheckIfDocumentIsValid("Could not parse XML from memory buffer `" + std::string(reinterpret_cast<const char *>(buffer)) + "'!");
  tCleanupHandler::Instance();
}

tDocument::tDocument(const void *buffer, size_t size, const std::string &encoding, bool validate)
  : document(xmlReadMemory(reinterpret_cast<const char *>(buffer), size, "noname.xml", encoding.c_str(), validate ? XML_PARSE_DTDVALID : 0)),
    root_node(reinterpret_cast<tNode *>(xmlDocGetRootElement(this->document)))
{
  this->CheckIfDocumentIsValid("Could not parse XML from memory buffer `" + std::string(reinterpret_cast<const char *>(buffer)) + "'!");
  tCleanupHandler::Instance();
}

tDocument::tDocument(tDocument && other)
  : document(0),
    root_node(0)
{
  std::swap(document, other.document);
  std::swap(root_node, other.root_node);
}

//----------------------------------------------------------------------
// tDocument destructor
//----------------------------------------------------------------------
tDocument::~tDocument()
{
  if (this->document)
  {
    xmlFreeDoc(this->document);
  }
}

//----------------------------------------------------------------------
// tDocument operator =
//----------------------------------------------------------------------
tDocument &tDocument::operator = (const tDocument & other)
{
  if (this == &other)
  {
    return *this;
  }
  xmlFreeDoc(this->document);
  this->document = xmlCopyDoc(other.document, true);
  this->root_node = reinterpret_cast<tNode *>(xmlDocGetRootElement(this->document));
  return *this;
}

//----------------------------------------------------------------------
// tDocument RootNode
//----------------------------------------------------------------------
tNode &tDocument::RootNode()
{
  if (!this->root_node)
  {
    throw tException("No root node defined for this document!");
  }
  return *this->root_node;
}

//----------------------------------------------------------------------
// tDocument AddRootNode
//----------------------------------------------------------------------
tNode &tDocument::AddRootNode(const std::string &name)
{
  if (this->root_node)
  {
    throw tException("Root node already exists with name `" + name + "'!");
  }
  this->root_node = reinterpret_cast<tNode *>(xmlNewNode(0, reinterpret_cast<const xmlChar *>(name.c_str())));
  xmlDocSetRootElement(this->document, this->root_node);
  return *this->root_node;
}

//----------------------------------------------------------------------
// tDocument FindNode
//----------------------------------------------------------------------
const tNode &tDocument::FindNode(const std::string &name) const
{
  auto cleanup_path_context = [](xmlXPathContext * ptr)
  {
    xmlXPathFreeContext(ptr);
  }; // clean up the XPath context
  std::unique_ptr<xmlXPathContext, decltype(cleanup_path_context)> xpath_context(xmlXPathNewContext(this->document), cleanup_path_context); // create and initialize an XPath context for this document with auto cleanup

  if (xpath_context == nullptr)
  {
    throw tException("Could not create the XPath context!");
  }

  auto cleanup_path_object = [](xmlXPathObject * ptr)
  {
    xmlXPathFreeObject(ptr);
  }; // clean up the XPath object
  std::unique_ptr<xmlXPathObject, decltype(cleanup_path_object)> xpath_object(xmlXPathEvalExpression((xmlChar*)name.c_str(), xpath_context.get()), cleanup_path_object); // create and initialize an XPath object as query result with auto cleanup

  if (xpath_object == nullptr)
  {
    throw tException("Could not evaluate the XPath expression!");
  }

  if ((xpath_object->nodesetval == nullptr) || (xpath_object->nodesetval->nodeTab == nullptr))
  {
    throw tException("Could not find the expected node!");
  }
  xmlNode *node = xpath_object->nodesetval->nodeTab[0]; // fetch node from query result
  return *reinterpret_cast<tNode *>(node);
}

//----------------------------------------------------------------------
// tDocument WriteToFile
//----------------------------------------------------------------------
void tDocument::WriteToFile(const std::string &file_name, int compression) const
{
  if (compression)
  {
    xmlSetDocCompressMode(this->document, compression);
  }
  xmlSaveFormatFileEnc(file_name.c_str(), this->document, "UTF-8", 1);
}

//----------------------------------------------------------------------
// tDocument CheckIfDocumentIsValid
//----------------------------------------------------------------------
void tDocument::CheckIfDocumentIsValid(const std::string &exception_message)
{
  if (!this->document)
  {
    throw tException(exception_message);
  }
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
