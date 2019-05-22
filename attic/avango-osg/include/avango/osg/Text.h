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

#if !defined(AVANGO_OSG_TEXT_H)
#define AVANGO_OSG_TEXT_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osgText/Text>
#include <avango/osg/Fields.h>
#include <avango/StandardFields.h>
#include <avango/osg/Drawable.h>

namespace av
{
namespace osg
{
/**
 * Node that creates an ::osgText::Text as Drawable of a Geode
 *
 * \ingroup av_osg
 */
class AV_OSG_DLL Text : public Drawable
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::osgText::Text is created.
     * Otherwise, the given ::osgText::Text is used.
     */
    Text(::osgText::Text* osgtext = new ::osgText::Text());

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Text();

  public:
    SFString String;
    SFString Fontname;
    SFUInt Resolution;
    SFFloat Size;
    SFVec4 Color;
    SFVec3 Position;
    SFFloat LineSpacing;
    SFInt Alignment;
    SFInt AxisAlignment;
    SFInt BackdropType;
    SFFloat BackdropOffset;
    SFInt BackdropImplementation;

    /**
     * Get the wrapped ::osgText::Text
     */
    ::osgText::Text* getOsgText() const;

  protected:
    virtual void getStringCB(const av::SFString::GetValueEvent& event);
    virtual void setStringCB(const av::SFString::SetValueEvent& event);
    virtual void getFontnameCB(const av::SFString::GetValueEvent& event);
    virtual void setFontnameCB(const av::SFString::SetValueEvent& event);
    virtual void getResolutionCB(const av::SFUInt::GetValueEvent& event);
    virtual void setResolutionCB(const av::SFUInt::SetValueEvent& event);
    virtual void getSizeCB(const av::SFFloat::GetValueEvent& event);
    virtual void setSizeCB(const av::SFFloat::SetValueEvent& event);
    virtual void getColorCB(const av::osg::SFVec4::GetValueEvent& event);
    virtual void setColorCB(const av::osg::SFVec4::SetValueEvent& event);
    virtual void getPositionCB(const av::osg::SFVec3::GetValueEvent& event);
    virtual void setPositionCB(const av::osg::SFVec3::SetValueEvent& event);
    virtual void getLineSpacingCB(const av::SFFloat::GetValueEvent& event);
    virtual void setLineSpacingCB(const av::SFFloat::SetValueEvent& event);
    virtual void getAlignmentCB(const av::SFInt::GetValueEvent& event);
    virtual void setAlignmentCB(const av::SFInt::SetValueEvent& event);
    virtual void getAxisAlignmentCB(const av::SFInt::GetValueEvent& event);
    virtual void setAxisAlignmentCB(const av::SFInt::SetValueEvent& event);
    virtual void getBackdropTypeCB(const av::SFInt::GetValueEvent& event);
    virtual void setBackdropTypeCB(const av::SFInt::SetValueEvent& event);
    virtual void getBackdropOffsetCB(const av::SFFloat::GetValueEvent& event);
    virtual void setBackdropOffsetCB(const av::SFFloat::SetValueEvent& event);
    virtual void getBackdropImplementationCB(const av::SFInt::GetValueEvent& event);
    virtual void setBackdropImplementationCB(const av::SFInt::SetValueEvent& event);

  private:
    float mBackdropOffset;
    ::osgText::Text* mOsgText;
};

typedef SingleField<Link<Text>> SFText;
typedef MultiField<Link<Text>> MFText;
} // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_OSG_DLL SingleField<Link<osg::Text>>;
template class AV_OSG_DLL MultiField<Link<osg::Text>>;
#endif

} // namespace av

#endif
