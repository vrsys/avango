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

#include "Segment.h"
#include <shade/formatter/Generator.h>
using namespace shade::parser;

Segment::Segment(const formatter::MarkBuffer::iterator& begin, const formatter::MarkBuffer::iterator& end) : begin_(begin), end_(end) {}

Segment::operator bool(void) const { return begin_ != end_; }

bool Segment::operator<(const Segment& other) const { return (this->begin_ < other.begin_); }

LiteralSegment Segment::operator-(const Segment& other) const
{
    if(other.end_ > this->begin_)
        return LiteralSegment(this->end_, this->end_);

    return LiteralSegment(other.end_, this->begin_);
}

LiteralSegment Segment::get_begin_offset(const formatter::MarkBuffer::iterator& begin) const
{
    if(begin > this->begin_)
        return LiteralSegment(this->begin_, this->begin_);

    return LiteralSegment(begin, this->begin_);
}

LiteralSegment Segment::get_end_offset(const formatter::MarkBuffer::iterator& end) const
{
    if(this->end_ > end)
        return LiteralSegment(this->end_, this->end_);

    return LiteralSegment(this->end_, end);
}

LiteralSegment::LiteralSegment(const formatter::MarkBuffer::iterator& begin, const formatter::MarkBuffer::iterator& end) : Segment(begin, end), content_(begin, end) {}

void LiteralSegment::get_content(formatter::Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) const { generator.handle_verbatim(content_); }
