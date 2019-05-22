// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#if !defined(AVANGO_OSG_CAPSULE_H)
#define AVANGO_OSG_CAPSULE_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/ShapeDrawable>
#include <osg/Geode>

#include <avango/StandardFields.h>
#include <avango/osg/MatrixTransform.h>

namespace av
{
namespace osg
{
/**
 * Node that creates an ::osg::Capsule as Child of a MatrixTransform
 *
 * \ingroup av_osg
 */
class AV_OSG_DLL Capsule : public MatrixTransform
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. Note that this is not a simple Wrapper for ::osg::Capsule
     */
    Capsule();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Capsule();

  public:
    /**
     * Radius and DetailRatio (Tesselation) of the Capsule
     */
    SFFloat Radius;
    SFFloat Height;
    SFVec4 Color;

    /* virtual */ void fieldHasChangedLocalSideEffect(const av::Field& field);
    /* virtual */ void evaluateLocalSideEffect();

    /**
     * Get the ::osg::Capsule object.
     * \return an osg::ref pointer to the Capsule object
     */
    ::osg::ref_ptr<::osg::Capsule> getOsgCapsule() const;

    /**
     * Get the ::osg::ShapeDrawable object.
     * \return an osg::ref pointer to the ShapeDrawable object
     */
    ::osg::ref_ptr<::osg::ShapeDrawable> getOsgShapeDrawable() const;

  private:
    ::osg::ref_ptr<::osg::ShapeDrawable> mShapeDrawable;
    ::osg::ref_ptr<::osg::Capsule> mCapsule;
    ::osg::ref_ptr<::osg::Geode> mGeode;

    bool mRadiusChanged;
    bool mHeightChanged;
    bool mColorChanged;
};

typedef SingleField<Link<Capsule>> SFCapsule;
typedef MultiField<Link<Capsule>> MFCapsule;
} // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_OSG_DLL SingleField<Link<osg::Capsule>>;
template class AV_OSG_DLL MultiField<Link<osg::Capsule>>;
#endif

} // namespace av

#endif
