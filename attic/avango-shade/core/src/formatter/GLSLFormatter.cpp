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

#include "GLSLFormatter.h"
#include <shade/formatter/Generator.h>
#include "GL2Generator.h"
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

namespace
{
void generate_void_objref_function(shade::formatter::Generator& generator, const std::string& name, shade::ObjectMap::Index ref)
{
    generator.handle_statement_begin();
    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_identifier(name);
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
    generator.handle_function_call_parameter_begin();
    generator.handle_literal_objref(ref);
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_list_end();
    generator.handle_function_call_end();
    generator.handle_statement_end();
}

void generate_call_base_self_function(shade::formatter::Generator& generator)
{
    generator.handle_statement_begin();
    generator.handle_var_definition_begin();
    generator.handle_var_definition_type_begin();
    generator.handle_type(shade::formatter::Constants::objref);
    generator.handle_var_definition_type_end();
    generator.handle_var_definition_name_begin();
    generator.handle_identifier("base_self");
    generator.handle_var_definition_name_end();
    generator.handle_var_definition_code_begin();
    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_identifier("shade_get_base_object");
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
    generator.handle_function_call_parameter_begin();
    generator.handle_identifier("self");
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_list_end();
    generator.handle_function_call_end();
    generator.handle_var_definition_code_end();
    generator.handle_var_definition_end();
    generator.handle_statement_end();
}

void generate_call_index_function(shade::formatter::Generator& generator)
{
    generator.handle_statement_begin();
    generator.handle_var_definition_begin();
    generator.handle_var_definition_type_begin();
    generator.handle_type(shade::formatter::Constants::objref);
    generator.handle_var_definition_type_end();
    generator.handle_var_definition_name_begin();
    generator.handle_identifier("index");
    generator.handle_var_definition_name_end();
    generator.handle_var_definition_code_begin();
    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_identifier("shade_get_object_index");
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
    generator.handle_function_call_parameter_begin();
    generator.handle_identifier("self");
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_list_end();
    generator.handle_function_call_end();
    generator.handle_var_definition_code_end();
    generator.handle_var_definition_end();
    generator.handle_statement_end();
}

void generate_property_access(shade::formatter::Generator& generator, const std::string& var, bool use_array)
{
    if(use_array)
    {
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_operator(shade::formatter::Constants::array_access);
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier(var);
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier("index");
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
        return;
    }

    generator.handle_identifier(var);
}
} // namespace

shade::GLSLFormatter::GLSLFormatter(ShaderEnvironment se) : Formatter(se), m_extern_generator(new formatter::GL2Generator(se)), m_main_generator(new formatter::GL2Generator(se))
{
    m_intern_generator[0] = m_main_generator->construct();
    m_intern_generator[1] = m_main_generator->construct();

    m_main_generator->handle_function_definition_begin();
    m_main_generator->handle_function_definition_return_type_begin();
    m_main_generator->handle_type(formatter::Constants::void_);
    m_main_generator->handle_function_definition_return_type_end();
    m_main_generator->handle_function_definition_name_begin();
    m_main_generator->handle_identifier("main");
    m_main_generator->handle_function_definition_name_end();
    m_main_generator->handle_function_definition_parameter_list_begin();
    m_main_generator->handle_function_definition_parameter_list_end();
    m_main_generator->handle_function_definition_code_begin();
}

shade::GLSLFormatter::GLSLFormatter(ShaderEnvironment se, const shade::formatter::Generator& generator)
    : Formatter(se), m_extern_generator(generator.construct()), m_main_generator(generator.construct())
{
    m_intern_generator[0] = generator.construct();
    m_intern_generator[1] = generator.construct();

    m_main_generator->handle_function_definition_begin();
    m_main_generator->handle_function_definition_return_type_begin();
    m_main_generator->handle_type(formatter::Constants::void_);
    m_main_generator->handle_function_definition_return_type_end();
    m_main_generator->handle_function_definition_name_begin();
    m_main_generator->handle_identifier("main");
    m_main_generator->handle_function_definition_name_end();
    m_main_generator->handle_function_definition_parameter_list_begin();
    m_main_generator->handle_function_definition_parameter_list_end();
    m_main_generator->handle_function_definition_code_begin();
}

void shade::GLSLFormatter::insert_attribute_pre(formatter::Constants::Qualifier qualifier)
{
    m_intern_generator[0]->handle_statement_begin();
    m_intern_generator[0]->handle_var_definition_begin();
    if(qualifier != formatter::Constants::none)
    {
        m_intern_generator[0]->handle_var_definition_qualifier_list_begin();
        m_intern_generator[0]->handle_var_definition_qualifier_begin();
        m_intern_generator[0]->handle_qualifier(qualifier);
        m_intern_generator[0]->handle_var_definition_qualifier_end();
        m_intern_generator[0]->handle_var_definition_qualifier_list_end();
    }
    m_intern_generator[0]->handle_var_definition_type_begin();
}

void shade::GLSLFormatter::insert_attribute_post(const std::string& obj, const std::string& name)
{
    m_intern_generator[0]->handle_var_definition_type_end();
    m_intern_generator[0]->handle_var_definition_name_begin();
    m_intern_generator[0]->handle_identifier(obj + "_" + name);
    m_intern_generator[0]->handle_var_definition_name_end();
}

void shade::GLSLFormatter::insert_attribute(formatter::Constants::Type type, const std::string& obj, const std::string& name, formatter::Constants::Qualifier qualifier)
{
    insert_attribute_pre(qualifier);
    m_intern_generator[0]->handle_type(type);
    insert_attribute_post(obj, name);
    m_intern_generator[0]->handle_var_definition_end();
    m_intern_generator[0]->handle_statement_end();
}

