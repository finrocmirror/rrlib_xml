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
// tXMLNode destructor
//----------------------------------------------------------------------
tXMLNode::~tXMLNode()
{
  static_assert(sizeof(tXMLNode) == sizeof(xmlNode), "Do not add any variables or virtual methods to tXMLNode!");
}

//----------------------------------------------------------------------
// tXMLNode IsInSubtreeOf
//----------------------------------------------------------------------
const bool tXMLNode::IsInSubtreeOf(const tXMLNode &node) const
{
  const tXMLNode *current_node = this;
  do
  {
    if (current_node == &node)
    {
      return true;
    }
  }
  while ((current_node = reinterpret_cast<tXMLNode *>(current_node->parent)));
  return false;
}

//----------------------------------------------------------------------
// tXMLNode GetName
//----------------------------------------------------------------------
const std::string tXMLNode::GetName() const
{
  return reinterpret_cast<const char *>(this->name);
}

//----------------------------------------------------------------------
// tXMLNode GetChildrenEnd
//----------------------------------------------------------------------
const tXMLNode::iterator &tXMLNode::GetChildrenEnd()
{
  static iterator end;
  return end;
}

const tXMLNode::const_iterator &tXMLNode::GetChildrenEnd() const
{
  static const_iterator end;
  return end;
}

//----------------------------------------------------------------------
// tXMLNode GetFirstChild
//----------------------------------------------------------------------
tXMLNode &tXMLNode::GetFirstChild()
{
  if (!this->HasChildren())
  {
    throw tXML2WrapperException("Node has no children!");
  }
  return *this->GetChildrenBegin();
}

//----------------------------------------------------------------------
// tXMLNode AddChildNode
//----------------------------------------------------------------------
tXMLNode &tXMLNode::AddChildNode(const std::string &name, const std::string &content)
{
  return reinterpret_cast<tXMLNode &>(*xmlNewChild(this, 0, reinterpret_cast<const xmlChar *>(name.c_str()), reinterpret_cast<const xmlChar *>(content.c_str())));
}

tXMLNode &tXMLNode::AddChildNode(tXMLNode &node, bool copy)
{
  tXMLNode *child = &node;
  if (copy)
  {
    child = reinterpret_cast<tXMLNode *>(xmlDocCopyNode(child, this->doc, 1));
  }
  if (child->doc != this->doc)
  {
    xmlUnlinkNode(child);
  }
  if (this->IsInSubtreeOf(*child))
  {
    assert(!copy);
    throw tXML2WrapperException("Cannot add node as child to its own subtree without copying!");
  }
  xmlAddChild(this, child);
  return *child;
}

//----------------------------------------------------------------------
// tXMLNode RemoveChildNode
//----------------------------------------------------------------------
void tXMLNode::RemoveChildNode(tXMLNode &node)
{
  iterator it = std::find(this->GetChildrenBegin(), this->GetChildrenEnd(), node);
  if (it == this->GetChildrenEnd())
  {
    throw tXML2WrapperException("Given node is not a child of this!");
  }
  it->FreeNode();
}

//----------------------------------------------------------------------
// tXMLNode GetNextSiblingsEnd
//----------------------------------------------------------------------
const tXMLNode::iterator &tXMLNode::GetNextSiblingsEnd()
{
  static iterator end;
  return end;
}

const tXMLNode::const_iterator &tXMLNode::GetNextSiblingsEnd() const
{
  static const_iterator end;
  return end;
}

//----------------------------------------------------------------------
// tXMLNode GetNextSibling
//----------------------------------------------------------------------
tXMLNode &tXMLNode::GetNextSibling()
{
  if (!this->HasNextSibling())
  {
    throw tXML2WrapperException("Node has no sibling!");
  }
  return *this->GetNextSiblingsBegin();
}

