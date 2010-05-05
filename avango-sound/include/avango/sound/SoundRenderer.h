// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#ifndef AV_SOUND_SOUNDRENDERER_H
#define AV_SOUND_SOUNDRENDERER_H

#include <avango/FieldContainer.h>
#include <avango/sound/SoundSource.h>
#include <avango/osg/Fields.h>
#include <boost/shared_ptr.hpp>

/**
 * \file
 * \ingroup av_sound
 */

namespace av {
  namespace sound {

    class SoundRenderer : public FieldContainer {

      AV_FC_DECLARE_ABSTRACT();

      public:
        SoundRenderer();
      protected:
        /* virtual */~SoundRenderer();
      public:
        av::osg::SFMatrix ListenerPosition;
        av::osg::SFVec3 ListenerVelocity;

        virtual boost::shared_ptr<SoundSource::LocalSource> createLocalSource();
        virtual void updateSoundSourcePosition(Link<SoundSource> source, const ::osg::Matrix& transform);
        virtual void resetRenderer() {}

      protected:
        virtual void setListenerPosition(const ::osg::Matrix& /* position */) {}
        virtual ::osg::Matrix getListenerPosition() const {return ::osg::Matrix::identity();}

        virtual void setListenerVelocity(const ::osg::Vec3& /* velocity */) {}
        virtual ::osg::Vec3 getListenerVelocity() const {return ::osg::Vec3f(0.0f, 0.0f, 0.0f);}
      private:
        void getListenerPosCB(const av::osg::SFMatrix::GetValueEvent& event);
        void setListenerPosCB(const av::osg::SFMatrix::SetValueEvent& event);
        
        void getListenerVeloCB(const av::osg::SFVec3::GetValueEvent& event);
        void setListenerVeloCB(const av::osg::SFVec3::SetValueEvent& event);

    };
    typedef SingleField<Link<SoundRenderer> > SFSoundRenderer;
    typedef MultiField<Link<SoundRenderer> > MFSoundRenderer;
  }
}
#endif /*AV_SOUND_SOUNDRENDERER_H*/
