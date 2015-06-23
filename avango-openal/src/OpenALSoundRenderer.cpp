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

#include <avango/sound/openal/OpenALSoundRenderer.h>

#include <AL/alc.h>
#include <AL/alut.h>

#include <vorbis/vorbisfile.h>

#include <vector>
#include <avango/sound/SampleBuffer.h>

#include <gua/math/math.hpp>

#include <iostream>

using namespace std;

AV_FC_DEFINE(av::sound::openal::OpenALSoundRenderer)

unsigned int av::sound::openal::OpenALSoundRenderer::gAlutOpenCount = 0;

#define BUFFER_SIZE     32768       // 32 KB buffers

namespace {
  av::Logger& logger(av::getLogger("av::sound::openal::OpenALSoundRenderer"));
}

namespace {
  const int debug = 1;

  /// This function loads a .ogg file into a memory buffer and returns the format and frequency.
  /** @return true on error, false otherwise */
  bool
  LoadOGG(const std::string& fileName, std::vector<char> &buffer, ALenum &format, ALsizei &frequency)
  {
    const int endian = 0;                         // 0 for Little-Endian, 1 for Big-Endian


    FILE *f = ::fopen(fileName.c_str(), "rb");

    if (f == NULL)
    {
      logger.warn() << "LoadOGG(): Could not open " << fileName << " for reading.";
      return true; // error
    }

    OggVorbis_File ogg_file;
    if (::ov_open(f, &ogg_file, NULL, 0) != 0)
    {
      logger.warn() << "LoadOGG(): Error opening '%s' for decoding.\n" ,fileName;
      return true; // error
    }

    // Get some information about the OGG file
    vorbis_info *p_info = ov_info(&ogg_file, -1);

    // Check the number of channels... always use 16-bit samples
    if (p_info->channels == 1)
    {
      format = AL_FORMAT_MONO16;
    } else {
      format = AL_FORMAT_STEREO16;
    }

    frequency = p_info->rate;

    long bytes = -1;
    char array[BUFFER_SIZE];                // Local fixed size array
    char buf[1000];
    int bit_stream;
    // Keep reading until all is read
    do
    {
      // Read up to a buffer's worth of decoded sound data
      bytes = ::ov_read(&ogg_file, array, BUFFER_SIZE, endian, 2, 1, &bit_stream);

      if (bytes < 0)
      {
        ::ov_clear(&ogg_file); // also closes f
        logger.warn() << "LoadOGG(): Error decoding '%s'.\n", fileName;
        return true; // error
      }

      // Append to end of buffer
      buffer.insert(buffer.end(), array, array + bytes);
    } while (bytes > 0);

    // Clean up!
    ::ov_clear(&ogg_file); // also closes f
    return false; // no error
  }
}

av::sound::openal::OpenALSoundRenderer::OpenALSoundRenderer()
  : mDevice(0),
    mDeviceName("none"),
    mContext(0)
{
  mPosition = ::gua::math::mat4::identity();

  AV_FC_ADD_FIELD(Device, mDeviceName);

  if (0 == gAlutOpenCount) {
    if (::alutInitWithoutContext(0,0)) {
      ++gAlutOpenCount;
    }
  }
}


av::sound::openal::OpenALSoundRenderer::~OpenALSoundRenderer()
{
  std::cerr << "~OpenALSoundRenderer() called" << std::endl;
  destroyContext();
  closeDevice();
  if (gAlutOpenCount > 0) {
    --gAlutOpenCount;
    if (0 == gAlutOpenCount) {
      ::alutExit();
    }
  }

}

/*static*/ void
av::sound::openal::OpenALSoundRenderer::initClass()
{
  if(!isTypeInitialized())
  {
    SoundRenderer::initClass();
    AV_FC_INIT(av::sound::SoundRenderer,
               av::sound::openal::OpenALSoundRenderer,
               false);
  }
}

