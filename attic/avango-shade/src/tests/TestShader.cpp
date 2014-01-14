// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#include "DummyShader.h"
#include "DummyShaderWrapper.h"
#include <avango/Link.h>
#include <avango/StandardFields.h>
#include <avango/FieldContainer.h>
#include <avango/osg/Fields.h>
#include <avango/shade/Shader.h>
#include <osg/Vec4>
#include <osg/Vec3>
#include <osg/Vec2>
#include <avango/osg/Texture.h>
#include "../../core/include/shade/osg/Texture.h"
#include <avango/UnitTest++/UnitTest++.h>

SUITE(avShadeShader)
{
  struct Fixture
  {
    Fixture(void)
    {
      initDummyShader();
      node = av::Link<av::shade::Shader>(dynamic_cast<av::shade::Shader*>(av::Type::createInstanceOfType("DummyShader")));
    }

    av::Link<av::shade::Shader> node;
  };

  TEST_FIXTURE(Fixture, makeShaderInstance)
  {
    CHECK(node.isValid());
  }

  TEST_FIXTURE(Fixture, checkFieldsInShaderInstance)
  {
    CHECK(node->hasField("diffuse"));
    CHECK(node->hasField("specular"));
  }

  TEST_FIXTURE(Fixture, changingIntFieldValuesInShaderInstance)
  {
    av::SFInt* field(dynamic_cast<av::SFInt*>(node->getField("intvalue")));
    CHECK(field != 0);
    if (field == 0)
      return;

    boost::shared_ptr<DummyShader> shader(boost::dynamic_pointer_cast<DummyShader>(node->getShader()));
    CHECK(shader);
    if (!shader)
      return;

    int value(42);
    field->setValue(value);
    
    int result;
    shader->intvalue.get(result);
    CHECK_EQUAL(value, result);

    int value2(23);
    shader->intvalue.set(value2);

    CHECK_EQUAL(value2, field->getValue());
  }

  TEST_FIXTURE(Fixture, changingFloatFieldValuesInShaderInstance)
  {
    av::SFFloat* field(dynamic_cast<av::SFFloat*>(node->getField("diffuse")));
    CHECK(field != 0);
    if (field == 0)
      return;

    boost::shared_ptr<DummyShader> shader(boost::dynamic_pointer_cast<DummyShader>(node->getShader()));
    CHECK(shader);
    if (!shader)
      return;

    float value(1.1);
    field->setValue(value);
    
    float result;
    shader->diffuse.get(result);
    CHECK_EQUAL(value, result);

    float value2(2.2);
    shader->diffuse.set(value2);

    CHECK_EQUAL(value2, field->getValue());
  }

  TEST_FIXTURE(Fixture, changingVec2FieldValuesInShaderInstance)
  {
    av::osg::SFVec2* field(dynamic_cast<av::osg::SFVec2*>(node->getField("vec2value")));
    CHECK(field != 0);
    if (field == 0)
      return;

    boost::shared_ptr<DummyShader> shader(boost::dynamic_pointer_cast<DummyShader>(node->getShader()));
    CHECK(shader);
    if (!shader)
      return;

    ::osg::Vec2 value(1.1, 2.2);
    field->setValue(value);

    float rx, ry;
    shader->vec2value.get(rx, ry);
    CHECK_EQUAL(value.x(), rx);
    CHECK_EQUAL(value.y(), ry);

    float vx(5.1), vy(5.2);
    shader->vec2value.set(vx, vy);

    ::osg::Vec2 result(field->getValue());
    CHECK_EQUAL(vx, result.x());
    CHECK_EQUAL(vy, result.y());
  }

  TEST_FIXTURE(Fixture, changingVec3FieldValuesInShaderInstance)
  {
    av::osg::SFVec3* field(dynamic_cast<av::osg::SFVec3*>(node->getField("vec3value")));
    CHECK(field != 0);
    if (field == 0)
      return;

    boost::shared_ptr<DummyShader> shader(boost::dynamic_pointer_cast<DummyShader>(node->getShader()));
    CHECK(shader);
    if (!shader)
      return;

    ::osg::Vec3 value(1.1, 2.2, 3.3);
    field->setValue(value);

    float rx, ry, rz;
    shader->vec3value.get(rx, ry, rz);
    CHECK_EQUAL(value.x(), rx);
    CHECK_EQUAL(value.y(), ry);
    CHECK_EQUAL(value.z(), rz);

    float vx(5.1), vy(5.2), vz(5.3);
    shader->vec3value.set(vx, vy, vz);

    ::osg::Vec3 result(field->getValue());
    CHECK_EQUAL(vx, result.x());
    CHECK_EQUAL(vy, result.y());
    CHECK_EQUAL(vz, result.z());
  }

  TEST_FIXTURE(Fixture, changingVec4FieldValuesInShaderInstance)
  {
    av::osg::SFVec4* field(dynamic_cast<av::osg::SFVec4*>(node->getField("color")));
    CHECK(field != 0);
    if (field == 0)
      return;

    boost::shared_ptr<DummyShader> shader(boost::dynamic_pointer_cast<DummyShader>(node->getShader()));
    CHECK(shader);
    if (!shader)
      return;

    ::osg::Vec4 value(1.1, 2.2, 3.3, 4.4);
    field->setValue(value);

    float rx, ry, rz, rw;
    shader->color.get(rx, ry, rz, rw);
    CHECK_EQUAL(value.x(), rx);
    CHECK_EQUAL(value.y(), ry);
    CHECK_EQUAL(value.z(), rz);
    CHECK_EQUAL(value.w(), rw);

    float vx(5.1), vy(5.2), vz(5.3), vw(5.4);
    shader->color.set(vx, vy, vz, vw);

    ::osg::Vec4 result(field->getValue());
    CHECK_EQUAL(vx, result.x());
    CHECK_EQUAL(vy, result.y());
    CHECK_EQUAL(vz, result.z());
    CHECK_EQUAL(vw, result.w());
  }

  TEST_FIXTURE(Fixture, changingMatrixFieldValuesInShaderInstance)
  {
    av::osg::SFMatrix* field(dynamic_cast<av::osg::SFMatrix*>(node->getField("mat4value")));
    CHECK(field != 0);
    if (field == 0)
      return;

    boost::shared_ptr<DummyShader> shader(boost::dynamic_pointer_cast<DummyShader>(node->getShader()));
    CHECK(shader);
    if (!shader)
      return;

    float vs[16] =
     {
      1., 2., 3., 4., 5., 6., 7., 8.,
      9., 10., 11., 12., 13., 14., 15., 16.
     };
    ::osg::Matrix value(vs);
    field->setValue(value);

    float rs[16];
    shader->mat4value.get(rs);
    for (int i = 0; i != 16; ++i)
     {
      CHECK_CLOSE(vs[i], rs[i], 0.001);
     }

    float vs2[16] =
     {
      11., 12., 13., 14., 15., 16., 17., 18.,
      19., 110., 111., 112., 113., 114., 115., 116.
     };
    shader->mat4value.set(vs2);

    ::osg::Matrix result(field->getValue());
    for (int i = 0; i != 16; ++i)
     {
      CHECK_CLOSE(vs[i], rs[i], 0.001);
     }
  }


  TEST_FIXTURE(Fixture, changingObjrefFieldValuesInShaderInstance)
  {
    av::SFContainer* field(dynamic_cast<av::SFContainer*>(node->getField("light")));
    CHECK(field != 0);
    if (field == 0)
      return;

    boost::shared_ptr<DummyShader> shader(boost::dynamic_pointer_cast<DummyShader>(node->getShader()));
    CHECK(shader);
    if (!shader)
      return;

    // Check if contained SHADE shader is really set
    av::Link<av::shade::Shader> value(dynamic_cast<av::shade::Shader*>(av::Type::createInstanceOfType("DummyShader")));
    field->setValue(value);
    CHECK_EQUAL(value->getShader(), shader->light.get_generic());

    // Setting only the SHADE shader should 'return' the wrapping field container
    av::Link<av::shade::Shader> value2(dynamic_cast<av::shade::Shader*>(av::Type::createInstanceOfType("DummyShader")));
    shader->light.set_generic(value2->getShader());
    CHECK(value2 == field->getValue());
  }

  struct ListFixture : public Fixture
  {
    ListFixture(void) :
      field(dynamic_cast<av::MFContainer*>(node->getField("listvalue"))),
      shader(boost::dynamic_pointer_cast<DummyShader>(node->getShader())),
      value1(dynamic_cast<av::shade::Shader*>(av::Type::createInstanceOfType("DummyShader"))),
      value2(dynamic_cast<av::shade::Shader*>(av::Type::createInstanceOfType("DummyShader")))
    {
      value_list.push_back(value1);
      value_list.push_back(value2);
    }

    av::MFContainer* field;
    boost::shared_ptr<DummyShader> shader;

    av::MFContainer::ContainerType value_list;
    av::Link<av::shade::Shader> value1;
    av::Link<av::shade::Shader> value2;
  };

  TEST_FIXTURE(ListFixture, settingListFieldValuesInShaderInstance)
  {
    CHECK(field != 0);
    if (field == 0)
      return;

    CHECK(shader);
    if (!shader)
      return;

    field->setValue(value_list);

    {
      DummyShader::ListType::Accessor accessor(shader->listvalue);
      CHECK_EQUAL(2u, accessor->size());
      if (2u != accessor->size())
        return;
      CHECK_EQUAL(value1->getShader(), (*accessor)[0]);
      CHECK_EQUAL(value2->getShader(), (*accessor)[1]);
    }
  }

  TEST_FIXTURE(ListFixture, gettingListFieldValuesInShaderInstance)
  {
    CHECK(field != 0);
    if (field == 0)
      return;

    CHECK(shader);
    if (!shader)
      return;

    {
      DummyShader::ListType::Accessor accessor(shader->listvalue);
      accessor->clear();
      accessor->push_back(boost::dynamic_pointer_cast<DummyShader>(value1->getShader()));
      accessor->push_back(boost::dynamic_pointer_cast<DummyShader>(value2->getShader()));
    }

    value_list = field->getValue();
    CHECK_EQUAL(2u, value_list.size());
    if (2u != value_list.size())
      return;
    CHECK(value1 == value_list[0]);
    CHECK(value2 == value_list[1]);
  }

  TEST_FIXTURE(Fixture, changingTextureFieldValuesInShaderInstance)
  {
    av::osg::SFTexture* field(dynamic_cast<av::osg::SFTexture*>(node->getField("texturevalue")));
    CHECK(field != 0);
    if (field == 0)
      return;

    boost::shared_ptr<DummyShader> shader(boost::dynamic_pointer_cast<DummyShader>(node->getShader()));
    CHECK(shader);
    if (!shader)
      return;

    // Check if contained SHADE texture is really set
    av::Link<av::osg::Texture> value(dynamic_cast<av::osg::Texture*>(av::Type::createInstanceOfType("av::osg::Texture2D")));
    CHECK(value.isValid());
    if (!value.isValid())
      return;
    field->setValue(value);
    boost::shared_ptr<shade::osg::Texture> set_result(boost::dynamic_pointer_cast<shade::osg::Texture>(shader->texturevalue.get()));
    CHECK(set_result);
    if (!set_result)
      return;
    CHECK(shade::osg::Texture(value->getOsgTexture()) == *set_result);

    // Setting only the SHADE texture should 'return' the wrapping field container
    av::Link<av::osg::Texture> value2(dynamic_cast<av::osg::Texture*>(av::Type::createInstanceOfType("av::osg::Texture2D")));
    CHECK(value2.isValid());
    if (!value2.isValid())
      return;
    shader->texturevalue.set(boost::shared_ptr<shade::osg::Texture>(new shade::osg::Texture(value2->getOsgTexture())));
    av::Link<av::osg::Texture> get_result = field->getValue();
    CHECK(get_result.isValid());
    if (!get_result.isValid())
      return;
    CHECK(value2->getOsgTexture() == get_result->getOsgTexture());
  }

  TEST(makeWrappedShaderInstance)
  {
    initDummyShaderWrapper();
    av::Link<DummyShaderWrapper> node(dynamic_cast<DummyShaderWrapper*>(av::Type::createInstanceOfType("DummyShaderWrapper")));
    CHECK(node.isValid());
    CHECK(node->hasField("diffuse"));
    CHECK(node->hasField("AStringField"));
  }
}
