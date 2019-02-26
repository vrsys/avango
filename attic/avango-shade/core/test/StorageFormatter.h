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

#ifndef shade_StorageFormatter_H
#define shade_StorageFormatter_H shade_StorageFormatter_H

#include <shade/Formatter.h>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <utility>
#include <boost/shared_ptr.hpp>

namespace shade
{
namespace formatter
{
class DumpGenerator;
}

class StorageFormatter : public Formatter
{
  public:
    StorageFormatter(ShaderEnvironment env = vertex_shader);

    /*virtual*/ void insert_attribute(formatter::Constants::Type type, const std::string& obj, const std::string& name, formatter::Constants::Qualifier qualifier = formatter::Constants::none);
    /*virtual*/ boost::shared_ptr<formatter::Generator>
    begin_insert_init_attribute(formatter::Constants::Type type, const std::string& obj, const std::string& name, formatter::Constants::Qualifier qualifier = formatter::Constants::none);
    /*virtual*/ void end_insert_init_attribute(void);
    /*virtual*/ void insert_array_attribute(formatter::Constants::Type type, const std::string& obj, const std::string& name);
    /*virtual*/ void insert_multiple_attribute(formatter::Constants::Type type, const std::string& obj, const std::string& name);

    /*virtual*/ void begin_self_argument(const std::string& name);
    /*virtual*/ void insert_self_argument(formatter::Constants::Type type, const std::string& name);
    /*virtual*/ void end_self_argument(void);

    /*virtual*/ void begin_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval);
    /*virtual*/ void begin_dispatcher_add_arg(formatter::Constants::Type arg);
    /*virtual*/ void begin_dispatcher_finish(void);
    /*virtual*/ void insert_dispatcher(shade::ObjectMap::Index i, const std::string& obj, const std::string& name);
    /*virtual*/ void insert_inline_dispatcher(shade::ObjectMap::Index i, const std::string& obj, const std::string& class_, const std::string& name);
    /*virtual*/ void insert_dispatcher_add_arg(const std::string& name);
    /*virtual*/ void insert_dispatcher_finish(void);
    /*virtual*/ boost::shared_ptr<shade::formatter::Generator> set_dispatcher_return(void);
    /*virtual*/ void end_dispatcher(void);

