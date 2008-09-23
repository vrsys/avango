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

#ifndef AV_SOUND_SOUNDRENDERER_H
#define AV_SOUND_SOUNDRENDERER_H

#include <avango/Object.h>
#include <avango/sound/SoundSource.h>
#include <avango/osg/Fields.h>
#include <boost/shared_ptr.hpp>

/**
 * \file
 * \ingroup av_sound
 */

namespace av {
  namespace sound {

    class SoundRenderer : public Object {

      AV_FC_DECLARE_ABSTRACT();

      public:
        SoundRenderer();
      protected:
        /* virtual */~SoundRenderer();
      public:
        av::osg::SFMatrix ListenerPosition;

        virtual boost::shared_ptr<SoundSource::LocalSource> createLocalSource();
        virtual void updateSoundSourcePosition(Link<SoundSource> source, const ::osg::Matrix& transform);
        virtual void resetRenderer() {}

      protected:
        virtual void setListenerPosition(const ::osg::Matrix& /* position */) {}
        virtual ::osg::Matrix getListenerPosition() const {return ::osg::Matrix::identity();}

      private:
        void getListenerPosCB(const av::osg::SFMatrix::GetValueEvent& event);
        void setListenerPosCB(const av::osg::SFMatrix::SetValueEvent& event);

    };
    typedef SingleField<Link<SoundRenderer> > SFSoundRenderer;
    typedef MultiField<Link<SoundRenderer> > MFSoundRenderer;
  }
}
#endif /*AV_SOUND_SOUNDRENDERER_H*/