//----------------------------------------------------------------------
// tXMLNode AddNextSibling
//----------------------------------------------------------------------
tXMLNode &tXMLNode::AddNextSibling(const std::string &name, const std::string &content)
{
  tXMLNode *sibling = reinterpret_cast<tXMLNode *>(xmlNewNode(0, reinterpret_cast<const xmlChar *>(name.c_str())));
  if (content != "")
  {
    xmlNodeSetContentLen(sibling, reinterpret_cast<const xmlChar *>(content.c_str()), content.length());
  }
  return reinterpret_cast<tXMLNode &>(*xmlAddNextSibling(this, sibling));
}

tXMLNode &tXMLNode::AddNextSibling(tXMLNode &node, bool copy)
{
  tXMLNode *sibling = &node;
  if (copy)
  {
    sibling = reinterpret_cast<tXMLNode *>(xmlDocCopyNode(sibling, this->doc, 1));
  }
  if (sibling->doc != this->doc)
  {
    xmlUnlinkNode(sibling);
  }
  if (this->IsInSubtreeOf(*sibling))
  {
    assert(!copy);
    throw tXML2WrapperException("Cannot add node as sibling in its own subtree without copying!");
  }
  xmlAddNextSibling(this, sibling);
  return *sibling;
}

//----------------------------------------------------------------------
// tXMLNode GetTextContent
//----------------------------------------------------------------------
const std::string tXMLNode::GetTextContent() const
{
  xmlChar *content = xmlNodeGetContent(const_cast<tXMLNode *>(this));
  std::string result(reinterpret_cast<const char *>(content));
  xmlFree(content);
  return result;
}

//----------------------------------------------------------------------
// tXMLNode AddTextContent
//----------------------------------------------------------------------
void tXMLNode::AddTextContent(const std::string &content)
{
  xmlNodeAddContentLen(this, reinterpret_cast<const xmlChar *>(content.c_str()), content.length());
}

//----------------------------------------------------------------------
// tXMLNode SetContent
//----------------------------------------------------------------------
void tXMLNode::SetContent(const std::string &content)
{
  xmlNodeSetContentLen(this, reinterpret_cast<const xmlChar *>(content.c_str()), content.length());
}

//----------------------------------------------------------------------
// tXMLNode RemoveTextContent
//----------------------------------------------------------------------
void tXMLNode::RemoveTextContent()
{
  std::vector<tXMLNode *> nodes_to_delete;
  for (xmlNodePtr child_node = this->children; child_node; child_node = child_node->next)
  {
    if (xmlNodeIsText(child_node))
    {
      nodes_to_delete.push_back(reinterpret_cast<tXMLNode *>(child_node));
    }
  }
  for (std::vector<tXMLNode *>::iterator it = nodes_to_delete.begin(); it != nodes_to_delete.end(); ++it)
  {
    (*it)->FreeNode();
  }
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
      xmlNewProp(this, reinterpret_cast<const xmlChar *>(name.c_str()), reinterpret_cast<const xmlChar *>(value.c_str()));
      return;
    }
    throw tXML2WrapperException("Attribute `" + name + "' does not exist in this node and creation was disabled!");
  }
  xmlSetProp(this, reinterpret_cast<const xmlChar *>(name.c_str()), reinterpret_cast<const xmlChar *>(value.c_str()));
}

//----------------------------------------------------------------------
// tXMLNode RemoveAttribute
//----------------------------------------------------------------------
void tXMLNode::RemoveAttribute(const std::string &name)
{
  xmlAttrPtr attr = xmlHasProp(this, reinterpret_cast<const xmlChar *>(name.c_str()));
  if (attr)
  {
    xmlRemoveProp(attr);
  }
}

//----------------------------------------------------------------------
// tXMLNode GetXMLDump
//----------------------------------------------------------------------
const std::string tXMLNode::GetXMLDump(bool format) const
{
  xmlBufferPtr buffer = xmlBufferCreate();
  xmlNodeDump(buffer, this->doc, const_cast<tXMLNode *>(this), 0, format);
  std::string result(reinterpret_cast<const char *>(buffer->content));
  xmlBufferFree(buffer);
  return result;
}