/* virtual */ void
av::sound::openal::OpenALSoundRenderer::fieldHasChanged(const av::Field& field)
{
  if (&field == &Device) {
    if (Device.getValue() == mDeviceName)
    {
      return;
    }
    if (deviceOpen())
    {
      destroyContext();
      closeDevice();
    }
    mDeviceName = Device.getValue();
    if (mDeviceName == "none")
    {
      return;
    }
    openDevice(mDeviceName);
    if (!deviceOpen())
    {
      return;
    }
    createContext();
    AV_ASSERT(mContext);
  }
}


void
av::sound::openal::OpenALSoundRenderer::openDevice(const std::string& deviceName)
{
  if (deviceName.empty())
  {
    mDevice = ::alcOpenDevice(NULL);
  } else
  {
    mDevice = ::alcOpenDevice(reinterpret_cast<const ALCchar*>(deviceName.c_str()));
  }

  if (mDevice)
  {
    ALCenum my_error = ::alcGetError(mDevice);
    if (my_error != ALC_NO_ERROR) {
      mDevice = 0;
    }
  }
}

void
av::sound::openal::OpenALSoundRenderer::closeDevice()
{
  if (mDevice)
  {
    ::alcCloseDevice(mDevice);
    mDevice = 0;
  }
}

bool
av::sound::openal::OpenALSoundRenderer::deviceOpen() const
{
  return mDevice != 0;
}


void
av::sound::openal::OpenALSoundRenderer::createContext()
{
  if (deviceOpen()) {
    ::alcGetError(mDevice);
    mContext = ::alcCreateContext(mDevice, 0);
    if (::alcGetError(mDevice) != ALC_NO_ERROR) {
      mContext = 0;
    } else {
      ::alcMakeContextCurrent(mContext);
    }
  }
}

void
av::sound::openal::OpenALSoundRenderer::destroyContext()
{
  if (hasContext())
  {
    if (::alcGetCurrentContext())
    {
      ::alcMakeContextCurrent(0);
    }
    ::alcDestroyContext(mContext);
    mContext = 0;
  }
}

bool
av::sound::openal::OpenALSoundRenderer::hasContext() const
{
  return mContext != 0;
}

boost::shared_ptr<av::sound::SoundSource::LocalSource>
av::sound::openal::OpenALSoundRenderer::createLocalSource()
{
  OpenALLocalSource* new_local_source = new OpenALLocalSource(this);
  return boost::shared_ptr<av::sound::SoundSource::LocalSource> (new_local_source);
}

/* virtual */
void
av::sound::openal::OpenALSoundRenderer::setListenerPosition(const ::gua::math::mat4& position)
{
  ::gua::math::quatf rotation(::scm::math::quat<double>::from_matrix(position));
  ::gua::math::vec3f translation(::gua::math::get_translation(position));

  ::alListenerfv(AL_POSITION, translation.data_array);
  ::gua::math::mat4f rotationMat = rotation.to_matrix();

  ::gua::math::vec3f atAndUpVec[2];

  atAndUpVec[0] = rotationMat * ::gua::math::vec3f(0.0f, 0.0f, -1.0f);
  atAndUpVec[1] = rotationMat * ::gua::math::vec3f(0.0f, 1.0f, 0.0f);

  ::alListenerfv(AL_ORIENTATION, atAndUpVec[0].data_array);

  mPosition = position;
}

/* virtual */
::gua::math::mat4
av::sound::openal::OpenALSoundRenderer::getListenerPosition() const
{
  return mPosition;
}

av::sound::openal::OpenALSoundRenderer::OpenALLocalSource::OpenALLocalSource(SoundRenderer* renderer)
  : SoundSource::LocalSource(renderer),
    mSource(0),
    mHaveSource(false),
    mBuffer(AL_NONE),
    mHaveBuffer(false),
    mPlayTime(0.0),
    mIsPlaying(false)
{
  ::alGetError();
  ::alGenSources(1, &mSource);
  if (::alGetError() == AL_NO_ERROR) {
    mHaveSource = true;
  }
}

