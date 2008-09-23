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

#if !defined(AVANGO_OSG_SPHERE_H)
#define AVANGO_OSG_SPHERE_H

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
     * Node that creates an ::osg::Sphere as Child of a MatrixTransform
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL Sphere : public MatrixTransform
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. Note that this is not a simple Wrapper for ::osg::Sphere
       */
      Sphere();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Sphere();

    public:

      /**
       * Radius and DetailRatio (Tesselation) of the Sphere
       */
      SFFloat Radius;
      SFFloat DetailRatio;
      SFVec4 Color;

      /* virtual */ void fieldHasChangedLocalSideEffect(const av::Field& field);
      /* virtual */ void evaluateLocalSideEffect();

      /**
       * Get the ::osg::Sphere object.
       * \return an osg::ref pointer to the Sphere object
       */
      ::osg::ref_ptr< ::osg::Sphere> getOsgSphere() const;

      /**
       * Get the ::osg::ShapeDrawable object.
       * \return an osg::ref pointer to the ShapeDrawable object
       */
      ::osg::ref_ptr< ::osg::ShapeDrawable> getOsgShapeDrawable() const;

    private:

      ::osg::ref_ptr< ::osg::ShapeDrawable> mShapeDrawable;
      ::osg::ref_ptr< ::osg::TessellationHints> mHints;
      ::osg::ref_ptr< ::osg::Sphere> mSphere;
      ::osg::ref_ptr< ::osg::Geode> mGeode;

      bool mRadiusChanged;
      bool mDetailRatioChanged;
      bool mColorChanged;

    };

    typedef SingleField<Link<Sphere> > SFSphere;
    typedef MultiField<Link<Sphere> > MFSphere;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::Sphere> >;
  template class AV_OSG_DLL MultiField<Link<osg::Sphere> >;
#endif

}

#endif
