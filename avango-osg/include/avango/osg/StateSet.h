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

#if !defined(AVANGO_OSG_STATESET_H)
#define AVANGO_OSG_STATESET_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/StateSet>

#include <avango/osg/Object.h>
#include <avango/osg/Fog.h>
#include <avango/osg/PolygonOffset.h>
#include <avango/osg/CullFace.h>
#include <avango/osg/Texture.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::StateSet OsgStateSet;

    /**
     * Wrapper for ::osg::StateSet
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL StateSet : public Object
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      StateSet(OsgStateSet* osgstateset = new OsgStateSet()); // use defined type to circumvent compiler bug in VS8

      SFFog Fog;
      SFInt FogMode;
      SFInt LightingMode;
      SFInt BlendMode;
      SFPolygonOffset PolygonOffset;
      SFInt PolygonOffsetMode;
      SFInt WireframeMode;
      SFInt RenderingHint;
      SFInt RenderBin;
      SFCullFace CullFace;
      SFInt CullFaceMode;
      SFInt RescaleNormalMode;
      SFInt NormalizeMode;

      SFTexture Texture0;

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~StateSet();

    public:
      /**
       * Get the wrapped ::osg::StateSet.
       */
      ::osg::StateSet* getOsgStateSet() const;

    protected:

      virtual void getFogCB(const av::osg::SFFog::GetValueEvent& event);
      virtual void setFogCB(const av::osg::SFFog::SetValueEvent& event);
      virtual void getFogModeCB(const av::SFInt::GetValueEvent& event);
      virtual void setFogModeCB(const av::SFInt::SetValueEvent& event);
      virtual void getLightingModeCB(const av::SFInt::GetValueEvent& event);
      virtual void setLightingModeCB(const av::SFInt::SetValueEvent& event);
      virtual void getBlendModeCB(const av::SFInt::GetValueEvent& event);
      virtual void setBlendModeCB(const av::SFInt::SetValueEvent& event);
      virtual void getWireframeModeCB(const av::SFInt::GetValueEvent& event);
      virtual void setWireframeModeCB(const av::SFInt::SetValueEvent& event);
      virtual void getPolygonOffsetCB(const av::osg::SFPolygonOffset::GetValueEvent& event);
      virtual void setPolygonOffsetCB(const av::osg::SFPolygonOffset::SetValueEvent& event);
      virtual void getPolygonOffsetModeCB(const av::SFInt::GetValueEvent& event);
      virtual void setPolygonOffsetModeCB(const av::SFInt::SetValueEvent& event);
      virtual void getRenderingHintCB(const av::SFInt::GetValueEvent& event);
      virtual void setRenderingHintCB(const av::SFInt::SetValueEvent& event);
      virtual void getRenderBinCB(const av::SFInt::GetValueEvent& event);
      virtual void setRenderBinCB(const av::SFInt::SetValueEvent& event);
      virtual void getCullFaceCB(const av::osg::SFCullFace::GetValueEvent& event);
      virtual void setCullFaceCB(const av::osg::SFCullFace::SetValueEvent& event);
      virtual void getCullFaceModeCB(const av::SFInt::GetValueEvent& event);
      virtual void setCullFaceModeCB(const av::SFInt::SetValueEvent& event);
      virtual void getRescaleNormalModeCB(const av::SFInt::GetValueEvent& event);
      virtual void setRescaleNormalModeCB(const av::SFInt::SetValueEvent& event);
      virtual void getNormalizeModeCB(const av::SFInt::GetValueEvent& event);
      virtual void setNormalizeModeCB(const av::SFInt::SetValueEvent& event);
      virtual void getTextureCB(const av::osg::SFTexture::GetValueEvent& event);
      virtual void setTextureCB(const av::osg::SFTexture::SetValueEvent& event);

    private:

      ::osg::StateSet *mOsgStateSet;
    };

    typedef SingleField<Link<StateSet> > SFStateSet;
    typedef MultiField<Link<StateSet> > MFStateSet;
  } // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::StateSet> >;
  template class AV_OSG_DLL MultiField<Link<osg::StateSet> >;
#endif

} // namespace av

#endif
