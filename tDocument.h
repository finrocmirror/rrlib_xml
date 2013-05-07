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
/*!\file    rrlib/xml/tDocument.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-09-18
 *
 * \brief   Contains tDocument
 *
 * \b tDocument
 *
 * If an XML document is loaded for full access to its content, a DOM
 * tree is generated consisting of nodes of with attributes. This class
 * implements parsing and validating an XML file as well as accessing
 * the DOM tree through instances of tNode, featuring lazy evaluation.
 * That means wrapping instances are not created before they are used.
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__xml__tDocument_h__
#define __rrlib__xml__tDocument_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <string>

extern "C"
{
#include <libxml/tree.h>
}
//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/xml/tNode.h"

//----------------------------------------------------------------------
// Debugging
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
// Class declaration
//----------------------------------------------------------------------
//! This class wraps creation and accessing the DOM tree of an XML document
/*! If an XML document is loaded for full access to its content, a DOM
 *  tree is generated consisting of nodes with attributes. This class
 *  implements parsing and validating an XML file as well as accessing
 *  the DOM tree through instances of tNode, featuring lazy evaluation.
 *  That means wrapping instances are not created before they are used.
 *
 */
class tDocument
{

//----------------------------------------------------------------------
// Public methods and typedefs
//----------------------------------------------------------------------
public:

  /*! The ctor of an empty tDocument
   *
   * This ctor creates a new xml document
   */
  tDocument();

  /*! The ctor of tDocument from a given file
   *
   * This ctor reads and parses a file with given name into a XML DOM
   * representation.
   * If needed, the XML document is also validated using an included
   * DTD specification.
   *
   * \exception tException is thrown if the file was not found or could not be parsed
   *
   * \param file_name   The name of the file to load
   * \param validate    Whether the validation should be processed or not
   */
  explicit tDocument(const std::string &file_name, bool validate = true);

  /*! The ctor of tDocument from a given file with explicit encoding
   *
   * This ctor reads and parses a file with given name into a XML DOM
   * representation.
   * If needed, the XML document is also validated using an included
   * DTD specification.
   *
   * \exception tException is thrown if the file was not found or could not be parsed
   *
   * \param file_name   The name of the file to load
   * \param encoding    The encoding of the input file
   * \param validate    Whether the validation should be processed or not
   */
  explicit tDocument(const std::string &file_name, const std::string &encoding, bool validate = true);

  /*! The ctor of tDocument from a memory buffer
   *
   * This ctor reads and parses XML content given in a memory buffer into a XML DOM
   * representation.
   * If needed, the XML document is also validated using an included
   * DTD specification.
   *
   * \exception tException is thrown if the memory buffer could not be parsed
   *
   * \param buffer      Pointer to the memory buffer with XML content to be parsed
   * \param size        Size of the memory buffer
   * \param validate    Whether the validation should be processed or not
   */
  tDocument(const void *buffer, size_t size, bool validate = true);

  /*! The ctor of tDocument from a memory buffer with explicit encoding
   *
   * This ctor reads and parses XML content given in a memory buffer into a XML DOM
   * representation.
   * If needed, the XML document is also validated using an included
   * DTD specification.
   *
   * \exception tException is thrown if the memory buffer could not be parsed
   *
   * \param buffer      Pointer to the memory buffer with XML content to be parsed
   * \param size        Size of the memory buffer
   * \param encoding    The encoding of the input file
   * \param validate    Whether the validation should be processed or not
   */
  tDocument(const void *buffer, size_t size, const std::string &encoding, bool validate = true);

  /*!
   * move constructor
   */
  tDocument(tDocument && other);

  /*! The dtor of tDocument
   */
  ~tDocument();

  /*! Assign operator for tDocument
   */
  tDocument& operator=(const tDocument& other);

  /*! Get the root node of the DOM tree stored for this document
   *
   * The XML document is stored as DOM tree in memory. This method
   * provides node-wise access to this tree starting at its root.
   *
   * \returns A reference to the root node
   */
  tNode &RootNode();

  inline const tNode &RootNode() const
  {
    return const_cast<tDocument *>(this)->RootNode();
  }

  /*! Add a root node to a new document in DOM representation
   *
   * If you create a new XML document the first thing to do is to
   * add a root node with a specified name. After that, the root node
   * is fixed and additional calls to this method will throw an exception.
   *
   * \exception tException is thrown if the document already had a root node
   *
   * \param name   The name of the root node
   *
   * \returns A reference to the newly created root node
   */
  tNode &AddRootNode(const std::string &name);

  /*! Write the XML document to a file
   *
   * This method creates or truncates a file with the given name and writes
   * the documents XML representation into it.
   *
   * \param file_name     The name of the file to use
   * \param compression   Compression level [0-9] where 0 is "no compression"
   */
  void WriteToFile(const std::string &file_name, int compression = 0) const;

//----------------------------------------------------------------------
// Private fields and methods
//----------------------------------------------------------------------
private:

  xmlDocPtr document;
  mutable tNode *root_node;

  tDocument(const tDocument&); // generated copy-constructor is not safe

  void CheckIfDocumentIsValid(const std::string &exception_message);

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
