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

#ifndef AV_SOUND_SAMPLEBUFFER_H
#define AV_SOUND_SAMPLEBUFFER_H

#include <avango/Object.h>
#include <boost/shared_array.hpp>
#include <inttypes.h>

/**
 * \file
 * \ingroup av_sound
 */

namespace av {
  namespace sound {

    class SampleBuffer : public av::Object {
      AV_FC_DECLARE();
      public:
        typedef int16_t SampleType;
        // static const unsigned int sampleRate = 48000;

        SFUInt NumSamples;
        SFBool IsStereo;

        SampleBuffer();
        SampleBuffer(boost::shared_array<SampleType> buffer, unsigned int numberOfSamples, bool stereo = false);
        /* virtual */ ~SampleBuffer();

        unsigned int getNumSamples() const;
        boost::shared_array<SampleType> getSamples() const;

        unsigned int getSampleRate() const;
        void setSampleRate( unsigned int sampleRate);

      private:

        void getNumSamplesCB(const av::SFUInt::GetValueEvent& event);
        void setNumSamplesCB(const av::SFUInt::SetValueEvent& );
        void getIsStereoCB(const av::SFBool::GetValueEvent& event);
        void setIsStereoCB(const av::SFBool::SetValueEvent& );


        boost::shared_array<SampleType> mBuffer;
        unsigned int mNumSamples;
        bool         mStereo;
        unsigned int mSampleRate;
    };
    typedef SingleField<Link<SampleBuffer> > SFSampleBuffer;
    typedef MultiField<Link<SampleBuffer> > MFSampleBuffer;
  }
}

#endif /*AV_SOUND_SAMPLEBUFFER_H*/
