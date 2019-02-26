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

#include "StorageFormatter.h"
#include "formatter/DumpGenerator.h"
#include <boost/lexical_cast.hpp>
#include <iostream>

shade::StorageFormatter::StorageFormatter(ShaderEnvironment env) : Formatter(env) {}

void shade::StorageFormatter::insert_attribute(formatter::Constants::Type type, const std::string& obj, const std::string& name, formatter::Constants::Qualifier qualifier)
{
    Attribute attr(type, qualifier);
    m_attributes[obj + name] = attr;
}

boost::shared_ptr<shade::formatter::Generator>
shade::StorageFormatter::begin_insert_init_attribute(formatter::Constants::Type type, const std::string& obj, const std::string& name, formatter::Constants::Qualifier qualifier)
{
    m_cur_key = obj + name;
    m_cur_type = type;
    m_cur_qualifier = qualifier;

    m_generator = boost::shared_ptr<shade::formatter::DumpGenerator>(new shade::formatter::DumpGenerator);
    return m_generator;
}

void shade::StorageFormatter::end_insert_init_attribute()
{
    std::string init(m_generator->get_code());
    m_generator.reset();

    Attribute attr(m_cur_type, m_cur_qualifier, init);
    m_attributes[m_cur_key] = attr;
}

void shade::StorageFormatter::insert_array_attribute(formatter::Constants::Type type, const std::string& obj, const std::string& name)
{
    Attribute attr(type, formatter::Constants::input, true);
    m_attributes[obj + name] = attr;
}

void shade::StorageFormatter::insert_multiple_attribute(formatter::Constants::Type type, const std::string& obj, const std::string& name)
{
    Attribute attr(type, formatter::Constants::none, get_num_vertices_in());
    m_attributes[obj + name] = attr;
}

void shade::StorageFormatter::begin_self_argument(const std::string& name)
{
    m_cur_self_name = name;
    m_self_arguments[name]; // Force initialization
}

void shade::StorageFormatter::insert_self_argument(formatter::Constants::Type type, const std::string& name) { m_self_arguments[m_cur_self_name].push_back(std::make_pair(type, name)); }

void shade::StorageFormatter::end_self_argument(void) {}

void shade::StorageFormatter::begin_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval)
{
    m_cur_fun_name = get_method_name(class_, name);
    m_args[m_cur_fun_name].clear();
    m_args[m_cur_fun_name].push_back(retval);
    m_dispatches[m_cur_fun_name];      // Force initialization
    m_dispatches_args[m_cur_fun_name]; // Force initialization
}

void shade::StorageFormatter::begin_dispatcher_add_arg(formatter::Constants::Type arg) { m_args[m_cur_fun_name].push_back(arg); }

void shade::StorageFormatter::begin_dispatcher_finish(void) {}

void shade::StorageFormatter::insert_dispatcher(shade::ObjectMap::Index self, const std::string& obj, const std::string& name)
{
    m_cur_dispatch_self = self;
    m_dispatches[m_cur_fun_name][self] = std::make_pair(obj, name);
    m_dispatches_args[m_cur_fun_name][self]; // Force initialization
}

void shade::StorageFormatter::insert_inline_dispatcher(shade::ObjectMap::Index i, const std::string& obj, const std::string& class_, const std::string& name) { insert_dispatcher(i, obj, name); }

void shade::StorageFormatter::insert_dispatcher_add_arg(const std::string& name) { m_dispatches_args[m_cur_fun_name][m_cur_dispatch_self].push_back(name); }

void shade::StorageFormatter::insert_dispatcher_finish(void) {}

boost::shared_ptr<shade::formatter::Generator> shade::StorageFormatter::set_dispatcher_return(void)
{
    m_generator = boost::shared_ptr<shade::formatter::DumpGenerator>(new shade::formatter::DumpGenerator);
    return m_generator;
}

