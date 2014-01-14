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

#include "../../src/parser/Segment.h"
#include "../../src/parser/ScopeLayer.h"
#include "../../src/parser/Scope.h"
#include "../formatter/DumpGenerator.h"
#include <UnitTest++.h>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace shade::parser;
using namespace shade::formatter;

namespace
{
  class ReplaceSegment : public Segment
  {
  public:
    ReplaceSegment(shade::formatter::MarkBuffer::iterator begin, shade::formatter::MarkBuffer::iterator end, std::string content) :
      Segment(begin, end),
      content_(content)
    {}

    /*virtual*/ void get_content(Generator& generator, Scope&, const FunctionCall&, std::ostream& error_log) const
    {
      generator.handle_verbatim(content_);
    }

  private:
    std::string content_;
  };

  std::string identity(const std::string& name, const ValueList&, const ScopeLayer&, std::ostream& error_log)
  {
    return name;
  }

}

SUITE(TestSegment)
{
  struct ErrorLog
  {
    ScopeLayer global;
    Scope scope;
    std::stringstream error_log;

    ErrorLog(void) :
      scope(global)
    {
    }
  };

  MarkBuffer::iterator advance(const MarkBuffer::iterator& start, unsigned int distance)
  {
    MarkBuffer::iterator result(start);
    std::advance(result, distance);
    return result;
  }

  struct Literals : public ErrorLog
  {
    const char* data;
    MarkBuffer buffer;
    LiteralSegment segment1;
    LiteralSegment segment2;
    LiteralSegment segment3;
    ScopeLayer global;
    Scope scope;

    Literals(void) :
      data("Test+Data"),
      buffer(data),
      segment1(buffer.begin(), advance(buffer.begin(), 4)),
      segment2(advance(buffer.begin(), 5), advance(buffer.begin(), 9)),
      segment3(advance(buffer.begin(), 4), advance(buffer.begin(), 5)),
      scope(global)
    {
    }
  };

  TEST_FIXTURE(Literals, LiteralConstruction)
  {
    DumpGenerator generator;
    segment1.get_content(generator, scope, identity, error_log);
    CHECK_EQUAL("<verbatim>"+std::string(data, data+4)+"</verbatim>", generator.get_code());
  }


  TEST_FIXTURE(Literals, Comparison)
  {
    CHECK(segment1 < segment2);
  }

  TEST_FIXTURE(Literals, Difference)
  {
    DumpGenerator generator;
    (segment2 - segment1).get_content(generator, scope, identity, error_log);
    CHECK_EQUAL("<verbatim>"+std::string(data+4, data+5)+"</verbatim>", generator.get_code());
  }

  TEST_FIXTURE(Literals, Offsets)
  {
    DumpGenerator generator_seg1;
    segment1.get_content(generator_seg1, scope, identity, error_log);

    DumpGenerator generator_seg2;
    segment2.get_content(generator_seg2, scope, identity, error_log);

    DumpGenerator generator_seg3_begin;
    segment3.get_begin_offset(buffer.begin()).get_content(generator_seg3_begin, scope, identity, error_log);

    DumpGenerator generator_seg3_end;
    segment3.get_end_offset(advance(buffer.begin(), 9)).get_content(generator_seg3_end, scope, identity, error_log);

    CHECK_EQUAL(generator_seg1.get_code(), generator_seg3_begin.get_code());
    CHECK_EQUAL(generator_seg2.get_code(), generator_seg3_end.get_code());
  }

  TEST_FIXTURE(Literals, NullTest)
  {
    CHECK(segment1);
    CHECK(!LiteralSegment(buffer.begin(), buffer.begin()));
  }

  TEST_FIXTURE(ErrorLog, ComposedSegment)
  {
    DumpGenerator generator;

    MarkBuffer buffer("0123456789ABCDEF");
    LiteralSegment segment1(advance(buffer.begin(), 2), advance(buffer.begin(), 4));
    ReplaceSegment segment2(advance(buffer.begin(), 8), advance(buffer.begin(), 10), "RRR");
    LiteralSegment segment3(advance(buffer.begin(), 12), advance(buffer.begin(), 14));
    std::vector<Segment*> segment_list;
    segment_list.push_back(&segment1);
    segment_list.push_back(&segment3);
    segment_list.push_back(&segment2);

    ComposedSegment<Segment*> composed(segment_list.begin(), segment_list.end(), buffer.begin(), advance(buffer.begin(), 16));
    composed.get_content(generator, scope, identity, error_log);
    CHECK_EQUAL("<verbatim>01</verbatim>"
        "<verbatim>23</verbatim>"
        "<verbatim>4567</verbatim>"
        "<verbatim>RRR</verbatim>"
        "<verbatim>AB</verbatim>"
        "<verbatim>CD</verbatim>"
        "<verbatim>EF</verbatim>"
        , generator.get_code());
  }

  TEST_FIXTURE(ErrorLog, BeginOfComposedSegment)
  {
    DumpGenerator generator;

    MarkBuffer buffer("012345");
    LiteralSegment segment(buffer.begin(), advance(buffer.begin(), 4));
    std::vector<Segment*> segment_list;
    segment_list.push_back(&segment);

    ComposedSegment<Segment*> composed(segment_list.begin(), segment_list.end(), buffer.begin(), buffer.end());
    composed.get_content(generator, scope, identity, error_log);
    CHECK_EQUAL("<verbatim>0123</verbatim><verbatim>45</verbatim>", generator.get_code());
  }

  TEST_FIXTURE(ErrorLog, EndOfComposedSegment)
  {
    DumpGenerator generator;

    MarkBuffer buffer("012345");
    LiteralSegment segment(advance(buffer.begin(), 2), buffer.end());
    std::vector<Segment*> segment_list;
    segment_list.push_back(&segment);

    ComposedSegment<Segment*> composed(segment_list.begin(), segment_list.end(), buffer.begin(), buffer.end());
    composed.get_content(generator, scope, identity, error_log);
    CHECK_EQUAL("<verbatim>01</verbatim><verbatim>2345</verbatim>", generator.get_code());
  }
}
