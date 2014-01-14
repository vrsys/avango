// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 2007 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#include <shade/io.h>
#include <string>
#include <sstream>
#include <map>
#include <UnitTest++.h>

SUITE(TestIO)
{
  TEST(ReadFile)
  {
    std::string input("This is a\n"
        "  little  \n"
        "// Test");
    std::istringstream hin(input);

    std::map<std::string, std::string> keywords;
    std::string content = shade::io::parse_input(hin, keywords);
    CHECK_EQUAL(input+"\n", content);
    CHECK_EQUAL(0u, keywords.size());
  }

  TEST(EmptyLineRemoval)
  {
    std::string input("AA\n\nBB\n\nCC");
    std::istringstream hin(input);

    std::map<std::string, std::string> keywords;
    std::string content = shade::io::parse_input(hin, keywords);
    CHECK_EQUAL("AA\nBB\nCC\n", content);
  }

  TEST(FindVersion)
  {
    std::string input("This is a\n"
        "  little  \n"
        "  // SHADE glsl_version:110\n"
        " //  SHADE   fragment silly: \n"
        "Test\n");
    std::istringstream hin(input);

    std::map<std::string, std::string> keywords;
    std::string content = shade::io::parse_input(hin, keywords);
    CHECK_EQUAL(input, content);
    CHECK_EQUAL(3u, keywords.size());
    CHECK_EQUAL("110", keywords.find("glsl_version")->second);
    CHECK_EQUAL("", keywords.find("fragment")->second);
    CHECK_EQUAL("", keywords.find("silly")->second);
  }
}
