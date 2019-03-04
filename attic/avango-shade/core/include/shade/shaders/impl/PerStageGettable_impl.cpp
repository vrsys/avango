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
SHADE_TEMPLATE_T_INIT(PerStageGettable, "", SHADE_ENV_DEFS(vertex_stage, (get_vertex)) SHADE_ENV_DEFS(fragment_stage, (get_fragment)) SHADE_ENV_DEFS(geometry_stage, (get_geometry)), SHADE_NONE)
}
} // namespace shade

template <template <class> class T>
T<shade::Type> shade::shaders::PerStageGettable<T>::get(void)
{
    return this->template invoke_inline<T<Type>>(boost::bind(&PerStageGettable<T>::get_inline, boost::ref(*this), _1));
}

template <template <class> class T>
void shade::shaders::PerStageGettable<T>::get_inline(formatter::Generator& generator)
{
    std::string class_name(PerStageGettable::get_class_name());

    generator.handle_environment_begin(vertex_stage);
    generator.handle_return_begin();
    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_identifier(class_name + "_get_vertex");
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
    generator.handle_function_call_parameter_begin();
    generator.handle_identifier("self");
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_list_end();
    generator.handle_function_call_end();
    generator.handle_return_end();
    generator.handle_environment_end();

    generator.handle_environment_begin(fragment_stage);
    generator.handle_return_begin();
    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_identifier(class_name + "_get_fragment");
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
    generator.handle_function_call_parameter_begin();
    generator.handle_identifier("self");
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_list_end();
    generator.handle_function_call_end();
    generator.handle_return_end();
    generator.handle_environment_end();

    generator.handle_environment_begin(geometry_stage);
    generator.handle_return_begin();
    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_identifier(class_name + "_get_geometry");
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
    generator.handle_function_call_parameter_begin();
    generator.handle_identifier("self");
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_list_end();
    generator.handle_function_call_end();
    generator.handle_return_end();
    generator.handle_environment_end();

    get_default_inline(generator);
}

template <template <class> class T>
void shade::shaders::PerStageGettable<T>::get_default_inline(formatter::Generator& generator)
{
    generator.handle_return_begin();
    T<shade::Type> type;
    type.generate_constructor(generator);
    generator.handle_return_end();
}

template <template <class> class T>
T<shade::Type> shade::shaders::PerStageGettable<T>::get_vertex(void)
{
    return this->template invoke_inline<T<Type>>(boost::bind(&PerStageGettable<T>::get_default_inline, boost::ref(*this), _1));
}

template <template <class> class T>
T<shade::Type> shade::shaders::PerStageGettable<T>::get_fragment(void)
{
    return this->template invoke_inline<T<Type>>(boost::bind(&PerStageGettable<T>::get_default_inline, boost::ref(*this), _1));
}

template <template <class> class T>
T<shade::Type> shade::shaders::PerStageGettable<T>::get_geometry(void)
{
    return this->template invoke_inline<T<Type>>(boost::bind(&PerStageGettable<T>::get_default_inline, boost::ref(*this), _1));
}
