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

#include "../src/Component.h"
#include <shade/ObjectMap.h>
#include <shade/shaders/Initializeable.h>
#include <shade/shaders/Enterable.h>
#include <shade/formatter/Generator.h>

#include <list>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/construct.hpp>
namespace bll=boost::lambda;
#include <boost/bind.hpp>
#include <boost/function.hpp>


namespace
{
  class IterateObjects
  {
  public:

    typedef boost::function<boost::shared_ptr<shade::Type::State> (const shade::Type*)> Callback;

    IterateObjects(Callback c) :
      m_get_state(c)
    {}

    virtual ~IterateObjects(void)
    {}

    void operator()(shade::Shader* obj)
    {
      if (m_mark.find(obj) == m_mark.end())
      {
        m_mark.insert(obj);
        if (visit_object_pre(obj))
        {
          obj->traverse_attributes(boost::ref(*this));
          visit_object_post(obj);
        }
      }
    }

    void operator()(const shade::Type& type, const std::string& name)
    {
      if (visit_attribute_pre(type, name))
      {
        type.collect_references(m_get_state(&type), boost::ref(*this));
        visit_attribute_post(type, name);
      }
    }

    virtual bool visit_object_pre(shade::Shader* obj)
    { return true; }

    virtual void visit_object_post(shade::Shader* obj)
    {}

    virtual bool visit_attribute_pre(const shade::Type& type, const std::string& name)
    { return true; }

    virtual void visit_attribute_post(const shade::Type& type, const std::string& name)
    {}

  private:
    Callback m_get_state;
    std::set<shade::Shader*> m_mark;
  };

  class CollectObjectInstances : public IterateObjects
  {
  public:
    typedef boost::function<void (const shade::Type*, const std::string&, const std::string&, shade::ObjectMap::Index)> Callback;

    CollectObjectInstances(shade::ObjectMap& objects, Callback c, IterateObjects::Callback ic) :
      IterateObjects(ic),
      m_objects(objects),
      m_callback(c)
    {
    }

    void register_attribute(const shade::Type& type, const std::string& name)
    {
      shade::ObjectMap::Index ref_index = m_objects.get_next_index();
      m_callback(&type, m_obj_id, name, ref_index);
    }

    /*virtual*/ bool visit_object_pre(shade::Shader* obj)
    {
      m_obj_id = obj->get_uniq_id();
      obj->traverse_attributes(bind(&CollectObjectInstances::register_attribute, this, _1, _2));
      return true;
    }

  private:
    shade::ObjectMap& m_objects;
    Callback m_callback;
    std::string m_obj_id;
  };

  class IterateAttributes : public IterateObjects
  {
  public:
    typedef boost::function<void (const shade::Type&, const std::string&)> Callback;

    IterateAttributes(Callback c, IterateObjects::Callback ic) :
      IterateObjects(ic),
      m_callback(c)
    {}

    /*virtual*/ bool visit_attribute_pre(const shade::Type& type, const std::string& name)
    {
      m_callback(type, name);
      return true;
    }

  private:
    Callback m_callback;
  };


  class SortClasses
  {
  public:
    SortClasses(shade::Component::Classbins& classbins, int i) :
      m_classbins(classbins),
      m_index(i)
    {}

    void operator()(const std::type_info* ti)
    {
      m_classbins[ti].push_back(m_index);
    }

  private:
    shade::Component::Classbins& m_classbins;
    int m_index;
  };

  class DispatchOutput
  {
  public:
    DispatchOutput(shade::Formatter* fmt,
        shade::ShaderEnvironment env,
        boost::shared_ptr<shade::Shader> obj,
        const shade::ObjectMap& objects,
        const std::list<int>& indices
        ) :
      m_fmt(*fmt),
      m_env(env),
      m_obj(obj),
      m_objects(objects),
      m_indices(indices)
    {
    }

    void operator()(const shade::Type& type, const std::string& name)
    {
      if ( (!type.exists(m_env)) || (type.is_writable(m_env)) )
        return;

      m_fmt.insert_dispatcher_add_arg(name);
    }

