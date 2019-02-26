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

#ifndef shade_parser_ForeachSegment_H
#define shade_parser_ForeachSegment_H shade_parser_ForeachSegment_H

#include "Segment.h"
#include <boost/shared_ptr.hpp>
#include <string>

namespace shade
{
namespace parser
{
class ForeachSegment : public Segment
{
  public:
    ForeachSegment(const std::string& index, const std::string& list, boost::shared_ptr<Segment> body, const formatter::MarkBuffer::iterator& begin, const formatter::MarkBuffer::iterator& end);

    /*virtual*/ void get_content(formatter::Generator&, Scope&, const FunctionCall&, std::ostream& error_log) const;

  private:
    std::string m_index;
    std::string m_list;
    boost::shared_ptr<Segment> m_body;
};

} // namespace parser
} // namespace shade

#endif /* shade_parser_ForeachSegment_H */
