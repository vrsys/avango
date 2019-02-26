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

template <template <class> class T>
void shade::shaders::Interpolator<T>::generate_weighting(formatter::Generator& generator, const std::string& component, int index) const
{
    std::string class_name(Interpolator<T>::get_class_name());

    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_operator(shade::formatter::Constants::multiply);
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
    generator.handle_function_call_parameter_begin();
    {
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_operator(shade::formatter::Constants::component_access);
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier("uvw");
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier(component);
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
    }
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_begin();
    {
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_identifier(class_name + "_get_input");
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        {
            generator.handle_function_call_begin();
            generator.handle_function_call_name_begin();
            generator.handle_identifier("shade_get_object");
            generator.handle_function_call_name_end();
            generator.handle_function_call_parameter_list_begin();
            generator.handle_function_call_parameter_begin();
            generator.handle_identifier("self");
            generator.handle_function_call_parameter_end();
            generator.handle_function_call_parameter_begin();
            generator.handle_literal_int(index);
            generator.handle_function_call_parameter_end();
            generator.handle_function_call_parameter_list_end();
            generator.handle_function_call_end();
        }
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
    }
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_list_end();
    generator.handle_function_call_end();
}

template <template <class> class T>
void shade::shaders::Interpolator<T>::init_post_geometry_inline(formatter::Generator& generator) const
{
    T<Type> type;
    std::string class_name(Interpolator<T>::get_class_name());

    // vec3 uvw = Geometry_get_uvw(geometry);
    generator.handle_statement_begin();
    generator.handle_var_definition_begin();
    generator.handle_var_definition_type_begin();
    generator.handle_type(shade::formatter::Constants::vec3);
    generator.handle_var_definition_type_end();
    generator.handle_var_definition_name_begin();
    generator.handle_identifier("uvw");
    generator.handle_var_definition_name_end();
    generator.handle_var_definition_code_begin();
    {
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_identifier("Geometry_get_uvw");
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier("geometry");
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
    }
    generator.handle_var_definition_code_end();
    generator.handle_var_definition_end();
    generator.handle_statement_end();

    // Type value = uvw.x * ClassName_get_input(shade_get_object(self,0));
    generator.handle_statement_begin();
    generator.handle_var_definition_begin();
    generator.handle_var_definition_type_begin();
    generator.handle_type(type.get_formatter_type());
    generator.handle_var_definition_type_end();
    generator.handle_var_definition_name_begin();
    generator.handle_identifier("value");
    generator.handle_var_definition_name_end();
    generator.handle_var_definition_code_begin();
    {
        generate_weighting(generator, "x", 0);
    }
    generator.handle_var_definition_code_end();
    generator.handle_var_definition_end();
    generator.handle_statement_end();

    // value += uvw.y * ClassName_get_input(shade_get_object(self,1));
    generator.handle_statement_begin();
    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_operator(shade::formatter::Constants::assignment_add);
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
    generator.handle_function_call_parameter_begin();
    generator.handle_identifier("value");
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_begin();
    {
        generate_weighting(generator, "y", 1);
    }
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_list_end();
    generator.handle_function_call_end();
    generator.handle_statement_end();

    // value += uvw.z * ClassName_get_input(shade_get_object(self,2));
    generator.handle_statement_begin();
    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_operator(shade::formatter::Constants::assignment_add);
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
    generator.handle_function_call_parameter_begin();
    generator.handle_identifier("value");
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_begin();
    {
        generate_weighting(generator, "z", 2);
    }
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_list_end();
    generator.handle_function_call_end();
    generator.handle_statement_end();

    // ClassName_set_output(self, value);
    generator.handle_statement_begin();
    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_identifier(class_name + "_set_output");
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
    generator.handle_function_call_parameter_begin();
    generator.handle_identifier("self");
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_begin();
    generator.handle_identifier("value");
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_list_end();
    generator.handle_function_call_end();
    generator.handle_statement_end();
}

template <template <class> class T>
shade::void_<> shade::shaders::Interpolator<T>::init_post_geometry(void)
{
    return this->template invoke_inline<void_<>>(boost::bind(&Interpolator<T>::init_post_geometry_inline, boost::ref(*this), _1));
}