boost::shared_ptr<shade::formatter::Generator>
shade::GLSLFormatter::begin_insert_init_attribute(formatter::Constants::Type type, const std::string& obj, const std::string& name, formatter::Constants::Qualifier qualifier)
{
    insert_attribute_pre(qualifier);

    m_intern_generator[0]->handle_type(type);
    insert_attribute_post(obj, name);
    m_intern_generator[0]->handle_var_definition_code_begin();

    return m_intern_generator[0];
}

void shade::GLSLFormatter::end_insert_init_attribute()
{
    m_intern_generator[0]->handle_var_definition_code_end();
    m_intern_generator[0]->handle_var_definition_end();
    m_intern_generator[0]->handle_statement_end();
}

void shade::GLSLFormatter::insert_array_attribute(formatter::Constants::Type type, const std::string& obj, const std::string& name)
{
    insert_attribute_pre(formatter::Constants::input);

    m_intern_generator[0]->handle_array_begin();
    m_intern_generator[0]->handle_array_type_begin();
    m_intern_generator[0]->handle_type(type);
    m_intern_generator[0]->handle_array_type_end();
    m_intern_generator[0]->handle_array_size_begin();
    m_intern_generator[0]->handle_literal_int(get_num_vertices_in());
    m_intern_generator[0]->handle_array_size_end();
    m_intern_generator[0]->handle_array_end();
    insert_attribute_post(obj, name);
    m_intern_generator[0]->handle_var_definition_end();
    m_intern_generator[0]->handle_statement_end();
}

void shade::GLSLFormatter::insert_multiple_attribute(formatter::Constants::Type type, const std::string& obj, const std::string& name)
{
    for(unsigned int i = 0; i != get_num_vertices_in(); ++i)
    {
        insert_attribute(type, get_indexed_object_name(obj, i), name);
    }
}

void shade::GLSLFormatter::begin_self_argument(const std::string& name)
{
    m_intern_generator[0]->handle_self_begin();
    m_intern_generator[0]->handle_self_name_begin();
    m_intern_generator[0]->handle_identifier(name + "_SELF");
    m_intern_generator[0]->handle_self_name_end();
    m_intern_generator[0]->handle_self_parameter_list_begin();

    m_translator.handle_self_begin();
    m_translator.handle_self_name_begin();
    m_translator.handle_identifier(name + "_SELF");
    m_translator.handle_self_name_end();
    m_translator.handle_self_parameter_list_begin();
}

void shade::GLSLFormatter::insert_self_argument(formatter::Constants::Type type, const std::string& name)
{
    m_intern_generator[0]->handle_self_parameter_type_begin();
    m_intern_generator[0]->handle_type(type);
    m_intern_generator[0]->handle_self_parameter_type_end();
    m_intern_generator[0]->handle_self_parameter_name_begin();
    m_intern_generator[0]->handle_identifier(name);
    m_intern_generator[0]->handle_self_parameter_name_end();

    m_translator.handle_self_parameter_type_begin();
    m_translator.handle_type(type);
    m_translator.handle_self_parameter_type_end();
    m_translator.handle_self_parameter_name_begin();
    m_translator.handle_identifier(name);
    m_translator.handle_self_parameter_name_end();
}

void shade::GLSLFormatter::end_self_argument(void)
{
    m_intern_generator[0]->handle_self_parameter_list_end();
    m_intern_generator[0]->handle_self_end();

    m_translator.handle_self_parameter_list_end();
    m_translator.handle_self_end();
}

void shade::GLSLFormatter::begin_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval)
{
    std::string method(get_method_name(class_, name));
    for(int i = 0; i != 2; ++i)
    {
        m_intern_generator[i]->handle_function_definition_begin();
        m_intern_generator[i]->handle_function_definition_return_type_begin();
        m_intern_generator[i]->handle_type(retval);
        m_intern_generator[i]->handle_function_definition_return_type_end();
        m_intern_generator[i]->handle_function_definition_name_begin();
        m_intern_generator[i]->handle_identifier(method);
        m_intern_generator[i]->handle_function_definition_name_end();
        m_intern_generator[i]->handle_function_definition_parameter_list_begin();
        m_intern_generator[i]->handle_function_definition_parameter_type_begin();
        m_intern_generator[i]->handle_type(shade::formatter::Constants::objref);
        m_intern_generator[i]->handle_function_definition_parameter_type_end();
        m_intern_generator[i]->handle_function_definition_parameter_name_begin();
        m_intern_generator[i]->handle_identifier("self");
        m_intern_generator[i]->handle_function_definition_parameter_name_end();
    }

    m_dispatcher_argcount = 0;
    m_dispatch_return_type = retval;
}

void shade::GLSLFormatter::begin_dispatcher_add_arg(formatter::Constants::Type arg)
{
    ++m_dispatcher_argcount;

    for(int i = 0; i != 2; ++i)
    {
        m_intern_generator[i]->handle_function_definition_parameter_type_begin();
        m_intern_generator[i]->handle_type(arg);
        m_intern_generator[i]->handle_function_definition_parameter_type_end();
        m_intern_generator[i]->handle_function_definition_parameter_name_begin();
        m_intern_generator[i]->handle_identifier("arg" + boost::lexical_cast<std::string>(m_dispatcher_argcount));
        m_intern_generator[i]->handle_function_definition_parameter_name_end();
    }
}

