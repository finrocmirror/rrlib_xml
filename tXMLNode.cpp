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
/*!\file    tXMLNode.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-24
 *
 */
//----------------------------------------------------------------------
#include "rrlib/xml2_wrapper/tXMLNode.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <cstring>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

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
// tXMLNode constructors
//----------------------------------------------------------------------
tXMLNode::tXMLNode(xmlNodePtr node)
    : node(node),
    text_content(0)
{
  assert(node);
  assert(node->type == XML_ELEMENT_NODE);
  if (node->type != XML_ELEMENT_NODE)
  {
    throw tXML2WrapperException("Trying to construct a tXMLNode from an xml2 element that is not a node!");
  }
}

tXMLNode::tXMLNode(const tXMLNode &other)
    : node(other.node),
    text_content(0)
{}

//----------------------------------------------------------------------
// tXMLNode destructor
//----------------------------------------------------------------------
tXMLNode::~tXMLNode()
{
  delete this->text_content;
}

//----------------------------------------------------------------------
// tXMLNode operator =
//----------------------------------------------------------------------
const tXMLNode &tXMLNode::operator = (const tXMLNode & other)
{
  this->node = other.node;
  return *this;
}

//----------------------------------------------------------------------
// tXMLNode GetName
//----------------------------------------------------------------------
const std::string tXMLNode::GetName() const
{
  return reinterpret_cast<const char *>(this->node->name);
}

//----------------------------------------------------------------------
// tXMLNode AddChildNode
//----------------------------------------------------------------------
tXMLNode tXMLNode::AddChildNode(const std::string &name)
{
  if (this->HasTextContent())
  {
    throw tXML2WrapperException("Tried to add a structural child to a node that already has text content!");
  }
  return xmlNewChild(this->node, 0, reinterpret_cast<const xmlChar*>(name.c_str()), 0);
}

tXMLNode tXMLNode::AddChildNode(const tXMLNode &node)
{
  xmlNodePtr child = node.node;
  if (child->doc != this->node->doc)
  {
    child = xmlDocCopyNode(child, this->node->doc, 1);
  }
  xmlAddChild(this->node, child);
  return child;
}

//----------------------------------------------------------------------
// tXMLNode RemoveChildNode
//----------------------------------------------------------------------
void tXMLNode::RemoveChildNode(tXMLNode &node)
{
  xmlNodePtr child_node = 0;
  for (child_node = this->node->children; child_node && child_node != node.node; child_node = child_node->next);
  if (!child_node)
  {
    throw tXML2WrapperException("Given node is not a child of this.");
  }
  xmlUnlinkNode(child_node);
  xmlFreeNode(child_node);
}

//----------------------------------------------------------------------
// tXMLNode AddSibling
//----------------------------------------------------------------------
tXMLNode tXMLNode::AddSibling(const tXMLNode &node)
{
  assert(*this != node);
  xmlNodePtr sibling = node.node;
  if (sibling->doc != this->node->doc)
  {
    sibling = xmlDocCopyNode(sibling, this->node->doc, 1);
  }
  xmlAddNextSibling(this->node, sibling);
  return sibling;
}

//----------------------------------------------------------------------
// tXMLNode HasTextContent
//----------------------------------------------------------------------
const bool tXMLNode::HasTextContent() const
{
  for (xmlNodePtr child_node = this->node->children; child_node; child_node = child_node->next)
  {
    if (xmlNodeIsText(child_node))
    {
      xmlChar *text_content = xmlNodeGetContent(this->node);
      if (this->text_content && std::strncmp(reinterpret_cast<const char *>(text_content), this->text_content->c_str(), this->text_content->length()) != 0)
      {
        delete this->text_content;
        this->text_content = 0;
      }
      if (!this->text_content)
      {
        this->text_content = new std::string(reinterpret_cast<const char *>(text_content));
      }
      xmlFree(text_content);
    }
  }
  return this->text_content != 0;
}

//----------------------------------------------------------------------
// tXMLNode GetTextContent
//----------------------------------------------------------------------
const std::string &tXMLNode::GetTextContent() const
{
  if (!this->HasTextContent())
  {
    throw tXML2WrapperException("This node does not have any text content!");
  }
  return *this->text_content;
}

//----------------------------------------------------------------------
// tXMLNode SetTextContent
//----------------------------------------------------------------------
void tXMLNode::SetTextContent(const std::string &content)
{
  if (this->HasChildren())
  {
    throw tXML2WrapperException("Tried to set text content in a node that already has structural children!");
  }
  this->RemoveTextContent();
  xmlNodeAddContentLen(this->node, reinterpret_cast<const xmlChar *>(content.c_str()), content.length());
}

//----------------------------------------------------------------------
// tXMLNode RemoveTextNode
//----------------------------------------------------------------------
void tXMLNode::RemoveTextContent()
{
  std::vector<xmlNodePtr> text_nodes;
  for (xmlNodePtr child_node = this->node->children; child_node; child_node = child_node->next)
  {
    if (xmlNodeIsText(child_node))
    {
      text_nodes.push_back(child_node);
    }
  }
  for (std::vector<xmlNodePtr>::iterator it = text_nodes.begin(); it != text_nodes.end(); ++it)
  {
    xmlUnlinkNode(*it);
    xmlFreeNode(*it);
  }
  delete this->text_content;
  this->text_content = 0;
}

//----------------------------------------------------------------------
// tXMLNode SetStringAttribute
//----------------------------------------------------------------------
void tXMLNode::SetStringAttribute(const std::string &name, const std::string &value, bool create)
{
  if (!this->HasAttribute(name))
  {
    if (create)
    {
      xmlNewProp(this->node, reinterpret_cast<const xmlChar *>(name.c_str()), reinterpret_cast<const xmlChar *>(value.c_str()));
      return;
    }
    throw tXML2WrapperException("Attribute `" + name + "' does not exist in this node and creation was disabled!");
  }
  xmlSetProp(this->node, reinterpret_cast<const xmlChar *>(name.c_str()), reinterpret_cast<const xmlChar *>(value.c_str()));
}
