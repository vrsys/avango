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

#if !defined(AVANGO_OSG_BILLBOARD_H)
#define AVANGO_OSG_BILLBOARD_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Billboard>
#include <avango/osg/Fields.h>
#include <avango/osg/Drawable.h>
#include <avango/osg/Geode.h>

namespace av
{
namespace osg
{
typedef ::osg::Billboard OsgBillboard;

/**
 * Wrapper for ::osg::Billboard
 *
 * \ingroup av_osg
 */
class AV_OSG_DLL Billboard : public Geode
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::osg::Billboard is created.
     * Otherwise, the given ::osg::Billboard is used.
     */
    Billboard(OsgBillboard* osgbillboard = new OsgBillboard());
    // use defined type to circumvent compiler bug in VS8

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Billboard();

  public:
    /**
     * Billboard-modes:
     * 0 = POINT_ROT_EYE,
     * 1 = POINT_ROT_WORLD,
     * 2 = AXIAL_ROT
     */
    SFUInt Mode;

    /**
     * Axis to rotate around. Used with AXIAL_ROT
     */
    SFVec3 Axis;

    /**
     * Normal of geometry that should be aligned with viewing direction
     */
    SFVec3 Normal;

    /**
     * Get the wrapped ::osg::Geometry.
     */
    OsgBillboard* getOsgBillboard() const;

  protected:
    virtual void getModeCB(const SFUInt::GetValueEvent& event);
    virtual void setModeCB(const SFUInt::SetValueEvent& event);
    virtual void getAxisCB(const av::osg::SFVec3::GetValueEvent& event);
    virtual void setAxisCB(const av::osg::SFVec3::SetValueEvent& event);
    virtual void getNormalCB(const av::osg::SFVec3::GetValueEvent& event);
    virtual void setNormalCB(const av::osg::SFVec3::SetValueEvent& event);

  private:
    OsgBillboard* mOsgBillboard;
};

typedef SingleField<Link<Billboard>> SFBillboard;
typedef MultiField<Link<Billboard>> MFBillboard;
} // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_OSG_DLL SingleField<Link<osg::Billboard>>;
template class AV_OSG_DLL MultiField<Link<osg::Billboard>>;
#endif

} // namespace av

#endif