    void operator()(const shade::MethodAccessor& ma)
    {
      if ( (ma.get_env() != shade::invariant_shader) && (ma.get_env() != m_env) )
      {
        return;
      }

      m_mark.clear();

      shade::MethodArgumentIterator iter(ma.type_begin());
      m_fmt.begin_dispatcher(m_obj->get_class_name(), ma.get_name(), (*iter).get_formatter_type());
      std::for_each(++iter, ma.type_end(),
          boost::bind(&shade::Formatter::begin_dispatcher_add_arg, boost::ref(m_fmt),
            boost::bind(&shade::Type::get_formatter_type, _1)));
      m_fmt.begin_dispatcher_finish();

      std::for_each(m_indices.begin(), m_indices.end(), bind(&DispatchOutput::iterate_indices, this, boost::ref(ma), _1));
      boost::shared_ptr<shade::formatter::Generator> generator(m_fmt.set_dispatcher_return());
      (*ma.type_begin()).generate_constructor(*generator);
      m_fmt.end_dispatcher();
    }

    void iterate_indices(const shade::MethodAccessor& ma, int index)
    {
      shade::Shader* curobj = m_objects[index];
      boost::shared_ptr<shade::Type> curtype = ma.invoke(curobj);
      if (!curtype->has_function())
        return;
      boost::shared_ptr<shade::Shader> convobj = curobj->convert(curtype->get_function_type());

      if (curtype->has_inline_function())
        m_fmt.insert_inline_dispatcher(index, curobj->get_uniq_id(), convobj->get_class_name(), ma.get_name());
      else
        m_fmt.insert_dispatcher(index, curobj->get_uniq_id(), curtype->get_function_name());
      convobj->iterate_attributes(*this);
      m_fmt.insert_dispatcher_finish();

      if (curtype->has_inline_function())
      {
        if (m_mark.find(curtype->get_function_type()) == m_mark.end())
        {
          m_mark.insert(curtype->get_function_type());

          shade::MethodArgumentIterator iter(ma.type_begin());
          m_fmt.begin_inline_method(convobj->get_class_name(), ma.get_name(), (*iter).get_formatter_type());
          std::for_each(++iter, ma.type_end(),
              boost::bind(&shade::Formatter::add_arg_inline_method, boost::ref(m_fmt),
                boost::bind(&shade::Type::get_formatter_type, _1)));
          boost::shared_ptr<shade::formatter::Generator> generator(m_fmt.insert_inline_method());
          curtype->generate_inline(*generator);
          m_fmt.end_inline_method();
        }
      }
    }

  private:
    shade::Formatter& m_fmt;
    shade::ShaderEnvironment m_env;
    boost::shared_ptr<shade::Shader> m_obj;
    const shade::ObjectMap& m_objects;
    const std::list<int>& m_indices;

    struct type_id_cmp : public std::binary_function<std::type_info*, std::type_info*, bool>
    {
    public:
      bool operator() (const std::type_info* a, const std::type_info* b) const
      {
        return a->before(*b);
      }
    };
    std::set<const std::type_info*, type_id_cmp> m_mark;
  };

  class InitializerOutput : public IterateObjects
  {
  public:
    InitializerOutput(shade::Formatter& fmt,
        shade::ShaderEnvironment env,
        const shade::ObjectMap& objects,
        IterateObjects::Callback ic
        ) :
      IterateObjects(ic),
      m_fmt(fmt),
      m_env(env),
      m_objects(objects)
    {
    }

    /*virtual*/ void visit_object_post(shade::Shader* obj)
    {
      shade::shaders::Initializeable* init = dynamic_cast<shade::shaders::Initializeable*>(obj);
      if (init)
      {
        std::string class_name(init->shade::shaders::Initializeable::get_class_name());
        std::string method_name(init->get_method_name(m_env));
        m_fmt.initialize(m_objects.get_index(obj), class_name, method_name);
      }
    }

    using IterateObjects::operator();

