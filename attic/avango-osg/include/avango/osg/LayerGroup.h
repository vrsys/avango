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

#if !defined(AVANGO_OSG_LAYERGROUP_H)
#define AVANGO_OSG_LAYERGROUP_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Group>
#include <avango/osg/Group.h>
#include <avango/osg/Drawable.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::Group osgGroup;

    /**
     * Wrapper for ::osg::Group with additional layering feature
     *
     * WARNING
     * - This Node is not fully implemented and has substantial bugs, so in the most cases
     *   it won't be of any use and the visual outcome is unpredictable
     * - Read LayerGeode.h for more information
     *
     * ISSUES
     * - The DrawCallbacks are not updated if the subgraph changes. You have to manually
     *   touch the Children field to do so.
     * - The inheritance of StateSets does not work
     * - Transformations are inherited from neighbours (should only from parents)
     * - LayerMode field has no effect
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL LayerGroup : public Group
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::osg::Group is created.
       * Otherwise, the given ::osg::Group is used.
       */
      LayerGroup(osgGroup* osggroup = new osgGroup());

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

      ::osg::Group *mOsgGroup;
      void updateDrawCallbacks(::osg::Node* osg_node, ::osg::Drawable*& firstdrawable);
      bool mLayerModeChanged;
      bool mChildrenChanged;
      ::osg::Drawable* mFirstDrawable;

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~LayerGroup();
      virtual void setChildrenCB(const av::osg::MFNode::SetValueEvent& event);

      struct DrawCallbackBaseLayer : public ::osg::Drawable::DrawCallback
      {
      public :

        DrawCallbackBaseLayer(::osg::Node* osg_node, int mode, ::osg::Drawable* firstdrawable);
        virtual void drawImplementation(::osg::RenderInfo& renderInfo, const ::osg::Drawable* drawable) const;
        bool drawWithStencil(::osg::RenderInfo& renderInfo, ::osg::Node* osg_node, bool firstdrawablefound) const;

      private :

        ::osg::Node* mNode;
        int mMode;
        ::osg::Drawable* mFirstDrawable;
      };

      struct DrawCallbackDisableDraw : public ::osg::Drawable::DrawCallback
      {
      public :

        DrawCallbackDisableDraw() {};
        virtual void drawImplementation(::osg::RenderInfo& renderInfo, const ::osg::Drawable* drawable) const {};
      };
    };

    typedef SingleField<Link<LayerGroup> > SFLayerGroup;
    typedef MultiField<Link<LayerGroup> > MFLayerGroup;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::LayerGroup> >;
  template class AV_OSG_DLL MultiField<Link<osg::LayerGroup> >;
#endif

}

#endif
