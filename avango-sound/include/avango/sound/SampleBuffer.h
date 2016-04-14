#ifndef AV_SOUND_SAMPLEBUFFER_H
#define AV_SOUND_SAMPLEBUFFER_H

/**
* \file
* \ingroup av_sound
*/

#include <avango/sound/Platform.h>
#include <avango/FieldContainer.h>
#include <boost/shared_array.hpp>
#include <inttypes.h>

namespace av {
  namespace sound {

    class AV_SOUND_DLL SampleBuffer : public av::FieldContainer 
    {
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

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_SOUND_DLL SingleField<Link<sound::SampleBuffer> >;
  template class AV_SOUND_DLL MultiField<Link<sound::SampleBuffer> >;
#endif
}



#endif /*AV_SOUND_SAMPLEBUFFER_H*/