  private:
    shade::Formatter& m_fmt;
    shade::ShaderEnvironment m_env;
    const shade::ObjectMap& m_objects;
  };

  struct MethodOutput
  {
  public:
    MethodOutput(shade::Formatter& fmt,
        shade::ShaderEnvironment env,
        const shade::ObjectMap& objects
        ) :
      m_fmt(fmt),
      m_env(env),
      m_objects(objects)
    {
    }

    void operator()(std::pair< const std::type_info*, std::list<int> > cls)
    {
      boost::shared_ptr<shade::Shader> curobj = m_objects[*cls.second.begin()]->convert(cls.first);
      curobj->iterate_methods(DispatchOutput(&m_fmt, m_env, curobj, m_objects, cls.second));
    }

  private:
    shade::Formatter& m_fmt;
    shade::ShaderEnvironment m_env;
    const shade::ObjectMap& m_objects;
  };

  class PropertyOutput
  {
  public:
    typedef boost::function<void (std::pair<const shade::Type*, shade::ObjectMap::Index>)> Callback;

    PropertyOutput(Callback c,
        shade::Formatter& fmt,
        const shade::ObjectMap& objects
        ) :
      m_fmt(fmt),
      m_objects(objects),
      m_callback(c)
    {
    }

    void output_attributes(const std::pair<std::string, std::list<std::pair<const shade::Type*, shade::ObjectMap::Index> > >& attribute,
        const std::string& class_name)
    {
      const shade::Type* type = attribute.second.begin()->first;

      if (!type->output_begin_property_dispatcher(&m_fmt, class_name, attribute.first))
        return;

      std::for_each(attribute.second.begin(), attribute.second.end(), m_callback);

      boost::shared_ptr<shade::formatter::Generator> generator(m_fmt.set_property_dispatcher_return());
      type->generate_constructor(*generator);
      m_fmt.end_property_dispatcher();
    }

    void iterate_per_name_attributes(const std::map<std::string, std::list<std::pair<const shade::Type*, shade::ObjectMap::Index> > >& attribute,
        const std::string& class_name)
    {
      std::for_each(attribute.begin(), attribute.end(),
          boost::bind(&PropertyOutput::output_attributes, this, _1, class_name));
    }


    void insert_attributes(const shade::Type& type, const std::string& name, shade::ObjectMap::Index index)
    {
      m_attributes[name].push_back(std::make_pair(&type, index));
    }

    void iterate_per_class_attributes(const std::type_info* type_info, int index)
    {
      boost::shared_ptr<shade::Shader> obj = m_objects[index]->convert(type_info);
      obj->iterate_attributes(boost::bind(&PropertyOutput::insert_attributes, this, _1, _2, index));
    }

    void operator()(std::pair< const std::type_info*, std::list<int> > cls)
    {
      m_attributes.clear();
      std::for_each(cls.second.begin(), cls.second.end(),
          boost::bind(&PropertyOutput::iterate_per_class_attributes, this, cls.first, _1));

      boost::shared_ptr<shade::Shader> obj = m_objects[*cls.second.begin()]->convert(cls.first);
      std::string class_name = obj->get_class_name();

      std::for_each(m_attributes.begin(), m_attributes.end(),
          boost::bind(&PropertyOutput::output_attributes, this, _1, boost::ref(class_name)));
    }

  private:
    shade::Formatter& m_fmt;
    const shade::ObjectMap& m_objects;
    Callback m_callback;

    typedef std::map<std::string, std::list<std::pair<const shade::Type*, shade::ObjectMap::Index> > > Attributes;
    Attributes m_attributes;
  };

  struct SelfArgumentOutput
  {
  public:
    SelfArgumentOutput(shade::Formatter& fmt,
        shade::ShaderEnvironment env,
        const shade::ObjectMap& objects
        ) :
      m_fmt(fmt),
      m_env(env),
      m_objects(objects)
    {
    }

