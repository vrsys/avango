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

#include <shade/Shader.h>
#include <shade/Type.h>
#include <shade/types/vec3.h>
#include <shade/types/float.h>
#include <shade/types/objref.h>
#include "StorageFormatter.h"

#include <boost/mpl/vector.hpp>
#include <boost/bind.hpp>
#include <list>
#include <map>

#include <UnitTest++.h>


namespace
{
  class BaseShader : public shade::ShaderBase<BaseShader>
  {
  public:
    virtual shade::vec3<> illuminate(shade::vec3<>, shade::vec3<>) { return shade::vec3<>(); };
    virtual shade::float_<> get_intensity(void)
    { return invoke< shade::float_<> >("get_intensity_impl"); };

  private:
    shade::float_<> m_angle;

    friend class shade::ShaderBase<BaseShader>;
    static std::string class_name;
    static shade::MethodAccessor methods[];
    static shade::AttributeAccessor attributes[];
  };

  std::string BaseShader::class_name("BaseShader");
  shade::MethodAccessor BaseShader::methods[] =
  {
    BaseShader::def(&BaseShader::get_intensity, "get_intensity"),
    BaseShader::def(&BaseShader::illuminate, "illuminate"),
    shade::MethodAccessor()
  };
  shade::AttributeAccessor BaseShader::attributes[] =
  {
    BaseShader::def(&BaseShader::m_angle, "angle"),
    shade::AttributeAccessor()
  };


  class SecondBaseShader : public shade::ShaderBase<SecondBaseShader>
  {
  public:
    virtual shade::vec3<> get_color(shade::float_<>) { return invoke< shade::vec3<> >("get_color_impl"); };

  private:
    friend class shade::ShaderBase<SecondBaseShader>;
    static std::string class_name;
    static shade::MethodAccessor methods[];
  };

  std::string SecondBaseShader::class_name("SecondBaseShader");
  shade::MethodAccessor SecondBaseShader::methods[] =
  {
    SecondBaseShader::def(&SecondBaseShader::get_color, "get_color"),
    shade::MethodAccessor()
  };


  class DerivedShader : public shade::ShaderBase<DerivedShader, BaseShader>
  {
  public:
    shade::vec3<> illuminate(shade::vec3<>, shade::vec3<>) { return invoke< shade::vec3<> >("illuminate_impl"); }

    shade::objref< boost::shared_ptr<SecondBaseShader> > m_ref_color;

  private:
    shade::vec3<> m_color;

    friend class shade::ShaderBase<DerivedShader, BaseShader>;
    static std::string class_name;
    static shade::AttributeAccessor attributes[];
  };

  std::string DerivedShader::class_name("DerivedShader");
  shade::AttributeAccessor DerivedShader::attributes[] =
  {
    DerivedShader::def(&DerivedShader::m_color, "color"),
    DerivedShader::def(&DerivedShader::m_ref_color, "ref_color"),
    shade::AttributeAccessor()
  };


  class DoubleDerivedShader : public shade::ShaderBase<DoubleDerivedShader, BaseShader, SecondBaseShader>
  {
    shade::vec3<> illuminate(shade::vec3<>, shade::vec3<>) { return invoke< shade::vec3<> >("illuminate_impl"); }
  private:
    shade::vec3<> m_color;

    friend class shade::ShaderBase<DoubleDerivedShader, BaseShader, SecondBaseShader>;
    static std::string class_name;
    static shade::AttributeAccessor attributes[];
  };

  std::string DoubleDerivedShader::class_name("DoubleDerivedShader");
  shade::AttributeAccessor DoubleDerivedShader::attributes[] =
  {
    DoubleDerivedShader::def(&DoubleDerivedShader::m_color, "color"),
    shade::AttributeAccessor()
  };

}

namespace
{
  struct type_collect
  {
    void operator()(const std::type_info* ti) { m_ti.push_back(ti); }
    std::list<const std::type_info*> m_ti;
  };

  struct method_collect
  {
    method_collect(void) : m_base(0) {}
    method_collect(shade::Shader* base) : m_base(base) {}
    void operator()(const shade::MethodAccessor& ma)
    {
      m_name = ma.get_name();
      if (m_base)
        m_type = ma.invoke(m_base);
    }
    std::string m_name;
    shade::Shader* m_base;
    boost::shared_ptr<shade::Type> m_type;
  };

  struct attribute_collect
  {
    attribute_collect(void) : m_has_angle(false), m_has_color(false) {}
    void operator()(const shade::Type& type, const std::string& name)
    {
      if (name == "angle")
      {
        m_has_angle = true;
        m_angle_type = type.get_formatter_type();
      }
      if (name == "color") 
      {
        m_has_color = true;
        m_color_type = type.get_formatter_type();
      }
    }
    bool m_has_angle;
    bool m_has_color;
    shade::formatter::Constants::Type m_angle_type;
    shade::formatter::Constants::Type m_color_type;
  };
}

