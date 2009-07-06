// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#include <avango/osg/StateSet.h>
#include <avango/Logger.h>
#include <boost/bind.hpp>
#include <osg/PolygonMode>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::StateSet"));
}

AV_FC_DEFINE(av::osg::StateSet);

AV_FIELD_DEFINE(av::osg::SFStateSet);
AV_FIELD_DEFINE(av::osg::MFStateSet);

av::osg::StateSet::StateSet(::osg::StateSet* osgstateset) :
  Object(osgstateset),
  mOsgStateSet(osgstateset)
{
  AV_FC_ADD_ADAPTOR_FIELD(Fog,
                            boost::bind(&StateSet::getFogCB, this, _1),
                            boost::bind(&StateSet::setFogCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(FogMode,
                            boost::bind(&StateSet::getFogModeCB, this, _1),
                            boost::bind(&StateSet::setFogModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(LightingMode,
                            boost::bind(&StateSet::getLightingModeCB, this, _1),
                            boost::bind(&StateSet::setLightingModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(BlendMode,
                            boost::bind(&StateSet::getBlendModeCB, this, _1),
                            boost::bind(&StateSet::setBlendModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(PolygonOffset,
                            boost::bind(&StateSet::getPolygonOffsetCB, this, _1),
                            boost::bind(&StateSet::setPolygonOffsetCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(PolygonOffsetMode,
                            boost::bind(&StateSet::getPolygonOffsetModeCB, this, _1),
                            boost::bind(&StateSet::setPolygonOffsetModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(WireframeMode,
                            boost::bind(&StateSet::getWireframeModeCB, this, _1),
                            boost::bind(&StateSet::setWireframeModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderingHint,
                            boost::bind(&StateSet::getRenderingHintCB, this, _1),
                            boost::bind(&StateSet::setRenderingHintCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderBin,
                            boost::bind(&StateSet::getRenderBinCB, this, _1),
                            boost::bind(&StateSet::setRenderBinCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(CullFace,
                            boost::bind(&StateSet::getCullFaceCB, this, _1),
                            boost::bind(&StateSet::setCullFaceCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(CullFaceMode,
                            boost::bind(&StateSet::getCullFaceModeCB, this, _1),
                            boost::bind(&StateSet::setCullFaceModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(RescaleNormalMode,
                            boost::bind(&StateSet::getRescaleNormalModeCB, this, _1),
                            boost::bind(&StateSet::setRescaleNormalModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(NormalizeMode,
                            boost::bind(&StateSet::getNormalizeModeCB, this, _1),
                            boost::bind(&StateSet::setNormalizeModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Texture0,
                            boost::bind(&StateSet::getTextureCB, this, _1),
                            boost::bind(&StateSet::setTextureCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(DepthTestMode,
                            boost::bind(&StateSet::getDepthTestModeCB, this, _1),
                            boost::bind(&StateSet::setDepthTestModeCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Program,
                            boost::bind(&StateSet::getProgramCB, this, _1),
                            boost::bind(&StateSet::setProgramCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Uniforms,
                            boost::bind(&StateSet::getUniformsCB, this, _1),
                            boost::bind(&StateSet::setUniformsCB, this, _1));
}

av::osg::StateSet::~StateSet()
{}

void
av::osg::StateSet::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Object::initClass();

    AV_FC_INIT(av::osg::Object, av::osg::StateSet, true);

    SFStateSet::initClass("av::osg::SFStateSet", "av::Field");
    MFStateSet::initClass("av::osg::MFStateSet", "av::Field");
  }
}

::osg::StateSet*
av::osg::StateSet::getOsgStateSet() const
{
  return mOsgStateSet;
}

/* virtual */ void
av::osg::StateSet::getFogCB(const av::osg::SFFog::GetValueEvent& event)
{
  *(event.getValuePtr()) = av::osg::get_from_osg_object<av::osg::Fog>(mOsgStateSet->getAttribute(::osg::StateAttribute::FOG));
}

/* virtual */ void
av::osg::StateSet::setFogCB(const av::osg::SFFog::SetValueEvent& event)
{
  ::osg::Fog *osg_fog = (event.getValue().isValid() ? event.getValue()->getOsgFog() : 0);
  if (osg_fog != 0)
    mOsgStateSet->setAttribute(osg_fog);
  else
    mOsgStateSet->removeAttribute(::osg::StateAttribute::FOG);
}

/* virtual */ void
av::osg::StateSet::getFogModeCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgStateSet->getMode(GL_FOG);
}

/* virtual */ void
av::osg::StateSet::setFogModeCB(const av::SFInt::SetValueEvent& event)
{
  mOsgStateSet->setMode(GL_FOG, event.getValue());
}

/* virtual */ void
av::osg::StateSet::getLightingModeCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgStateSet->getMode(GL_LIGHTING);
}

/* virtual */ void
av::osg::StateSet::setLightingModeCB(const av::SFInt::SetValueEvent& event)
{
  mOsgStateSet->setMode(GL_LIGHTING, event.getValue());
}

/* virtual */ void
av::osg::StateSet::getBlendModeCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgStateSet->getMode(GL_BLEND);
}

/* virtual */ void
av::osg::StateSet::setBlendModeCB(const av::SFInt::SetValueEvent& event)
{
  mOsgStateSet->setMode(GL_BLEND, event.getValue());
}

/* virtual */ void
av::osg::StateSet::getWireframeModeCB(const av::SFInt::GetValueEvent& event)
{
  ::osg::PolygonMode* polyModeObj = dynamic_cast< ::osg::PolygonMode* >(mOsgStateSet->getAttribute( ::osg::StateAttribute::POLYGONMODE ));

  if (polyModeObj && polyModeObj->getMode( ::osg::PolygonMode::FRONT_AND_BACK ) == ::osg::PolygonMode::LINE )
    *(event.getValuePtr()) = 1;
  else if (polyModeObj && polyModeObj->getMode( ::osg::PolygonMode::FRONT_AND_BACK ) == ::osg::PolygonMode::POINT )
    *(event.getValuePtr()) = 2;
  else
    *(event.getValuePtr()) = 0;
}

/* virtual */ void
av::osg::StateSet::setWireframeModeCB(const av::SFInt::SetValueEvent& event)
{
  ::osg::PolygonMode* polyModeObj = dynamic_cast< ::osg::PolygonMode* >(mOsgStateSet->getAttribute( ::osg::StateAttribute::POLYGONMODE ));
  if (!polyModeObj)
    polyModeObj = new ::osg::PolygonMode;

  if (event.getValue() == 0)
    mOsgStateSet->removeAttribute(polyModeObj);
  else
  {
    mOsgStateSet->setAttribute( polyModeObj );
    if (event.getValue() == 1)
      polyModeObj->setMode( ::osg::PolygonMode::FRONT_AND_BACK, ::osg::PolygonMode::LINE );
    if (event.getValue() == 2)
      polyModeObj->setMode( ::osg::PolygonMode::FRONT_AND_BACK, ::osg::PolygonMode::POINT );
  }
}

/* virtual */ void
av::osg::StateSet::getPolygonOffsetCB(const av::osg::SFPolygonOffset::GetValueEvent& event)
{
  *(event.getValuePtr()) = av::osg::get_from_osg_object<av::osg::PolygonOffset>(mOsgStateSet->getAttribute(::osg::StateAttribute::POLYGONOFFSET));
}

/* virtual */ void
av::osg::StateSet::setPolygonOffsetCB(const av::osg::SFPolygonOffset::SetValueEvent& event)
{
  ::osg::PolygonOffset *osg_polygon_offset =
    (event.getValue().isValid() ? event.getValue()->getOsgPolygonOffset() : 0);
  if (osg_polygon_offset != 0)
    mOsgStateSet->setAttribute(osg_polygon_offset);
  else
    mOsgStateSet->removeAttribute(::osg::StateAttribute::POLYGONOFFSET);
}

/* virtual */ void
av::osg::StateSet::getPolygonOffsetModeCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgStateSet->getMode(GL_POLYGON_OFFSET_FILL);
}

/* virtual */ void
av::osg::StateSet::setPolygonOffsetModeCB(const av::SFInt::SetValueEvent& event)
{
  mOsgStateSet->setMode(GL_POLYGON_OFFSET_FILL, event.getValue());
}

/* virtual */ void
av::osg::StateSet::getRenderingHintCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgStateSet->getRenderingHint();
}

/* virtual */ void
av::osg::StateSet::setRenderingHintCB(const av::SFInt::SetValueEvent& event)
{
  mOsgStateSet->setRenderingHint(event.getValue());
}

/* virtual */ void
av::osg::StateSet::getRenderBinCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgStateSet->getBinNumber();
}

/* virtual */ void
av::osg::StateSet::setRenderBinCB(const av::SFInt::SetValueEvent& event)
{
  mOsgStateSet->setBinNumber(event.getValue());
}

/* virtual */ void
av::osg::StateSet::getCullFaceCB(const av::osg::SFCullFace::GetValueEvent& event)
{
  *(event.getValuePtr()) = av::osg::get_from_osg_object<av::osg::CullFace>(mOsgStateSet->getAttribute(::osg::CullFace::CULLFACE));
}

/* virtual */ void
av::osg::StateSet::setCullFaceCB(const av::osg::SFCullFace::SetValueEvent& event)
{
  ::osg::CullFace *osg_cull_face =
    (event.getValue().isValid() ? event.getValue()->getOsgCullFace() : 0);
  if (osg_cull_face != 0)
    mOsgStateSet->setAttribute(osg_cull_face);
  else
    mOsgStateSet->removeAttribute(::osg::StateAttribute::CULLFACE);
}

/* virtual */ void
av::osg::StateSet::getCullFaceModeCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgStateSet->getMode(GL_CULL_FACE);
}

/* virtual */ void
av::osg::StateSet::setCullFaceModeCB(const av::SFInt::SetValueEvent& event)
{
  mOsgStateSet->setMode(GL_CULL_FACE, event.getValue());
}

/* virtual */ void
av::osg::StateSet::getRescaleNormalModeCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgStateSet->getMode(GL_RESCALE_NORMAL);
}

/* virtual */ void
av::osg::StateSet::setRescaleNormalModeCB(const av::SFInt::SetValueEvent& event)
{
  mOsgStateSet->setMode(GL_RESCALE_NORMAL, event.getValue());
}

/* virtual */ void
av::osg::StateSet::getNormalizeModeCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgStateSet->getMode(GL_NORMALIZE);
}

/* virtual */ void
av::osg::StateSet::setNormalizeModeCB(const av::SFInt::SetValueEvent& event)
{
  mOsgStateSet->setMode(GL_NORMALIZE, event.getValue());
}

/* virtual */ void
av::osg::StateSet::getTextureCB(const av::osg::SFTexture::GetValueEvent& event)
{
  *(event.getValuePtr()) = av::osg::get_from_osg_object<av::osg::Texture>(mOsgStateSet->getTextureAttribute(0, ::osg::StateAttribute::TEXTURE));
}

/* virtual */ void
av::osg::StateSet::setTextureCB(const av::osg::SFTexture::SetValueEvent& event)
{
  ::osg::Texture *osg_tex = (event.getValue().isValid() ? event.getValue()->getOsgTexture() : 0);
  if (osg_tex != 0)
    mOsgStateSet->setTextureAttributeAndModes(0, osg_tex);
  else
  {
    ::osg::Texture *osg_old_tex = dynamic_cast< ::osg::Texture*>(mOsgStateSet->getTextureAttribute(0, ::osg::StateAttribute::TEXTURE));
    if (!osg_old_tex)
      return;
    mOsgStateSet->removeTextureAttribute(0, osg_old_tex->getType());
  }

}

/* virtual */ void
av::osg::StateSet::getDepthTestModeCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgStateSet->getMode(GL_DEPTH_TEST);
}

/* virtual */ void
av::osg::StateSet::setDepthTestModeCB(const av::SFInt::SetValueEvent& event)
{
  mOsgStateSet->setMode(GL_DEPTH_TEST, event.getValue());
}

/* virtual */ void
av::osg::StateSet::getProgramCB(const av::osg::SFProgram::GetValueEvent& event)
{
  *(event.getValuePtr()) = av::osg::get_from_osg_object<av::osg::Program>(mOsgStateSet->getAttribute(::osg::StateAttribute::PROGRAM));
}

/* virtual */ void
av::osg::StateSet::setProgramCB(const av::osg::SFProgram::SetValueEvent& event)
{
  ::osg::Program *osg_program = (event.getValue().isValid() ? event.getValue()->getOsgProgram() : 0);
  if (osg_program != 0)
    //mOsgStateSet->setAttribute(osg_program);
    mOsgStateSet->setAttributeAndModes(osg_program,::osg::StateAttribute::ON);
  else
    mOsgStateSet->removeAttribute(::osg::StateAttribute::PROGRAM);
}

/* virtual */ void
av::osg::StateSet::getUniformsCB(const av::osg::MFUniform::GetValueEvent& event)
{

  av::osg::MFUniform::ContainerType& container(*event.getValuePtr());
  container.clear();
  typedef ::osg::StateSet::UniformList::iterator itType;
  ::osg::StateSet::UniformList& m = mOsgStateSet->getUniformList();
  for (itType i = m.begin(); i!=m.end();++i)
  {
    container.push_back(av::osg::get_from_osg_object<av::osg::Uniform>(mOsgStateSet->getUniform(i->first)));
  }
}

/* virtual */ void
av::osg::StateSet::setUniformsCB(const av::osg::MFUniform::SetValueEvent& event)
{
  const av::osg::MFUniform::ContainerType& container(event.getValue());
  for (av::osg::MFUniform::ContainerType::const_iterator i = container.begin(); i != container.end(); ++i)
  {
    mOsgStateSet->addUniform((*i)->getOsgUniform());
  }
}

