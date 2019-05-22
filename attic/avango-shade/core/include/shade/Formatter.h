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

#ifndef shade_Formatter_H
#define shade_Formatter_H shade_Formatter_H

#include "ObjectMap.h"
#include "ShaderEnvironment.h"
#include "formatter/Constants.h"
#include "formatter/Generator.h"
#include <boost/shared_ptr.hpp>
#include <string>

namespace shade
{
class Formatter
{
  public:
    Formatter(ShaderEnvironment env) : m_env(env) {}

    virtual ~Formatter(void) {}

    enum ExtensionBehavior
    {
        require,
        enable,
        warn,
        disable
    };

    ShaderEnvironment get_shader_env(void);
    void setup(shade::ObjectMap::Index num_classes, formatter::Constants::Primitive primtive_type);
    shade::ObjectMap::Index get_num_classes(void) const;
    unsigned int get_num_vertices_in(void) const;

    virtual void insert_attribute(formatter::Constants::Type type, const std::string& obj, const std::string& name, formatter::Constants::Qualifier qualifier = formatter::Constants::none) = 0;
    virtual boost::shared_ptr<formatter::Generator>
    begin_insert_init_attribute(formatter::Constants::Type type, const std::string& obj, const std::string& name, formatter::Constants::Qualifier qualifier = formatter::Constants::none) = 0;
    virtual void end_insert_init_attribute(void) = 0;
    virtual void insert_array_attribute(formatter::Constants::Type type, const std::string& obj, const std::string& name) = 0;
    virtual void insert_multiple_attribute(formatter::Constants::Type type, const std::string& obj, const std::string& name) = 0;

    virtual void begin_self_argument(const std::string& name) = 0;
    virtual void insert_self_argument(formatter::Constants::Type type, const std::string& name) = 0;
    virtual void end_self_argument(void) = 0;

    virtual void begin_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval) = 0;
    virtual void begin_dispatcher_add_arg(formatter::Constants::Type arg) = 0;
    virtual void begin_dispatcher_finish(void) = 0;
    virtual void insert_dispatcher(shade::ObjectMap::Index i, const std::string& obj, const std::string& name) = 0;
    virtual void insert_inline_dispatcher(shade::ObjectMap::Index i, const std::string& obj, const std::string& class_, const std::string& name) = 0;
    virtual void insert_dispatcher_add_arg(const std::string& name) = 0;
    virtual void insert_dispatcher_finish(void) = 0;
    virtual boost::shared_ptr<formatter::Generator> set_dispatcher_return(void) = 0;
    virtual void end_dispatcher(void) = 0;

    virtual void begin_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval) = 0;
    virtual void begin_array_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval) = 0;
    virtual void begin_multiple_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval) = 0;
    virtual void begin_deferred_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval) = 0;
    virtual void insert_deferred_property_dispatcher(
        shade::ObjectMap::Index i, shade::ObjectMap::Index obj, const std::string& iface_getter, const std::string& getter, const std::string& iface_setter, const std::string& setter) = 0;
    virtual void insert_property_dispatcher(shade::ObjectMap::Index i, const std::string& obj) = 0;
    virtual boost::shared_ptr<formatter::Generator> set_property_dispatcher_return(void) = 0;
    virtual void end_property_dispatcher(void) = 0;

    virtual void begin_inline_method(const std::string& class_, const std::string& name, formatter::Constants::Type retval) = 0;
    virtual void add_arg_inline_method(formatter::Constants::Type arg) = 0;
    virtual boost::shared_ptr<formatter::Generator> insert_inline_method(void) = 0;
    virtual void end_inline_method(void) = 0;

    virtual void invoke(const std::string& class_, const std::string& main) = 0;
    virtual void initialize(shade::ObjectMap::Index obj, const std::string& class_, const std::string& method) = 0;
    virtual void begin_initialize(const std::string& name, bool invoke = true) = 0;
    virtual void end_initialize(void) = 0;
    virtual std::string get_initializer_name(void);

    virtual void insert_sources(const std::string& sources) = 0;

  protected:
    const ShaderEnvironment m_env;
    shade::ObjectMap::Index m_num_classes;
    unsigned int m_num_primitives;
    formatter::Constants::Primitive m_primitive_type;
};
} // namespace shade

#endif /* shade_Formatter_H */