void shade::GLSLFormatter::begin_dispatcher_finish(void)
{
    m_pre_code << ");\n";

    for(int i = 0; i != 2; ++i)
    {
        m_intern_generator[i]->handle_function_definition_parameter_list_end();
    }
    m_intern_generator[1]->handle_function_definition_code_begin();

    if(m_env == geometry_shader)
    {
        m_intern_generator[1]->handle_statement_begin();
        m_intern_generator[1]->handle_var_definition_begin();
        m_intern_generator[1]->handle_var_definition_type_begin();
        m_intern_generator[1]->handle_type(shade::formatter::Constants::objref);
        m_intern_generator[1]->handle_var_definition_type_end();
        m_intern_generator[1]->handle_var_definition_name_begin();
        m_intern_generator[1]->handle_identifier("base_self");
        m_intern_generator[1]->handle_var_definition_name_end();
        m_intern_generator[1]->handle_var_definition_code_begin();
        m_intern_generator[1]->handle_function_call_begin();
        m_intern_generator[1]->handle_function_call_name_begin();
        m_intern_generator[1]->handle_identifier("shade_get_base_object");
        m_intern_generator[1]->handle_function_call_name_end();
        m_intern_generator[1]->handle_function_call_parameter_list_begin();
        m_intern_generator[1]->handle_function_call_parameter_begin();
        m_intern_generator[1]->handle_identifier("self");
        m_intern_generator[1]->handle_function_call_parameter_end();
        m_intern_generator[1]->handle_function_call_parameter_list_end();
        m_intern_generator[1]->handle_function_call_end();
        m_intern_generator[1]->handle_var_definition_code_end();
        m_intern_generator[1]->handle_var_definition_end();
        m_intern_generator[1]->handle_statement_end();
    }

    m_is_first_dispatcher = true;
}

void shade::GLSLFormatter::insert_dispatcher(shade::ObjectMap::Index self, const std::string& obj, const std::string& name)
{
    if(m_is_first_dispatcher)
    {
        m_is_first_dispatcher = false;
        m_intern_generator[1]->handle_switch_begin();
        m_intern_generator[1]->handle_switch_value_begin();
        m_intern_generator[1]->handle_identifier(get_base_self_name());
        m_intern_generator[1]->handle_switch_value_end();
    }

    m_intern_generator[1]->handle_switch_case_begin();
    m_intern_generator[1]->handle_literal_objref(self);
    m_intern_generator[1]->handle_switch_case_end();
    m_intern_generator[1]->handle_switch_code_begin();
    if(m_dispatch_return_type != formatter::Constants::void_)
        m_intern_generator[1]->handle_return_begin();
    else
        m_intern_generator[1]->handle_statement_begin();
    m_intern_generator[1]->handle_function_call_begin();
    m_intern_generator[1]->handle_function_call_name_begin();
    m_intern_generator[1]->handle_identifier(name);
    m_intern_generator[1]->handle_function_call_name_end();
    m_intern_generator[1]->handle_function_call_parameter_list_begin();
    m_intern_generator[1]->handle_function_call_parameter_begin();
    generate_self_name(*m_intern_generator[1], self);
    m_intern_generator[1]->handle_function_call_parameter_end();

    m_dispatch_obj = obj;
}

void shade::GLSLFormatter::insert_inline_dispatcher(shade::ObjectMap::Index self, const std::string& obj, const std::string& class_, const std::string& name)
{
    insert_dispatcher(self, obj, class_ + "_" + name + "_impl");
}

void shade::GLSLFormatter::insert_dispatcher_add_arg(const std::string& name)
{
    m_intern_generator[1]->handle_function_call_parameter_begin();
    m_intern_generator[1]->handle_identifier(m_dispatch_obj + "_" + name);
    m_intern_generator[1]->handle_function_call_parameter_end();
}

void shade::GLSLFormatter::insert_dispatcher_finish(void)
{
    for(unsigned int i = 1; i != m_dispatcher_argcount + 1; ++i)
    {
        m_intern_generator[1]->handle_function_call_parameter_begin();
        m_intern_generator[1]->handle_identifier("arg" + boost::lexical_cast<std::string>(i));
        m_intern_generator[1]->handle_function_call_parameter_end();
    }

    m_intern_generator[1]->handle_function_call_parameter_list_end();
    m_intern_generator[1]->handle_function_call_end();
    if(m_dispatch_return_type != formatter::Constants::void_)
        m_intern_generator[1]->handle_return_end();
    else
        m_intern_generator[1]->handle_statement_end();
    m_intern_generator[1]->handle_switch_code_end();
}

boost::shared_ptr<shade::formatter::Generator> shade::GLSLFormatter::set_dispatcher_return(void)
{
    if(!m_is_first_dispatcher)
        m_intern_generator[1]->handle_switch_end();

    if(m_dispatch_return_type == formatter::Constants::void_)
    {
        return m_main_generator->construct();
    }

    m_intern_generator[1]->handle_return_begin();
    return m_intern_generator[1];
}

void shade::GLSLFormatter::end_dispatcher(void)
{
    if(m_dispatch_return_type != formatter::Constants::void_)
        m_intern_generator[1]->handle_return_end();
    m_intern_generator[1]->handle_function_definition_code_end();

    for(int i = 0; i != 2; ++i)
    {
        m_intern_generator[i]->handle_function_definition_end();
    }
}

