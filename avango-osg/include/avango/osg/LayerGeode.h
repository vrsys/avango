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

#if !defined(AVANGO_OSG_LAYERGEODE_H)
#define AVANGO_OSG_LAYERGEODE_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Geode>
#include <avango/osg/Geode.h>
#include <avango/osg/Drawable.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::Geode osgGeode;

    /**
     * Wrapper for ::osg::Geode with additional layering feature
     *
     * A LayerGeode is a group node for Drawables. With LayerMode=OFF, the Geode behaves mostly like an osg::Geode.
     * With LayerMode=STENCIL (default) or NO_DEPTH_TEST, the DrawCallbacks of all drawables are replaced by an empty
     * Callback, that simply skips drawing of the appropriate Drawable. Only the DrawCallback of the first Drawable
     * in list is hijacked to achieve the layering effect.
     *
     * How it works:
     * When the first Drawable's DrawCallback is called with STENCIL mode on, a stencil buffer is set up and the
     * Drawables drawImplementation is called, with depth test and stencil writing enabled. After that, all
     * subsequent Drawables are painted with depth test disabled, but stencil test enabled. This technique
     * eliminates z-fighting and preserves correct depth values and transparency. For transparency effects,
     * this node must be placed in a depth sorted render bin. With NO_DEPTH_TEST mode on, the stencil test is
     * skipped and all Drawables are painted without depth test.
     *
     * Known issues:
     * - In some cases, StateSets are not inherited correctly
     * - The Drawables DrawCallbacks are replaced, so previously assigned DrawCallbacks will not work.
     * - The first Drawables useDisplayList property will be changed to false.
     * - As DisplayList and DrawCallback settings are changed, it is not possible to share Drawables
     *   between multiple LayerGeode or Geode nodes.
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL LayerGeode : public Geode
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::osg::Geode is created.
       * Otherwise, the given ::osg::Geode is used.
       */
      LayerGeode(osgGeode* osggeode = new osgGeode());

      enum LayerModes
      {
        OFF = 0,
        STENCIL,
        NO_DEPTH_TEST
      };

      SFInt LayerMode;

      /* virtual */ void fieldHasChangedLocalSideEffect(const av::Field& field);
      /* virtual */ void evaluateLocalSideEffect();

    private:

      bool mLayerModeChanged;
      bool mDrawablesChanged;
      void updateDrawCallbacks();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~LayerGeode();

      virtual void setDrawablesCB(const av::osg::MFDrawable::SetValueEvent& event);

      struct DrawCallbackBaseLayer : public ::osg::Drawable::DrawCallback
      {
      public :

        DrawCallbackBaseLayer(av::osg::LayerGeode* geode, int mode);
        virtual void drawImplementation(::osg::RenderInfo& renderInfo, const ::osg::Drawable* drawable) const;
        void drawWithStencil(::osg::RenderInfo& renderInfo) const;
        void drawWithNoDepthTest(::osg::RenderInfo& renderInfo) const;

      private :

        ::osg::Geode* mGeode;
        int mMode;
      };

      struct DrawCallbackDisableDraw : public ::osg::Drawable::DrawCallback
      {
      public :

        DrawCallbackDisableDraw() {};
        virtual void drawImplementation(::osg::RenderInfo& renderInfo, const ::osg::Drawable* drawable) const {};
      };
    };

    typedef SingleField<Link<LayerGeode> > SFLayerGeode;
    typedef MultiField<Link<LayerGeode> > MFLayerGeode;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::LayerGeode> >;
  template class AV_OSG_DLL MultiField<Link<osg::LayerGeode> >;
#endif

}

#endif
