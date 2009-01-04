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

#ifndef AV_SOUND_SOUNDSOURCE_H
#define AV_SOUND_SOUNDSOURCE_H

#include <avango/osg/Group.h>
#include <avango/osg/Fields.h>
#include <avango/sound/SampleBuffer.h>
#include <queue>

/**
 * \file
 * \ingroup av_sound
 */

namespace av {
  namespace sound {

    class SoundRenderer;
    /**
    * This class represents a sound source in the scenegraph. Its position in world
    * coordinates depends on teh transformations of its parent nodes.
    * This class doesn't do the sound rendering itself, but holds a list of
    * LocalSources, which are implemented by the concrete SoundRenderers.
    */
    class SoundSource : public av::osg::Group {
      AV_FC_DECLARE();
      public:
        SoundSource();
        /* virtual */ ~SoundSource();

        /**
         * Location of a sound file to play. Supported at the moment are .wav and .ogg files.
         */
        SFString URL;
        /**
         * Should the sound be looped ( Ignored when in streaming mode )
         */
        SFBool   Loop;
        /**
         * The velocity of the source.
         */
        av::osg::SFVec3   Velocity;
        /**
         * The direction of the source.
         */
        av::osg::SFVec3   Direction;
        /**
         * The inner angle of the cone for a directional source (in deg)
         */
        SFFloat ConeInnerAngle;
        /**
         * The outer angle of the cone for a directional source (in deg)
         */
        SFFloat ConeOuterAngle;
        /**
         * Gain for the sound source ( in dB )
         */
        SFFloat  Gain;
        /**
         * Pitch value for the sound source
         */
        SFFloat  Pitch;
        /**
         * Start playing
         */
        SFBool   Play;
        /**
         * Pause playing ( can be resumed )
         */
        SFBool   Pause;
        /**
         * Stop playing ( restarts from the beginning )
         */
        SFBool   Stop;
        /**
         * Rewind sample to the start
         */
        SFBool   Rewind;
        /**
         * Is set to true when sound finishes playing
         */
        SFBool   FinishedPlaying;
        /**
         * The duration of the sample in seconds
         */
        SFDouble  PlayTime;
        /**
         * Input field for sample buffer. Can be used for streaming sound, for instance together
         * with the gstreamer-module
         */
        SFSampleBuffer NewSampleBuffer;
        /**
         * To switch between a static or dynamic sound source
         */
        SFBool Spatialize;
        /**
         * for resource management
         */
        SFFloat Priority;

        /* virtual */ void fieldHasChangedLocalSideEffect(const Field&);
        /* virtual */ void evaluateLocalSideEffect();

        /**
         * Base class for concrete implementation of sound sources. SoundRenderers provide
         * a class derived from this base class to implement their concrete sound rendering.
         */
        class LocalSource {
          public:
            /**
             * Constructor. Default constructor is disabled.
             * @param soundRenderer The sound renderer which created this LocalSource.
             */
            LocalSource(SoundRenderer* soundRenderer);
            virtual ~LocalSource();

            // data

            /**
             * Sets the position of this source in world coordinates.
             * @param transform The matrix transform of this source.
             */
            virtual void setWorldTransform(const ::osg::Matrixf& transform) = 0;
            /**
             * Should this source loop?
             * @param loop Enable/Disable looping
             */
            virtual void setLooping(bool loop) = 0;
            /**
             * Set location of sound file
             * @param url File localtion of sound file
             */
            virtual void setURL(const std::string& url) = 0;
            /**
             * Should sound source be spatial?
             * @param spatialize Enable/Disable spatial sound source
             */
            virtual void setSpatialize(bool spatialize) {}
            /**
             * Set velocity of sound source ( in m/s )
             * @param velocity The velocity vector for this source
             */
            virtual void setVelocity(const ::osg::Vec3f& velocity) {}
            /**
             * Set direction of sound source
             * @param direction The direction vector for this source
             */
            virtual void setDirection(const ::osg::Vec3f& direction) {}
            /**
             * Set inner angle of the cone for a directional sound source
             * @param angle The inner angle for the cone
             */
            virtual void setConeInnerAngle(float angle) {}
            /**
             * Set outer angle of the cone for a directional sound source
             * @param angle The outer angle for the cone
             */
            virtual void setConeOuterAngle(float angle) {}
            /**
             * Set gain for this source
             * @param gain The gain value ( in dB)
             */
            virtual void setGain(float gain) {}
            /**
             * Set pitch for this source
             * @param pitch The pitch value
             */
            virtual void setPitch(float pitch) {}
            /**
             * Set priority for this source
             * @param priority The priority value
             */
            virtual void setPriority(float priority) {}

            /**
             * Start playback
             */
            virtual void play() {}
            /**
             * Pause playback
             */
            virtual void pause() {}
            /**
             * Stop playback
             */
            virtual void stop() {}
            /**
             * Rewind source
             */
            virtual void rewind() {}
            /**
             * Is source done with playing?
             * @return true if done, false otherwise
             */
            virtual bool donePlaying() const { return false; }
            /**
             * Return play time of sample
             * @return Sample duration in seconds
             */
            virtual double getPlayTime() const { return 0.0; }

            // managing
            /**
             * Return stored pointer to  sound renderer
             * @return the sound renderer that created this source
             */
            const SoundRenderer* getRenderer();

            /**
             * Enqueue the sample buffer for playback. This can be used for audio streaming.
             * @param buffer The buffer to play
             */
            virtual void enqueueSampleBuffer(Link<SampleBuffer> buffer) {}

          private:
            // Hidden
            LocalSource();
            LocalSource(const LocalSource&);
            LocalSource& operator=(const LocalSource&);

            SoundRenderer* mSoundRenderer;
        };

        /**
         * Add LocalSource instance to this SoundSource. The source accepts ownership of
         * this LocalSource
         * @param localSource The LocalSource to add to this SoundSource
         */
        void         addLocalSource(boost::shared_ptr<LocalSource> localSource);
        /**
         * Remove LocalSource from this SoundSource.
         * @param localSource LocalSource to remove
         */
        void         removeLocalSource(boost::shared_ptr<LocalSource> localSource);
        /**
         * The number of LocalSource this source manages
         * @return The number of local sources
         */
        unsigned int numLocalSources() const;
        /**
         * Return a specific local source
         * @param index Index of the local source to return (0 <= index < numLocalSources())
         * @return LocalSource with index
         */
        boost::shared_ptr<LocalSource> getLocalSource(unsigned int index);
        /**
         * Initialize LocalSource instance with data from this soundsource
         * @param localSource The LocalSource to initialize
         */
        void         initializeLocalSource(boost::shared_ptr<SoundSource::LocalSource> localSource);

      private:
        typedef std::vector<boost::shared_ptr<LocalSource> > LocalSources;
        LocalSources                     mLocalSources;
        bool                            mPollPlaying;

        template <class MemFun>
        void for_each_local_source(MemFun f);

        template <class MemFun, class Arg>
        void for_each_local_source(MemFun f, Arg arg);
    };
  }
}

#endif /*AV_SOUND_SOUNDSOURCE_H*/
