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

#include <avango/UnitTest++/UnitTest++.h>

#include <utility>

#include <avango/sound/SoundSource.h>
#include <avango/sound/SampleBuffer.h>
#include <boost/shared_array.hpp>
#include <inttypes.h>

#include <queue>
#include <string>

using namespace av;

namespace {

  class SampleBufferFixture {
    public:
      SampleBufferFixture() { sound::SampleBuffer::initClass(); sound::SoundSource::initClass(); }
  };

  /// TESTS
  TEST(TypeCanBeInitialized)
  {
    sound::SampleBuffer::initClass();
    CHECK(Type::getByName("av::sound::SampleBuffer") != Type::badType());
  }

  TEST_FIXTURE(SampleBufferFixture, CreateWithArray)
  {
    const unsigned int sample_size = 1000;
    boost::shared_array<int16_t> sample_buffer(new int16_t [sample_size]);
    {
      sound::SampleBuffer buffer_object(sample_buffer, sample_size);
      CHECK_EQUAL(2, sample_buffer.use_count());
    }
    CHECK_EQUAL(1, sample_buffer.use_count());
  }

  TEST_FIXTURE(SampleBufferFixture, CheckSize)
  {
    const unsigned int sample_size = 1000;
    boost::shared_array<int16_t> sample_buffer(new int16_t [sample_size]);
    sound::SampleBuffer buffer_object(sample_buffer, sample_size);
    CHECK_EQUAL(sample_size, buffer_object.getNumSamples());
  }

  TEST_FIXTURE(SampleBufferFixture, CheckNumSamplesField)
  {
    const unsigned int sample_size = 1000;
    boost::shared_array<int16_t> sample_buffer(new int16_t [sample_size]);
    sound::SampleBuffer buffer_object(sample_buffer, sample_size);
    CHECK_EQUAL(sample_size, buffer_object.NumSamples.getValue());
  }

  TEST_FIXTURE(SampleBufferFixture, SamplesFieldIsReadOnly)
  {
    const unsigned int sample_size = 1000;
    boost::shared_array<int16_t> sample_buffer(new int16_t [sample_size]);
    sound::SampleBuffer buffer_object(sample_buffer, sample_size);
    buffer_object.NumSamples.setValue(2 * sample_size);
    CHECK(2 * sample_size !=  buffer_object.NumSamples.getValue());
  }

  TEST_FIXTURE(SampleBufferFixture, CheckStereoField)
  {
    const unsigned int sample_size = 1000;
    boost::shared_array<int16_t> sample_buffer(new int16_t [sample_size]);
    sound::SampleBuffer buffer_object(sample_buffer, sample_size, true);
    CHECK_EQUAL(true, buffer_object.IsStereo.getValue());
  }

  TEST_FIXTURE(SampleBufferFixture, DefaultIsMono)
  {
    const unsigned int sample_size = 1000;
    boost::shared_array<int16_t> sample_buffer(new int16_t [sample_size]);
    sound::SampleBuffer buffer_object(sample_buffer, sample_size);
    CHECK_EQUAL(false, buffer_object.IsStereo.getValue());
  }

  TEST_FIXTURE(SampleBufferFixture, StereoFieldIsReadOnly)
  {
    const unsigned int sample_size = 1000;
    boost::shared_array<int16_t> sample_buffer(new int16_t [sample_size]);
    sound::SampleBuffer buffer_object(sample_buffer, sample_size);
    buffer_object.IsStereo.setValue(true);
    CHECK_EQUAL(false, buffer_object.IsStereo.getValue());
  }

  TEST_FIXTURE(SampleBufferFixture, GetSampleArray)
  {
    const unsigned int sample_size = 1000;
    boost::shared_array<int16_t> sample_buffer(new int16_t [sample_size]);
    sound::SampleBuffer buffer_object(sample_buffer, sample_size);
    CHECK_EQUAL(sample_buffer, buffer_object.getSamples());
  }

  TEST_FIXTURE(SampleBufferFixture, SetSampleBufferOnSoundSource)
  {
    const unsigned int sample_size = 1000;
    boost::shared_array<int16_t> sample_buffer(new int16_t [sample_size]);
    Link<sound::SampleBuffer> buffer_object(new sound::SampleBuffer(sample_buffer, sample_size));

    Link<sound::SoundSource> source(new sound::SoundSource);
    source->NewSampleBuffer.setValue(buffer_object);

    CHECK_EQUAL(buffer_object.getPtr(), source->NewSampleBuffer.getValue().getPtr());
  }

  class TestLocalSource : public av::sound::SoundSource::LocalSource
  {
    public:
      TestLocalSource(sound::SoundRenderer* soundRenderer) : LocalSource(soundRenderer) {}

      /* virtual */ void setWorldTransform(const ::osg::Matrixf& )  {}
      /* virtual */ void setLooping(bool loop) {}
      /* virtual */ void setURL(const std::string& url) {}

      std::queue<av::Link<sound::SampleBuffer> > mBufferQueue;
      const std::queue<Link<sound::SampleBuffer> >& getBufferQueue() const { return mBufferQueue; }

      /* virtual */ void enqueueSampleBuffer(av::Link<sound::SampleBuffer> sampleBuffer) { mBufferQueue.push(sampleBuffer); }
  };

  TEST_FIXTURE(SampleBufferFixture, PropagateSampleBufferToLocalSource)
  {
    const unsigned int sample_size = 1000;
    boost::shared_array<int16_t> sample_buffer(new int16_t [sample_size]);
    Link<sound::SampleBuffer> buffer_object(new sound::SampleBuffer(sample_buffer, sample_size));

    Link<sound::SoundSource> source(new sound::SoundSource);
    boost::shared_ptr<TestLocalSource> local_source(new TestLocalSource(0));
    source->addLocalSource(local_source);

    source->NewSampleBuffer.setValue(buffer_object);

    CHECK_EQUAL(1u, local_source->getBufferQueue().size());
  }
}