void shade::StorageFormatter::end_dispatcher(void)
{
    if(!m_generator)
        return;

    m_dispatches_default_return[m_cur_fun_name] = m_generator->get_code();
    m_generator.reset();
}

void shade::StorageFormatter::begin_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval)
{
    m_cur_property = get_method_name(class_, name);
    m_property_type[m_cur_property] = normal;
}

void shade::StorageFormatter::begin_array_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval)
{
    m_cur_property = get_method_name(class_, name);
    m_property_type[m_cur_property] = array;
}

void shade::StorageFormatter::begin_multiple_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval)
{
    m_cur_property = get_method_name(class_, name);
    m_property_type[m_cur_property] = multiple;
}

void shade::StorageFormatter::begin_deferred_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval)
{
    m_cur_property = get_method_name(class_, name);
    m_property_type[m_cur_property] = deferred;
}

void shade::StorageFormatter::insert_property_dispatcher(shade::ObjectMap::Index self, const std::string& obj) { m_properties[m_cur_property][self] = obj; }

void shade::StorageFormatter::insert_deferred_property_dispatcher(
    shade::ObjectMap::Index i, shade::ObjectMap::Index obj, const std::string& iface_getter, const std::string& getter, const std::string& iface_setter, const std::string& setter)
{
    std::string key(m_cur_property + ";" + boost::lexical_cast<std::string>(i));
    DeferredProperty property(obj, iface_getter, getter, iface_setter, setter);
    m_deferred_properties[key] = property;
}

boost::shared_ptr<shade::formatter::Generator> shade::StorageFormatter::set_property_dispatcher_return(void)
{
    m_generator = boost::shared_ptr<shade::formatter::DumpGenerator>(new shade::formatter::DumpGenerator);
    return m_generator;
}

void shade::StorageFormatter::end_property_dispatcher(void)
{
    if(!m_generator)
        return;

    m_property_return[m_cur_property] = m_generator->get_code();
    m_generator.reset();
}

bool shade::StorageFormatter::has_attribute(const std::string& obj, const std::string& name) const { return (m_attributes.find(obj + name) != m_attributes.end()); }

shade::StorageFormatter::Attribute shade::StorageFormatter::get_attribute(const std::string& obj, const std::string& name) const { return m_attributes.find(obj + name)->second; }

const std::vector<std::pair<shade::formatter::Constants::Type, std::string>>& shade::StorageFormatter::get_self_argument(const std::string& name) const { return m_self_arguments.find(name)->second; }

bool shade::StorageFormatter::has_function(const std::string& class_, const std::string& name) const { return (m_args.find(get_method_name(class_, name)) != m_args.end()); }

const shade::StorageFormatter::arguments& shade::StorageFormatter::get_function_args(const std::string& class_, const std::string& name) const
{
    return m_args.find(get_method_name(class_, name))->second;
}

bool shade::StorageFormatter::has_function_dispatch(const std::string& class_, const std::string& name, ObjectMap::Index i) const
{
    if(!has_function(class_, name))
        return false;

    const std::map<ObjectMap::Index, std::pair<std::string, std::string>>& function = m_dispatches.find(get_method_name(class_, name))->second;
    return (function.find(i) != function.end());
}

std::pair<std::string, std::string> shade::StorageFormatter::get_function_dispatch(const std::string& class_, const std::string& name, ObjectMap::Index i) const
{
    return m_dispatches.find(get_method_name(class_, name))->second.find(i)->second;
}

const std::vector<std::string>& shade::StorageFormatter::get_function_dispatch_args(const std::string& class_, const std::string& name, ObjectMap::Index i) const
{
    return m_dispatches_args.find(get_method_name(class_, name))->second.find(i)->second;
}

std::string shade::StorageFormatter::get_function_return(const std::string& class_, const std::string& name) const { return m_dispatches_default_return.find(get_method_name(class_, name))->second; }