av::sound::openal::OpenALSoundRenderer::OpenALLocalSource::~OpenALLocalSource()
{
  std::cerr << "~OpenALLocalSource() called" << std::endl;
  flushBufferQueue();
  if (AL_NONE != mBuffer) {
    if (mHaveSource) {
      ::alSourcei(mSource, AL_BUFFER, 0);
    }
    ::alDeleteBuffers(1, &mBuffer);
  }
  if (mHaveSource) {
    ::alDeleteSources(1,&mSource);
  }
}

void
av::sound::openal::OpenALSoundRenderer::OpenALLocalSource::setWorldTransform(const ::gua::math::mat4& worldMatrix)
{

  ::gua::math::vec3f position = ::gua::math::vec3f(::gua::math::get_translation(worldMatrix));
  //::alSourcefv(mSource, AL_POSITION, position.ptr());
  ::alSourcefv(mSource, AL_POSITION, position.data_array);
}

void
av::sound::openal::OpenALSoundRenderer::OpenALLocalSource::setLooping(bool loop)
{
  ::alSourcei(mSource, AL_LOOPING, loop);
}

void
av::sound::openal::OpenALSoundRenderer::OpenALLocalSource::setURL(const std::string& soundURL)
{
  stop();
  freeUnusedStreamingBuffers();
  removeStaticBufferFromSource();
  flushBufferQueue();

  mPlayTime = 0.0;

  char buf[1000];
  ALuint buffer_id;
  const char *url_str = soundURL.c_str();
  const char *ext = strrchr(url_str,'.');
  if(ext!=0 && strcasecmp(ext,".ogg")==0)
  {
    ALenum format;                          // The sound data format
    ALsizei freq;                           // The frequency of the sound data
    vector<char> bufferData;                // The sound buffer data from file
    // we have an OGG file
    ::alGenBuffers(1, &buffer_id);
    if(buffer_id != AL_NONE) {
      // Load the OGG file into memory
      if( LoadOGG(url_str, bufferData, format, freq) == false ) {
        // Upload sound data to buffer
        ::alBufferData(buffer_id, format, &bufferData[0],
                     static_cast<ALsizei>(bufferData.size()), freq);

      }
    }
  } else
  {
    // any other extension is treated as wav
    buffer_id = ::alutCreateBufferFromFile(url_str);
  }

  if (buffer_id == AL_NONE)
  {
    logger.warn() << "Error at %s:%d while reading sound from '%s': %s.\n",  __FILE__ ,  __LINE__, url_str,
    ::alutGetErrorString(::alutGetError());
  } else
  {
    ALint size, freq, bits, channels;
    ::alGetBufferi(buffer_id, AL_SIZE,     &size );
    ::alGetBufferi(buffer_id, AL_FREQUENCY,&freq );
    ::alGetBufferi(buffer_id, AL_BITS,     &bits );
    ::alGetBufferi(buffer_id, AL_CHANNELS, &channels );

    ALint numSamples = size*8/bits/channels;
    mPlayTime = (double)numSamples/(double)freq;

    logger.info() << "Loaded sound '%s'.\n   Bytes=%d, Freq=%d, Bits=%d, Channels=%d, Time=%g.\n",
    url_str, size, freq, bits, channels, mPlayTime;

    ::alSourcei(mSource, AL_BUFFER, buffer_id);
  }
  mBuffer = buffer_id;
}

void
av::sound::openal::OpenALSoundRenderer::OpenALLocalSource::setVelocity(const ::gua::math::vec3& velocity)
{
  //::alSourcefv(mSource, AL_VELOCITY, velocity.ptr());
  ::gua::math::vec3f vel(velocity);
  ::alSourcefv(mSource, AL_VELOCITY, static_cast<const ALfloat*>(vel.data_array));
}

void
av::sound::openal::OpenALSoundRenderer::OpenALLocalSource::setGain(float gain)
{
  ::alSourcef(mSource, AL_GAIN, gain);
}

void
av::sound::openal::OpenALSoundRenderer::OpenALLocalSource::setPitch(float pitch)
{
  ::alSourcef(mSource, AL_GAIN, pitch);
}

