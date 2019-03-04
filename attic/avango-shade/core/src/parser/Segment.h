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

#ifndef shade_parser_Segment_H
#define shade_parser_Segment_H shade_parser_Segment_H

#include "FunctionCall.h"
#include "Element.h"
#include "../formatter/MarkBuffer.h"
#include <string>
#include <vector>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/function.hpp>

namespace shade
{
namespace formatter
{
class Generator;
}

namespace parser
{
class LiteralSegment;
class Scope;
class ValueList;

class Segment : public Element
{
  public:
    Segment(const formatter::MarkBuffer::iterator& begin, const formatter::MarkBuffer::iterator& end);

    operator bool(void) const;
    bool operator<(const Segment& other) const;
    LiteralSegment operator-(const Segment& other) const;
    LiteralSegment get_begin_offset(const formatter::MarkBuffer::iterator& begin) const;
    LiteralSegment get_end_offset(const formatter::MarkBuffer::iterator& end) const;

  private:
    formatter::MarkBuffer::iterator begin_;
    formatter::MarkBuffer::iterator end_;
};

class LiteralSegment : public Segment
{
  public:
    LiteralSegment(const formatter::MarkBuffer::iterator& begin, const formatter::MarkBuffer::iterator& end);

    /*virtual*/ void get_content(formatter::Generator&, Scope&, const FunctionCall&, std::ostream& error_log) const;

    bool is_empty(void) { return content_.empty(); }

  private:
    std::string content_;
};

template <class Holder>
class ComposedSegment : public Segment
{
  public:
    template <class Iter>
    ComposedSegment(Iter begin, Iter end, const formatter::MarkBuffer::iterator& text_begin, const formatter::MarkBuffer::iterator& text_end) : Segment(text_begin, text_end)
    {
        using namespace boost::lambda;

        if(begin == end)
        {
            container_.push_back(Holder(new LiteralSegment(text_begin, text_end)));
            return;
        }

        std::vector<Holder> segments;
        std::copy(begin, end, back_inserter(segments));
        std::sort(segments.begin(), segments.end(), (*_1) < (*_2));

        LiteralSegment first((*segments.begin())->get_begin_offset(text_begin));
        if(!first.is_empty())
            container_.push_back(Holder(new LiteralSegment(first)));
        std::transform(segments.begin(), segments.end() - 1, segments.begin() + 1, segments.begin(), bind(&ComposedSegment::do_difference, this, _1, _2));
        container_.push_back(*segments.rbegin());
        LiteralSegment last((*segments.rbegin())->get_end_offset(text_end));
        if(!last.is_empty())
            container_.push_back(Holder(new LiteralSegment(last)));
    }

    /*virtual*/ void get_content(formatter::Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) const
    {
        for(typename std::vector<Holder>::const_iterator i = container_.begin(); i != container_.end(); ++i)
        {
            (*i)->get_content(generator, scope, call, error_log);
        }
    }

  private:
    Holder do_difference(Holder a, Holder b)
    {
        container_.push_back(a);
        container_.push_back(Holder(new LiteralSegment((*b) - (*a))));
        return a;
    }

    std::vector<Holder> container_;
};
} // namespace parser
} // namespace shade

#endif /* shade_parser_Segment_H */
