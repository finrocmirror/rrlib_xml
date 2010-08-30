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
  document1.AddRootNode("test1");
  document1.GetRootNode().AddAttribute("prop_1", "val_1");
  document1.GetRootNode().AddAttribute("prop_2", true);
  document1.GetRootNode().AddAttribute("prop_3", 4.3);
  document1.GetRootNode().AddAttribute("prop_4", 123);

  document1.WriteToFile("test.xml");

  tXMLDocument document2("test.xml", false);

//  tXMLDocument document3("test3", true);

  return EXIT_SUCCESS;
}
