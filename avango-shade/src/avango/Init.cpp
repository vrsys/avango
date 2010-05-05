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

#include <avango/shade/Init.h>
#include <avango/Logger.h>
#include <avango/shade/LightSource.h>
#include <avango/shade/State.h>
#include <avango/shade/StateGroup.h>
#include <avango/shade/FieldAdapter.h>
#include "../../core/include/shade/Program.h"
#include "../../core/include/shade/shaders/CameraSpace.h"
#include "../../core/include/shade/shaders/Constant.h"
#include "../../core/include/shade/shaders/DirectionalLight.h"
#include "../../core/include/shade/shaders/GLLightPosition.h"
#include "../../core/include/shade/shaders/Plastic.h"
#include "../../core/include/shade/shaders/PointLight.h"
#include "../../core/include/shade/shaders/Surface.h"
#include "../../core/include/shade/shaders/Texture2D.h"
#include "../../core/include/shade/shaders/UVCoord.h"
#include "../../core/include/shade/shaders/Value.h"
#include "../../core/include/shade/shaders/Vec4ToVec3.h"
#include "../../core/include/shade/shaders/XYCoord.h"
#include "../../core/include/shade/types/float.h"
#include "../../core/include/shade/types/vec2.h"
#include "../../core/include/shade/types/vec3.h"
#include "../../core/include/shade/types/vec4.h"
#include "../../core/include/shade/types/uniform.h"
#include "../../core/include/shade/types/const.h"
#include "../../core/include/shade/types/inout.h"
#include "FloatFieldAdapter.h"
#include "IntFieldAdapter.h"
#include "ListFieldAdapter.h"
#include "ObjrefFieldAdapter.h"
#include "TextureFieldAdapter.h"
#include "Vec2FieldAdapter.h"
#include "Vec3FieldAdapter.h"
#include "Vec4FieldAdapter.h"
#include "MatrixFieldAdapter.h"
#include <algorithm>

namespace
{
  av::Logger& logger(av::getLogger("av::shade::Init"));

  class PrependPath
  {
  public:

    PrependPath(boost::shared_ptr< ::shade::Program> program) :
      mProgram(program)
    {
    }

    void operator()(const std::string& path) const
    {
      mProgram->prepend_search_path(path);
    }

  private:
    boost::shared_ptr< ::shade::Program> mProgram;
  };
}

AV_TYPED_DEFINE_ABSTRACT(av::shade::Init);

/*static*/ void
av::shade::Init::initClass()
{
  if (!isTypeInitialized())
  {
    State::initClass();
    StateGroup::initClass();
    LightSource::initClass();

    registerFieldAdapter(boost::shared_ptr<FieldAdapter>(new FloatFieldAdapter));
    registerFieldAdapter(boost::shared_ptr<FieldAdapter>(new IntFieldAdapter));
    registerFieldAdapter(boost::shared_ptr<FieldAdapter>(new ListFieldAdapter));
    registerFieldAdapter(boost::shared_ptr<FieldAdapter>(new ObjrefFieldAdapter));
    registerFieldAdapter(boost::shared_ptr<FieldAdapter>(new TextureFieldAdapter));
    registerFieldAdapter(boost::shared_ptr<FieldAdapter>(new Vec2FieldAdapter));
    registerFieldAdapter(boost::shared_ptr<FieldAdapter>(new Vec3FieldAdapter));
    registerFieldAdapter(boost::shared_ptr<FieldAdapter>(new Vec4FieldAdapter));
    registerFieldAdapter(boost::shared_ptr<FieldAdapter>(new MatrixFieldAdapter));

    registerShader< ::shade::shaders::CameraSpace >("av::shade::CameraSpace");
    registerShader< ::shade::shaders::Constant >("av::shade::Constant");
    registerShader< ::shade::shaders::DirectionalLight >("av::shade::DirectionalLight");
    registerShader< ::shade::shaders::GLLightPosition >("av::shade::GLLightPosition");
    registerShader< ::shade::shaders::Plastic >("av::shade::Plastic");
    registerShader< ::shade::shaders::PointLight >("av::shade::PointLight");
    registerShader< ::shade::shaders::Surface >("av::shade::Surface");
    registerShader< ::shade::shaders::Texture2D >("av::shade::Texture2D");
    registerShader< ::shade::shaders::UVCoord >("av::shade::UVCoord");
    registerShader< ::shade::shaders::Value< ::shade::float_, ::shade::const_ > >("av::shade::Value_float_const");
    registerShader< ::shade::shaders::Value< ::shade::vec2, ::shade::const_ > >("av::shade::Value_vec2_const");
    registerShader< ::shade::shaders::Value< ::shade::vec3, ::shade::const_ > >("av::shade::Value_vec3_const");
    registerShader< ::shade::shaders::Value< ::shade::vec4, ::shade::const_ > >("av::shade::Value_vec4_const");
    registerShader< ::shade::shaders::Value< ::shade::float_, ::shade::uniform > >("av::shade::Value_float_uniform");
    registerShader< ::shade::shaders::Value< ::shade::vec2, ::shade::uniform > >("av::shade::Value_vec2_uniform");
    registerShader< ::shade::shaders::Value< ::shade::vec3, ::shade::uniform > >("av::shade::Value_vec3_uniform");
    registerShader< ::shade::shaders::Value< ::shade::vec4, ::shade::uniform > >("av::shade::Value_vec4_uniform");
    registerShader< ::shade::shaders::Value< ::shade::float_, ::shade::raw_varying > >("av::shade::Value_float_raw_varying");
    registerShader< ::shade::shaders::Value< ::shade::vec2, ::shade::raw_varying > >("av::shade::Value_vec2_raw_varying");
    registerShader< ::shade::shaders::Value< ::shade::vec3, ::shade::raw_varying > >("av::shade::Value_vec3_raw_varying");
    registerShader< ::shade::shaders::Value< ::shade::vec4, ::shade::raw_varying > >("av::shade::Value_vec4_raw_varying");
    registerShader< ::shade::shaders::Vec4ToVec3 >("av::shade::Vec4ToVec3");
    registerShader< ::shade::shaders::XYCoord >("av::shade::XYCoord");

    AV_TYPED_INIT_ABSTRACT(av::Type::badType(), "av::shade::Init", true);
  }
}

/*static*/ void
av::shade::Init::registerFieldAdapter(boost::shared_ptr<FieldAdapter> adapter)
{
  mFieldAdapters[adapter->hash()] = adapter;
}

/*static*/ boost::shared_ptr<av::shade::FieldAdapter>
av::shade::Init::getFieldAdapter(::shade::types::TypeAccessor::HashType hash)
{
  return mFieldAdapters[hash];
}

/*static*/ void av::shade::Init::initProgram(boost::shared_ptr< ::shade::Program> program)
{
  std::for_each(mSearchPaths.rbegin(), mSearchPaths.rend(), PrependPath(program));
}

/*static*/ void av::shade::Init::appendSearchPath(const std::string& path)
{
  mSearchPaths.push_back(path);
}

std::map< ::shade::types::TypeAccessor::HashType, boost::shared_ptr<av::shade::FieldAdapter> > av::shade::Init::mFieldAdapters;
std::vector<std::string> av::shade::Init::mSearchPaths;