void shade::GLSLFormatter::begin_property_dispatch_declaration(
    formatter::Generator& generator, const std::string& class_, const std::string& name, formatter::Constants::Type retval, bool setter) const
{
    generator.handle_function_definition_begin();
    generator.handle_function_definition_return_type_begin();
    generator.handle_type(retval);
    generator.handle_function_definition_return_type_end();
    generator.handle_function_definition_name_begin();
    if(!setter)
        generator.handle_identifier(get_property_get_name(class_, name));
    else
        generator.handle_identifier(get_property_set_name(class_, name));
    generator.handle_function_definition_name_end();
    generator.handle_function_definition_parameter_list_begin();
    generator.handle_function_definition_parameter_type_begin();
    generator.handle_type(formatter::Constants::objref);
    generator.handle_function_definition_parameter_type_end();
    generator.handle_function_definition_parameter_name_begin();
    generator.handle_identifier("self");
    generator.handle_function_definition_parameter_name_end();
    if(setter)
    {
        generator.handle_function_definition_parameter_type_begin();
        generator.handle_type(retval);
        generator.handle_function_definition_parameter_type_end();
        generator.handle_function_definition_parameter_name_begin();
        generator.handle_identifier("arg");
        generator.handle_function_definition_parameter_name_end();
    }
    generator.handle_function_definition_parameter_list_end();
}

void shade::GLSLFormatter::begin_property_dispatch_trunk(const std::string& class_, const std::string& name, formatter::Constants::Type retval)
{
    for(int i = 0; i != 2; ++i)
        m_dispatch_property_generator[i] = m_main_generator->construct();

    m_is_first_property_dispatcher = true;
    m_is_array_dispatch = false;
    m_is_multiple_dispatch = false;
    m_dispatch_property_name = name;

    for(int i = 0; i != 2; ++i)
    {
        begin_property_dispatch_declaration(*m_intern_generator[0], class_, name, retval, i != 0);
        m_intern_generator[0]->handle_function_definition_end();

        begin_property_dispatch_declaration(*m_dispatch_property_generator[i], class_, name, retval, i != 0);
        m_dispatch_property_generator[i]->handle_function_definition_code_begin();
    }
}

void shade::GLSLFormatter::begin_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval)
{
    begin_property_dispatch_trunk(class_, name, retval);

    if(m_env == geometry_shader)
    {
        for(int i = 0; i != 2; ++i)
        {
            generate_call_base_self_function(*m_dispatch_property_generator[i]);
        }
    }
}

void shade::GLSLFormatter::begin_array_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval)
{
    begin_property_dispatch_trunk(class_, name, retval);

    if(m_env == geometry_shader)
    {
        for(int i = 0; i != 2; ++i)
        {
            generate_call_base_self_function(*m_dispatch_property_generator[i]);
        }

        for(int i = 0; i != 2; ++i)
        {
            generate_call_index_function(*m_dispatch_property_generator[i]);
        }

        m_is_array_dispatch = true;
    }
}

void shade::GLSLFormatter::begin_multiple_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval)
{
    begin_property_dispatch_trunk(class_, name, retval);
    m_is_multiple_dispatch = true;
}

void shade::GLSLFormatter::begin_deferred_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval)
{
    begin_property_dispatcher(class_, name, retval);
}

void shade::GLSLFormatter::insert_property_dispatcher(shade::ObjectMap::Index self, const std::string& obj)
{
    if((!m_dispatch_property_generator[0]) || (!m_dispatch_property_generator[1]))
        return;

    if(m_is_first_property_dispatcher)
    {
        m_is_first_property_dispatcher = false;

        for(int i = 0; i != 2; ++i)
        {
            m_dispatch_property_generator[i]->handle_switch_begin();
            m_dispatch_property_generator[i]->handle_switch_value_begin();
            if(m_is_multiple_dispatch)
                m_dispatch_property_generator[i]->handle_identifier("self");
            else
                m_dispatch_property_generator[i]->handle_identifier(get_base_self_name());
            m_dispatch_property_generator[i]->handle_switch_value_end();
        }
    }

    int num_vertices(m_is_multiple_dispatch ? get_num_vertices_in() : 1);
    for(int j = 0; j != num_vertices; ++j)
    {
        for(int i = 0; i != 2; ++i)
        {
            m_dispatch_property_generator[i]->handle_switch_case_begin();
            m_dispatch_property_generator[i]->handle_literal_objref(j * get_num_classes() + self);
            m_dispatch_property_generator[i]->handle_switch_case_end();
            m_dispatch_property_generator[i]->handle_switch_code_begin();
            m_dispatch_property_generator[i]->handle_return_begin();
            std::string var((m_is_multiple_dispatch ? get_indexed_object_name(obj, j) : obj) + "_" + m_dispatch_property_name);
            if(i == 0)
            {
                generate_property_access(*m_dispatch_property_generator[i], var, m_is_array_dispatch);
            }
            else
            {
                m_dispatch_property_generator[i]->handle_function_call_begin();
                m_dispatch_property_generator[i]->handle_function_call_name_begin();
                m_dispatch_property_generator[i]->handle_operator(formatter::Constants::assignment);
                m_dispatch_property_generator[i]->handle_function_call_name_end();
                m_dispatch_property_generator[i]->handle_function_call_parameter_list_begin();
                m_dispatch_property_generator[i]->handle_function_call_parameter_begin();
                generate_property_access(*m_dispatch_property_generator[i], var, m_is_array_dispatch);
                m_dispatch_property_generator[i]->handle_function_call_parameter_end();
                m_dispatch_property_generator[i]->handle_function_call_parameter_begin();
                m_dispatch_property_generator[i]->handle_identifier("arg");
                m_dispatch_property_generator[i]->handle_function_call_parameter_end();
                m_dispatch_property_generator[i]->handle_function_call_parameter_list_end();
                m_dispatch_property_generator[i]->handle_function_call_end();
            }
            m_dispatch_property_generator[i]->handle_return_end();
            m_dispatch_property_generator[i]->handle_switch_code_end();
        }
    }
}

