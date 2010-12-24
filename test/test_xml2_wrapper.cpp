//
// You received this file as part of MCA2
// Modular Controller Architecture Version 2
//
//Copyright (C) AG Robotersysteme TU Kaiserslautern
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-
//----------------------------------------------------------------------
/*!\file    test_xml2_wrapper.cpp
 *
 * \author  Tobias Foehst
 * \date    2010-08-26
 *
 */
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// non MCA Includes - include with <>
// MCA Includes - include with ""
//----------------------------------------------------------------------
#include <cstdlib>
#include <iostream>

#include "rrlib/xml2_wrapper/tXMLDocument.h"

using namespace rrlib::xml2;
//----------------------------------------------------------------------
// typedefs and enums
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// defines and consts
// global vars (prefer static class vars!)
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Forward class declarations
// Extern methods
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------
#include <cassert>

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

int main(int argc, char **argv)
{
  tXMLDocument document1;
  tXMLNode &root_node = document1.AddRootNode("test");
  root_node.SetAttribute("prop_1", "val");
  root_node.SetAttribute("prop_2", true);
  root_node.SetAttribute("prop_3", 4.3);
  root_node.SetAttribute("prop_4", 123);
  tXMLNode &node1 = root_node.AddChildNode("test1");
  node1.SetAttribute("prop_1", "val_1");
  node1.SetAttribute("prop_2", true);
  node1.SetAttribute("prop_3", 4.3);
  node1.SetAttribute("prop_4", 123);
//  node1.SetTextContent("blubb");
//  std::cout << "Content = " << node1.GetTextContent() << std::endl;
//  node1.SetTextContent("fn0rd&<bla");
//  std::cout << "Content = " << node1.GetTextContent() << std::endl;
//
  node1.RemoveAttribute("prop_4");
////  node1.RemoveTextContent();

  tXMLNode &node2 = node1.AddChildNode("test2");
  node2.SetAttribute("prop_1", "val_2");
  node2.SetAttribute("prop_2", true);
  node2.SetAttribute("prop_3", 4.3);
  node2.SetAttribute("prop_4", 123);

//  node2.AddNextSibling(node1);

  node1.RemoveChildNode(node2);

  std::cout << "Accessing node with content: " << document1.GetRootNode().GetXMLDump() << std::endl;

  std::cout << document1.GetRootNode().GetName() << std::endl;
  std::cout << document1.GetRootNode().GetStringAttribute("prop_1") << std::endl;
  std::cout << document1.GetRootNode().GetBoolAttribute("prop_2") << std::endl;
  std::cout << document1.GetRootNode().GetDoubleAttribute("prop_3") << std::endl;
  std::cout << document1.GetRootNode().GetIntAttribute("prop_4") << std::endl;

  for (tXMLNode::iterator it = document1.GetRootNode().GetChildrenBegin(); it != document1.GetRootNode().GetChildrenEnd(); ++it)
  {
    std::cout << "Accessing node with content: " << it->GetXMLDump() << std::endl;

    std::cout << it->GetName() << std::endl;
    std::cout << it->GetStringAttribute("prop_1") << std::endl;
    std::cout << it->GetBoolAttribute("prop_2") << std::endl;
    std::cout << it->GetDoubleAttribute("prop_3") << std::endl;
    if (it->HasAttribute("prop_4"))
    {
      std::cout << it->GetIntAttribute("prop_4") << std::endl;
    }
  }

  document1.WriteToFile("test.xml");

  tXMLDocument document2("test.xml", false);

  std::cout << "doc 2" << std::endl;

  std::cout << "Accessing node with content: " << document2.GetRootNode().GetXMLDump() << std::endl;

  std::cout << document2.GetRootNode().GetName() << std::endl;
  std::cout << document2.GetRootNode().GetStringAttribute("prop_1") << std::endl;
  std::cout << document2.GetRootNode().GetBoolAttribute("prop_2") << std::endl;
  std::cout << document2.GetRootNode().GetDoubleAttribute("prop_3") << std::endl;
  std::cout << document2.GetRootNode().GetIntAttribute("prop_4") << std::endl;

  document2.GetRootNode().GetFirstChild().AddNextSibling(document1.GetRootNode().GetFirstChild());

  std::cout << "Accessing node with content: " << document2.GetRootNode().GetXMLDump(true) << std::endl;

  for (tXMLNode::iterator it = document2.GetRootNode().GetChildrenBegin(); it != document2.GetRootNode().GetChildrenEnd(); ++it)
  {
    std::cout << "Accessing node with content: " << it->GetXMLDump() << std::endl;

    std::cout << "name = " << it->GetName() << std::endl;
    std::cout << it->GetStringAttribute("prop_1") << std::endl;
    std::cout << it->GetBoolAttribute("prop_2") << std::endl;
    std::cout << it->GetDoubleAttribute("prop_3") << std::endl;
    if (it->HasAttribute("prop_4"))
    {
      std::cout << it->GetIntAttribute("prop_4") << std::endl;
    }
  }

  std::cout << "doc 1: " << document1.GetRootNode().GetXMLDump() << std::endl;
  std::cout << "doc 2: " << document2.GetRootNode().GetXMLDump() << std::endl;

  tXMLDocument document3;

  document3.AddRootNode("foo");

  xmlNewChild(reinterpret_cast<xmlNode *>(&document3.GetRootNode()), 0, reinterpret_cast<const xmlChar *>("child1"), reinterpret_cast<const xmlChar *>("text1"));
  xmlNewChild(reinterpret_cast<xmlNode *>(&document3.GetRootNode().GetFirstChild()), 0, reinterpret_cast<const xmlChar *>("child2"), reinterpret_cast<const xmlChar *>("text2"));

  xmlNodeAddContent(reinterpret_cast<xmlNode *>(&document3.GetRootNode().GetFirstChild()), reinterpret_cast<const xmlChar *>("text3"));

  std::cout << "doc 3: " << document3.GetRootNode().GetXMLDump() << std::endl;

  std::cout << "content = " << document3.GetRootNode().GetFirstChild().GetTextContent() << std::endl;

  document3.GetRootNode().GetFirstChild().RemoveTextContent();

  std::cout << "content = " << document3.GetRootNode().GetFirstChild().GetTextContent() << std::endl;

  for (tXMLNode::iterator it = document3.GetRootNode().GetFirstChild().GetChildrenBegin(); it != document3.GetRootNode().GetFirstChild().GetChildrenEnd(); ++it)
  {
    std::cout << "child: " << it->GetXMLDump() << std::endl;
  }



  return EXIT_SUCCESS;
}
