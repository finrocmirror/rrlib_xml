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
 * \brief
 *
 * \b
 *
 * A few words for tXMLNode.cpp
 *
 */
//----------------------------------------------------------------------
#include "tXMLNode.h"

//----------------------------------------------------------------------
// External includes with <>
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
// Forward declarations
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Const values
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// class tXMLNode constructors
//----------------------------------------------------------------------
tXMLNode::tXMLNode(xmlNodePtr node)
    : node(node),
    name(0),
    children(0)
{
  assert(node->type == XML_ELEMENT_NODE);
  if (node->type != XML_ELEMENT_NODE)
  {
    throw tXML2WrapperException("Trying to construct a tXMLNode from an xml2 element that is not a node!");
  }
}

//----------------------------------------------------------------------
// class tXMLNode destructor
//----------------------------------------------------------------------
tXMLNode::~tXMLNode()
{
  delete this->name;
}

//----------------------------------------------------------------------
// class tXMLNode GetChildren
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
// class tXMLNode GetName
//----------------------------------------------------------------------
const std::string &tXMLNode::GetName() const
{
  if (!this->name)
  {
    this->name = new std::string(reinterpret_cast<const char *>(this->node->name));
  }
  return *this->name;
}