void shade::GLSLFormatter::insert_deferred_property_dispatcher(
    shade::ObjectMap::Index self, shade::ObjectMap::Index obj, const std::string& iface_getter, const std::string& getter, const std::string& iface_setter, const std::string& setter)
{
    if((!m_dispatch_property_generator[0]) || (!m_dispatch_property_generator[1]))
        return;

    if(m_is_first_property_dispatcher)
    {
        m_is_first_property_dispatcher = false;

        for(int i = 0; i != 2; ++i)
        {
            m_dispatch_property_generator[i]->handle_switch_begin();
            m_dispatch_property_generator[i]->handle_switch_value_begin();
            m_dispatch_property_generator[i]->handle_identifier(get_base_self_name());
            m_dispatch_property_generator[i]->handle_switch_value_end();
        }
    }

    for(int i = 0; i != 2; ++i)
    {
        if(((i == 0) ? getter : setter).empty())
            continue;

        m_dispatch_property_generator[i]->handle_switch_case_begin();
        m_dispatch_property_generator[i]->handle_literal_objref(self);
        m_dispatch_property_generator[i]->handle_switch_case_end();
        m_dispatch_property_generator[i]->handle_switch_code_begin();
        m_dispatch_property_generator[i]->handle_return_begin();
        m_dispatch_property_generator[i]->handle_function_call_begin();
        m_dispatch_property_generator[i]->handle_function_call_name_begin();
        std::string func((i == 0) ? (iface_getter + "_" + getter) : (iface_setter + "_" + setter));
        m_dispatch_property_generator[i]->handle_identifier(func);
        m_dispatch_property_generator[i]->handle_function_call_name_end();
        m_dispatch_property_generator[i]->handle_function_call_parameter_list_begin();
        m_dispatch_property_generator[i]->handle_function_call_parameter_begin();
        if(m_env == geometry_shader)
        {
            m_dispatch_property_generator[i]->handle_function_call_begin();
            m_dispatch_property_generator[i]->handle_function_call_name_begin();
            m_dispatch_property_generator[i]->handle_identifier("shade_get_linked_object");
            m_dispatch_property_generator[i]->handle_function_call_name_end();
            m_dispatch_property_generator[i]->handle_function_call_parameter_list_begin();
            m_dispatch_property_generator[i]->handle_function_call_parameter_begin();
            m_dispatch_property_generator[i]->handle_identifier("self");
            m_dispatch_property_generator[i]->handle_function_call_parameter_end();
            m_dispatch_property_generator[i]->handle_function_call_parameter_begin();
            m_dispatch_property_generator[i]->handle_literal_objref(obj);
            m_dispatch_property_generator[i]->handle_function_call_parameter_end();
            m_dispatch_property_generator[i]->handle_function_call_parameter_list_end();
            m_dispatch_property_generator[i]->handle_function_call_end();
        }
        else
        {
            m_dispatch_property_generator[i]->handle_literal_objref(obj);
        }
        m_dispatch_property_generator[i]->handle_function_call_parameter_end();
        if(i == 1)
        {
            m_dispatch_property_generator[i]->handle_function_call_parameter_begin();
            m_dispatch_property_generator[i]->handle_identifier("arg");
            m_dispatch_property_generator[i]->handle_function_call_parameter_end();
        }
        m_dispatch_property_generator[i]->handle_function_call_parameter_list_end();
        m_dispatch_property_generator[i]->handle_function_call_end();
        m_dispatch_property_generator[i]->handle_return_end();
        m_dispatch_property_generator[i]->handle_switch_code_end();
    }
}

boost::shared_ptr<shade::formatter::Generator> shade::GLSLFormatter::set_property_dispatcher_return(void)
{
    m_generator = m_main_generator->construct();

    if((!m_dispatch_property_generator[0]) || (!m_dispatch_property_generator[1]))
        return m_generator;

    if(!m_is_first_property_dispatcher)
    {
        for(int i = 0; i != 2; ++i)
            m_dispatch_property_generator[i]->handle_switch_end();
    }

    return m_generator;
}

void shade::GLSLFormatter::end_property_dispatcher(void)
{
    if((!m_dispatch_property_generator[0]) || (!m_dispatch_property_generator[1]))
        return;

    for(int i = 0; i != 2; ++i)
    {
        m_dispatch_property_generator[i]->handle_return_begin();
        m_dispatch_property_generator[i]->append(*m_generator);
        m_dispatch_property_generator[i]->handle_return_end();

        m_dispatch_property_generator[i]->handle_function_definition_code_end();
        m_dispatch_property_generator[i]->handle_function_definition_end();

        m_intern_generator[1]->append(*m_dispatch_property_generator[i]);
    }
}

void shade::GLSLFormatter::invoke(const std::string& class_, const std::string& main) { generate_void_objref_function(*m_main_generator, get_method_name(class_, main), 0); }

void shade::GLSLFormatter::initialize(shade::ObjectMap::Index obj, const std::string& class_, const std::string& method)
{
    generate_void_objref_function(*m_intern_generator[1], get_method_name(class_, method), obj);
}

