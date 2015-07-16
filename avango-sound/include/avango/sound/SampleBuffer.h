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

#ifndef AV_SOUND_SAMPLEBUFFER_H
#define AV_SOUND_SAMPLEBUFFER_H

#include <avango/FieldContainer.h>
#include <boost/shared_array.hpp>
#include <inttypes.h>

/**
 * \file
 * \ingroup av_sound
 */

namespace av {
  namespace sound {

    class SampleBuffer : public av::FieldContainer {
      AV_FC_DECLARE();
      public:
        using SampleType = int16_t;
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
    using SFSampleBuffer = SingleField<Link<SampleBuffer> >;
    using MFSampleBuffer = MultiField<Link<SampleBuffer> >;
  }
}

#endif /*AV_SOUND_SAMPLEBUFFER_H*/
