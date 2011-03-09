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
#ifndef __rrlib__xml2_wrapper__tXMLNode_h__
#define __rrlib__xml2_wrapper__tXMLNode_h__

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
#include <iterator>
#include <boost/noncopyable.hpp>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/xml2_wrapper/tXML2WrapperException.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------
#include <cassert>

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
class tXMLNode : protected xmlNode, boost::noncopyable
{
  friend class tXMLDocument;

//----------------------------------------------------------------------
// Public methods and typedefs
//----------------------------------------------------------------------
public:

  class const_iterator;

  class iterator : public std::iterator<std::forward_iterator_tag, tXMLNode, size_t>
  {
    friend class const_iterator;
    pointer element;

  public:
    inline iterator() : element(0) {}
    inline iterator(pointer element) : element(element)
    {
      if (this->element && this->element->type != XML_ELEMENT_NODE)
      {
        operator++();
      }
    }

    inline reference operator*() const
    {
      return *element;
    }
    inline pointer operator->() const
    {
      return &(operator*());
    }

    inline iterator &operator ++ ()
    {
      do
      {
        this->element = reinterpret_cast<tXMLNode *>(this->element->next);
      }
      while (this->element && this->element->type != XML_ELEMENT_NODE);
      return *this;
    }
    inline iterator operator ++ (int)
    {
      iterator temp(*this);
      operator++();
      return temp;
    }

    inline const bool operator == (const iterator &other) const
    {
      return element == other.element;
    }
    inline const bool operator != (const iterator &other) const
    {
      return !(*this == other);
    }
  };

  class const_iterator : public std::iterator<std::forward_iterator_tag, const tXMLNode, size_t>
  {
    pointer element;

  public:
    inline const_iterator() : element(0) {}
    inline const_iterator(pointer element) : element(element)
    {
      if (this->element && this->element->type != XML_ELEMENT_NODE)
      {
        operator++();
      }
    }
    // NOTE: the clarification tXMLNode::iterator is necessary for gcc 4.5.2 -> DO NOT REMOVE IT!!!
    inline const_iterator(const  tXMLNode::iterator &other) : element(other.element) {};

    inline reference operator*() const
    {
      return *element;
    }
    inline pointer operator->() const
    {
      return &(operator*());
    }

    inline const_iterator &operator ++ ()
    {
      do
      {
        this->element = reinterpret_cast<tXMLNode *>(this->element->next);
      }
      while (this->element && this->element->type != XML_ELEMENT_NODE);
      return *this;
    }
    inline const_iterator operator ++ (int)
    {
      const_iterator temp(*this);
      operator++();
      return temp;
    }

    inline const bool operator == (const const_iterator &other) const
    {
      return element == other.element;
    }
    inline const bool operator != (const const_iterator &other) const
    {
      return !(*this == other);
    }
  };

  /*! The dtor of tXMLNode
   */
  ~tXMLNode();

  /*! Comparison of XML node objects (inequality)
   *
   * \param other   The other node to compare to this one
   *
   * \returns Whether the two nodes are the same or not
   */
  void FreeNode()
  {
    xmlUnlinkNode(this);
    xmlFreeNode(this);
  }

  /*! Comparison of XML node objects (equality)
   *
   * \param other   The other node to compare to this one
   *
   * \returns Whether the two nodes are the same or not
   */
  inline bool operator == (const tXMLNode &other) const
  {
    return this == &other;
  }

  /*! Comparison of XML node objects (inequality)
   *
   * \param other   The other node to compare to this one
   *
   * \returns Whether the two nodes are the same or not
   */
  inline bool operator != (const tXMLNode &other) const
  {
    return !(*this == other);
  }

  /*! Comparison of name with given string (equality)
   *
   * This method can be used to find a specific node using e.g. std::find
   *
   * \param name   The name to be compared to the node's name
   *
   * \returns Whether this has the given name or not
   */
  inline bool operator == (const std::string &name) const
  {
    return this->GetName() == name;
  }

  /*! Check if this node is part of the subtree of the given node
   *
   * This method can be used to check if a node is contained in
   * the subtree of another one. If this is the case it is not
   * possible to e.g. add the root of the subtree as child to the
   * contained node without copying.
   *
   * \returns Whether \a this is contained within the subtree of \a node
   */
  const bool IsInSubtreeOf(const tXMLNode &node) const;

