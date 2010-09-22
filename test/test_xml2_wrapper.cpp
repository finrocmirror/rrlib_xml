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

#include "xml2_wrapper/tXMLDocument.h"

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
  node1.SetTextContent("blubb");
  std::cout << "Content = " << node1.GetTextContent() << std::endl;
  node1.SetTextContent("fn0rd&<bla");
  std::cout << "Content = " << node1.GetTextContent() << std::endl;

  node1.RemoveAttribute("prop_4");
//  node1.RemoveTextContent();

//  tXMLNode &node2 = node1.AddChildNode("test2");
//  node2.SetAttribute("prop_1", "val_2");
//  node2.SetAttribute("prop_2", true);
//  node2.SetAttribute("prop_3", 4.3);
//  node2.SetAttribute("prop_4", 123);

//  node1.RemoveChildNode(node2);

  document1.WriteToFile("test.xml");

  tXMLDocument document2("test.xml", false);

  std::cout << document2.GetRootNode().GetName() << std::endl;
  std::cout << document2.GetRootNode().GetStringAttribute("prop_1") << std::endl;
  std::cout << document2.GetRootNode().GetBoolAttribute("prop_2") << std::endl;
  std::cout << document2.GetRootNode().GetDoubleAttribute("prop_3") << std::endl;
  std::cout << document2.GetRootNode().GetIntAttribute("prop_4") << std::endl;

  for (std::vector<tXMLNode>::const_iterator it = document2.GetRootNode().GetChildren().begin(); it != document2.GetRootNode().GetChildren().end(); ++it)
  {
    std::cout << it->GetName() << std::endl;
    std::cout << it->GetStringAttribute("prop_1") << std::endl;
    std::cout << it->GetBoolAttribute("prop_2") << std::endl;
    std::cout << it->GetDoubleAttribute("prop_3") << std::endl;
    if (it->HasAttribute("prop_4"))
    {
      std::cout << it->GetIntAttribute("prop_4") << std::endl;
    }
  }

  return EXIT_SUCCESS;
}