    /*virtual*/ void begin_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval);
    /*virtual*/ void begin_array_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval);
    /*virtual*/ void begin_multiple_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval);
    /*virtual*/ void begin_deferred_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval);
    /*virtual*/ void insert_property_dispatcher(shade::ObjectMap::Index i, const std::string& obj);
    /*virtual*/ void insert_deferred_property_dispatcher(
        shade::ObjectMap::Index i, shade::ObjectMap::Index obj, const std::string& iface_getter, const std::string& getter, const std::string& iface_setter, const std::string& setter);
    /*virtual*/ boost::shared_ptr<formatter::Generator> set_property_dispatcher_return(void);
    /*virtual*/ void end_property_dispatcher(void);

    /*virtual*/ void begin_inline_method(const std::string& class_, const std::string& name, formatter::Constants::Type retval);
    /*virtual*/ void add_arg_inline_method(formatter::Constants::Type arg);
    /*virtual*/ boost::shared_ptr<formatter::Generator> insert_inline_method(void);
    /*virtual*/ void end_inline_method(void);

    /*virtual*/ void invoke(const std::string& class_, const std::string& main);
    /*virtual*/ void initialize(shade::ObjectMap::Index obj, const std::string& class_, const std::string& method);
    /*virtual*/ void begin_initialize(const std::string& name, bool invoke = true);
    /*virtual*/ void end_initialize(void);

    /*virtual*/ void insert_sources(const std::string& sources);

    struct Attribute
    {
        Attribute() {}
        Attribute(formatter::Constants::Type t, formatter::Constants::Qualifier q) : type(t), qualifier(q), is_array(false), num_copies(1) {}
        Attribute(formatter::Constants::Type t, formatter::Constants::Qualifier q, std::string i) : type(t), qualifier(q), initializer(i), is_array(false), num_copies(1) {}
        Attribute(formatter::Constants::Type t, formatter::Constants::Qualifier q, bool a) : type(t), qualifier(q), is_array(a), num_copies(1) {}
        Attribute(formatter::Constants::Type t, formatter::Constants::Qualifier q, unsigned int c) : type(t), qualifier(q), is_array(false), num_copies(c) {}

        formatter::Constants::Type type;
        formatter::Constants::Qualifier qualifier;
        std::string initializer;
        bool is_array;
        unsigned int num_copies;
    };
    bool has_attribute(const std::string& obj, const std::string& name) const;
    Attribute get_attribute(const std::string& obj, const std::string& name) const;

    const std::vector<std::pair<formatter::Constants::Type, std::string>>& get_self_argument(const std::string& name) const;

    bool has_function(const std::string& class_, const std::string& name) const;
    typedef std::vector<formatter::Constants::Type> arguments;
    const arguments& get_function_args(const std::string& class_, const std::string& name) const;
    bool has_function_dispatch(const std::string& class_, const std::string& name, ObjectMap::Index i) const;
    std::pair<std::string, std::string> get_function_dispatch(const std::string& class_, const std::string& name, ObjectMap::Index i) const;
    const std::vector<std::string>& get_function_dispatch_args(const std::string& class_, const std::string& name, ObjectMap::Index i) const;
    std::string get_function_return(const std::string& class_, const std::string& name) const;

    enum PropertyDispatchType
    {
        none,
        normal,
        array,
        multiple,
        deferred
    };
    std::string get_property_dispatch(const std::string& class_, const std::string& name, ObjectMap::Index i) const;
    PropertyDispatchType get_property_dispatch_type(const std::string& class_, const std::string& name) const;
    std::string get_property_dispatch_return(const std::string& class_, const std::string& name) const;
    struct DeferredProperty
    {
        DeferredProperty() {}
        DeferredProperty(ObjectMap::Index o, const std::string& ig, const std::string& g, const std::string& is, const std::string& s)
            : object(o), getter_class(ig), getter_method(g), setter_class(is), setter_method(s)
        {
        }

        ObjectMap::Index object;
        std::string getter_class;
        std::string getter_method;
        std::string setter_class;
        std::string setter_method;
    };
    DeferredProperty get_deferred_property_dispatch(const std::string& class_, const std::string& name, ObjectMap::Index i) const;

    bool has_inline_method(const std::string& class_, const std::string& name) const;
    std::pair<formatter::Constants::Type, std::string> get_inline_method(const std::string& class_, const std::string& name) const;
    const arguments& get_inline_method_args(const std::string& class_, const std::string& name) const;

    const std::string& get_invoke(void);
    const std::vector<std::pair<std::string, shade::ObjectMap::Index>>& get_initializers(void);

    const std::string& get_imported_sources(void);

    std::string get_method_name(const std::string& class_, const std::string& name) const;

  private:
    boost::shared_ptr<formatter::DumpGenerator> m_generator;

    shade::formatter::Constants::Type m_cur_type;
    shade::formatter::Constants::Qualifier m_cur_qualifier;
    std::string m_cur_key;
    std::map<std::string, Attribute> m_attributes;

    std::string m_cur_self_name;
    std::map<std::string, std::vector<std::pair<formatter::Constants::Type, std::string>>> m_self_arguments;

    std::string m_cur_fun_name;
    ObjectMap::Index m_cur_dispatch_self;
    std::map<std::string, arguments> m_args;
    std::map<std::string, std::map<ObjectMap::Index, std::pair<std::string, std::string>>> m_dispatches;
    std::map<std::string, std::map<ObjectMap::Index, std::vector<std::string>>> m_dispatches_args;
    std::map<std::string, std::string> m_dispatches_default_return;

    std::string m_cur_property;
    typedef std::map<std::string, std::map<ObjectMap::Index, std::string>> property_map;
    property_map m_properties;
    std::map<std::string, PropertyDispatchType> m_property_type;
    std::map<std::string, std::string> m_property_return;
    std::map<std::string, DeferredProperty> m_deferred_properties;

    std::string m_cur_inline_method_name;
    std::map<std::string, std::pair<formatter::Constants::Type, std::string>> m_inline_methods;
    std::map<std::string, arguments> m_inline_methods_args;

    std::string m_invoke;

    std::string m_cur_initialize_function;
    std::vector<std::pair<std::string, shade::ObjectMap::Index>> m_initializers;

    std::string m_sources;
};
} // namespace shade

#endif /* shade_StorageFormatter_H */
