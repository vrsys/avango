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

#if !defined(AVANGO_OSG_PANEL_H)
#define AVANGO_OSG_PANEL_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/PrimitiveSet>
#include <avango/osg/Fields.h>
#include <avango/StandardFields.h>
#include <avango/osg/Geometry.h>

namespace av
{
  namespace osg
  {
    /**
     * Node that creates a Panel geometry as Drawable
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL Panel : public Geometry
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      Panel();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Panel();

    public:

      SFFloat Width;
      SFFloat Height;
      SFFloat EdgeRadius;
      SFInt EdgeSmooth;
      SFVec4 PanelColor;
      SFVec4 BorderColor;
      SFFloat BorderWidth;
      SFVec3 Position;
      //Indicates the visibility of the panel interior
      SFBool ShowPanel;
      //Indicates the visibility of the panel border
      SFBool ShowBorder;


      /* virtual */ void fieldHasChangedLocalSideEffect(const av::Field& field);
      /* virtual */ void evaluateLocalSideEffect();

    private:

      bool mGeometryChanged;
      bool mColorChanged;
      bool mVisibilityChanged;

      void updateGeometry();
      void updateColor();
      void updatePanelVisibility();

    };

    typedef SingleField<Link<Panel> > SFPanel;
    typedef MultiField<Link<Panel> > MFPanel;
  } // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::Panel> >;
  template class AV_OSG_DLL MultiField<Link<osg::Panel> >;
#endif

} // namespace av

#endif
