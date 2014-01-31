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

#include <avango/osg/Text.h>
#include <avango/Logger.h>
#include <boost/bind.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::Text"));
}

AV_FC_DEFINE(av::osg::Text);

AV_FIELD_DEFINE(av::osg::SFText);
AV_FIELD_DEFINE(av::osg::MFText);

av::osg::Text::Text(::osgText::Text* osgtext) :
  Drawable(osgtext),
  mBackdropOffset(0.07f),
  mOsgText(osgtext)
{
  AV_FC_ADD_ADAPTOR_FIELD(String,
                          boost::bind(&Text::getStringCB, this, _1),
                          boost::bind(&Text::setStringCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Fontname,
                          boost::bind(&Text::getFontnameCB, this, _1),
                          boost::bind(&Text::setFontnameCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Resolution,
                          boost::bind(&Text::getResolutionCB, this, _1),
                          boost::bind(&Text::setResolutionCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Size,
                          boost::bind(&Text::getSizeCB, this, _1),
                          boost::bind(&Text::setSizeCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Color,
                          boost::bind(&Text::getColorCB, this, _1),
                          boost::bind(&Text::setColorCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Position,
                          boost::bind(&Text::getPositionCB, this, _1),
                          boost::bind(&Text::setPositionCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(LineSpacing,
                          boost::bind(&Text::getLineSpacingCB, this, _1),
                          boost::bind(&Text::setLineSpacingCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Alignment,
                          boost::bind(&Text::getAlignmentCB, this, _1),
                          boost::bind(&Text::setAlignmentCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(AxisAlignment,
                          boost::bind(&Text::getAxisAlignmentCB, this, _1),
                          boost::bind(&Text::setAxisAlignmentCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(BackdropType,
                          boost::bind(&Text::getBackdropTypeCB, this, _1),
                          boost::bind(&Text::setBackdropTypeCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(BackdropOffset,
                          boost::bind(&Text::getBackdropOffsetCB, this, _1),
                          boost::bind(&Text::setBackdropOffsetCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(BackdropImplementation,
                          boost::bind(&Text::getBackdropImplementationCB, this, _1),
                          boost::bind(&Text::setBackdropImplementationCB, this, _1));

  mOsgText->setAlignment(osgText::Text::LEFT_TOP);
  mOsgText->setAxisAlignment(osgText::Text::XY_PLANE);
}

/* virtual */
av::osg::Text::~Text()
{}

/* static */ void
av::osg::Text::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Drawable::initClass();

    AV_FC_INIT(av::osg::Drawable, av::osg::Text, true);

    SFText::initClass("av::osg::SFText", "av::Field");
    MFText::initClass("av::osg::MFText", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

::osgText::Text*
av::osg::Text::getOsgText() const
{
  return mOsgText;
}

/* virtual */ void
av::osg::Text::getStringCB(const av::SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgText->getText().createUTF8EncodedString();
}

/* virtual */ void
av::osg::Text::setStringCB(const av::SFString::SetValueEvent& event)
{
  mOsgText->setText(event.getValue());
}

/* virtual */ void
av::osg::Text::getFontnameCB(const av::SFString::GetValueEvent& event)
{
  const ::osgText::Font* font = mOsgText->getFont();
  if (font == 0)
  {
    *(event.getValuePtr()) = "";
  }
  else
  {
    *(event.getValuePtr()) = font->getFileName();
  }
}

/* virtual */ void
av::osg::Text::setFontnameCB(const av::SFString::SetValueEvent& event)
{
  mOsgText->setFont(event.getValue());
}

/* virtual */ void
av::osg::Text::getResolutionCB(const av::SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgText->getFontWidth();
}

/* virtual */ void
av::osg::Text::setResolutionCB(const av::SFUInt::SetValueEvent& event)
{
  mOsgText->setFontResolution(event.getValue(), event.getValue());
}

/* virtual */ void
av::osg::Text::getSizeCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgText->getCharacterHeight();
}

/* virtual */ void
av::osg::Text::setSizeCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgText->setCharacterSize(event.getValue());
}

/* virtual */ void
av::osg::Text::getColorCB(const av::osg::SFVec4::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgText->getColor();
}

/* virtual */ void
av::osg::Text::setColorCB(const av::osg::SFVec4::SetValueEvent& event)
{
  mOsgText->setColor(event.getValue());
}

/* virtual */ void
av::osg::Text::getPositionCB(const av::osg::SFVec3::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgText->getPosition();
}

/* virtual */ void
av::osg::Text::setPositionCB(const av::osg::SFVec3::SetValueEvent& event)
{
  mOsgText->setPosition(event.getValue());
}

/* virtual */ void
av::osg::Text::getLineSpacingCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgText->getLineSpacing();
}

/* virtual */ void
av::osg::Text::setLineSpacingCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgText->setLineSpacing(event.getValue());
}

/* virtual */ void
av::osg::Text::getAlignmentCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgText->getAlignment();
}

/* virtual */ void
av::osg::Text::setAlignmentCB(const av::SFInt::SetValueEvent& event)
{
  mOsgText->setAlignment((::osgText::Text::AlignmentType)(event.getValue()));
}

/* virtual */ void
av::osg::Text::getAxisAlignmentCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgText->getAxisAlignment();
}

/* virtual */ void
av::osg::Text::setAxisAlignmentCB(const av::SFInt::SetValueEvent& event)
{
  mOsgText->setAxisAlignment((::osgText::Text::AxisAlignment)(event.getValue()));
}

/* virtual */ void
av::osg::Text::getBackdropTypeCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgText->getBackdropType();
}

/* virtual */ void
av::osg::Text::setBackdropTypeCB(const av::SFInt::SetValueEvent& event)
{
  mOsgText->setBackdropType((::osgText::Text::BackdropType)(event.getValue()));
}

/* virtual */ void
av::osg::Text::getBackdropOffsetCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mBackdropOffset;
}

/* virtual */ void
av::osg::Text::setBackdropOffsetCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgText->setBackdropOffset(event.getValue());
  mBackdropOffset = event.getValue();
}

/* virtual */ void
av::osg::Text::getBackdropImplementationCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgText->getBackdropImplementation();
}

/* virtual */ void
av::osg::Text::setBackdropImplementationCB(const av::SFInt::SetValueEvent& event)
{
  mOsgText->setBackdropImplementation((::osgText::Text::BackdropImplementation)(event.getValue()));
}