  /*! Get the name of this node
   *
   * Each XML element has an unique name within its document type. This
   * method provides access to the name of this node.
   *
   * \returns A reference to the node's name
   */
  const std::string GetName() const;

  /*! Get an iterator to the first of this node's children of type XML_ELEMENT_NODE
   *
   * \returns A begin-iterator
   */
  inline const iterator GetChildrenBegin()
  {
    return iterator(reinterpret_cast<tXMLNode *>(this->children));
  }

  /*! Get a const_iterator to the first of this node's children of type XML_ELEMENT_NODE
   *
   * \returns A begin-const_iterator
   */
  inline const const_iterator GetChildrenBegin() const
  {
    return const_iterator(reinterpret_cast<tXMLNode *>(this->children));
  }

  /*! Get an end-iterator to mark the end of children traversal
   *
   * \returns An end-iterator
   */
  const iterator &GetChildrenEnd();

  /*! Get a const_iterator to mark the end of children traversal
   *
   * \returns An end-const_iterator
   */
  const const_iterator &GetChildrenEnd() const;

  /*! Check if this node has children of type XML_ELEMENT_NODE
   *
   * This method can be used to check if a node has children before
   * trying to access those, which would result in an exception if
   * no child exists.
   *
   * \returns Whether \a this has children or not
   */
  inline const bool HasChildren() const
  {
    return this->GetChildrenBegin() != this->GetChildrenEnd();
  }

  /*! Get the number of children of this XML node.
   *
   *  Counts children.
   *
   *  \returns The number of XML nodes reachable as children of this node
   */
  inline const size_t GetNumberOfChildren() const
  {
    return std::distance(this->GetChildrenBegin(), this->GetChildrenEnd());
  }

  /*! Get access to first child of this node
   *
   * This method gives access to the first child of \a this
   * which is itself of type XML_ELEMENT_NODE.
   *
   * \exception tXML2WrapperException is thrown if this node has not children of type XML_ELEMENT_NODE
   *
   * \returns Whether \a this has children or not
   */
  tXMLNode &GetFirstChild();

  /*! Get access to first child of this node in const context
   *
   * This method gives access to the first child of \a this
   * which is itself of type XML_ELEMENT_NODE in const context.
   *
   * \exception tXML2WrapperException is thrown if this node has not children of type XML_ELEMENT_NODE
   *
   * \returns Whether \a this has children or not
   */
  inline const tXMLNode &GetFirstChild() const
  {
    return const_cast<tXMLNode *>(this)->GetFirstChild();
  }

  /*! Add a child to this node
   *
   * In XML DOM trees a node can have several child nodes which are XML
   * nodes themselves. This method add such a node with a given name to
   * the structure which then can be extended by further children or
   * attributes.
   *
   * \param name   The name of the new node
   *
   * \returns A reference to the newly created node
   */
  tXMLNode &AddChildNode(const std::string &name, const std::string &content = "");

  /*! Add an existing node as child to this node
   *
   * This methods adds an existing node to \a this children. By default,
   * \a node is moved with its complete subtree to its new place. It is
   * not possible to move a node into its own subtree.
   *
   * If \a copy is set to true the node and its complete subtree is copied
   * to its new place and the old version remains at its origin.
   *
   * \exception tXML2WrapperException is thrown if \this is contained in the subtree of \a node and \a copy is false
   *
   * \param node   The node to be added
   * \param copy   Set to true if a copy of \a node should be added instead of \a node itself
   *
   * \returns A reference to the new child
   */
  tXMLNode &AddChildNode(tXMLNode &node, bool copy = false);

  /*! Remove a child node
   *
   * Removes a given node from the children list of this node.
   *
   * \exception tXML2WrapperException is thrown if \a node is not a child of \a this
   *
   * \param node   The node to remove from the list
   */
  void RemoveChildNode(tXMLNode &node);

  /*! Get an iterator to the next of this node's siblings of type XML_ELEMENT_NODE
   *
   * \returns A begin-iterator
   */
  inline const iterator GetNextSiblingsBegin()
  {
    return iterator(reinterpret_cast<tXMLNode *>(this->next));
  }

  /*! Get a const_iterator to the next of this node's siblings of type XML_ELEMENT_NODE
   *
   * \returns A begin-const_iterator
   */
  inline const const_iterator GetNextSiblingsBegin() const
  {
    return const_iterator(reinterpret_cast<tXMLNode *>(this->next));
  }

