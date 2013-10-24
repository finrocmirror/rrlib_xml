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
/*!\file    rrlib/xml/test/test.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-08-26
 *
 */
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <cstdlib>
#include <iostream>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/xml/tDocument.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------
#include <cassert>

//----------------------------------------------------------------------
// Namespace usage
//----------------------------------------------------------------------
using namespace rrlib::xml;

//----------------------------------------------------------------------
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Const values
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

int main(int argc, char **argv)
{
  tDocument document1;
  tNode &root_node = document1.AddRootNode("test");
  root_node.SetAttribute("prop_1", "val");
  root_node.SetAttribute("prop_2", true);
  root_node.SetAttribute("prop_3", 4.3);
  root_node.SetAttribute("prop_4", 123);
  tNode &node1 = root_node.AddChildNode("test1");
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

  tNode &node2 = node1.AddChildNode("test2");
  node2.SetAttribute("prop_1", "val_2");
  node2.SetAttribute("prop_2", true);
  node2.SetAttribute("prop_3", 4.3);
  node2.SetAttribute("prop_4", 123);

//  node2.AddNextSibling(node1);

  node1.RemoveChildNode(node2);

  std::cout << "Accessing node with content: " << document1.RootNode().GetXMLDump() << std::endl;

  std::cout << document1.RootNode().Name() << std::endl;
  std::cout << document1.RootNode().GetStringAttribute("prop_1") << std::endl;
  std::cout << document1.RootNode().GetBoolAttribute("prop_2") << std::endl;
  std::cout << document1.RootNode().GetDoubleAttribute("prop_3") << std::endl;
  std::cout << document1.RootNode().GetIntAttribute("prop_4") << std::endl;

  for (tNode::iterator it = document1.RootNode().ChildrenBegin(); it != document1.RootNode().ChildrenEnd(); ++it)
  {
    std::cout << "Accessing node with content: " << it->GetXMLDump() << std::endl;

    std::cout << it->Name() << std::endl;
    std::cout << it->GetStringAttribute("prop_1") << std::endl;
    std::cout << it->GetBoolAttribute("prop_2") << std::endl;
    std::cout << it->GetDoubleAttribute("prop_3") << std::endl;
    if (it->HasAttribute("prop_4"))
    {
      std::cout << it->GetIntAttribute("prop_4") << std::endl;
    }
  }

  document1.WriteToFile("test.xml");

  tDocument document2(std::string("test.xml"), false);

  std::cout << "doc 2" << std::endl;

  std::cout << "Accessing node with content: " << document2.RootNode().GetXMLDump()
            << " and " << document2.RootNode().GetNumberOfChildren() << " children." << std::endl;

  std::cout << document2.RootNode().Name() << std::endl;
  std::cout << document2.RootNode().GetStringAttribute("prop_1") << std::endl;
  std::cout << document2.RootNode().GetBoolAttribute("prop_2") << std::endl;
  std::cout << document2.RootNode().GetDoubleAttribute("prop_3") << std::endl;
  std::cout << document2.RootNode().GetIntAttribute("prop_4") << std::endl;

  document2.RootNode().FirstChild().AddNextSibling(document1.RootNode().FirstChild());

  std::cout << "Accessing node with content: " << document2.RootNode().GetXMLDump(true) << std::endl;

  for (tNode::iterator it = document2.RootNode().ChildrenBegin(); it != document2.RootNode().ChildrenEnd(); ++it)
  {
    std::cout << "Accessing node with content: " << it->GetXMLDump() << std::endl;

    std::cout << "name = " << it->Name() << std::endl;
    std::cout << it->GetStringAttribute("prop_1") << std::endl;
    std::cout << it->GetBoolAttribute("prop_2") << std::endl;
    std::cout << it->GetDoubleAttribute("prop_3") << std::endl;
    if (it->HasAttribute("prop_4"))
    {
      std::cout << it->GetIntAttribute("prop_4") << std::endl;
    }
  }

  std::cout << "doc 1: " << document1.RootNode().GetXMLDump() << std::endl;
  std::cout << "doc 2: " << document2.RootNode().GetXMLDump() << std::endl;

  {
    tDocument document3;

    document3.AddRootNode("foo");

    xmlNewChild(reinterpret_cast<xmlNode *>(&document3.RootNode()), 0, reinterpret_cast<const xmlChar *>("child1"), reinterpret_cast<const xmlChar *>("text1"));
    xmlNewChild(reinterpret_cast<xmlNode *>(&document3.RootNode().FirstChild()), 0, reinterpret_cast<const xmlChar *>("child2"), reinterpret_cast<const xmlChar *>("text2"));

    xmlNodeAddContent(reinterpret_cast<xmlNode *>(&document3.RootNode().FirstChild()), reinterpret_cast<const xmlChar *>("text3"));

    std::cout << "doc 3: " << document3.RootNode().GetXMLDump() << std::endl;

    std::cout << "content = " << document3.RootNode().FirstChild().GetTextContent() << std::endl;

    document3.RootNode().FirstChild().RemoveTextContent();

    std::cout << "content = " << document3.RootNode().FirstChild().GetTextContent() << std::endl;

    for (tNode::iterator it = document3.RootNode().FirstChild().ChildrenBegin(); it != document3.RootNode().FirstChild().ChildrenEnd(); ++it)
    {
      std::cout << "child: " << it->GetXMLDump() << std::endl;
    }

    document1.RootNode().AddChildNode(document3.RootNode().FirstChild());
  }


  std::cout << "doc 1: " << document1.RootNode().GetXMLDump() << std::endl
            << " has " << document1.RootNode().GetNumberOfChildren() << " children." << std::endl;

  return EXIT_SUCCESS;
}
