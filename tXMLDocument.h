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
/*!\file    tXMLDocument.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-24
 *
 * \brief Contains tXMLDocument
 *
 * \b tXMLDocument
 *
 * If an XML document is loaded for full access to its content, a DOM
 * tree is generated consisting of nodes of with attributes. This class
 * implements parsing and validating an XML file as well as accessing
 * the DOM tree through instances of tXMLNode, featuring lazy evaluation.
 * That means wrapping instances are not created before they are used.
 *
 */
//----------------------------------------------------------------------
#ifndef rrlib_xml2_wrapper_tXMLDocument_h_
#define rrlib_xml2_wrapper_tXMLDocument_h_

//----------------------------------------------------------------------
// External includes with <>
//----------------------------------------------------------------------
#include <string>

extern "C"
{
#include <libxml/tree.h>
}
//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/xml2_wrapper/tXMLNode.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Namespace declaration
//----------------------------------------------------------------------
namespace rrlib
{
namespace xml2
{

//----------------------------------------------------------------------
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Class declaration
//----------------------------------------------------------------------
//! This class wraps creation and accessing the DOM tree of an XML document
/*! If an XML document is loaded for full access to its content, a DOM
 *  tree is generated consisting of nodes with attributes. This class
 *  implements parsing and validating an XML file as well as accessing
 *  the DOM tree through instances of tXMLNode, featuring lazy evaluation.
 *  That means wrapping instances are not created before they are used.
 *
 */
class tXMLDocument
{
  xmlDocPtr document;
  mutable tXMLNode *root_node;

//----------------------------------------------------------------------
// Public methods
//----------------------------------------------------------------------
public:

  /*! The ctor of tXMLDocument
   *
   * This ctor reads and parses a file with given name into a XML DOM
   * representation.
   * If needed, the XML document is also validated using an included
   * DTD specification.
   *
   * \param file_name   The name of the file to load
   * \param validate    Whether the validation should be processed or not
   *
   * \exception tXML2WrapperException is thrown if the file was not found or could not be parsed
   */
  tXMLDocument(const std::string &file_name, bool validate = true);

  /*! The dtor of tXMLDocument
   */
  ~tXMLDocument();

  /*! Get the root node of the DOM tree stored for this document
   *
   * The XML document is stored as DOM tree in memory. This method
   * provides node-wise access to this tree starting at its root.
   *
   * \returns A reference to the root node
   */
  const tXMLNode &GetRootNode() const;

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