void
av::sound::openal::OpenALSoundRenderer::OpenALLocalSource::play()
{
  ::alSourcePlay(mSource);
  mIsPlaying = true;
}

void
av::sound::openal::OpenALSoundRenderer::OpenALLocalSource::pause()
{
  ::alSourcePause(mSource);
}

void
av::sound::openal::OpenALSoundRenderer::OpenALLocalSource::stop()
{
  ::alSourceStop(mSource);
  mIsPlaying = false;
}

void
av::sound::openal::OpenALSoundRenderer::OpenALLocalSource::rewind()
{
  ::alSourceRewind(mSource);
}

bool
av::sound::openal::OpenALSoundRenderer::OpenALLocalSource::donePlaying() const
{
  ALint state;
  ::alGetSourcei(mSource, AL_SOURCE_STATE, &state) ;
  return state == AL_STOPPED;
}

static std::string
stateToString(ALint state)
{
  switch (state) {
  case AL_STOPPED:
    return "AL_STOPPED";
  case AL_PAUSED:
    return "AL_PAUSED";
  case AL_PLAYING:
    return "AL_PLAYING";
  case AL_INITIAL:
    return "AL_INITIAL";
  default:
    return "Unknown";
  }
}

/* virtual */
void
av::sound::openal::OpenALSoundRenderer::OpenALLocalSource::enqueueSampleBuffer(av::Link<SampleBuffer> sampleBuffer)
{
  if (!sampleBuffer.isValid())
  {
    return;
  }
  if (AL_NONE != mBuffer)
  {
    removeStaticBufferFromSource();
  }
  ALuint new_buffer_id = getNewStreamingBuffer();

  std::cerr << "queueing buffer " << sampleBuffer.getPtr() << "with " << sampleBuffer->getNumSamples() << " samples  in "
            << (sampleBuffer->IsStereo.getValue() ? "stereo" : "mono") << std::endl;

  alBufferData(new_buffer_id, sampleBuffer->IsStereo.getValue() ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
               sampleBuffer->getSamples().get(), sampleBuffer->getNumSamples() * sizeof(sound::SampleBuffer::SampleType), sampleBuffer->getSampleRate());
  alSourceQueueBuffers(mSource, 1, &new_buffer_id);
  ALint state;
  ::alGetSourcei(mSource, AL_SOURCE_STATE, &state);
  std::cerr << "source state = " << stateToString(state) << std::endl;
  if (mIsPlaying && (state == AL_STOPPED || state == AL_INITIAL)) {
    ::alSourcePlay(mSource);
  }
}

void
av::sound::openal::OpenALSoundRenderer::OpenALLocalSource::removeStaticBufferFromSource()
{
  stop();
  if (mHaveSource) {
    ::alSourcei(mSource, AL_BUFFER, 0);
  }
  ::alDeleteBuffers(1, &mBuffer);
  mBuffer = AL_NONE;
}

ALuint
av::sound::openal::OpenALSoundRenderer::OpenALLocalSource::getNewStreamingBuffer()
{
  freeUnusedStreamingBuffers();
  if (!mBufferQueue.empty())
  {
    ALuint buffer_id = mBufferQueue.front();
    mBufferQueue.pop_front();
    return buffer_id;
  }

  ALuint new_buffer;
  alGenBuffers(1, &new_buffer);
  return new_buffer;
}

void
av::sound::openal::OpenALSoundRenderer::OpenALLocalSource::freeUnusedStreamingBuffers()
{
  ALint processed_buffers;
  alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &processed_buffers);
  std::vector<ALuint> processed_buffer_ids(processed_buffers);
  alSourceUnqueueBuffers(mSource, processed_buffers, &(processed_buffer_ids[0]));
  std::copy(processed_buffer_ids.begin(), processed_buffer_ids.end(), std::back_inserter(mBufferQueue));
}

void
av::sound::openal::OpenALSoundRenderer::OpenALLocalSource::flushBufferQueue()
{
  while (!mBufferQueue.empty())
  {
    alDeleteBuffers(1, &(mBufferQueue.front()));
    mBufferQueue.pop_front();
  }
}