    void operator()(const shade::Type& type, const std::string& name)
    {
      if ( (!type.exists(m_env)) || (type.is_writable(m_env)) )
        return;

      m_fmt.insert_self_argument(type.get_formatter_type(), name);
    }

    void operator()(std::pair< const std::type_info*, std::list<int> > cls)
    {
      boost::shared_ptr<shade::Shader> curobj = m_objects[*cls.second.begin()]->convert(cls.first);

      m_fmt.begin_self_argument(curobj->get_class_name());
      curobj->iterate_attributes(*this);
      m_fmt.end_self_argument();
    }

  private:
    shade::Formatter& m_fmt;
    shade::ShaderEnvironment m_env;
    const shade::ObjectMap& m_objects;
  };

  struct LoadFiles
  {
  public:
    LoadFiles(shade::FileAccumulator& acc,
        const shade::ObjectMap& objects
        ) :
      m_acc(acc),
      m_objects(objects)
    {
    }

    void operator()(std::pair< const std::type_info*, std::list<int> > cls)
    {
      boost::shared_ptr<shade::Shader> curobj = m_objects[*cls.second.begin()]->convert(cls.first);
      m_acc.add_file(curobj->get_source_name());
    }

  private:
    shade::FileAccumulator& m_acc;
    const shade::ObjectMap& m_objects;
  };

}


shade::Component::Component(boost::shared_ptr<Shader> shader) :
  m_shader(shader),
  m_requires_upload(true),
  m_requires_compilation(true)
{
}

void shade::Component::setup(FileAccumulator& acc, PipelineState ps)
{
  m_pipeline_state = ps;

  // First clean attribute list.
  std::for_each(m_attributes.begin(), m_attributes.end(),
      bind(&boost::signals::connection::disconnect,
        bind(&TypeState::connection,
          bind(&attribute_map::value_type::second, _1)
          )));
  m_attributes.clear();

  // Now collect all attributes and connect notifiers
  m_objects.add_object(get_pointer(m_shader));
  CollectObjectInstances collect_instances(m_objects,
      bind(&Component::add_attribute, this, _1, _2, _3, _4),
      bind(&Component::get_type_state, this, _1)
      );
  collect_instances(get_pointer(m_shader));
  m_requires_upload = true;

  // Then find unique classes of these objects
  for (ObjectMap::Index i = 0; i != m_objects.get_next_index(); ++i)
  {
    m_objects[i]->traverse_hierarchy(SortClasses(m_classbins, i));
  }

  // Load source files
  std::for_each(m_classbins.begin(), m_classbins.end(), LoadFiles(acc, m_objects));
  m_sources = acc.get_str();
}

void shade::Component::compile(Formatter& fmt, const FileAccumulator& acc, ShaderEnvironment env, formatter::Constants::Primitive primitive_type)
{
  // Setup shader environment
  fmt.setup(m_objects.get_next_index(), primitive_type);
  fmt.insert_sources(m_sources);

  // Output class methods and properties
  IterateAttributes instance_var_output(bind(&Component::output_attribute, boost::cref(*this), boost::ref(fmt), _1),
      bind(&Component::get_type_state, boost::ref(*this), _1)
      );
  instance_var_output(get_pointer(m_shader));
  std::for_each(m_classbins.begin(), m_classbins.end(), SelfArgumentOutput(fmt, env, m_objects));
  std::for_each(m_classbins.begin(), m_classbins.end(), MethodOutput(fmt, env, m_objects));
  std::for_each(m_classbins.begin(), m_classbins.end(),
      PropertyOutput(bind(&Component::output_property_dispatcher, boost::cref(*this), boost::ref(fmt), _1),
        fmt, m_objects));

  // Write initializer in (if possible) correct order
  fmt.begin_initialize("shade_initialize");
  InitializerOutput initializer_output(fmt, env, m_objects, bind(&Component::get_type_state, this, _1));
  initializer_output(&*m_shader);
  fmt.end_initialize();
  if (env == geometry_shader)
  {
    fmt.begin_initialize("shade_initialize_post", false);
    InitializerOutput initializer_output(fmt, post_geometry_shader, m_objects, bind(&Component::get_type_state, this, _1));
    initializer_output(&*m_shader);
    fmt.end_initialize();
  }

  // Finally write main entry function
  shade::shaders::Enterable* entry = dynamic_cast<shade::shaders::Enterable*>(get_pointer(m_shader));
  if (entry)
  {
    std::string class_name(entry->shade::shaders::Enterable::get_class_name());
    std::string method_name(entry->get_entry_name(env));
    fmt.invoke(class_name, method_name);
  }

  m_requires_compilation = false;
}