std::string shade::StorageFormatter::get_property_dispatch(const std::string& class_, const std::string& name, ObjectMap::Index i) const
{
    property_map::const_iterator property(m_properties.find(get_method_name(class_, name)));
    if(property != m_properties.end())
    {
        if(property->second.find(i) != property->second.end())
        {
            return property->second.find(i)->second;
        }
    }
    return "";
}

shade::StorageFormatter::PropertyDispatchType shade::StorageFormatter::get_property_dispatch_type(const std::string& class_, const std::string& name) const
{
    std::string property(get_method_name(class_, name));
    if(m_property_type.find(property) != m_property_type.end())
        return m_property_type.find(property)->second;
    return none;
}

std::string shade::StorageFormatter::get_property_dispatch_return(const std::string& class_, const std::string& name) const
{
    std::string property(get_method_name(class_, name));
    if(m_property_return.find(property) != m_property_return.end())
        return m_property_return.find(property)->second;
    return "";
}

shade::StorageFormatter::DeferredProperty shade::StorageFormatter::get_deferred_property_dispatch(const std::string& class_, const std::string& name, ObjectMap::Index i) const
{
    std::string key(get_method_name(class_, name) + ";" + boost::lexical_cast<std::string>(i));
    return m_deferred_properties.find(key)->second;
}

void shade::StorageFormatter::begin_inline_method(const std::string& class_, const std::string& name, formatter::Constants::Type retval)
{
    m_cur_inline_method_name = get_method_name(class_, name);
    m_inline_methods[m_cur_inline_method_name].first = retval;
    m_inline_methods_args[m_cur_inline_method_name].clear();
}

void shade::StorageFormatter::add_arg_inline_method(formatter::Constants::Type arg) { m_inline_methods_args[m_cur_inline_method_name].push_back(arg); }

boost::shared_ptr<shade::formatter::Generator> shade::StorageFormatter::insert_inline_method(void)
{
    m_generator = boost::shared_ptr<shade::formatter::DumpGenerator>(new shade::formatter::DumpGenerator);
    return m_generator;
}

void shade::StorageFormatter::end_inline_method(void)
{
    if(!m_generator)
        return;

    m_inline_methods[m_cur_inline_method_name].second = m_generator->get_code();
}

void shade::StorageFormatter::invoke(const std::string& class_, const std::string& main) { m_invoke = get_method_name(class_, main); }

const std::string& shade::StorageFormatter::get_invoke(void) { return m_invoke; }

void shade::StorageFormatter::initialize(shade::ObjectMap::Index obj, const std::string& class_, const std::string& method)
{
    m_initializers.push_back(std::make_pair(get_method_name(m_cur_initialize_function, get_method_name(class_, method)), obj));
}

void shade::StorageFormatter::begin_initialize(const std::string& name, bool invoke) { m_cur_initialize_function = name; }

void shade::StorageFormatter::end_initialize(void) {}

void shade::StorageFormatter::insert_sources(const std::string& sources) { m_sources = sources; }

const std::vector<std::pair<std::string, shade::ObjectMap::Index>>& shade::StorageFormatter::get_initializers(void) { return m_initializers; }

const std::string& shade::StorageFormatter::get_imported_sources(void) { return m_sources; }

std::string shade::StorageFormatter::get_method_name(const std::string& class_, const std::string& main) const { return class_ + "_" + main; }

bool shade::StorageFormatter::has_inline_method(const std::string& class_, const std::string& name) const { return (m_inline_methods.find(get_method_name(class_, name)) != m_inline_methods.end()); }

std::pair<shade::formatter::Constants::Type, std::string> shade::StorageFormatter::get_inline_method(const std::string& class_, const std::string& name) const
{
    return m_inline_methods.find(get_method_name(class_, name))->second;
}

const shade::StorageFormatter::arguments& shade::StorageFormatter::get_inline_method_args(const std::string& class_, const std::string& name) const
{
    return m_inline_methods_args.find(get_method_name(class_, name))->second;
}
