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

#ifndef shade_GLSLFormatter_H
#define shade_GLSLFormatter_H shade_GLSLFormatter_H


#include <shade/Formatter.h>
#include <shade/ShaderEnvironment.h>
#include "GL2Generator.h"
#include "SelfTranslator.h"
#include <string>
#include <sstream>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>


namespace shade
{
  namespace formatter
  {
    class Generator;
  }

  class GLSLFormatter : public Formatter
  {
  public:

    GLSLFormatter(ShaderEnvironment st);
    GLSLFormatter(ShaderEnvironment st, const formatter::Generator& generator);

    /*virtual*/ void insert_instance_var(formatter::Constants::Type type, const std::string& obj, const std::string& name, ObjectMap::Index ri = -1);
    /*virtual*/ void insert_attribute(formatter::Constants::Type type, const std::string& obj, const std::string& name, formatter::Constants::Qualifier qualifier = formatter::Constants::none);
    /*virtual*/ boost::shared_ptr<formatter::Generator> begin_insert_init_attribute(formatter::Constants::Type type, const std::string& obj, const std::string& name, formatter::Constants::Qualifier qualifier = formatter::Constants::none);
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
    /*virtual*/ boost::shared_ptr<formatter::Generator> set_dispatcher_return(void);
    /*virtual*/ void end_dispatcher(void);

    /*virtual*/ void begin_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval);
    /*virtual*/ void begin_array_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval);
    /*virtual*/ void begin_multiple_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval);
    /*virtual*/ void begin_deferred_property_dispatcher(const std::string& class_, const std::string& name, formatter::Constants::Type retval);
    /*virtual*/ void insert_property_dispatcher(shade::ObjectMap::Index i, const std::string& obj);
    /*virtual*/ void insert_deferred_property_dispatcher(shade::ObjectMap::Index i, shade::ObjectMap::Index obj, const std::string& iface_getter, const std::string& getter, const std::string& iface_setter, const std::string& setter);
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

    void generate(formatter::Generator& generator);

    std::string get_type_str(formatter::Constants::Type type);
    std::string get_qualifier_str(formatter::Constants::Qualifier qualifier);

  private:

    void insert_attribute_pre(formatter::Constants::Qualifier qualifier);
    void insert_attribute_post(const std::string& obj, const std::string& name);

    std::string get_method_name(const std::string& class_, const std::string& name);
    std::string get_property_get_name(const std::string& class_, const std::string& name) const;
    std::string get_property_set_name(const std::string& class_, const std::string& name) const;
    std::string get_indexed_object_name(const std::string& object, unsigned int index);
    std::string get_base_self_name(void) const;
    std::string get_self_name(ObjectMap::Index) const;

    void generate_self_name(formatter::Generator& generator, ObjectMap::Index self) const;

    void begin_property_dispatch_declaration(formatter::Generator& generator, const std::string& class_, const std::string& name, formatter::Constants::Type retval, bool setter) const;
    void begin_property_dispatch_trunk(const std::string& class_, const std::string& name, formatter::Constants::Type retval);

    void generate_header_code(formatter::Generator& generator);

    ShaderEnvironment m_shader_env;

    boost::shared_ptr<formatter::Generator> m_intern_generator[2];
    boost::shared_ptr<formatter::Generator> m_extern_generator;
    boost::shared_ptr<formatter::Generator> m_main_generator;

    boost::shared_ptr<formatter::Generator> m_generator;
    boost::shared_ptr<formatter::Generator> m_dispatch_property_generator[2];

    std::ostringstream m_pre_code;
    std::ostringstream m_code;
    std::ostringstream m_post_code;
    std::ostringstream m_initialize_code;
    std::ostringstream m_main_code;
    std::ostringstream m_extensions;

    unsigned int m_dispatcher_argcount;
    bool m_is_first_dispatcher;
    std::string m_dispatch_obj;
    formatter::Constants::Type m_dispatch_return_type;

    std::ostringstream m_dispatch_property_get_code;
    std::ostringstream m_dispatch_property_set_code;
    std::string m_dispatch_property_name;
    bool m_is_first_property_dispatcher;
    bool m_is_array_dispatch;
    bool m_is_multiple_dispatch;

    unsigned int m_inline_method_argcount;

    bool m_init_struct;
    std::string m_class_name;
    std::list<std::string> m_initializers;

    formatter::SelfTranslator m_translator;

    std::string m_sources;
  };
}


#endif /* shade_GLSLFormatter_H */
