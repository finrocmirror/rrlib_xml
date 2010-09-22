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
/*!\file    tXMLNode.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-24
 *
 * \brief Contains tXMLNode
 *
 * \b tXMLNode
 *
 * If an XML document is loaded for full access to its content, a DOM
 * tree is generated consisting of nodes with attributes. This class
 * implements the interface between libxml2 data storage and C++ types,
 * featuring lazy evaluation. That means instances are not created before
 * they are used.
 *
 */
//----------------------------------------------------------------------
#ifndef _rrlib_xml2_wrapper_tXMLNode_h_
#define _rrlib_xml2_wrapper_tXMLNode_h_

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
extern "C"
{
#include <libxml/tree.h>
}

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cerrno>
#include <cstdlib>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "xml2_wrapper/tXML2WrapperException.h"

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
//! This class wraps accessing the nodes of the DOM tree of an XML document
/*! If an XML document is loaded for full access to its content, a DOM
 *  tree is generated consisting of nodes with attributes. This class
 *  implements the interface between libxml2 data storage and C++ types,
 *  featuring lazy evaluation. That means instances are not created before
 *  they are used.
 *
 */
class tXMLNode
{
  friend class tXMLDocument;

  struct tData
  {
    unsigned int ref_counter;
    xmlNodePtr node;
    mutable std::string *name;
    mutable std::vector<tXMLNode> *children;
    mutable std::string *text_content;
    tData(xmlNodePtr node);
    ~tData();
  };

  tData *data;

  /*! The ctor of tXMLNode
   *
   * This ctor is declared private and thus can only be called from other instances
   * of tXMLNode or friends like tXMLDocument.
   *
   * \exception tXML2WrapperException is thrown if the given libxml2 element is not a node
   *
   * \param node   The libxml2 node that is wrapped by the new object
   */
  tXMLNode(xmlNodePtr node);

  template <typename TNumber>
  static const TNumber ConvertStringToNumber(const std::string &value, TNumber(&convert_function)(const char *, char **, int), int base)
  {
    errno = 0;
    char *endptr;
    TNumber result = convert_function(value.c_str(), &endptr, base);
    if (errno || *endptr)
    {
      printf("errno = %d\n*endptr = '%c'\n", errno, *endptr);
      printf("ERANGE = %d\n", ERANGE);
      throw tXML2WrapperException("Could not convert `" + value + "' to number!");
    }
    return result;
  }

  template <typename TNumber>
  static const TNumber ConvertStringToNumber(const std::string &value, TNumber(&convert_function)(const char *, char **))
  {
    errno = 0;
    char *endptr;
    TNumber result = convert_function(value.c_str(), &endptr);
    if (errno || *endptr)
    {
      printf("errno = %d\n*endptr = '%c'\n", errno, *endptr);
      printf("ERANGE = %d\n", ERANGE);
      throw tXML2WrapperException("Could not convert `" + value + "' to number!");
    }
    return result;
  }

  void SetStringAttribute(const std::string &name, const std::string &value, bool create);

//----------------------------------------------------------------------
// Public methods
//----------------------------------------------------------------------
public:

  tXMLNode(const tXMLNode &other);

  /*! The dtor of tXMLNode
   */
  ~tXMLNode();

  /*! Comparison of XML node objects for find algorithm
   *
   * \param other   The other node to compare to this one
   *
   * \returns Whether the two nodes are the same or not
   */
  bool operator == (const tXMLNode &other)
  {
    return this->data == other.data;
  }

  /*! Get the name of that node
   *
   * Each XML element has an unique name within its document type. This
   * method provides access to the name of this node.
   *
   * \returns A reference to the node's name
   */
  const std::string &GetName() const;

  /*! Get the children of this node
   *
   * Within the DOM tree, each node can have a list of children. Access
   * to this list is provided by this method. The internal vector the
   * method returns a reference to is not created before the first call
   * to this method (lazy evaluation)
   *
   * \returns A reference to the node's vector containing its children
   */
  const std::vector<tXMLNode> &GetChildren() const;

  /*! Add a child to this node
   *
   * In XML DOM trees a node can have several child nodes which are XML
   * nodes themselves. This method add such a node with a given name to
   * the structure which then can be extended by further children or
   * attributes.
   *
   * \note Each node can either have structural child nodes or text content,
   * but not both at the same time.
   *
   * \exception tXML2WrapperException is thrown if the node already contains text content
   *
   * \param name   The name of the new node
   *
   * \returns A reference to the newly created node
   */
  tXMLNode &AddChildNode(const std::string &name);

  /*! Remove a structural child node
   *
   * Removes a given node from the children list of this node.
   *
   * \exception tXML2WrapperException is thrown if the given node is not a child node
   *
   * \param node   The node to remove from the list
   */
  void RemoveChildNode(tXMLNode &node);

