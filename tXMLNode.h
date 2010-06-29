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
 * \brief
 *
 * \b
 *
 * A few words for tXMLNode.h
 *
 */
//----------------------------------------------------------------------
#ifndef rrlib_xml2_wrapper_tXMLNode_h_
#define rrlib_xml2_wrapper_tXMLNode_h_

//----------------------------------------------------------------------
// External includes with <>
//----------------------------------------------------------------------
extern "C"
{
#include <libxml/tree.h>
}

#include <string>
#include <vector>
#include <algorithm>
#include <cerrno>
#include <cstdlib>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/xml2_wrapper/tXML2WrapperException.h"

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
//! Short description of tXMLNode
/*! A more detailed description of tXMLNode, which
 *  Tobias Foehst hasn't done yet!
 *
 */
class tXMLNode
{
  xmlNodePtr node;
  mutable std::string *name;
  mutable std::vector<tXMLNode> *children;

  template <typename TNumber>
  static const TNumber ConvertStringToNumber(const std::string &value, TNumber(&convert_function)(const char *, char **, int), int base)
  {
    errno = 0;
    char *endptr;
    TNumber result = convert_function(value.c_str(), &endptr, base);
    if (errno || endptr)
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
    if (errno || endptr)
    {
      throw tXML2WrapperException("Could not convert `" + value + "' to number!");
    }
    return result;
  }

//----------------------------------------------------------------------
// Public methods
//----------------------------------------------------------------------
public:

  tXMLNode(xmlNodePtr node);
  ~tXMLNode();

  inline const std::string &GetName() const
  {
    if (!this->name)
    {
      this->name = new std::string(reinterpret_cast<const char *>(this->node->name));
    }
    return *this->name;
  }

  const std::vector<tXMLNode> &GetChildren() const;

  inline const bool HasAttribute(const std::string &name) const
  {
    return xmlGetProp(this->node, reinterpret_cast<const xmlChar *>(name.c_str())) != 0;
  }

  inline const std::string GetStringAttribute(const std::string &name) const
  {
    const char *result = reinterpret_cast<const char *>(xmlGetProp(this->node, reinterpret_cast<const xmlChar *>(name.c_str())));
    if (!result)
    {
      throw tXML2WrapperException("Requested attribute `" + name + "' does not exist in this node!");
    }
    return result;
  }

  inline const long int GetLongIntAttribute(const std::string &name, int base = 10) const
  {
    return tXMLNode::ConvertStringToNumber(this->GetStringAttribute(name), strtol, base);
  }

  inline const long long int GetLongLongIntAttribute(const std::string &name, int base = 10) const
  {
    return tXMLNode::ConvertStringToNumber(this->GetStringAttribute(name), strtoll, base);
  }

  inline const float GetFloatAttribute(const std::string &name) const
  {
    return tXMLNode::ConvertStringToNumber(this->GetStringAttribute(name), strtof);
  }

  inline const double GetDoubleAttribute(const std::string &name) const
  {
    return tXMLNode::ConvertStringToNumber(this->GetStringAttribute(name), strtod);
  }

  inline const long double GetLongDoubleAttribute(const std::string &name) const
  {
    return tXMLNode::ConvertStringToNumber(this->GetStringAttribute(name), strtold);
  }

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

  inline const bool GetBoolAttribute(const std::string &name) const
  {
    static const char *bool_names_init[2] = { "false", "true" };
    static const std::vector<std::string> bool_names(bool_names_init, bool_names_init + 2);
    // FIXME: with c++0x this can be static const std::vector<std::string> bool_strings = { "false", "true" };

    return this->GetEnumAttribute<bool>(name, bool_names);
  }

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