void shade::GLSLFormatter::begin_initialize(const std::string& name, bool invoke)
{
    m_intern_generator[1]->handle_function_definition_begin();
    m_intern_generator[1]->handle_function_definition_return_type_begin();
    m_intern_generator[1]->handle_type(formatter::Constants::void_);
    m_intern_generator[1]->handle_function_definition_return_type_end();
    m_intern_generator[1]->handle_function_definition_name_begin();
    m_intern_generator[1]->handle_identifier(name);
    m_intern_generator[1]->handle_function_definition_name_end();
    m_intern_generator[1]->handle_function_definition_parameter_list_begin();
    m_intern_generator[1]->handle_function_definition_parameter_list_end();
    m_intern_generator[1]->handle_function_definition_code_begin();

    if(invoke)
    {
        m_main_generator->handle_statement_begin();
        m_main_generator->handle_function_call_begin();
        m_main_generator->handle_function_call_name_begin();
        m_main_generator->handle_identifier(name);
        m_main_generator->handle_function_call_name_end();
        m_main_generator->handle_function_call_parameter_list_begin();
        m_main_generator->handle_function_call_parameter_list_end();
        m_main_generator->handle_function_call_end();
        m_main_generator->handle_statement_end();
    }
}

void shade::GLSLFormatter::end_initialize(void)
{
    m_intern_generator[1]->handle_function_definition_code_end();
    m_intern_generator[1]->handle_function_definition_end();
}

void shade::GLSLFormatter::insert_sources(const std::string& sources) { m_sources = sources; }

