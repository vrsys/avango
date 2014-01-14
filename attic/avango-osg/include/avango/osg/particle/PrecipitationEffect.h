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

#if !defined(AVANGO_OSG_PARTICLE_PRECIPITATIONEFFECT_H)
#define AVANGO_OSG_PARTICLE_PRECIPITATIONEFFECT_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osgParticle/PrecipitationEffect>
#include <osg/Vec3>
#include <avango/osg/Fields.h>
#include <avango/osg/Node.h>
#include <avango/osg/Fog.h>
#include <avango/osg/particle/windows_specific_osg_particle.h>


namespace av
{
  namespace osg
  {
    namespace particle
    {
      /**
       * Wrapper for ::osgParticle::PrecipitationEffect
       *
       * \ingroup av_osg
       */
      class AV_OSG_PARTICLE_DLL PrecipitationEffect : public Node
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::osg::PrecipitationEffect is created.
         * Otherwise, the given ::osg::PrecipitationEffect is used.
         */
        PrecipitationEffect(::osgParticle::PrecipitationEffect* osgprecipitationeffect =
                              new ::osgParticle::PrecipitationEffect());

      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
        virtual ~PrecipitationEffect();

      public:

        SFFloat Rain;
        SFFloat Snow;
        SFVec3 Wind;
        SFVec3 Position;
        SFVec3 CellSize;
        SFFloat ParticleSpeed;
        SFFloat ParticleSize;
        SFVec4 ParticleColor;
        SFFloat NearTransition;
        SFFloat FarTransition;
        SFFloat MaximumParticleDensity;
        SFBool UseFarLineSegments;
        SFFog Fog;

        /**
         * Get the wrapped ::osg::PrecipitationEffect.
         */
        ::osgParticle::PrecipitationEffect* getOsgPrecipitationEffect() const;

      protected:

        virtual void getRainCB(const av::SFFloat::GetValueEvent& event);
        virtual void setRainCB(const av::SFFloat::SetValueEvent& event);
        virtual void getSnowCB(const av::SFFloat::GetValueEvent& event);
        virtual void setSnowCB(const av::SFFloat::SetValueEvent& event);
        virtual void getWindCB(const av::osg::SFVec3::GetValueEvent& event);
        virtual void setWindCB(const av::osg::SFVec3::SetValueEvent& event);
        virtual void getPositionCB(const av::osg::SFVec3::GetValueEvent& event);
        virtual void setPositionCB(const av::osg::SFVec3::SetValueEvent& event);
        virtual void getCellSizeCB(const av::osg::SFVec3::GetValueEvent& event);
        virtual void setCellSizeCB(const av::osg::SFVec3::SetValueEvent& event);
        virtual void getParticleSpeedCB(const av::SFFloat::GetValueEvent& event);
        virtual void setParticleSpeedCB(const av::SFFloat::SetValueEvent& event);
        virtual void getParticleSizeCB(const av::SFFloat::GetValueEvent& event);
        virtual void setParticleSizeCB(const av::SFFloat::SetValueEvent& event);
        virtual void getParticleColorCB(const av::osg::SFVec4::GetValueEvent& event);
        virtual void setParticleColorCB(const av::osg::SFVec4::SetValueEvent& event);
        virtual void getNearTransitionCB(const av::SFFloat::GetValueEvent& event);
        virtual void setNearTransitionCB(const av::SFFloat::SetValueEvent& event);
        virtual void getFarTransitionCB(const av::SFFloat::GetValueEvent& event);
        virtual void setFarTransitionCB(const av::SFFloat::SetValueEvent& event);
        virtual void getMaximumParticleDensityCB(const av::SFFloat::GetValueEvent& event);
        virtual void setMaximumParticleDensityCB(const av::SFFloat::SetValueEvent& event);
        virtual void getUseFarLineSegmentsCB(const av::SFBool::GetValueEvent& event);
        virtual void setUseFarLineSegmentsCB(const av::SFBool::SetValueEvent& event);
        virtual void getFogCB(const av::osg::SFFog::GetValueEvent& event);
        virtual void setFogCB(const av::osg::SFFog::SetValueEvent& event);

      private:

        ::osgParticle::PrecipitationEffect *mOsgPrecipitationEffect;

        float mRain;
        float mSnow;

        void touchFields();
      };

      typedef SingleField<Link<PrecipitationEffect> > SFPrecipitationEffect;
      typedef MultiField<Link<PrecipitationEffect> > MFPrecipitationEffect;
    }
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_PARTICLE_DLL SingleField<Link<osg::particle::PrecipitationEffect> >;
  template class AV_OSG_PARTICLE_DLL MultiField<Link<osg::particle::PrecipitationEffect> >;
#endif

}

#endif
