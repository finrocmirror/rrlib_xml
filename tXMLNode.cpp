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
#include "xml2_wrapper/tXMLNode.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <cassert>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------

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
// tXMLNode::tData constructors
//----------------------------------------------------------------------
tXMLNode::tData::tData(xmlNodePtr node)
    : ref_counter(1),
    node(node),
    name(0),
    children(0),
    text_content(0)
{
  assert(node);
  assert(node->type == XML_ELEMENT_NODE);
  if (node->type != XML_ELEMENT_NODE)
  {
    throw tXML2WrapperException("Trying to construct a tXMLNode from an xml2 element that is not a node!");
  }
}

//----------------------------------------------------------------------
// tXMLNode::tData destructor
//----------------------------------------------------------------------
tXMLNode::tData::~tData()
{
  delete this->name;
  delete this->children;
  delete this->text_content;
//  xmlUnlinkNode(this->node);
//  xmlFreeNode(this->node);
}

//----------------------------------------------------------------------
// tXMLNode constructors
//----------------------------------------------------------------------
tXMLNode::tXMLNode(xmlNodePtr node)
    : data(new tData(node))
{}

tXMLNode::tXMLNode(const tXMLNode &other)
    : data(other.data)
{
  this->data->ref_counter++;
}

//----------------------------------------------------------------------
// tXMLNode destructor
//----------------------------------------------------------------------
tXMLNode::~tXMLNode()
{
  if (--this->data->ref_counter == 0)
  {
    delete this->data;
  }
}

//----------------------------------------------------------------------
// tXMLNode GetName
//----------------------------------------------------------------------
const std::string &tXMLNode::GetName() const
{
  if (!this->data->name)
  {
    this->data->name = new std::string(reinterpret_cast<const char *>(this->data->node->name));
  }
  return *this->data->name;
}

//----------------------------------------------------------------------
// tXMLNode GetChildren
//----------------------------------------------------------------------
const std::vector<tXMLNode> &tXMLNode::GetChildren() const
{
  if (!this->data->children)
  {
    this->data->children = new std::vector<tXMLNode>();
    for (xmlNodePtr child_node = this->data->node->children; child_node; child_node = child_node->next)
    {
      if (child_node->type == XML_ELEMENT_NODE)
      {
        this->data->children->push_back(tXMLNode(child_node));
      }
    }
  }
  return *this->data->children;
}

//----------------------------------------------------------------------
// tXMLNode AddChildNode
//----------------------------------------------------------------------
tXMLNode &tXMLNode::AddChildNode(const std::string &name)
{
  if (this->HasTextContent())
  {
    throw tXML2WrapperException("Tried to add a structural child to a node that already has text content!");
  }
  xmlNodePtr child_node = xmlNewChild(this->data->node, 0, reinterpret_cast<const xmlChar*>(name.c_str()), 0);
  this->GetChildren();
  this->data->children->push_back(tXMLNode(child_node));
  return this->data->children->back();
}

//----------------------------------------------------------------------
// tXMLNode RemoveChildNode
//----------------------------------------------------------------------
void tXMLNode::RemoveChildNode(tXMLNode &node)
{
  this->GetChildren();
  std::vector<tXMLNode>::iterator child_node = std::find(this->data->children->begin(), this->data->children->end(), node);
  if (child_node == this->data->children->end())
  {
    throw tXML2WrapperException("Given node is not a child of this.");
  }
  xmlUnlinkNode(child_node->data->node);
  xmlFreeNode(child_node->data->node);
  this->data->children->erase(child_node);
}

//----------------------------------------------------------------------
// tXMLNode HasTextContent
//----------------------------------------------------------------------
const bool tXMLNode::HasTextContent() const
{
  if (!this->data->text_content)
  {
    for (xmlNodePtr child_node = this->data->node->children; child_node; child_node = child_node->next)
    {
      if (xmlNodeIsText(child_node))
      {
        xmlChar *text_content = xmlNodeGetContent(this->data->node);
        this->data->text_content = new std::string(reinterpret_cast<const char *>(text_content));
        xmlFree(text_content);
      }
    }
  }
  return this->data->text_content != 0;
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
  return *this->data->text_content;
}

//----------------------------------------------------------------------
// tXMLNode SetTextContent
//----------------------------------------------------------------------
void tXMLNode::SetTextContent(const std::string &content)
{
  this->GetChildren();
  if (!this->data->children->empty())
  {
    throw tXML2WrapperException("Tried to set text content in a node that already has structural children!");
  }
  this->RemoveTextContent();
  xmlNodeAddContentLen(this->data->node, reinterpret_cast<const xmlChar *>(content.c_str()), content.length());
}

//----------------------------------------------------------------------
// tXMLNode RemoveTextNode
//----------------------------------------------------------------------
void tXMLNode::RemoveTextContent()
{
  std::vector<xmlNodePtr> text_nodes;
  for (xmlNodePtr child_node = this->data->node->children; child_node; child_node = child_node->next)
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
  delete this->data->text_content;
  this->data->text_content = 0;
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
      xmlNewProp(this->data->node, reinterpret_cast<const xmlChar *>(name.c_str()), reinterpret_cast<const xmlChar *>(value.c_str()));
      return;
    }
    throw tXML2WrapperException("Attribute `" + name + "' does not exist in this node and creation was disabled!");
  }
  xmlSetProp(this->data->node, reinterpret_cast<const xmlChar *>(name.c_str()), reinterpret_cast<const xmlChar *>(value.c_str()));
}