  /*! Get whether this node has text content or not
   *
   * Instead of structural child nodes each node can have plain text content.
   * This method determines the existence of text content and creates an
   * internal representation for fast access (lazy evaluation). Furthermore,
   * calling this method befor accessing the text content can be used to
   * avoid runtim errors in form of instances of tXML2WrapperException.
   *
   * \returns Whether this node has plain text content or not
   */
  const bool HasTextContent() const;

  /*! Get the plain text content of this node
   *
   * If the node contains plain text content this method grants access via
   * a std::string reference.
   *
   * \exception tXML2WrapperException is thrown if the node does not contain plain text content
   *
   * \returns A reference to the plain text content
   */
  const std::string &GetTextContent() const;

  /*! Set the plain text content of this node
   *
   * \exception tXML2WrapperException is thrown if the node already has structural children
   *
   * \param content   The new plain text content of this node
   */
  void SetTextContent(const std::string &content);

  /*! Remove the plain text content of this node
   */
  void RemoveTextContent();

  /*! Get whether this node has the given attribute or not
   *
   * Each XML node can have several attributes. Calling this method
   * before accessing an attribute using its name gives information
   * about its availability and thus can be used to avoid runtime
   * errors in form of instances of tXML2WrapperException.
   *
   * \returns Whether this node has the given attribute or not
   */
  inline const bool HasAttribute(const std::string &name) const
  {
    return xmlHasProp(this->data->node, reinterpret_cast<const xmlChar *>(name.c_str())) != 0;
  }

  /*! Get an XML attribute as std::string
   *
   * If the XML node wrapped by this instance has an attribute with
   * the given name, its value is returned by this method as string.
   *
   * \exception tXML2WrapperException is thrown if the requested attribute is not available
   *
   * \param name   The name of the attribute
   *
   * \returns The attribute as std::string
   */
  inline const std::string GetStringAttribute(const std::string &name) const
  {
    xmlChar *temp = xmlGetProp(this->data->node, reinterpret_cast<const xmlChar *>(name.c_str()));
    if (!temp)
    {
      throw tXML2WrapperException("Requested attribute `" + name + "' does not exist in this node!");
    }
    std::string result(reinterpret_cast<char *>(temp));
    xmlFree(temp);
    return result;
  }

  /*! Get an XML attribute as int
   *
   * If the XML node wrapped by this instance has an attribute with
   * the given name, its value is returned by this method as int.
   *
   * \exception tXML2WrapperException is thrown if the requested attribute's value is available or not a number
   *
   * \param name   The name of the attribute
   * \param base   The base that should be used for number interpretation
   *
   * \returns The attribute as int
   */
  inline const int GetIntAttribute(const std::string &name, int base = 10) const
  {
    return this->GetLongIntAttribute(name, base);
  }

  /*! Get an XML attribute as long int
   *
   * If the XML node wrapped by this instance has an attribute with
   * the given name, its value is returned by this method as long int.
   *
   * \exception tXML2WrapperException is thrown if the requested attribute's value is available or not a number
   *
   * \param name   The name of the attribute
   * \param base   The base that should be used for number interpretation
   *
   * \returns The attribute as long int
   */
  inline const long int GetLongIntAttribute(const std::string &name, int base = 10) const
  {
    return tXMLNode::ConvertStringToNumber(this->GetStringAttribute(name), strtol, base);
  }

  /*! Get an XML attribute as long long int
   *
   * If the XML node wrapped by this instance has an attribute with
   * the given name, its value is returned by this method as long long
   * int.
   *
   * \exception tXML2WrapperException is thrown if the requested attribute's value is available or not a number
   *
   * \param name   The name of the attribute
   * \param base   The base that should be used for number interpretation
   *
   * \returns The attribute as long long int
   */
  inline const long long int GetLongLongIntAttribute(const std::string &name, int base = 10) const
  {
    return tXMLNode::ConvertStringToNumber(this->GetStringAttribute(name), strtoll, base);
  }

  /*! Get an XML attribute as float
   *
   * If the XML node wrapped by this instance has an attribute with
   * the given name, its value is returned by this method as float.
   *
   * \exception tXML2WrapperException is thrown if the requested attribute's value is available or not a number
   *
   * \param name   The name of the attribute
   *
   * \returns The attribute as float
   */
  inline const float GetFloatAttribute(const std::string &name) const
  {
    return tXMLNode::ConvertStringToNumber(this->GetStringAttribute(name), strtof);
  }

  /*! Get an XML attribute as double
   *
   * If the XML node wrapped by this instance has an attribute with
   * the given name, its value is returned by this method as double.
   *
   * \exception tXML2WrapperException is thrown if the requested attribute's value is available or not a number
   *
   * \param name   The name of the attribute
   *
   * \returns The attribute as double
   */
  inline const double GetDoubleAttribute(const std::string &name) const
  {
    return tXMLNode::ConvertStringToNumber(this->GetStringAttribute(name), strtod);
  }

