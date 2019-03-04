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

#if !defined(AVANGO_OSG_BLEND_FUNC_H)
#define AVANGO_OSG_BLEND_FUNC_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/BlendFunc>
#include <avango/osg/Fields.h>
#include <avango/osg/StateAttribute.h>

namespace av
{
namespace osg
{
typedef ::osg::BlendFunc OsgBlendFunc;

/**
 * Wrapper for ::osg::BlendFunc
 *
 * \ingroup av_osg
 */
class AV_OSG_DLL BlendFunc : public StateAttribute
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::osg::BlendFunc is created.
     * Otherwise, the given ::osg::BlendFunc is used.
     */
    BlendFunc(OsgBlendFunc* osgblendfunc = new OsgBlendFunc());
    // use defined type to circumvent compiler bug in VS8

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~BlendFunc();

  public:
    SFInt SourceRGB;
    SFInt SourceAlpha;
    SFInt DestinationRGB;
    SFInt DestinationAlpha;

    /**
     * Get the wrapped ::osg::BlendFunc.
     */
    ::osg::BlendFunc* getOsgBlendFunc() const;

  protected:
    virtual void getSourceRGBCB(const av::SFInt::GetValueEvent& event);
    virtual void setSourceRGBCB(const av::SFInt::SetValueEvent& event);
    virtual void getSourceAlphaCB(const av::SFInt::GetValueEvent& event);
    virtual void setSourceAlphaCB(const av::SFInt::SetValueEvent& event);
    virtual void getDestinationRGBCB(const av::SFInt::GetValueEvent& event);
    virtual void setDestinationRGBCB(const av::SFInt::SetValueEvent& event);
    virtual void getDestinationAlphaCB(const av::SFInt::GetValueEvent& event);
    virtual void setDestinationAlphaCB(const av::SFInt::SetValueEvent& event);

  private:
    ::osg::BlendFunc* mOsgBlendFunc;
};

typedef SingleField<Link<BlendFunc>> SFBlendFunc;
typedef MultiField<Link<BlendFunc>> MFBlendFunc;
} // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_OSG_DLL SingleField<Link<osg::BlendFunc>>;
template class AV_OSG_DLL MultiField<Link<osg::BlendFunc>>;
#endif

} // namespace av

#endif
