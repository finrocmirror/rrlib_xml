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
/*!\file    rrlib/xml/tests/test.cpp
 *
 * \author  Tobias Föhst
 *
 * \date    2010-08-26
 *
 */
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include "rrlib/util/tUnitTestSuite.h"

#include <cstdlib>
#include <unistd.h>

#include "rrlib/xml/tDocument.h"

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
class Test : public util::tUnitTestSuite
{
  RRLIB_UNIT_TESTS_BEGIN_SUITE(Test);
  RRLIB_UNIT_TESTS_ADD_TEST(CreateDocument);
  RRLIB_UNIT_TESTS_ADD_TEST(AccessAttributes);
  RRLIB_UNIT_TESTS_ADD_TEST(Children);
  RRLIB_UNIT_TESTS_ADD_TEST(WriteReadFile);
  RRLIB_UNIT_TESTS_ADD_TEST(Exceptions);
  RRLIB_UNIT_TESTS_ADD_TEST(Content);
  RRLIB_UNIT_TESTS_END_SUITE;

private:

  tDocument document;

  virtual void InitializeTests() override
  {
    tNode &root_node = document.AddRootNode("test");
    root_node.SetAttribute("prop_1", "val");
    root_node.SetAttribute("prop_2", true);
    root_node.SetAttribute("prop_3", 4.3);
    root_node.SetAttribute("prop_4", 123);
    tNode &node1 = root_node.AddChildNode("test1");
    node1.SetAttribute("prop_1", "val_1");
    node1.SetAttribute("prop_2", true);
    node1.SetAttribute("prop_3", 4.3);
    node1.SetAttribute("prop_4", 123);

    node1.RemoveAttribute("prop_4");

    tNode &node2 = node1.AddChildNode("test2");
    node2.SetAttribute("prop_1", "val_2");
    node2.SetAttribute("prop_2", true);
    node2.SetAttribute("prop_3", 4.3);
    node2.SetAttribute("prop_4", 123);

    node1.RemoveChildNode(node2);
  }

  void CreateDocument()
  {
    RRLIB_UNIT_TESTS_EQUALITY(std::string("<test prop_1=\"val\" prop_2=\"true\" prop_3=\"4.3\" prop_4=\"123\"><test1 prop_1=\"val_1\" prop_2=\"true\" prop_3=\"4.3\"/></test>"), this->document.RootNode().GetXMLDump());
  }

  void AccessAttributes()
  {
    RRLIB_UNIT_TESTS_EQUALITY(std::string("test"), this->document.RootNode().Name());
    RRLIB_UNIT_TESTS_EQUALITY(std::string("val"), this->document.RootNode().GetStringAttribute("prop_1"));
    RRLIB_UNIT_TESTS_EQUALITY(true, this->document.RootNode().GetBoolAttribute("prop_2"));
    RRLIB_UNIT_TESTS_EQUALITY(4.3, this->document.RootNode().GetDoubleAttribute("prop_3"));
    RRLIB_UNIT_TESTS_EQUALITY(123, this->document.RootNode().GetIntAttribute("prop_4"));
  }

  void Children()
  {
    RRLIB_UNIT_TESTS_EQUALITY(size_t(1), std::distance(this->document.RootNode().ChildrenBegin(), this->document.RootNode().ChildrenEnd()));
  }

  void WriteReadFile()
  {
    char filename[] = "/tmp/tmp.XXXXXX";
    close(mkstemp(filename));

    this->document.WriteToFile(filename);
    tDocument read(std::string(filename), false);

    remove(filename);

    RRLIB_UNIT_TESTS_EQUALITY(this->document.RootNode().GetXMLDump(true), read.RootNode().GetXMLDump(true));
  }

  void Exceptions()
  {
    RRLIB_UNIT_TESTS_EXCEPTION(this->document.RootNode().FirstChild().AddNextSibling(this->document.RootNode().FirstChild()), tException);
    RRLIB_UNIT_TESTS_EXCEPTION(tDocument().RootNode().GetXMLDump(), tException);
  }

  void Content()
  {
    tDocument doc;

    doc.AddRootNode("foo");

    xmlNewChild(reinterpret_cast<xmlNode *>(&doc.RootNode()), 0, reinterpret_cast<const xmlChar *>("child1"), reinterpret_cast<const xmlChar *>("text1"));
    xmlNewChild(reinterpret_cast<xmlNode *>(&doc.RootNode().FirstChild()), 0, reinterpret_cast<const xmlChar *>("child2"), reinterpret_cast<const xmlChar *>("text2"));

    xmlNodeAddContent(reinterpret_cast<xmlNode *>(&doc.RootNode().FirstChild()), reinterpret_cast<const xmlChar *>("text3"));

    RRLIB_UNIT_TESTS_EQUALITY(std::string("<foo><child1>text1<child2>text2</child2>text3</child1></foo>"), doc.RootNode().GetXMLDump());

    RRLIB_UNIT_TESTS_EQUALITY(std::string("text1text2text3"), doc.RootNode().FirstChild().GetTextContent());

    RRLIB_UNIT_TESTS_EQUALITY(std::string("<child2>text2</child2>"), doc.FindNode("/foo/child1/child2").GetXMLDump());

    doc.RootNode().FirstChild().RemoveTextContent();

    RRLIB_UNIT_TESTS_EQUALITY(std::string("text2"), doc.RootNode().FirstChild().GetTextContent());

    RRLIB_UNIT_TESTS_EQUALITY(std::string("<child2>text2</child2>"), doc.RootNode().FirstChild().ChildrenBegin()->GetXMLDump());

    this->document.RootNode().AddChildNode(doc.RootNode().FirstChild());

    RRLIB_UNIT_TESTS_EQUALITY(std::string("<test prop_1=\"val\" prop_2=\"true\" prop_3=\"4.3\" prop_4=\"123\"><test1 prop_1=\"val_1\" prop_2=\"true\" prop_3=\"4.3\"/><child1><child2>text2</child2></child1></test>"), this->document.RootNode().GetXMLDump());

    RRLIB_UNIT_TESTS_EQUALITY(std::string("<test1 prop_1=\"val_1\" prop_2=\"true\" prop_3=\"4.3\"/>"), this->document.FindNode("/test/test1").GetXMLDump());

    RRLIB_UNIT_TESTS_EXCEPTION(this->document.FindNode("/test/test2"), tException);
  }
};

RRLIB_UNIT_TESTS_REGISTER_SUITE(Test);

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
