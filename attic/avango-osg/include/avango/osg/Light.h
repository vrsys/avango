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

#if !defined(AVANGO_OSG_LIGHT_H)
#define AVANGO_OSG_LIGHT_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Light>
#include <avango/osg/Fields.h>
#include <avango/osg/StateAttribute.h>


namespace av
{
  namespace osg
  {
    typedef ::osg::Light OsgLight;

    /**
     * Wrapper for ::osg::Light
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL Light : public StateAttribute
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::osg::Light is created.
       * Otherwise, the given ::osg::Light is used.
       */
      Light(OsgLight *osglight = new OsgLight()); // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Light();

    public:

      SFInt LightNum;
      SFVec4 Ambient;                           // r, g, b, w
      SFVec4 Diffuse;                           // r, g, b, w
      SFVec4 Specular;                          // r, g, b, w
      SFVec4 Position;                          // x, y, z, w
      SFVec3 Direction;                         // x, y, z
      SFFloat ConstantAttenuation;             // constant
      SFFloat LinearAttenuation;               // linear
      SFFloat QuadraticAttenuation;            // quadratic
      SFFloat SpotExponent;                    // exponent
      SFFloat SpotCutoff;                      // spread



      /**
       * Get the wrapped ::osg::Light.
       */
      ::osg::Light* getOsgLight() const;

      void touchFields();

    protected:

      virtual void getLightNumCB(const av::SFInt::GetValueEvent& event);
      virtual void setLightNumCB(const av::SFInt::SetValueEvent& event);

      virtual void getAmbientCB(const av::osg::SFVec4::GetValueEvent& event);
      virtual void setAmbientCB(const av::osg::SFVec4::SetValueEvent& event);

      virtual void getDiffuseCB(const av::osg::SFVec4::GetValueEvent& event);
      virtual void setDiffuseCB(const av::osg::SFVec4::SetValueEvent& event);

      virtual void getSpecularCB(const av::osg::SFVec4::GetValueEvent& event);
      virtual void setSpecularCB(const av::osg::SFVec4::SetValueEvent& event);

      virtual void getPositionCB(const av::osg::SFVec4::GetValueEvent& event);
      virtual void setPositionCB(const av::osg::SFVec4::SetValueEvent& event);

      virtual void getDirectionCB(const av::osg::SFVec3::GetValueEvent& event);
      virtual void setDirectionCB(const av::osg::SFVec3::SetValueEvent& event);

      virtual void getConstantAttenuationCB(const av::SFFloat::GetValueEvent& event);
      virtual void setConstantAttenuationCB(const av::SFFloat::SetValueEvent& event);

      virtual void getLinearAttenuationCB(const av::SFFloat::GetValueEvent& event);
      virtual void setLinearAttenuationCB(const av::SFFloat::SetValueEvent& event);

      virtual void getQuadraticAttenuationCB(const av::SFFloat::GetValueEvent& event);
      virtual void setQuadraticAttenuationCB(const av::SFFloat::SetValueEvent& event);

      virtual void getSpotExponentCB(const av::SFFloat::GetValueEvent& event);
      virtual void setSpotExponentCB(const av::SFFloat::SetValueEvent& event);

      virtual void getSpotCutoffCB(const av::SFFloat::GetValueEvent& event);
      virtual void setSpotCutoffCB(const av::SFFloat::SetValueEvent& event);





    private:

      ::osg::Light *mOsgLight;
    };

    typedef SingleField<Link<Light> > SFLight;
    typedef MultiField<Link<Light> > MFLight;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::Light> >;
  template class AV_OSG_DLL MultiField<Link<osg::Light> >;
#endif

}

#endif