TEST(TraverseHierarchyEquals)
{
  BaseShader b1, b2;
  type_collect callback1, callback2;
  b1.traverse_hierarchy(boost::ref(callback1));
  b2.traverse_hierarchy(boost::ref(callback2));
  CHECK_EQUAL(1u, callback1.m_ti.size());
  CHECK_EQUAL(1u, callback2.m_ti.size());
  CHECK(**callback1.m_ti.begin() == **callback2.m_ti.begin());
}

TEST(TraverseHierarchyUnequals)
{
  BaseShader b1;
  SecondBaseShader b2;
  type_collect callback1, callback2;
  b1.traverse_hierarchy(boost::ref(callback1));
  b2.traverse_hierarchy(boost::ref(callback2));
  CHECK_EQUAL(1u, callback1.m_ti.size());
  CHECK_EQUAL(1u, callback2.m_ti.size());
  CHECK(**callback1.m_ti.begin() != **callback2.m_ti.begin());
}

TEST(TraverseHierarchyDerived)
{
  DerivedShader d;
  type_collect callback;
  d.traverse_hierarchy(boost::ref(callback));
  CHECK_EQUAL(2u, callback.m_ti.size());
  CHECK( **callback.m_ti.begin() != **(++(callback.m_ti.begin())) );
}

TEST(TraverseMethods)
{
  DerivedShader deriv;
  type_collect tc;
  deriv.traverse_hierarchy(boost::ref(tc));

  method_collect mc(&deriv);
  boost::shared_ptr<shade::Shader> conv_deriv = deriv.convert(*tc.m_ti.begin());
  conv_deriv->iterate_methods(boost::ref(mc));

  CHECK_EQUAL("illuminate", mc.m_name);
  CHECK_EQUAL("illuminate_impl", mc.m_type->get_function_name());
  CHECK_EQUAL((*(++tc.m_ti.begin()))->name(), mc.m_type->get_function_type()->name());
}

TEST(TraverseAttributes)
{
  DerivedShader deriv;
  attribute_collect ac;
  deriv.traverse_attributes(boost::ref(ac));

  CHECK(ac.m_has_angle);
  CHECK(ac.m_has_color);
  CHECK_EQUAL(shade::formatter::Constants::float_, ac.m_angle_type);
  CHECK_EQUAL(shade::formatter::Constants::vec3, ac.m_color_type);
}

TEST(IterateSpecificAttributes)
{
  DerivedShader deriv;
  type_collect tc;
  deriv.traverse_hierarchy(boost::ref(tc));

  attribute_collect ac;
  boost::shared_ptr<shade::Shader> conv_deriv = deriv.convert(*++tc.m_ti.begin());
  conv_deriv->iterate_attributes(boost::ref(ac));

  CHECK(!ac.m_has_angle);
  CHECK(ac.m_has_color);
  CHECK_EQUAL(shade::formatter::Constants::vec3, ac.m_color_type);
}

TEST(CheckClassName)
{
  DerivedShader deriv;
  CHECK_EQUAL("BaseShader", deriv.BaseShader::get_class_name());
  CHECK_EQUAL("DerivedShader", deriv.get_class_name());
}

TEST(Convert)
{
  DerivedShader deriv;
  type_collect tc;
  deriv.traverse_hierarchy(boost::ref(tc));

  CHECK_EQUAL("BaseShader", deriv.convert(*tc.m_ti.begin())->get_class_name());
  CHECK_EQUAL("DerivedShader", deriv.convert(*(++tc.m_ti.begin()))->get_class_name());
}

TEST(TraverseHierarchyDoubleDerived)
{
  DoubleDerivedShader d;
  type_collect callback;
  d.traverse_hierarchy(boost::ref(callback));
  CHECK_EQUAL(3u, callback.m_ti.size());
}

TEST(TraverseDoubleDerivedMethods)
{
  DoubleDerivedShader deriv;
  type_collect tc;
  deriv.traverse_hierarchy(boost::ref(tc));

  method_collect mc(&deriv);
  boost::shared_ptr<shade::Shader> conv_deriv = deriv.convert(*tc.m_ti.begin());
  conv_deriv->iterate_methods(boost::ref(mc));

  CHECK_EQUAL("illuminate", mc.m_name);
  CHECK_EQUAL("illuminate_impl", mc.m_type->get_function_name());

  conv_deriv = deriv.convert(*(++tc.m_ti.begin()));
  conv_deriv->iterate_methods(boost::ref(mc));
  CHECK_EQUAL("get_color", mc.m_name);
  CHECK_EQUAL("get_color_impl", mc.m_type->get_function_name());
}

