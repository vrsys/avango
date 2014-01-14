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

#include "DumpGenerator.h"
#include "diff_dump.h"
#include <UnitTest++.h>
#include <string>
#include <vector>

SUITE(DiffDump)
{
  TEST(IsToken)
  {
    CHECK(shade::formatter::is_token("<A>"));
    CHECK(shade::formatter::is_token("</array>"));
    CHECK(!shade::formatter::is_token("</array"));
    CHECK(!shade::formatter::is_token("array"));
  }

  TEST(IsEndToken)
  {
    CHECK(shade::formatter::is_end_token("</array>"));
    CHECK(!shade::formatter::is_end_token("<array>"));
  }

  TEST(GetNextToken)
  {
    std::pair<std::string, std::string> split(shade::formatter::get_next_token("</array><other>"));
    CHECK_EQUAL("</array>", split.first);
    CHECK_EQUAL("<other>", split.second);
  }

  TEST(GetNextTokenWithEmptyRest)
  {
    std::pair<std::string, std::string> split(shade::formatter::get_next_token("</array>"));
    CHECK_EQUAL("</array>", split.first);
    CHECK(split.second.empty());
  }

  TEST(GetNextTokenWithBrokenToken)
  {
    std::pair<std::string, std::string> split(shade::formatter::get_next_token("</array"));
    CHECK_EQUAL("</array", split.first);
    CHECK(split.second.empty());
  }

  TEST(GetNextTokenWithLiteralText)
  {
    std::pair<std::string, std::string> split(shade::formatter::get_next_token("array<other>"));
    CHECK_EQUAL("array", split.first);
    CHECK_EQUAL("<other>", split.second);
  }

  TEST(GetNextTokenWithNoTagLeft)
  {
    std::pair<std::string, std::string> split(shade::formatter::get_next_token("array"));
    CHECK_EQUAL("array", split.first);
    CHECK(split.second.empty());
  }


  TEST(FormattingWithIdenticalText)
  {
    std::string code("<A><B><C></C><D><E></E></D></B></A>");

    CHECK_EQUAL(" <A>\n"
        "   <B>\n"
        "     <C>\n"
        "     </C>\n"
        "     <D>\n"
        "       <E>\n"
        "       </E>\n"
        "     </D>\n"
        "   </B>\n"
        " </A>\n"
        , shade::formatter::diff_dump(code, code));
  }

  TEST(FormattingWithIdenticalTextAndLiteral)
  {
    std::string code("<A><B><C>Test</C></B></A>");

    CHECK_EQUAL(" <A>\n"
        "   <B>\n"
        "     <C>Test</C>\n"
        "   </B>\n"
        " </A>\n"
        , shade::formatter::diff_dump(code, code));
  }

  TEST(FormattingWithNonIdenticalText)
  {
    std::string codeA("<A><B><C></C><D><E></E></D></B></A>");
    std::string codeB("<A><B><C></C><G><E><F></F></E></G></B></A>");

    CHECK_EQUAL(" <A>\n"
        "   <B>\n"
        "     <C>\n"
        "     </C>\n"
        "A    <D>\n"
        "B    <G>\n"
        "       <E>\n"
        "A      </E>\n"
        "A    </D>\n"
        "B        <F>\n"
        "B        </F>\n"
        "B      </E>\n"
        "B    </G>\n"
        "   </B>\n"
        " </A>\n"
        , shade::formatter::diff_dump(codeA, codeB));
  }

  TEST(FormattingWithCorruptData)
  {
    std::string code("</A></B></C></C></B></A>");

    CHECK_EQUAL(" </A>\n"
        " </B>\n"
        " </C>\n"
        " </C>\n"
        " </B>\n"
        " </A>\n"
        , shade::formatter::diff_dump(code, code));
  }

  TEST(SplitAtTopLevel)
  {
    std::string code("<A><B><C></C></B></A><C></C><D><G></G></D>");
    std::vector<std::string> result = shade::formatter::split_toplevel(code);

    CHECK_EQUAL(3u, result.size());
    CHECK_EQUAL("<A><B><C></C></B></A>", result[0]);
    CHECK_EQUAL("<C></C>", result[1]);
    CHECK_EQUAL("<D><G></G></D>", result[2]);
  }

  TEST(SplitAtTopLevelWithData)
  {
    std::string code("<A><B></B></A><C>Test</C><G></G>");
    std::vector<std::string> result = shade::formatter::split_toplevel(code);

    CHECK_EQUAL(3u, result.size());
    CHECK_EQUAL("<A><B></B></A>", result[0]);
    CHECK_EQUAL("<C>Test</C>", result[1]);
    CHECK_EQUAL("<G></G>", result[2]);
  }

  TEST(SplitAtTopLevelWithCorruptStructure)
  {
    std::string code("<A><B></B></A><C>");
    std::vector<std::string> result = shade::formatter::split_toplevel(code);

    CHECK_EQUAL(2u, result.size());
    CHECK_EQUAL("<A><B></B></A>", result[0]);
    CHECK_EQUAL("<C>", result[1]);
  }

  TEST(CountWithNonIdenticalText)
  {
    std::string codeA("<A><B><C></C><D><E></E></D></B></A>");
    std::string codeB("<A><B><C></C><G><E><F></F></E></G></B></A>");

    int count;
    shade::formatter::diff_dump(codeA, codeB, count);
    CHECK_EQUAL(5, count);
  }

  TEST(PartialFormattingWithIdenticalText)
  {
    std::string code("<A><B><C></C></B></A><D><E></E></D>");
    std::string fragment("<D><E></E></D>");
    bool match;

    CHECK_EQUAL(" <D>\n"
        "   <E>\n"
        "   </E>\n"
        " </D>\n"
        , shade::formatter::partial_diff_dump(fragment, code, match));
    CHECK(match);
  }

  TEST(PartialFormattingWithDifferentText)
  {
    std::string code("<A><B><C></C></B></A><D><E></E></D>");
    std::string fragment("<D><G></G></D>");
    bool match;

    CHECK_EQUAL(" <D>\n"
        "A  <G>\n"
        "A  </G>\n"
        "B  <E>\n"
        "B  </E>\n"
        " </D>\n"
        , shade::formatter::partial_diff_dump(fragment, code, match));
    CHECK(!match);
  }

  TEST(PartialFormattingWithCompleteDifferentText)
  {
    std::string code("<A><B><C></C></B></A><D><E></E></D>");
    std::string fragment("<I><J></J></I>");
    bool match;

    CHECK_EQUAL("A<I>\n"
        "A  <J>\n"
        "A  </J>\n"
        "A</I>\n"
        "B<D>\n"
        "B  <E>\n"
        "B  </E>\n"
        "B</D>\n"
        , shade::formatter::partial_diff_dump(fragment, code, match));
    CHECK(!match);
  }

  TEST(StripLevels)
  {
    std::string code("<A><B><C><X></X></C></B></A><D><E><F>abc</F></E></D>");
    CHECK_EQUAL("<C><X></X></C><F>abc</F>", shade::formatter::strip_levels(code, 2));
  }

}
