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

#ifndef shade_formatter_SelfTranslator_H
#define shade_formatter_SelfTranslator_H shade_formatter_SelfTranslator_H

#include <shade/formatter/Generator.h>
#include <string>
#include <vector>
#include <map>

namespace shade
{
  namespace formatter
  {
    class SelfTranslator
    {
    public:

      SelfTranslator(void);

      void handle_self_begin(void);
      void handle_self_name_begin(void);
      void handle_self_name_end(void);
      void handle_self_parameter_list_begin(void);
      void handle_self_parameter_type_begin(void);
      void handle_self_parameter_type_end(void);
      void handle_self_parameter_name_begin(void);
      void handle_self_parameter_name_end(void);
      void handle_self_parameter_list_end(void);
      void handle_self_end(void);
      void handle_identifier(const std::string& id);
      void handle_type(Constants::Type type);

      void generate(Generator& generator, const std::string& name) const;
      void append(const SelfTranslator& other);

    private:
      bool m_store_self_name;
      bool m_store_type;
      bool m_store_name;

      std::string m_cur_self_name;
      Constants::Type m_cur_type;
      std::string m_cur_name;

      typedef std::pair<Constants::Type, std::string> Parameter;
      typedef std::vector<Parameter> ParameterList;
      typedef std::map<std::string, ParameterList> NamedParameterLists;
      NamedParameterLists m_parameters;
    };
  }
}

#endif /* shade_formatter_SelfTranslator_H */
