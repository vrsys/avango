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

#include <boost/lexical_cast.hpp>

template <shade::ShaderEnvironment output, shade::ShaderEnvironment input>
shade::inout<output, input>::inout(formatter::Constants::Type type) : Type(type)
{
}

template <shade::ShaderEnvironment output, shade::ShaderEnvironment input>
bool shade::inout<output, input>::is_writable(const ShaderEnvironment& se) const
{
    return true;
}

template <shade::ShaderEnvironment output, shade::ShaderEnvironment input>
bool shade::inout<output, input>::exists(const ShaderEnvironment& se) const
{
    return ((se == input) || (se == output));
}

template <shade::ShaderEnvironment output, shade::ShaderEnvironment input>
void shade::inout<output, input>::output_attribute(boost::shared_ptr<Type::State> state, Formatter* fmt, const std::string& obj, const std::string& name) const
{
    if(fmt->get_shader_env() == output)
        fmt->insert_attribute(m_type, obj, name, formatter::Constants::output);
    else if(fmt->get_shader_env() == input)
    {
        if(fmt->get_shader_env() == geometry_shader)
            fmt->insert_array_attribute(m_type, obj, name);
        else
            fmt->insert_attribute(m_type, obj, name, formatter::Constants::input);
    }
}

template <shade::ShaderEnvironment output, shade::ShaderEnvironment input>
bool shade::inout<output, input>::output_begin_property_dispatcher(Formatter* fmt, const std::string& obj, const std::string name) const
{
    if((fmt->get_shader_env() != input) && (fmt->get_shader_env() != output))
        return false;

    if((fmt->get_shader_env() == geometry_shader) && (fmt->get_shader_env() == input))
        fmt->begin_array_property_dispatcher(obj, name, m_type);
    else
        fmt->begin_property_dispatcher(obj, name, m_type);

    return true;
}

template <shade::ShaderEnvironment output, shade::ShaderEnvironment input>
std::string shade::inout<output, input>::get_uniq_id(void) const
{
    std::string front_part(join_names("inout", boost::lexical_cast<std::string>(output)));
    return join_names(front_part, boost::lexical_cast<std::string>(input));
}