  /*! Get an end-iterator to mark the end of sibling traversal
   *
   * \returns An end-iterator
   */
  const iterator &GetNextSiblingsEnd();

  /*! Get a const_iterator to mark the end of sibling traversal
   *
   * \returns An end-const_iterator
   */
  const const_iterator &GetNextSiblingsEnd() const;

  /*! Check if siblings of type XML_ELEMENT_NODE are reachable via \c GetNextSibling from this node
   *
   * This method can be used to check if a node has siblings before
   * trying to access those, which would result in an exception if
   * no sibling is reachable.
   *
   * \returns Whether a sibling is reachable or not
   */
  inline const bool HasNextSibling()
  {
    return this->GetNextSiblingsBegin() != this->GetNextSiblingsEnd();
  }

  /*! Get access to first child of this node
   *
   * This method gives access to the first child of \a this
   * which is itself of type XML_ELEMENT_NODE.
   *
   * \exception tXML2WrapperException is thrown if this node has not children of type XML_ELEMENT_NODE
   *
   * \returns Whether \a this has children or not
   */
  tXMLNode &GetNextSibling();

  /*! Get access to first child of this node in const context
   *
   * This method gives access to the first child of \a this
   * which is itself of type XML_ELEMENT_NODE in const context.
   *
   * \exception tXML2WrapperException is thrown if this node has not children of type XML_ELEMENT_NODE
   *
   * \returns Whether \a this has children or not
   */
  inline const tXMLNode &GetNextSibling() const
  {
    return const_cast<tXMLNode *>(this)->GetNextSibling();
  }

  tXMLNode &AddNextSibling(const std::string &name, const std::string &content = "");

  /*! Add an existing node as next sibling to this node
   *
   * This methods adds an existing node to \a this siblings. By default,
   * \a node is moved with its complete subtree to its new place. It is
   * not possible to move a node into its own subtree.
   *
   * If \a copy is set to true the node and its complete subtree is copied
   * to its new place and the old version remains at its origin.
   *
   * \exception tXML2WrapperException is thrown if \this is contained in the subtree of \a node and \a copy is false
   *
   * \param node   The node to be added
   * \param copy   Set to true if a copy of \a node should be added instead of \a node itself
   *
   * \returns A reference to the new sibling
   */
  tXMLNode &AddNextSibling(tXMLNode &node, bool copy = false);

  /*! Get the plain text content of this node
   *
   * If the node contains plain text content this method grants access via
   * a std::string.
   *
   * \exception tXML2WrapperException is thrown if the node does not contain plain text content
   *
   * \returns The plain text content
   */
  const std::string GetTextContent() const;

  /*! Add plain text content to this node
   *
   * \param content   The plain text to be added to this node
   */
  void AddTextContent(const std::string &content);

  /*! Remove the plain text content of this node
   */
  void RemoveTextContent();

  /*! Set content of node
   *
   * \param content   The new content
   */
  void SetContent(const std::string &content);

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
    return xmlHasProp(const_cast<tXMLNode *>(this), reinterpret_cast<const xmlChar *>(name.c_str())) != 0;
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
    xmlChar *temp = xmlGetProp(const_cast<tXMLNode *>(this), reinterpret_cast<const xmlChar *>(name.c_str()));
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

  /*! Get list of XML attributes of this node
   *
   * \returns The XML attribute list
   */
  inline xmlAttrPtr GetAttributeList() const
  {
    return this->properties;
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
  void RemoveAttribute(const std::string &name);

  /*! Get a dump in form of xml code of the subtree starting at \a this
   *
   * \param format   Set to true if the dumped text should be indented
   */
  const std::string GetXMLDump(bool format = false) const;

//----------------------------------------------------------------------
// Private fields and methods
//----------------------------------------------------------------------
private:

  template <typename TNumber>
  static const TNumber ConvertStringToNumber(const std::string &value, TNumber(&convert_function)(const char *, char **, int), int base)
  {
    errno = 0;
    char *endptr;
    TNumber result = convert_function(value.c_str(), &endptr, base);
    if (errno || *endptr)
    {
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
      throw tXML2WrapperException("Could not convert `" + value + "' to number!");
    }
    return result;
  }

  void SetStringAttribute(const std::string &name, const std::string &value, bool create);

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