void shade::GLSLFormatter::generate_header_code(formatter::Generator& generator)
{
    if(m_env != geometry_shader)
        return;

    const std::string num_classes(boost::lexical_cast<std::string>(get_num_classes()));

    generator.handle_statement_begin();
    generator.handle_var_definition_begin();
    generator.handle_var_definition_type_begin();
    generator.handle_type(shade::formatter::Constants::int_);
    generator.handle_var_definition_type_end();
    generator.handle_var_definition_name_begin();
    generator.handle_identifier("shade_num_classes");
    generator.handle_var_definition_name_end();
    generator.handle_var_definition_code_begin();
    generator.handle_literal_int(get_num_classes());
    generator.handle_var_definition_code_end();
    generator.handle_var_definition_end();
    generator.handle_statement_end();

    generator.handle_statement_begin();
    generator.handle_var_definition_begin();
    generator.handle_var_definition_type_begin();
    generator.handle_type(shade::formatter::Constants::int_);
    generator.handle_var_definition_type_end();
    generator.handle_var_definition_name_begin();
    generator.handle_identifier("shade_vertices_in");
    generator.handle_var_definition_name_end();
    generator.handle_var_definition_code_begin();
    generator.handle_literal_int(get_num_vertices_in());
    generator.handle_var_definition_code_end();
    generator.handle_var_definition_end();
    generator.handle_statement_end();

    generator.handle_function_definition_begin();
    generator.handle_function_definition_return_type_begin();
    generator.handle_type(formatter::Constants::objref);
    generator.handle_function_definition_return_type_end();
    generator.handle_function_definition_name_begin();
    generator.handle_identifier("shade_get_base_object");
    generator.handle_function_definition_name_end();
    generator.handle_function_definition_parameter_list_begin();
    generator.handle_function_definition_parameter_type_begin();
    generator.handle_type(formatter::Constants::objref);
    generator.handle_function_definition_parameter_type_end();
    generator.handle_function_definition_parameter_name_begin();
    generator.handle_identifier("index");
    generator.handle_function_definition_parameter_name_end();
    generator.handle_function_definition_parameter_list_end();
    generator.handle_function_definition_code_begin();
    {
        // return int(mod(index, num_classes));
        generator.handle_return_begin();
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_type(formatter::Constants::int_);
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        {
            generator.handle_function_call_begin();
            generator.handle_function_call_name_begin();
            generator.handle_identifier("mod");
            generator.handle_function_call_name_end();
            generator.handle_function_call_parameter_list_begin();
            generator.handle_function_call_parameter_begin();
            generator.handle_identifier("index");
            generator.handle_function_call_parameter_end();
            generator.handle_function_call_parameter_begin();
            generator.handle_literal_int(get_num_classes());
            generator.handle_function_call_parameter_end();
            generator.handle_function_call_parameter_list_end();
            generator.handle_function_call_end();
        }
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
        generator.handle_return_end();
    }
    generator.handle_function_definition_code_end();
    generator.handle_function_definition_end();

    generator.handle_function_definition_begin();
    generator.handle_function_definition_return_type_begin();
    generator.handle_type(formatter::Constants::objref);
    generator.handle_function_definition_return_type_end();
    generator.handle_function_definition_name_begin();
    generator.handle_identifier("shade_get_object_index");
    generator.handle_function_definition_name_end();
    generator.handle_function_definition_parameter_list_begin();
    generator.handle_function_definition_parameter_type_begin();
    generator.handle_type(formatter::Constants::objref);
    generator.handle_function_definition_parameter_type_end();
    generator.handle_function_definition_parameter_name_begin();
    generator.handle_identifier("index");
    generator.handle_function_definition_parameter_name_end();
    generator.handle_function_definition_parameter_list_end();
    generator.handle_function_definition_code_begin();
    {
        // return (index - shade_get_base_object(index))/num_classes;
        generator.handle_return_begin();
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_operator(formatter::Constants::divide);
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        {
            generator.handle_function_call_begin();
            generator.handle_function_call_name_begin();
            generator.handle_operator(formatter::Constants::subtract);
            generator.handle_function_call_name_end();
            generator.handle_function_call_parameter_list_begin();
            generator.handle_function_call_parameter_begin();
            generator.handle_identifier("index");
            generator.handle_function_call_parameter_end();
            generator.handle_function_call_parameter_begin();
            {
                generator.handle_function_call_begin();
                generator.handle_function_call_name_begin();
                generator.handle_identifier("shade_get_base_object");
                generator.handle_function_call_name_end();
                generator.handle_function_call_parameter_list_begin();
                generator.handle_function_call_parameter_begin();
                generator.handle_identifier("index");
                generator.handle_function_call_parameter_end();
                generator.handle_function_call_parameter_list_end();
                generator.handle_function_call_end();
            }
            generator.handle_function_call_parameter_end();
            generator.handle_function_call_parameter_list_end();
            generator.handle_function_call_end();
        }
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_begin();
        generator.handle_literal_int(get_num_classes());
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
        generator.handle_return_end();
    }
    generator.handle_function_definition_code_end();
    generator.handle_function_definition_end();

    generator.handle_function_definition_begin();
    generator.handle_function_definition_return_type_begin();
    generator.handle_type(formatter::Constants::objref);
    generator.handle_function_definition_return_type_end();
    generator.handle_function_definition_name_begin();
    generator.handle_identifier("shade_get_linked_object");
    generator.handle_function_definition_name_end();
    generator.handle_function_definition_parameter_list_begin();
    generator.handle_function_definition_parameter_type_begin();
    generator.handle_type(formatter::Constants::objref);
    generator.handle_function_definition_parameter_type_end();
    generator.handle_function_definition_parameter_name_begin();
    generator.handle_identifier("reference");
    generator.handle_function_definition_parameter_name_end();
    generator.handle_function_definition_parameter_type_begin();
    generator.handle_type(formatter::Constants::objref);
    generator.handle_function_definition_parameter_type_end();
    generator.handle_function_definition_parameter_name_begin();
    generator.handle_identifier("index");
    generator.handle_function_definition_parameter_name_end();
    generator.handle_function_definition_parameter_list_end();
    generator.handle_function_definition_code_begin();
    {
        // return shade_get_base_object(index)+num_classes*shade_get_object_index(reference);
        generator.handle_return_begin();
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_operator(formatter::Constants::add);
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        {
            generator.handle_function_call_begin();
            generator.handle_function_call_name_begin();
            generator.handle_identifier("shade_get_base_object");
            generator.handle_function_call_name_end();
            generator.handle_function_call_parameter_list_begin();
            generator.handle_function_call_parameter_begin();
            generator.handle_identifier("index");
            generator.handle_function_call_parameter_end();
            generator.handle_function_call_parameter_list_end();
            generator.handle_function_call_end();
        }
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_begin();
        {
            generator.handle_function_call_begin();
            generator.handle_function_call_name_begin();
            generator.handle_operator(formatter::Constants::multiply);
            generator.handle_function_call_name_end();
            generator.handle_function_call_parameter_list_begin();
            generator.handle_function_call_parameter_begin();
            generator.handle_literal_int(get_num_classes());
            generator.handle_function_call_parameter_end();
            generator.handle_function_call_parameter_begin();
            {
                generator.handle_function_call_begin();
                generator.handle_function_call_name_begin();
                generator.handle_identifier("shade_get_object_index");
                generator.handle_function_call_name_end();
                generator.handle_function_call_parameter_list_begin();
                generator.handle_function_call_parameter_begin();
                generator.handle_identifier("reference");
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
        generator.handle_return_end();
    }
    generator.handle_function_definition_code_end();
    generator.handle_function_definition_end();

    generator.handle_function_definition_begin();
    generator.handle_function_definition_return_type_begin();
    generator.handle_type(formatter::Constants::objref);
    generator.handle_function_definition_return_type_end();
    generator.handle_function_definition_name_begin();
    generator.handle_identifier("shade_get_object");
    generator.handle_function_definition_name_end();
    generator.handle_function_definition_parameter_list_begin();
    generator.handle_function_definition_parameter_type_begin();
    generator.handle_type(formatter::Constants::objref);
    generator.handle_function_definition_parameter_type_end();
    generator.handle_function_definition_parameter_name_begin();
    generator.handle_identifier("obj");
    generator.handle_function_definition_parameter_name_end();
    generator.handle_function_definition_parameter_type_begin();
    generator.handle_type(formatter::Constants::objref);
    generator.handle_function_definition_parameter_type_end();
    generator.handle_function_definition_parameter_name_begin();
    generator.handle_identifier("index");
    generator.handle_function_definition_parameter_name_end();
    generator.handle_function_definition_parameter_list_end();
    generator.handle_function_definition_code_begin();
    {
        // return shade_get_base_object(obj)+num_classes*index;
        generator.handle_return_begin();
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_operator(formatter::Constants::add);
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        {
            generator.handle_function_call_begin();
            generator.handle_function_call_name_begin();
            generator.handle_identifier("shade_get_base_object");
            generator.handle_function_call_name_end();
            generator.handle_function_call_parameter_list_begin();
            generator.handle_function_call_parameter_begin();
            generator.handle_identifier("obj");
            generator.handle_function_call_parameter_end();
            generator.handle_function_call_parameter_list_end();
            generator.handle_function_call_end();
        }
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_begin();
        {
            generator.handle_function_call_begin();
            generator.handle_function_call_name_begin();
            generator.handle_operator(formatter::Constants::multiply);
            generator.handle_function_call_name_end();
            generator.handle_function_call_parameter_list_begin();
            generator.handle_function_call_parameter_begin();
            generator.handle_literal_int(get_num_classes());
            generator.handle_function_call_parameter_end();
            generator.handle_function_call_parameter_begin();
            generator.handle_identifier("index");
            generator.handle_function_call_parameter_end();
            generator.handle_function_call_parameter_list_end();
            generator.handle_function_call_end();
        }
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
        generator.handle_return_end();
    }
    generator.handle_function_definition_code_end();
    generator.handle_function_definition_end();

    generator.handle_function_definition_begin();
    generator.handle_function_definition_return_type_begin();
    generator.handle_type(formatter::Constants::void_);
    generator.handle_function_definition_return_type_end();
    generator.handle_function_definition_name_begin();
    generator.handle_identifier("shade_initialize_post");
    generator.handle_function_definition_name_end();
    generator.handle_function_definition_parameter_list_begin();
    generator.handle_function_definition_parameter_list_end();
    generator.handle_function_definition_end();

    generator.handle_function_definition_begin();
    generator.handle_function_definition_return_type_begin();
    generator.handle_type(formatter::Constants::void_);
    generator.handle_function_definition_return_type_end();
    generator.handle_function_definition_name_begin();
    generator.handle_identifier("shade_emit_vertex");
    generator.handle_function_definition_name_end();
    generator.handle_function_definition_parameter_list_begin();
    generator.handle_function_definition_parameter_list_end();
    generator.handle_function_definition_code_begin();
    {
        generator.handle_statement_begin();
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_identifier("shade_initialize_post");
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
        generator.handle_statement_end();
        generator.handle_statement_begin();
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_identifier("EmitVertex");
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
        generator.handle_statement_end();
    }
    generator.handle_function_definition_code_end();
    generator.handle_function_definition_end();
}

void shade::GLSLFormatter::generate(formatter::Generator& generator)
{
    generate_header_code(generator);
    generator.append(*m_intern_generator[0]);
    if(!m_sources.empty())
        generator.handle_verbatim(m_sources);
    generator.append(*m_extern_generator);
    generator.append(*m_intern_generator[1]);

    m_main_generator->handle_function_definition_code_end();
    m_main_generator->handle_function_definition_end();
    generator.append(*m_main_generator);
}

std::string shade::GLSLFormatter::get_type_str(formatter::Constants::Type type)
{
    static const char* type_names[] = {"float", "vec2", "vec3", "vec4", "mat4", "sampler2D", "sampler2DShadow", "sampler2DArray", "usampler2D", "samplerCube", "OBJREF", "LIST", "void"};

    return type_names[type];
}

std::string shade::GLSLFormatter::get_qualifier_str(formatter::Constants::Qualifier qualifier)
{
    static const char* qualifier_names[] = {
        "",
        "uniform",
        "const",
    };

    if(qualifier == formatter::Constants::input)
    {
        static const char* qualifier_input_names[] = {
            "attribute",
            "varying",
            "varying in",
        };
        return qualifier_input_names[m_env];
    }
    else if(qualifier == formatter::Constants::output)
    {
        static const char* qualifier_output_names[] = {
            "varying",
            "",
            "varying out",
        };
        return qualifier_output_names[m_env];
    }

    return qualifier_names[qualifier];
}

std::string shade::GLSLFormatter::get_method_name(const std::string& class_, const std::string& name) { return class_ + "_" + name; }

std::string shade::GLSLFormatter::get_property_get_name(const std::string& class_, const std::string& name) const { return class_ + "_get_" + name; }

std::string shade::GLSLFormatter::get_property_set_name(const std::string& class_, const std::string& name) const { return class_ + "_set_" + name; }

std::string shade::GLSLFormatter::get_indexed_object_name(const std::string& object, unsigned int index)
{
    std::ostringstream name;
    name << object << "_" << index;
    return name.str();
}

std::string shade::GLSLFormatter::get_base_self_name(void) const
{
    if(m_env == geometry_shader)
        return "base_self";
    else
        return "self";
}

std::string shade::GLSLFormatter::get_self_name(ObjectMap::Index self) const
{
    if(m_env == geometry_shader)
        return "self";
    else
        return boost::lexical_cast<std::string>(self);
}

void shade::GLSLFormatter::generate_self_name(formatter::Generator& generator, ObjectMap::Index self) const
{
    if(m_env == geometry_shader)
        generator.handle_identifier("self");
    else
        generator.handle_literal_objref(self);
}

void shade::GLSLFormatter::begin_inline_method(const std::string& class_, const std::string& name, formatter::Constants::Type retval)
{
    m_extern_generator->handle_function_definition_begin();
    m_extern_generator->handle_function_definition_return_type_begin();
    m_extern_generator->handle_type(retval);
    m_extern_generator->handle_function_definition_return_type_end();
    m_extern_generator->handle_function_definition_name_begin();
    m_extern_generator->handle_identifier(get_method_name(class_, name + "_impl"));
    m_extern_generator->handle_function_definition_name_end();
    m_extern_generator->handle_function_definition_parameter_list_begin();
    m_translator.generate(*m_extern_generator, class_ + "_SELF");

    m_inline_method_argcount = 0;
}

void shade::GLSLFormatter::add_arg_inline_method(formatter::Constants::Type arg)
{
    ++m_inline_method_argcount;
    m_extern_generator->handle_function_definition_parameter_type_begin();
    m_extern_generator->handle_type(arg);
    m_extern_generator->handle_function_definition_parameter_type_end();
    m_extern_generator->handle_function_definition_parameter_name_begin();
    m_extern_generator->handle_identifier("arg" + boost::lexical_cast<std::string>(m_inline_method_argcount));
    m_extern_generator->handle_function_definition_parameter_name_end();
}

boost::shared_ptr<shade::formatter::Generator> shade::GLSLFormatter::insert_inline_method(void)
{
    m_extern_generator->handle_function_definition_parameter_list_end();
    m_extern_generator->handle_function_definition_code_begin();

    return m_extern_generator;
}

void shade::GLSLFormatter::end_inline_method(void)
{
    m_extern_generator->handle_function_definition_code_end();
    m_extern_generator->handle_function_definition_end();
}
