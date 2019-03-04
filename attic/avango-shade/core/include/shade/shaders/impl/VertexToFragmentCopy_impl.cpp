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

#include <boost/bind.hpp>

namespace shade
{
namespace shaders
{
SHADE_TEMPLATE_T_INIT(VertexToFragmentCopy, "", SHADE_NONE, SHADE_ENV_DEFS(application_stage, (value)) SHADE_DEFS((copy)))
}
} // namespace shade

template <template <class> class T>
void shade::shaders::VertexToFragmentCopy<T>::init_vertex_inline(formatter::Generator& generator) const
{
    std::string class_name(VertexToFragmentCopy::get_class_name());

    generator.handle_statement_begin();
    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_identifier(class_name + "_set_copy");
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
    generator.handle_function_call_parameter_begin();
    generator.handle_identifier("self");
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_begin();
    {
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_identifier("Gettable_" + T<Type>().get_uniq_id() + "_get");
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier("value");
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
    }
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_list_end();
    generator.handle_function_call_end();
    generator.handle_statement_end();
}

template <template <class> class T>
void shade::shaders::VertexToFragmentCopy<T>::get_fragment_inline(formatter::Generator& generator) const
{
    std::string class_name(VertexToFragmentCopy::get_class_name());

    generator.handle_return_begin();
    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_identifier(class_name + "_get_copy");
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
    generator.handle_function_call_parameter_begin();
    generator.handle_identifier("self");
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_list_end();
    generator.handle_function_call_end();
    generator.handle_return_end();
}

template <template <class> class T>
shade::void_<> shade::shaders::VertexToFragmentCopy<T>::init_vertex(void)
{
    return this->template invoke_inline<void_<>>(boost::bind(&VertexToFragmentCopy<T>::init_vertex_inline, boost::ref(*this), _1));
};

template <template <class> class T>
T<shade::Type> shade::shaders::VertexToFragmentCopy<T>::get_fragment(void)
{
    return this->template invoke_inline<T<Type>>(boost::bind(&VertexToFragmentCopy<T>::get_fragment_inline, boost::ref(*this), _1));
};
