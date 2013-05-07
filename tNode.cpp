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
/*!\file    rrlib/xml/tNode.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-24
 *
 */
//----------------------------------------------------------------------
#include "rrlib/xml/tNode.h"

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
// tNode destructor
//----------------------------------------------------------------------
tNode::~tNode()
{
  static_assert(sizeof(tNode) == sizeof(xmlNode), "Do not add any variables or virtual methods to tNode!");
}

//----------------------------------------------------------------------
// tNode IsInSubtreeOf
//----------------------------------------------------------------------
const bool tNode::IsInSubtreeOf(const tNode &node) const
{
  const tNode *current_node = this;
  do
  {
    if (current_node == &node)
    {
      return true;
    }
  }
  while ((current_node = reinterpret_cast<tNode *>(current_node->parent)));
  return false;
}

//----------------------------------------------------------------------
// tNode Name
//----------------------------------------------------------------------
const std::string tNode::Name() const
{
  return reinterpret_cast<const char *>(this->name);
}

//----------------------------------------------------------------------
// tNode ChildrenEnd
//----------------------------------------------------------------------
const tNode::iterator &tNode::ChildrenEnd()
{
  static iterator end;
  return end;
}

const tNode::const_iterator &tNode::ChildrenEnd() const
{
  static const_iterator end;
  return end;
}

//----------------------------------------------------------------------
// tNode FirstChild
//----------------------------------------------------------------------
tNode &tNode::FirstChild()
{
  if (!this->HasChildren())
  {
    throw tException("Node has no children!");
  }
  return *this->ChildrenBegin();
}

//----------------------------------------------------------------------
// tNode AddChildNode
//----------------------------------------------------------------------
tNode &tNode::AddChildNode(const std::string &name, const std::string &content)
{
  const char* c = (content.length() == 0) ? NULL : content.c_str();
  return reinterpret_cast<tNode &>(*xmlNewChild(this, 0, reinterpret_cast<const xmlChar *>(name.c_str()), reinterpret_cast<const xmlChar *>(c)));
}

tNode &tNode::AddChildNode(tNode &node, bool copy)
{
  tNode *child = &node;
  if (copy)
  {
    child = reinterpret_cast<tNode *>(xmlDocCopyNode(child, this->doc, 1));
  }
  if (child->doc != this->doc)
  {
    xmlUnlinkNode(child);
  }
  if (this->IsInSubtreeOf(*child))
  {
    assert(!copy);
    throw tException("Cannot add node as child to its own subtree without copying!");
  }
  xmlAddChild(this, child);
  return *child;
}

//----------------------------------------------------------------------
// tNode RemoveChildNode
//----------------------------------------------------------------------
void tNode::RemoveChildNode(tNode &node)
{
  iterator it = std::find(this->ChildrenBegin(), this->ChildrenEnd(), node);
  if (it == this->ChildrenEnd())
  {
    throw tException("Given node is not a child of this!");
  }
  it->FreeNode();
}

//----------------------------------------------------------------------
// tNode NextSiblingsEnd
//----------------------------------------------------------------------
const tNode::iterator &tNode::NextSiblingsEnd()
{
  static iterator end;
  return end;
}

const tNode::const_iterator &tNode::NextSiblingsEnd() const
{
  static const_iterator end;
  return end;
}

//----------------------------------------------------------------------
// tNode NextSibling
//----------------------------------------------------------------------
tNode &tNode::NextSibling()
{
  if (!this->HasNextSibling())
  {
    throw tException("Node has no sibling!");
  }
  return *this->NextSiblingsBegin();
}

//----------------------------------------------------------------------
// tNode AddNextSibling
//----------------------------------------------------------------------
tNode &tNode::AddNextSibling(const std::string &name, const std::string &content)
{
  tNode *sibling = reinterpret_cast<tNode *>(xmlNewNode(0, reinterpret_cast<const xmlChar *>(name.c_str())));
  if (content != "")
  {
    xmlNodeSetContentLen(sibling, reinterpret_cast<const xmlChar *>(content.c_str()), content.length());
  }
  return reinterpret_cast<tNode &>(*xmlAddNextSibling(this, sibling));
}

tNode &tNode::AddNextSibling(tNode &node, bool copy)
{
  tNode *sibling = &node;
  if (copy)
  {
    sibling = reinterpret_cast<tNode *>(xmlDocCopyNode(sibling, this->doc, 1));
  }
  if (sibling->doc != this->doc)
  {
    xmlUnlinkNode(sibling);
  }
  if (this->IsInSubtreeOf(*sibling))
  {
    assert(!copy);
    throw tException("Cannot add node as sibling in its own subtree without copying!");
  }
  xmlAddNextSibling(this, sibling);
  return *sibling;
}

//----------------------------------------------------------------------
// tNode GetTextContent
//----------------------------------------------------------------------
const std::string tNode::GetTextContent() const
{
  xmlChar *content = xmlNodeGetContent(const_cast<tNode *>(this));
  std::string result(reinterpret_cast<const char *>(content));
  xmlFree(content);
  return result;
}

//----------------------------------------------------------------------
// tNode AddTextContent
//----------------------------------------------------------------------
void tNode::AddTextContent(const std::string &content)
{
  xmlNodeAddContentLen(this, reinterpret_cast<const xmlChar *>(content.c_str()), content.length());
}

//----------------------------------------------------------------------
// tNode SetContent
//----------------------------------------------------------------------
void tNode::SetContent(const std::string &content)
{
  xmlNodeSetContentLen(this, reinterpret_cast<const xmlChar *>(content.c_str()), content.length());
}

//----------------------------------------------------------------------
// tNode RemoveTextContent
//----------------------------------------------------------------------
void tNode::RemoveTextContent()
{
  std::vector<tNode *> nodes_to_delete;
  for (xmlNodePtr child_node = this->children; child_node; child_node = child_node->next)
  {
    if (xmlNodeIsText(child_node))
    {
      nodes_to_delete.push_back(reinterpret_cast<tNode *>(child_node));
    }
  }
  for (std::vector<tNode *>::iterator it = nodes_to_delete.begin(); it != nodes_to_delete.end(); ++it)
  {
    (*it)->FreeNode();
  }
}

//----------------------------------------------------------------------
// tNode SetStringAttribute
//----------------------------------------------------------------------
void tNode::SetStringAttribute(const std::string &name, const std::string &value, bool create)
{
  if (!this->HasAttribute(name))
  {
    if (create)
    {
      xmlNewProp(this, reinterpret_cast<const xmlChar *>(name.c_str()), reinterpret_cast<const xmlChar *>(value.c_str()));
      return;
    }
    throw tException("Attribute `" + name + "' does not exist in this node and creation was disabled!");
  }
  xmlSetProp(this, reinterpret_cast<const xmlChar *>(name.c_str()), reinterpret_cast<const xmlChar *>(value.c_str()));
}

//----------------------------------------------------------------------
// tNode RemoveAttribute
//----------------------------------------------------------------------
void tNode::RemoveAttribute(const std::string &name)
{
  xmlAttrPtr attr = xmlHasProp(this, reinterpret_cast<const xmlChar *>(name.c_str()));
  if (attr)
  {
    xmlRemoveProp(attr);
  }
}

//----------------------------------------------------------------------
// tNode GetXMLDump
//----------------------------------------------------------------------
const std::string tNode::GetXMLDump(bool format) const
{
  xmlBufferPtr buffer = xmlBufferCreate();
  xmlNodeDump(buffer, this->doc, const_cast<tNode *>(this), 0, format);
  std::string result(reinterpret_cast<const char *>(buffer->content));
  xmlBufferFree(buffer);
  return result;
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