bool shade::Component::requires_compilation(void)
{
  return m_requires_compilation;
}

void shade::Component::link(boost::shared_ptr<GLSLWrapper> wrapper)
{
  std::for_each(m_attributes.begin(), m_attributes.end(),
      bll::bind(&TypeState::link_index,
        bll::bind(&attribute_map::value_type::second, bll::_1))
      =
      bll::bind(&shade::Type::get_link_index,
        bll::bind(&attribute_map::value_type::first, bll::_1),
        wrapper,
        bll::bind(&TypeState::full_name,
          bll::bind(&attribute_map::value_type::second, bll::_1))
        ));
}

void shade::Component::upload(boost::shared_ptr<GLSLWrapper> wrapper)
{
  if (m_requires_upload)
  {
    std::for_each(m_attributes.begin(), m_attributes.end(),
        bll::bind(&shade::Type::upload,
          bll::bind(&attribute_map::value_type::first, bll::_1),
          wrapper,
          bll::bind(&TypeState::link_index,
            bll::bind(&attribute_map::value_type::second, bll::_1))
          ));
    m_requires_upload = false;
  }
  else
  {
    std::for_each(m_modified_attributes.begin(), m_modified_attributes.end(),
        bll::bind(&shade::Type::upload, bll::_1,
          wrapper,
          bll::bind(&TypeState::link_index,
            bll::var(m_attributes)[bll::_1])));
  }

  m_modified_attributes.clear();
}

bool shade::Component::requires_upload(void)
{
  return (m_requires_upload || !m_modified_attributes.empty());
}

void shade::Component::add_attribute(const Type* type, const std::string& obj_id, const std::string& name, ObjectMap::Index ri)
{
  TypeState ts;
  ts.connection = type->register_notify(bind(&shade::Component::attribute_notify, this, _1));
  ts.obj_id = obj_id;
  ts.name = name;
  ts.full_name = obj_id + "_" + name; // FIXME proper name from formatter
  ts.state = type->create_state(m_pipeline_state);
  m_attributes[type] = ts;

  type->insert_references(ts.state, m_objects);
}

void shade::Component::output_attribute(Formatter& fmt, const Type& type) const
{
  const TypeState& ts = m_attributes.find(&type)->second;
  const std::string& obj_id(ts.obj_id);
  const std::string& name(ts.name);

  type.output_attribute(ts.state, &fmt, obj_id, name);
}

void shade::Component::output_property_dispatcher(Formatter& fmt, std::pair<const Type*, ObjectMap::Index> type) const
{
  const TypeState& ts = m_attributes.find(type.first)->second;
  type.first->output_insert_property_dispatcher(ts.state, &fmt, type.second, m_objects[type.second]->get_uniq_id());
}

void shade::Component::attribute_notify(const Type* type)
{
  if (type->requires_initializer())
  {
    m_requires_compilation = true;
  }
  else  // FIXME actually need to test for uniform qualifier
  {
    m_modified_attributes.insert(type);
  }
}

bool shade::Component::has_entry(ShaderEnvironment env)
{
  shade::shaders::Enterable* entry = dynamic_cast<shade::shaders::Enterable*>(get_pointer(m_shader));
  if (entry)
  {
    return entry->has_entry(env);
  }
  return false;
}

boost::shared_ptr<shade::Type::State> shade::Component::get_type_state(const Type* type)
{
  const TypeState& ts = m_attributes.find(type)->second;
  return ts.state;
}
