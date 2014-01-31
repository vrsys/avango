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

#include "../../src/formatter/MarkBuffer.h"
#include "../../src/formatter/Mark.h"
#include "../../src/formatter/FileMark.h"
#include <boost/shared_ptr.hpp>
#include <boost/version.hpp>
#if BOOST_VERSION >= 103600
#include <boost/spirit/include/classic_core.hpp>
#else
#include <boost/spirit/core.hpp>
#include "../../src/parser/boost_spirit_classic.h"
#endif
#include <iterator>
#include <algorithm>
#include <UnitTest++.h>

SUITE(TestMarkBuffer)
{
  TEST(StringOutput)
  {
    shade::formatter::MarkBuffer buffer;
    std::string text("Flip\nFlop\n");

    buffer.append(text);

    CHECK_EQUAL(text, buffer.str());
  }

  TEST(StringStreamOutput)
  {
    shade::formatter::MarkBuffer buffer;
    std::string text("Flip\nFlop\n");

    buffer << text;

    CHECK_EQUAL(text, buffer.str());
  }

  class TestMark :
    public shade::formatter::Mark
  {
  };

  TEST(SetMark)
  {
    shade::formatter::MarkBuffer buffer;
    boost::shared_ptr<TestMark> mark(new TestMark);

    buffer << "{\n";
    buffer.set_mark(mark);
    buffer << "5;\n";
    buffer << "}\n";

    CHECK_EQUAL(2u, buffer.get_mark_line_number(mark));
  }

  TEST(CopyStringWithIterators)
  {
    shade::formatter::MarkBuffer buffer;
    buffer.append("Flip\nFlop\n");

    std::stringstream result;
    std::copy(buffer.begin(), buffer.end(), std::ostream_iterator<char>(result));

    CHECK_EQUAL(buffer.str(), result.str());
  }

  TEST(ParsingWithIterators)
  {
    shade::formatter::MarkBuffer buffer;
    buffer.append("1.234");

    CHECK(boost::spirit::classic::parse(buffer.begin(), buffer.end(), boost::spirit::classic::real_p).full);
  }

  TEST(GetLineNumberOfIterator)
  {
    shade::formatter::MarkBuffer buffer;
    buffer.append("A\nB\n");

    shade::formatter::MarkBuffer::iterator iter(buffer.begin());
    CHECK_EQUAL(0u, iter.get_file_mark().get_line_number());
    ++iter;
    CHECK_EQUAL(0u, iter.get_file_mark().get_line_number());
    ++iter;
    CHECK_EQUAL(1u, iter.get_file_mark().get_line_number());
    ++iter;
    CHECK_EQUAL(1u, iter.get_file_mark().get_line_number());
  }

  TEST(GetLineNumberOfIteratorWithFileMarks)
  {
    shade::formatter::MarkBuffer buffer;
    boost::shared_ptr<shade::formatter::FileMark> markA(new shade::formatter::FileMark("A", 10));
    boost::shared_ptr<shade::formatter::FileMark> markB(new shade::formatter::FileMark("B", 111));

    buffer.set_mark(markA);
    buffer.append("x\ny\n");
    buffer.set_mark(markB);
    buffer.append("x\ny\n");

    shade::formatter::MarkBuffer::iterator iter(buffer.begin());

    CHECK_EQUAL(markA->get_line_number(), iter.get_file_mark().get_line_number());
    CHECK_EQUAL(markA->get_filename(), iter.get_file_mark().get_filename());
    ++iter;
    CHECK_EQUAL(markA->get_line_number(), iter.get_file_mark().get_line_number());
    CHECK_EQUAL(markA->get_filename(), iter.get_file_mark().get_filename());
    ++iter;
    CHECK_EQUAL(markA->get_line_number()+1, iter.get_file_mark().get_line_number());
    CHECK_EQUAL(markA->get_filename(), iter.get_file_mark().get_filename());
    ++iter;
    CHECK_EQUAL(markA->get_line_number()+1, iter.get_file_mark().get_line_number());
    CHECK_EQUAL(markA->get_filename(), iter.get_file_mark().get_filename());

    ++iter;
    CHECK_EQUAL(markB->get_line_number(), iter.get_file_mark().get_line_number());
    CHECK_EQUAL(markB->get_filename(), iter.get_file_mark().get_filename());
    ++iter;
    CHECK_EQUAL(markB->get_line_number(), iter.get_file_mark().get_line_number());
    CHECK_EQUAL(markB->get_filename(), iter.get_file_mark().get_filename());
    ++iter;
    CHECK_EQUAL(markB->get_line_number()+1, iter.get_file_mark().get_line_number());
    CHECK_EQUAL(markB->get_filename(), iter.get_file_mark().get_filename());
    ++iter;
    CHECK_EQUAL(markB->get_line_number()+1, iter.get_file_mark().get_line_number());
    CHECK_EQUAL(markB->get_filename(), iter.get_file_mark().get_filename());
  }
}
