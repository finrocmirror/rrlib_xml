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
// tXMLNode constructors
//----------------------------------------------------------------------
tXMLNode::tXMLNode(xmlNodePtr node)
    : node(node),
    name(0),
    children(0)
{
  assert(node);
  assert(node->type == XML_ELEMENT_NODE);
  if (node->type != XML_ELEMENT_NODE)
  {
    throw tXML2WrapperException("Trying to construct a tXMLNode from an xml2 element that is not a node!");
  }
}

//----------------------------------------------------------------------
// tXMLNode destructor
//----------------------------------------------------------------------
tXMLNode::~tXMLNode()
{
  delete this->name;
  delete this->children;
}

//----------------------------------------------------------------------
// tXMLNode GetChildren
//----------------------------------------------------------------------
const std::vector<tXMLNode> &tXMLNode::GetChildren() const
{
  if (!this->children)
  {
    this->children = new std::vector<tXMLNode>();
    for (xmlNodePtr child_node = this->node->children; child_node; child_node = child_node->next)
    {
      if (child_node->type == XML_ELEMENT_NODE)
      {
        this->children->push_back(tXMLNode(child_node));
      }
    }
  }
  return *this->children;
}

//----------------------------------------------------------------------
// tXMLNode GetName
//----------------------------------------------------------------------
const std::string &tXMLNode::GetName() const
{
  if (!this->name)
  {
    this->name = new std::string(reinterpret_cast<const char *>(this->node->name));
  }
  return *this->name;
}

//----------------------------------------------------------------------
// tXMLNode AddChildNode
//----------------------------------------------------------------------
tXMLNode &tXMLNode::AddChildNode(const std::string &name)
{
  xmlNodePtr child_node = xmlNewChild(this->node, 0, reinterpret_cast<const xmlChar*>(name.c_str()), 0);
  this->GetChildren();
  this->children->push_back(tXMLNode(child_node));
  return this->children->back();
}
