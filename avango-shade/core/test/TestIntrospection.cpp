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
#include <shade/ShaderEnvironment.h>
#include <shade/utility.h>
#include <shade/types/float.h>
#include <shade/types/uniform.h>
#include <shade/types/FloatAccessor.h>
#include <shade/types/vec4.h>
#include <shade/types/Vec4Accessor.h>
#include <shade/types/vec3.h>
#include <shade/types/Vec3Accessor.h>
#include <shade/types/vec2.h>
#include <shade/types/Vec2Accessor.h>
#include <shade/types/matrix4.h>
#include <shade/types/Matrix4Accessor.h>
#include <shade/types/int.h>
#include <shade/types/IntAccessor.h>
#include <shade/types/TypeAccessor.h>
#include <shade/types/objref.h>
#include <shade/types/ObjrefAccessor.h>
#include <shade/types/list.h>
#include <shade/types/ListAccessor.h>
#include <shade/types/const.h>
#include <shade/types/sampler2D.h>
#include <shade/types/sampler2DArray.h>
#include <shade/types/sampler2DShadow.h>
#include <shade/types/usampler2D.h>
#include <shade/types/samplerCube.h>
#include <shade/types/TextureAccessor.h>
#include "GLSLMockupWrapper.h"
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <UnitTest++.h>

namespace
{
  class Simple : public shade::ShaderBase<Simple>
  {
  public:
    shade::float_<shade::uniform> public_simple_value;
    shade::float_<shade::uniform> private_simple_value;

  private:
    SHADE_BASE_DECL(Simple)
  };

  SHADE_CLASS_INIT(Simple, "<undefined>", SHADE_NONE,
      SHADE_ENV_DEFS(shade::application_stage, (public_simple_value))
      SHADE_DEFS((private_simple_value))
      )


    class Derived : public shade::ShaderBase<Derived, Simple>
  {
  public:
    shade::float_<shade::uniform> public_derived_value;
    shade::float_<shade::uniform> private_derived_value;

  private:
    SHADE_DERIVED_DECL(Derived, Simple)
  };

  SHADE_CLASS_INIT(Derived, "<undefined>", SHADE_NONE,
      SHADE_ENV_DEFS(shade::application_stage, (public_derived_value))
      SHADE_DEFS((private_derived_value))
      )


    struct StoreNames
    {
      void operator()(shade::Type* type, const std::string& name)
      {
        attributes[name] = type;
      }

      typedef std::map<std::string, shade::Type*> Attributes;
      Attributes attributes;
    };
}


