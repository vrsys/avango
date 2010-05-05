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

#include "CallSegment.h"
#include <shade/formatter/Generator.h>
using namespace shade::parser;

void CallSegment::get_content(formatter::Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) const
{
  generator.handle_function_call_begin();
  generator.handle_function_call_name_begin();
  generator.handle_identifier(m_name);
  generator.handle_function_call_name_end();
  generator.handle_function_call_parameter_list_begin();
  for (Parameters::const_iterator param = m_parameters.begin(); param != m_parameters.end(); ++param)
  {
    generator.handle_function_call_parameter_begin();
    (*param)->get_content(generator, scope, call, error_log);
    generator.handle_function_call_parameter_end();
  }
  generator.handle_function_call_parameter_list_end();
  generator.handle_function_call_end();
}

