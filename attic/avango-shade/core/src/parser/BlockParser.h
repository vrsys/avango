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

#ifndef shade_parser_BlockParser_H
#define shade_parser_BlockParser_H shade_parser_BlockParser_H

#include "SingleAssignmentString.h"
#include "Segment.h"
#include "ForeachSegment.h"
#include "IdentifierSegment.h"
#include "CallSegment.h"
#include "Scope.h"
#include "ReferenceValue.h"
#include "ValueList.h"
#include <shade/formatter/Generator.h>
#include <boost/version.hpp>
#if BOOST_VERSION >= 103600
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_attribute.hpp>
#include <boost/spirit/include/classic_increment_actor.hpp>
#include <boost/spirit/include/classic_chset.hpp>
#include <boost/spirit/include/phoenix1_binders.hpp>
#else
#include <boost/spirit/core.hpp>
#include <boost/spirit/attribute.hpp>
#include <boost/spirit/actor/increment_actor.hpp>
#include <boost/spirit/utility/chset.hpp>
#include <boost/spirit/phoenix/binders.hpp>
#include "boost_spirit_classic.h"
#endif
#include <string>
#include <vector>
#include <sstream>
#include <iterator>

namespace shade
{
  namespace parser
  {
    struct IdentifierClosure : public boost::spirit::classic::closure<IdentifierClosure, SingleAssignmentString>
    {
      member1 val;
    };

    struct CallValues
    {
      std::string name;
      std::vector<boost::shared_ptr<Segment> > parameters;
      formatter::MarkBuffer::iterator begin;
      formatter::MarkBuffer::iterator end;
    };

    struct CallClosure : public boost::spirit::classic::closure<CallClosure, CallValues>
    {
      member1 values;

      void insert_parameter(boost::shared_ptr<Segment> param)
      {
        values().parameters.push_back(param);
      }

      void set_region(const formatter::MarkBuffer::iterator& begin, const formatter::MarkBuffer::iterator& end)
      {
        values().begin = begin;
        values().end = end;
      }
    };

    struct ForeachValues
    {
      std::string index;
      std::string list;
      boost::shared_ptr<Segment> body;
      formatter::MarkBuffer::iterator begin;
      formatter::MarkBuffer::iterator end;
    };

    struct ForeachClosure : public boost::spirit::classic::closure<ForeachClosure, ForeachValues>
    {
      member1 values;

      void set_region(const formatter::MarkBuffer::iterator& begin, const formatter::MarkBuffer::iterator& end)
      {
        values().begin = begin;
        values().end = end;
      }
    };

    struct BlockClosure :
      public boost::spirit::classic::closure
      <
      BlockClosure,
      boost::shared_ptr<Segment>,
      std::vector<boost::shared_ptr<Segment> >
      >
    {
      member1 body;
      member2 segments;

      void build(const formatter::MarkBuffer::iterator& begin, const formatter::MarkBuffer::iterator& end) const
      {
        typedef ComposedSegment<boost::shared_ptr<Segment> > ResultType;
        body() = boost::shared_ptr<ResultType>(new ResultType(segments().begin(), segments().end(), begin, end));
      }

    };

    void push_callinvocation(std::vector<boost::shared_ptr<Segment> >& segments, const CallValues& values)
    {
      segments.push_back(boost::shared_ptr<Segment>(
            new CallSegment(values.name, values.parameters.begin(), values.parameters.end(), values.begin, values.end))
          );
    }

    void push_foreach(std::vector<boost::shared_ptr<Segment> >& segments, const ForeachValues& values)
    {
      segments.push_back(boost::shared_ptr<Segment>(new ForeachSegment(values.index, values.list, values.body, values.begin, values.end)));
    }

    void push_identifier(std::vector<boost::shared_ptr<Segment> >& segments, const formatter::MarkBuffer::iterator& begin, const formatter::MarkBuffer::iterator& end)
    {
      segments.push_back(boost::shared_ptr<Segment>(new IdentifierSegment(begin, end)));
    }

    void push_segment(std::vector<boost::shared_ptr<Segment> >& segments, boost::shared_ptr<Segment> obj)
    {
      segments.push_back(obj);
    }


    struct Block : public boost::spirit::classic::grammar<Block, BlockClosure::context_t>
    {

      template <typename ScannerT>
        struct definition
        {
          boost::spirit::classic::rule<ScannerT> identifier;
          boost::spirit::classic::rule<ScannerT> parameter;
          boost::spirit::classic::rule<ScannerT, BlockClosure::context_t> expression;
          boost::spirit::classic::rule<ScannerT, BlockClosure::context_t> expression_list;
          boost::spirit::classic::rule<ScannerT> statement;
          boost::spirit::classic::rule<ScannerT, ForeachClosure::context_t> foreach;
          boost::spirit::classic::rule<ScannerT, CallClosure::context_t> function;
          boost::spirit::classic::rule<ScannerT> top;

          definition(Block const& self)
          {
            using namespace boost::spirit::classic;
            using namespace phoenix;

            identifier = lexeme_d[alpha_p >> *(alnum_p|'_')];

            foreach = (
                str_p("FOREACH")
                >> ch_p('(')
                >> identifier[bind(&ForeachValues::index)(foreach.values) = construct_<std::string>(arg1, arg2)]
                >> ch_p(',')
                >> identifier[bind(&ForeachValues::list)(foreach.values) = construct_<std::string>(arg1, arg2)]
                >> ch_p(')')
                >> ch_p('{') >> self[bind(&ForeachValues::body)(foreach.values) = arg1] >> ch_p('}')
                )
              [bind(&ForeachClosure::set_region)(var(foreach), arg1, arg2)];

            expression
              = (+(
                    function[bind(&push_callinvocation)(expression.segments, arg1)]
                    | identifier[bind(&push_identifier)(expression.segments, arg1, arg2)]
                    | ('(' >> expression_list[bind(&push_segment)(expression.segments, arg1)] >> ')')
                    | (~chset<>(",;){}"))
                  ))[bind(&BlockClosure::build)(var(expression), arg1, arg2)]
              ;
            expression_list = (!(expression[bind(&push_segment)(expression_list.segments, arg1)])
                >> *(',' >> expression[bind(&push_segment)(expression_list.segments, arg1)])
                )[bind(&BlockClosure::build)(var(expression_list), arg1, arg2)];
            statement = 
              *(
                  str_p("if") | str_p("switch") | str_p("case")
                  | str_p("for") >> '(' >> expression >> ';' >> expression >> ';' >> expression >> ')'  
                  | foreach[bind(&push_foreach)(self.segments, arg1)]
                  | expression[bind(&push_segment)(self.segments, arg1)]
                  | chset<>(";,")
                  | '{' >> statement >> '}'
               );

            parameter = expression[bind(&CallClosure::insert_parameter)(var(function), arg1)];
            function
              = (identifier[bind(&CallValues::name)(function.values) = construct_<std::string>(arg1, arg2)]
                  >> '(' >> !(parameter >> *(',' >> parameter)) >> ')'
                )[bind(&CallClosure::set_region)(var(function), arg1, arg2)]
              ;

            top = statement[bind(&BlockClosure::build)(var(self), arg1, arg2)];
          }

          boost::spirit::classic::rule<ScannerT> const& start() const
          {
            return top;
          }

        };
    };

  }
}

#endif /* shade_parser_BlockParser_H */