  /*! Get an XML attribute as long double
   *
   * If the XML node wrapped by this instance has an attribute with
   * the given name, its value is returned by this method as long double.
   *
   * \exception tXML2WrapperException is thrown if the requested attribute's value is available or not a number
   *
   * \param name   The name of the attribute
   *
   * \returns The attribute as long double
   */
  inline const long double GetLongDoubleAttribute(const std::string &name) const
  {
    return tXMLNode::ConvertStringToNumber(this->GetStringAttribute(name), strtold);
  }

  /*! Get an XML attribute as enum
   *
   * If the XML node wrapped by this instance has an attribute with
   * the given name, its value is interpreted as name of an element
   * in an enumeration. Therefore, a vector with the enum's names must
   * be provided. The method then returns the index of the name that
   * was found in the attribute.
   *
   * \exception tXML2WrapperException is thrown if the requested attribute's value is available or not a member of given vector
   *
   * \param name         The name of the attribute
   * \param enum_names   The names of the enumeration elements
   *
   * \returns The index of the matching element name
   */
  template <typename TEnum>
  inline const TEnum GetEnumAttribute(const std::string &name, const std::vector<std::string> &enum_names) const
  {
    const std::string value = this->GetStringAttribute(name);
    std::vector<std::string>::const_iterator it = std::find(enum_names.begin(), enum_names.end(), value);
    if (it == enum_names.end())
    {
      throw tXML2WrapperException("Invalid value for " + this->GetName() + "." + name + ": `" + value + "'");
    }
    return static_cast<TEnum>(std::distance(enum_names.begin(), it));
  }

  /*! Get an XML attribute as bool
   *
   * If the XML node wrapped by this instance has an attribute with
   * the given name, its value is returned by this method interpreted
   * as bool.
   *
   * \exception tXML2WrapperException is thrown if the requested attribute's value is available or not true/false
   *
   * \param name   The name of the attribute
   *
   * \returns Whether the attribute's value was "true" or "false"
   */
  inline const bool GetBoolAttribute(const std::string &name) const
  {
    static const char *bool_names_init[2] = { "false", "true" };
    static const std::vector<std::string> bool_names(bool_names_init, bool_names_init + 2);
    // FIXME: with c++0x this can be static const std::vector<std::string> bool_strings = { "false", "true" };

    return this->GetEnumAttribute<bool>(name, bool_names);
  }

  /*! Set an XML attribute of this node
   *
   * This methods sets the attribute with the given name to the given value.
   * If the node does not have the specified attribute yet, it will be created
   * depending on the given parameter.
   *
   * \note The value type must support streaming to be serialized
   *
   * \exception tXML2WrapperException is thrown if the requested attribute does not exist and should not be created
   *
   * \param name     The name of the attribute
   * \param value    The new value
   * \param create   Whether a non-existing attribute should be created or not
   */
  template <typename TValue>
  inline void SetAttribute(const std::string &name, const TValue &value, bool create = true)
  {
    std::stringstream converted_value;
    converted_value << value;
    this->SetStringAttribute(name, converted_value.str(), create);
  }

  /*! Set a bool XML attribute of this node
   *
   * This is a method for special handling of bool values which should be serialized
   * into "true" and "false" instead of int representation.
   *
   * \exception tXML2WrapperException is thrown if the requested attribute does not exist and should not be created
   *
   * \param name     The name of the attribute
   * \param value    The new value
   * \param create   Whether a non-existing attribute should be created or not
   */
  inline void SetAttribute(const std::string &name, bool value, bool create = true)
  {
    this->SetStringAttribute(name, value ? "true" : "false", create);
  }

  /*! Set a string XML attribute of this node
   *
   * This is a method for special handling of string values which do not have to be
   * serialized via stringstream.
   *
   * \exception tXML2WrapperException is thrown if the requested attribute does not exist and should not be created
   *
   * \param name     The name of the attribute
   * \param value    The new value
   * \param create   Whether a non-existing attribute should be created or not
   */
  inline void SetAttribute(const std::string &name, const std::string &value, bool create = true)
  {
    this->SetStringAttribute(name, value, create);
  }

  /*! Set a char * XML attribute of this node
   *
   * This is a method for special handling of string values which do not have to be
   * serialized via stringstream (needed because implicit conversion does not work due
   * to the template version of this method).
   *
   * \exception tXML2WrapperException is thrown if the requested attribute does not exist and should not be created
   *
   * \param name     The name of the attribute
   * \param value    The new value
   * \param create   Whether a non-existing attribute should be created or not
   */
  inline void SetAttribute(const std::string &name, const char *value, bool create = true)
  {
    this->SetStringAttribute(name, value, create);
  }

  /*! Remove an attribute from this node
   *
   * \param name     The name of the attribute
   */
  inline void RemoveAttribute(const std::string &name)
  {
    xmlAttrPtr attr = xmlHasProp(this->data->node, reinterpret_cast<const xmlChar *>(name.c_str()));
    if (attr)
    {
      xmlRemoveProp(attr);
    }
  }

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
