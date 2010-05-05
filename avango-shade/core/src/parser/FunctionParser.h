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

#ifndef shade_parser_FunctionParser_H
#define shade_parser_FunctionParser_H shade_parser_FunctionParser_H

#include "FunctionHandler.h"
#include "BlockParser.h"
#include "Segment.h"
#include "ScopeLayer.h"
#include "Scope.h"
#include <shade/formatter/Generator.h>
#include "../formatter/SelfTranslator.h"
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
#include <sstream>
#include <vector>
#include <iostream>

namespace shade
{
  namespace parser
  {
    class ValueList;
    class ScopeLayer;

    namespace details
    {
      std::string identity(const std::string& name, const ValueList&, const ScopeLayer&, std::ostream&)
      {
        return name;
      }
    }

    class Parameter
    {
    public:

      Parameter(void) :
        m_count(-1)
      {
      }

      void add(const std::string& identifier)
      {
        m_identifiers.push_back(identifier);
      }

      void set_array(int count)
      {
        m_count = count;
      }

      std::string get_name(void) const
      {
        return m_identifiers.back();
      }

      void generate(formatter::Generator& generator, const formatter::SelfTranslator& translator) const
      {
        if (m_identifiers.size() == 0)
          return;

        if (m_identifiers.size() == 1)
        {
          translator.generate(generator, m_identifiers.front());
          return;
        }

        if (m_identifiers.size() > 2)
        {
          generator.handle_function_definition_parameter_qualifier_list_begin();
          typedef std::vector<std::string>::const_iterator Iterator;
          Iterator end = m_identifiers.end()-2;
          for (Iterator i = m_identifiers.begin(); i != end; ++i)
          {
            generator.handle_function_definition_parameter_qualifier_begin();
            generator.handle_verbatim(*i);
            generator.handle_function_definition_parameter_qualifier_end();
          }
          generator.handle_function_definition_parameter_qualifier_list_end();
        }

        generator.handle_function_definition_parameter_type_begin();
        std::string type(m_identifiers[m_identifiers.size()-2]);
        if (m_count != -1)
        {
          generator.handle_array_begin();
          generator.handle_array_type_begin();
          generator.handle_verbatim(type);
          generator.handle_array_type_end();
          generator.handle_array_size_begin();
          generator.handle_literal_int(m_count);
          generator.handle_array_size_end();
          generator.handle_array_end();
        }
        else
          generator.handle_verbatim(type);
        generator.handle_function_definition_parameter_type_end();
        generator.handle_function_definition_parameter_name_begin();
        generator.handle_identifier(m_identifiers[m_identifiers.size()-1]);
        generator.handle_function_definition_parameter_name_end();
      }

    private:

      std::vector<std::string> m_identifiers;
      int m_count;
    };

    struct ParameterClosure :
      public boost::spirit::classic::closure
      <
      ParameterClosure,
      Parameter
      >
    {
      member1 value;
    };

    struct ParameterListClosure :
      public boost::spirit::classic::closure
      <
      ParameterListClosure,
      std::vector<Parameter>
      >
    {
      member1 parameters;

      void add(const Parameter& p)
      {
        parameters().push_back(p);
      }

    };

    struct FunctionValues
    {
      std::string name;
      std::vector<Parameter> parameters;
      std::string parameter_string;
      std::string return_type;
      boost::shared_ptr<Segment> body;
    };

    struct FunctionClosure :
      public boost::spirit::classic::closure<FunctionClosure, FunctionValues>
    {
      member1 values;
    };

    struct Function : public boost::spirit::classic::grammar<Function, FunctionClosure::context_t>
    {
      Function(formatter::Generator& g, const formatter::SelfTranslator& t) :
        generator(g),
        translator(t)
      {
      }

      void invoke(void) const
      {
        ScopeLayer global;
        Scope scope(global);
        std::ostringstream error_log;
        generator.handle_function_definition_begin();
        generator.handle_function_definition_return_type_begin();
        generator.handle_verbatim(values().return_type);
        generator.handle_function_definition_return_type_end();
        generator.handle_function_definition_name_begin();
        generator.handle_identifier(values().name);
        generator.handle_function_definition_name_end();
        generator.handle_function_definition_parameter_list_begin();
        for (std::vector<Parameter>::const_iterator param = values().parameters.begin();
            param != values().parameters.end(); ++param)
        {
          param->generate(generator, translator);
        }
        generator.handle_function_definition_parameter_list_end();
        generator.handle_function_definition_code_begin();
        values().body->get_content(generator, scope, details::identity, error_log);
        generator.handle_function_definition_code_end();
        generator.handle_function_definition_end();
      }

      template <typename ScannerT>
      struct definition
        {
          boost::spirit::classic::rule<ScannerT> identifier;
          boost::spirit::classic::rule<ScannerT, ParameterClosure::context_t> parameter;
          boost::spirit::classic::rule<ScannerT, ParameterListClosure::context_t> function_parameters;
          boost::spirit::classic::rule<ScannerT> function;
          boost::spirit::classic::rule<ScannerT> top;
          Block block_p;

          definition(Function const& self)
          {
            using namespace boost::spirit::classic;
            using namespace phoenix;

            identifier = lexeme_d[alpha_p >> *(alnum_p|'_')];

            parameter = (+(identifier[bind(&Parameter::add)(parameter.value, construct_<std::string>(arg1, arg2))])
                >> !(ch_p('[') >> int_p[bind(&Parameter::set_array)(parameter.value, arg1)] >> ch_p(']')))
              [bind(&ParameterListClosure::add)(var(function_parameters), parameter.value)];

            function_parameters = !(parameter >> *( ch_p(',') >> parameter));

            function =
              (+(identifier - (identifier >> ch_p('('))))
              [bind(&FunctionValues::return_type)(self.values) = construct_<std::string>(arg1, arg2)]
              >> identifier[bind(&FunctionValues::name)(self.values) = construct_<std::string>(arg1, arg2)]
              >> ch_p('(')
              >> (function_parameters[bind(&FunctionValues::parameters)(self.values) = arg1] >> eps_p)
              [bind(&FunctionValues::parameter_string)(self.values) = construct_<std::string>(arg1, arg2)]
              >> ch_p(')')
              >> ch_p('{')
              >> block_p[bind(&FunctionValues::body)(self.values) = arg1]
              >> ch_p('}');

            top = function[bind(&Function::invoke)(var(self))];
          }

          boost::spirit::classic::rule<ScannerT> const& start() const
          {
            return top;
          }

        };

      formatter::Generator& generator;
      const formatter::SelfTranslator& translator;
    };

  }
}

#endif /* shade_parser_FunctionParser_H */