SUITE(Introspection)
{
  TEST(HasOnlyPublicAttributes)
  {
    StoreNames names;
    Derived obj;

    obj.traverse_public_attributes(boost::ref(names));

    CHECK_EQUAL(2u, names.attributes.size());
    CHECK(names.attributes.find("public_simple_value") != names.attributes.end());
    CHECK(names.attributes.find("public_derived_value") != names.attributes.end());
  }

  TEST(GenericFloatAccess)
  {
    StoreNames names;
    Derived obj;
    obj.traverse_public_attributes(boost::ref(names));

    StoreNames::Attributes::const_iterator iter(names.attributes.find("public_simple_value"));
    if (iter == names.attributes.end())
      return;

    float value(1.1);
    obj.public_simple_value.set(value);

    shade::types::FloatAccessor* accessor(dynamic_cast<shade::types::FloatAccessor*>(iter->second));
    CHECK(accessor != 0);
    if (accessor == 0)
      return;

    float result;
    accessor->get(result);
    CHECK_EQUAL(value, result);

    float value2(2.2);
    accessor->set(value2);
    obj.public_simple_value.get(result);
    CHECK_EQUAL(value2, result);
  }

  TEST(GenericVec4Access)
  {
    float vx(1.1), vy(2.2), vz(3.3), vw(4.4);
    shade::vec4<> value(vx, vy, vz, vw);
    shade::types::Vec4Accessor* accessor(dynamic_cast<shade::types::Vec4Accessor*>(&value));
    CHECK(accessor != 0);
    if (accessor == 0)
      return;

    float rx, ry, rz, rw;
    accessor->get(rx, ry, rz, rw);
    CHECK_EQUAL(vx, rx);
    CHECK_EQUAL(vy, ry);
    CHECK_EQUAL(vz, rz);
    CHECK_EQUAL(vw, rw);
  }

  TEST(GenericVec3Access)
  {
    float vx(1.1), vy(2.2), vz(3.3);
    shade::vec3<> value(vx, vy, vz);
    shade::types::Vec3Accessor* accessor(dynamic_cast<shade::types::Vec3Accessor*>(&value));
    CHECK(accessor != 0);
    if (accessor == 0)
      return;

    float rx, ry, rz;
    accessor->get(rx, ry, rz);
    CHECK_EQUAL(vx, rx);
    CHECK_EQUAL(vy, ry);
    CHECK_EQUAL(vz, rz);
  }

  TEST(GenericVec2Access)
  {
    float vx(1.1), vy(2.2);
    shade::vec2<> value(vx, vy);
    shade::types::Vec2Accessor* accessor(dynamic_cast<shade::types::Vec2Accessor*>(&value));
    CHECK(accessor != 0);
    if (accessor == 0)
      return;

    float rx, ry;
    accessor->get(rx, ry);
    CHECK_EQUAL(vx, rx);
    CHECK_EQUAL(vy, ry);
  }

  TEST(GenericMatrix4Access)
  {
    float vs[16] =
    {
      1., 2., 3., 4., 5., 6., 7., 8.,
      9., 10., 11., 12., 13., 14., 15., 16.
    };
    shade::matrix4<> value(vs);
    shade::types::Matrix4Accessor* accessor(dynamic_cast<shade::types::Matrix4Accessor*>(&value));
    CHECK(accessor != 0);
    if (accessor == 0)
      return;

    float rs[16];
    accessor->get(rs);
    for (int i = 0; i != 16; ++i)
    {
      CHECK_EQUAL(vs[i], rs[i]);
    }
  }


  TEST(GenericIntAccess)
  {
    int v(42);
    shade::int_<> value(v);
    shade::types::IntAccessor* accessor(dynamic_cast<shade::types::IntAccessor*>(&value));
    CHECK(accessor != 0);
    if (accessor == 0)
      return;

    int result;
    accessor->get(result);
    CHECK_EQUAL(v, result);
  }

  TEST(HashValue)
  {
    shade::vec4<> value_vec4;
    shade::float_<> value_float;

    shade::types::TypeAccessor* accessor_vec4(dynamic_cast<shade::types::TypeAccessor*>(&value_vec4));
    shade::types::TypeAccessor* accessor_float(dynamic_cast<shade::types::TypeAccessor*>(&value_float));

    CHECK(accessor_vec4->hash() != accessor_float->hash());
  }

  struct ObjRefFixture
  {
    ObjRefFixture(void) : 
      accessor(dynamic_cast<shade::types::ObjrefAccessor*>(&value))
    {
    }

    bool is_accessor_valid(void) const
    {
      return (accessor != 0);
    }

    shade::objref<boost::shared_ptr<Derived>, shade::uniform> value;
    shade::types::ObjrefAccessor* accessor;
  };

  TEST_FIXTURE(ObjRefFixture, GenericObjRefAccess)
  {
    CHECK(is_accessor_valid());
    if (!is_accessor_valid())
      return;

    boost::shared_ptr<Derived> instance(new Derived);
    value = instance;
    CHECK_EQUAL(instance, accessor->get_generic());

    instance = boost::shared_ptr<Derived>(new Derived);
    accessor->set_generic(instance);
    CHECK_EQUAL(instance, value.get());
  }

  TEST_FIXTURE(ObjRefFixture, GenericObjRefWrongClassAccess)
  {
    CHECK(is_accessor_valid());
    if (!is_accessor_valid())
      return;

    boost::shared_ptr<Simple> instance(new Simple);
    CHECK_THROW(accessor->set_generic(instance), shade::types::ObjrefAccessor::BadCast);
  }

  TEST_FIXTURE(ObjRefFixture, GenericObjRefNullPointerAccess)
  {
    CHECK(is_accessor_valid());
    if (!is_accessor_valid())
      return;

    boost::shared_ptr<Derived> instance;
    accessor->set_generic(instance);
    CHECK(!accessor->get_generic());
  }

  struct ListFixture
  {
    typedef shade::list< std::vector< boost::shared_ptr<Derived> >, shade::const_ > DerivedList;

    ListFixture(void) :
      accessor(dynamic_cast<shade::types::ListAccessor*>(&derived_list)),
      value1(new Derived),
      value2(new Derived)
    {
    }

    bool is_accessor_valid(void) const
    {
      return (accessor != 0);
    }

    DerivedList derived_list;
    shade::types::ListAccessor* accessor;
    boost::shared_ptr<Derived> value1;
    boost::shared_ptr<Derived> value2;
  };

  TEST_FIXTURE(ListFixture, GenericListGetter)
  {
    CHECK(is_accessor_valid());
    if (!is_accessor_valid())
      return;

    {
      DerivedList::Accessor list_accessor(derived_list);
      list_accessor->push_back(value1);
      list_accessor->push_back(value2);
    }

    shade::types::ListAccessor::Range range(accessor->get());
    shade::types::ListAccessor::InIterator iter(range.first);
    CHECK(iter != range.second);
    if (iter == range.second)
      return;
    CHECK_EQUAL(value1, *iter);
    ++iter;
    CHECK(iter != range.second);
    if (iter == range.second)
      return;
    CHECK_EQUAL(value2, *iter);
    ++iter;
    CHECK(iter == range.second);
  }

  TEST_FIXTURE(ListFixture, GenericListSetter)
  {
    CHECK(is_accessor_valid());
    if (!is_accessor_valid())
      return;

    {
      shade::types::ListAccessor::OutIterator setter(accessor->set());
      *setter = value1;
      ++setter;
      *setter = value2;
      ++setter;
    }

    DerivedList::Accessor list_accessor(derived_list);
    CHECK_EQUAL(2u, list_accessor->size());
    if (list_accessor->size() != 2)
      return;
    CHECK_EQUAL(value1, (*list_accessor)[0]);
    CHECK_EQUAL(value2, (*list_accessor)[1]);
  }

  TEST_FIXTURE(ListFixture, GenericListWrongClassAccess)
  {
    CHECK(is_accessor_valid());
    if (!is_accessor_valid())
      return;

    boost::shared_ptr<Simple> instance(new Simple);
    shade::types::ListAccessor::OutIterator setter(accessor->set());
    CHECK_THROW((*setter = instance, ++setter), shade::types::ListAccessor::BadCast);
  }

  TEST(GenericSampler2DAccess)
  {
    shade::sampler2D value;
    shade::types::TextureAccessor* accessor(dynamic_cast<shade::types::TextureAccessor*>(&value));
    CHECK(accessor != 0);
    if (accessor == 0)
      return;

    boost::shared_ptr<MockupTexture> instance(new MockupTexture(0));
    value.set(instance);
    CHECK_EQUAL(instance, accessor->get());

    instance = boost::shared_ptr<MockupTexture>(new MockupTexture(1));
    accessor->set(instance);
    CHECK_EQUAL(instance, value.get());
  }

  TEST(GenericSampler2DArrayAccess)
  {
    shade::sampler2DArray value;
    shade::types::TextureAccessor* accessor(dynamic_cast<shade::types::TextureAccessor*>(&value));
    CHECK(accessor != 0);
    if (accessor == 0)
      return;

    boost::shared_ptr<MockupTexture> instance(new MockupTexture(0));
    value.set(instance);
    CHECK_EQUAL(instance, accessor->get());

    instance = boost::shared_ptr<MockupTexture>(new MockupTexture(1));
    accessor->set(instance);
    CHECK_EQUAL(instance, value.get());
  }

  TEST(GenericSampler2DShadowAccess)
  {
    shade::sampler2DShadow value;
    shade::types::TextureAccessor* accessor(dynamic_cast<shade::types::TextureAccessor*>(&value));
    CHECK(accessor != 0);
    if (accessor == 0)
      return;

    boost::shared_ptr<MockupTexture> instance(new MockupTexture(0));
    value.set(instance);
    CHECK_EQUAL(instance, accessor->get());

    instance = boost::shared_ptr<MockupTexture>(new MockupTexture(1));
    accessor->set(instance);
    CHECK_EQUAL(instance, value.get());
  }

  TEST(GenericUSampler2DAccess)
  {
    shade::usampler2D value;
    shade::types::TextureAccessor* accessor(dynamic_cast<shade::types::TextureAccessor*>(&value));
    CHECK(accessor != 0);
    if (accessor == 0)
      return;

    boost::shared_ptr<MockupTexture> instance(new MockupTexture(0));
    value.set(instance);
    CHECK_EQUAL(instance, accessor->get());

    instance = boost::shared_ptr<MockupTexture>(new MockupTexture(1));
    accessor->set(instance);
    CHECK_EQUAL(instance, value.get());
  }

  TEST(GenericSamplerCuberAccess)
  {
    shade::samplerCube value;
    shade::types::TextureAccessor* accessor(dynamic_cast<shade::types::TextureAccessor*>(&value));
    CHECK(accessor != 0);
    if (accessor == 0)
      return;

    boost::shared_ptr<MockupTexture> instance(new MockupTexture(0));
    value.set(instance);
    CHECK_EQUAL(instance, accessor->get());

    instance = boost::shared_ptr<MockupTexture>(new MockupTexture(1));
    accessor->set(instance);
    CHECK_EQUAL(instance, value.get());
  }

}
