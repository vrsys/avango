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

#include <avango/sound/SampleBuffer.h>

#include <boost/bind.hpp>
#include <stdexcept>

AV_FC_DEFINE(av::sound::SampleBuffer)

AV_FIELD_DEFINE(av::sound::SFSampleBuffer);
AV_FIELD_DEFINE(av::sound::MFSampleBuffer);


av::sound::SampleBuffer::SampleBuffer()
    : mBuffer(0), mNumSamples(0), mSampleRate(0)
{
  throw std::logic_error("Default Constructor of av::sound::SampleBuffer::SampleBuffer called");
}


av::sound::SampleBuffer::SampleBuffer(boost::shared_array<SampleType> buffer, unsigned int numberOfSamples, bool stereo)
    : mBuffer(buffer), mNumSamples(numberOfSamples), mStereo(stereo), mSampleRate(0)
{
  AV_FC_ADD_ADAPTOR_FIELD(NumSamples, boost::bind(&av::sound::SampleBuffer::getNumSamplesCB, this, _1),
                          boost::bind(&av::sound::SampleBuffer::setNumSamplesCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(IsStereo, boost::bind(&av::sound::SampleBuffer::getIsStereoCB, this, _1),
                          boost::bind(&av::sound::SampleBuffer::setIsStereoCB, this, _1));
}

/* virtual */
av::sound::SampleBuffer::~SampleBuffer()
{
}

unsigned int
av::sound::SampleBuffer::getNumSamples() const
{
  return mNumSamples;
}

boost::shared_array<av::sound::SampleBuffer::SampleType>
av::sound::SampleBuffer::getSamples() const
{
  return mBuffer;
}

unsigned int
av::sound::SampleBuffer::getSampleRate() const
{
    return mSampleRate;
}

void
av::sound::SampleBuffer::setSampleRate( unsigned int sampleRate)
{
    mSampleRate = sampleRate;
}

/*static*/ void
av::sound::SampleBuffer::initClass()
{
  if (!isTypeInitialized()) {
    av::Object::initClass();

    AV_FC_INIT(av::Object,
               av::sound::SampleBuffer,
               true);
    SFSampleBuffer::initClass("av::sound::SFSampleBuffer", "av::Field");
    MFSampleBuffer::initClass("av::sound::MFSampleBuffer", "av::Field");
  }
}

/* virtual */ void
av::sound::SampleBuffer::getNumSamplesCB(const av::SFUInt::GetValueEvent& event)
{
  av::SFUInt::ValueType &value(*event.getValuePtr());
  value = mNumSamples;
}

/* virtual */ void
av::sound::SampleBuffer::setNumSamplesCB(const av::SFUInt::SetValueEvent& )
{
}

/* virtual */ void
av::sound::SampleBuffer::getIsStereoCB(const av::SFBool::GetValueEvent& event)
{
  av::SFBool::ValueType &value(*event.getValuePtr());
  value = mStereo;
}

/* virtual */ void
av::sound::SampleBuffer::setIsStereoCB(const av::SFBool::SetValueEvent& )
{
}

