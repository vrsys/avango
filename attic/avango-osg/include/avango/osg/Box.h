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

#if !defined(AVANGO_OSG_BOX_H)
#define AVANGO_OSG_BOX_H

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
 * Node that creates an ::osg::Box as Child of a MatrixTransform
 *
 * \ingroup av_osg
 */
class AV_OSG_DLL Box : public MatrixTransform
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. Note that this is not a simple Wrapper for ::osg::Box
     */
    Box();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Box();

  public:
    /**
     * Radius and DetailRatio (Tesselation) of the Box
     */
    SFFloat Radius;
    SFFloat Width;
    SFFloat Height;
    SFFloat Length;

    SFVec4 Color;

    /* virtual */ void fieldHasChangedLocalSideEffect(const av::Field& field);
    /* virtual */ void evaluateLocalSideEffect();

    /**
     * Get the ::osg::Box object.
     * \return an osg::ref pointer to the Box object
     */
    ::osg::ref_ptr<::osg::Box> getOsgBox() const;

    /**
     * Get the ::osg::ShapeDrawable object.
     * \return an osg::ref pointer to the ShapeDrawable object
     */
    ::osg::ref_ptr<::osg::ShapeDrawable> getOsgShapeDrawable() const;

  private:
    ::osg::ref_ptr<::osg::ShapeDrawable> mShapeDrawable;
    //::osg::ref_ptr< ::osg::TessellationHints> mHints;
    ::osg::ref_ptr<::osg::Box> mBox;
    ::osg::ref_ptr<::osg::Geode> mGeode;

    bool mDimChanged;
    // bool mDetailRatioChanged;
    bool mColorChanged;
};

typedef SingleField<Link<Box>> SFBox;
typedef MultiField<Link<Box>> MFBox;
} // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_OSG_DLL SingleField<Link<osg::Box>>;
template class AV_OSG_DLL MultiField<Link<osg::Box>>;
#endif

} // namespace av

#endif
