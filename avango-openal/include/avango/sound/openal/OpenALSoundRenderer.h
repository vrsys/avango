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

#ifndef AV_SOUND_OPENALSOUNDRENDERER_H
#define AV_SOUND_OPENALSOUNDRENDERER_H

#include <avango/sound/SoundRenderer.h>
#include <avango/sound/SoundSource.h>
#include <AL/alc.h>
#include <AL/al.h>
#include <deque>
#include <gua/math/math.hpp>

/**
 * \namespace av::sound::openal
 * Avango Sound Library
 *
 * \ingroup av_sound
 */

/**
 * \file
 * \ingroup av_sound
 */

namespace av
{
  namespace sound
  {
    class SampleBuffer;

    namespace openal
    {
      class OpenALSoundRenderer : public SoundRenderer {
        AV_FC_DECLARE();
        public:
          OpenALSoundRenderer();
          /* virtual */~OpenALSoundRenderer();
          SFString Device;

          void openDevice(const std::string& deviceName);
          void closeDevice();
          bool deviceOpen() const;

          void createContext();
          void destroyContext();
          bool hasContext() const;

          // inherited from SoundRenderer
          /* virtual */ boost::shared_ptr<SoundSource::LocalSource> createLocalSource();
          // this node is not distributable !
          /* virtual */ void fieldHasChanged(const Field& field);

        protected:

          /* virtual */ void setListenerPosition(const ::gua::math::mat4& position);
          /* virtual */ ::gua::math::mat4 getListenerPosition() const;

          class OpenALLocalSource : public SoundSource::LocalSource {
            public:
              OpenALLocalSource(SoundRenderer* soundRenderer);
              ~OpenALLocalSource();

              /* virtual */ void setWorldTransform(const ::gua::math::mat4& worldMatrix);
              /* virtual */ void setLooping(bool loop );
              /* virtual */ void setURL(const std::string& soundURL );

              /* virtual */ void setVelocity( const ::gua::math::vec3& velocity);
              /* virtual */ void setGain(float gain);
              /* virtual */ void setPitch(float pitch);

              /* virtual */ void play();
              /* virtual */ void pause();
              /* virtual */ void stop();
              /* virtual */ void rewind();
              /* virtual */ bool donePlaying() const;

              virtual double getPlayTime() const { return mPlayTime; }
              /* virtual */ void enqueueSampleBuffer(Link<SampleBuffer> buffer);
            private:
              void removeStaticBufferFromSource();
              ALuint getNewStreamingBuffer();
              void freeUnusedStreamingBuffers();
              void flushBufferQueue();
              ALuint mSource;
              bool   mHaveSource;
              ALuint mBuffer;
              using BufferQueue = std::deque<ALuint>;
              BufferQueue mBufferQueue;
              bool   mHaveBuffer;
              double mPlayTime;
              bool       mIsPlaying;
          };

          ALCdevice* mDevice;
          std::string mDeviceName;
          ALCcontext* mContext;
          ::gua::math::mat4 mPosition;
          static unsigned int gAlutOpenCount;
      };
    }
  }
}

#endif /*AV_SOUND_OPENALSOUNDRENDERER_H*/
