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

#include "avango/sound/SoundSource.h"

#include <avango/Field.h>
#include <avango/gua/Types.hpp>

AV_FC_DEFINE(av::sound::SoundSource);

AV_FIELD_DEFINE(av::sound::SFSoundSource);
AV_FIELD_DEFINE(av::sound::MFSoundSource);

av::sound::SoundSource::SoundSource()
  : mPollPlaying(false)
{
  AV_FC_ADD_FIELD(URL, "");
  AV_FC_ADD_FIELD(Loop, false);
  AV_FC_ADD_FIELD(Velocity, ::gua::math::vec3(0,0,0));
  AV_FC_ADD_FIELD(Direction, ::gua::math::vec3(0,0,0));
  AV_FC_ADD_FIELD(ConeInnerAngle, 360.0f);
  AV_FC_ADD_FIELD(ConeOuterAngle, 360.0f);
  AV_FC_ADD_FIELD(ConeOuterGain, 0.0);
  AV_FC_ADD_FIELD(Gain, 1.0f);
  AV_FC_ADD_FIELD(Pitch, 1.0f);
  AV_FC_ADD_FIELD(Play, false);
  AV_FC_ADD_FIELD(Pause, false);
  AV_FC_ADD_FIELD(Stop, false);
  AV_FC_ADD_FIELD(Rewind, false);
  AV_FC_ADD_FIELD(FinishedPlaying, false);
  AV_FC_ADD_FIELD(PlayTime, 0.0);
  AV_FC_ADD_FIELD(NewSampleBuffer, Link<av::sound::SampleBuffer>());
  AV_FC_ADD_FIELD(Spatialize, true);
  AV_FC_ADD_FIELD(Priority, 1.0f);
  AV_FC_ADD_FIELD(ReverbLevel, 0.0f);
  AV_FC_ADD_FIELD(Free, false);
}

/* virtual */
av::sound::SoundSource::~SoundSource()
{}


/*static*/ void
av::sound::SoundSource::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::TransformNode::initClass();
    //av::osg::Group::initClass();

    AV_FC_INIT(av::gua::TransformNode,
    //AV_FC_INIT(av::osg::Group,
               av::sound::SoundSource,
               true);
  }
}

template <class MemFun>
void
av::sound::SoundSource::for_each_local_source(MemFun f)
{
  for (LocalSources::iterator local_source = mLocalSources.begin();
       local_source != mLocalSources.end();
       ++local_source)
  {
    LocalSource& obj = **local_source;
    (obj.*f)();
  }
}

template <class MemFun, class Arg>
void
av::sound::SoundSource::for_each_local_source(MemFun f , Arg arg)
{
  for (LocalSources::iterator local_source = mLocalSources.begin();
       local_source != mLocalSources.end();
       ++local_source)
  {
    LocalSource& obj = **local_source;
    (obj.*f)(arg);
  }
}

/* virtual */ void
av::sound::SoundSource::fieldHasChangedLocalSideEffect(const Field& field)
{
  if (&field == &URL) {
    std::string url = URL.getValue();
    for_each_local_source(&LocalSource::setURL, url);
    PlayTime.setValue ( mLocalSources.empty() ? 0.0 : mLocalSources.front()->getPlayTime() );
  } else if (&field == &Spatialize) {
    bool spatialize = Spatialize.getValue();
    for_each_local_source(&LocalSource::setSpatialize, spatialize);
  } else if (&field == &Loop) {
    bool loop = Loop.getValue();
    for_each_local_source(&LocalSource::setLooping, loop);
  } else if (&field == &Velocity) {
    ::gua::math::vec3 velocity = Velocity.getValue();
    for_each_local_source(&LocalSource::setVelocity, velocity);
  } else if (&field == &Direction) {
    ::gua::math::vec3 direction = Direction.getValue();
    for_each_local_source(&LocalSource::setDirection, direction);
  } else if (&field == &ConeInnerAngle) {
    float angle = ConeInnerAngle.getValue();
    for_each_local_source(&LocalSource::setConeInnerAngle, angle);
  } else if (&field == &ConeOuterAngle) {
    float angle = ConeOuterAngle.getValue();
    for_each_local_source(&LocalSource::setConeOuterAngle, angle);
  } else if (&field == &ConeOuterGain) {
    float gain = ConeOuterGain.getValue();
    for_each_local_source(&LocalSource::setConeOuterGain, gain);
  } else if (&field == &Gain) {
    float gain = Gain.getValue();
    for_each_local_source(&LocalSource::setGain, gain);
  } else if (&field == &Pitch) {
    float pitch = Pitch.getValue();
    for_each_local_source(&LocalSource::setPitch, pitch);
  } else if (&field == &Priority) {
    float priority = Priority.getValue();
    for_each_local_source(&LocalSource::setPriority, priority);
  } else if (&field == &ReverbLevel) {
    float revlevel = ReverbLevel.getValue();
    for_each_local_source(&LocalSource::setReverbLevel, revlevel);
  } else if (&field == &Free) {
    for_each_local_source(&LocalSource::setFree);
  } else if (&field == &Play) {
    for_each_local_source(&LocalSource::play);
    mPollPlaying = true;
    alwaysEvaluate(true);
  } else if (&field == &Pause) {
    for_each_local_source(&LocalSource::pause);
  } else if (&field == &Stop) {
    for_each_local_source(&LocalSource::stop);
  } else if (&field == &Rewind) {
    for_each_local_source(&LocalSource::rewind);
  } else   if (&field == &NewSampleBuffer) {
    for_each_local_source(&LocalSource::enqueueSampleBuffer, NewSampleBuffer.getValue());
  } else {
    //av::osg::Group::fieldHasChangedLocalSideEffect(field);
    av::gua::TransformNode::fieldHasChangedLocalSideEffect(field);
  }
}

void
av::sound::SoundSource::initializeLocalSource(boost::shared_ptr<SoundSource::LocalSource> localSource)
{
    localSource->setSpatialize(Spatialize.getValue());
    localSource->setURL(URL.getValue());
    localSource->setLooping(Loop.getValue());
    localSource->setVelocity(Velocity.getValue());
    localSource->setGain(Gain.getValue());
    localSource->setPitch(Pitch.getValue());
    localSource->setDirection(Direction.getValue());
    localSource->setConeInnerAngle(ConeInnerAngle.getValue());
    localSource->setConeOuterAngle(ConeOuterAngle.getValue());
    localSource->setConeOuterGain(ConeOuterGain.getValue());
    localSource->setPriority(Priority.getValue());
    localSource->setReverbLevel(ReverbLevel.getValue());

}

void
av::sound::SoundSource::evaluateLocalSideEffect()
{
  if (!mPollPlaying)
  {
    return;
  }

  bool all_done = true;
  for (LocalSources::iterator local_source = mLocalSources.begin();
  local_source != mLocalSources.end();
  ++local_source)
  {
    LocalSource& obj = **local_source;
    all_done &= obj.donePlaying();
  }
  if (all_done)
  {
    mPollPlaying = false;
    alwaysEvaluate(false);
    FinishedPlaying.touch();
  }
}

void
av::sound::SoundSource::addLocalSource(boost::shared_ptr<SoundSource::LocalSource> localSource)
{
  initializeLocalSource(localSource);
  mLocalSources.push_back(localSource);
}

void
av::sound::SoundSource::removeLocalSource(boost::shared_ptr<SoundSource::LocalSource> localSource)
{
  mLocalSources.erase(std::remove(mLocalSources.begin(), mLocalSources.end(), localSource), mLocalSources.end());
}

unsigned int
av::sound::SoundSource::numLocalSources() const
{
  return mLocalSources.size();
}

boost::shared_ptr<av::sound::SoundSource::LocalSource>
av::sound::SoundSource::getLocalSource(unsigned int index)
{
  return mLocalSources.at(index);
}


av::sound::SoundSource::LocalSource::LocalSource(SoundRenderer* renderer)
  : mSoundRenderer(renderer)
{}

/* virtual */
av::sound::SoundSource::LocalSource::~LocalSource()
{}

const av::sound::SoundRenderer*
av::sound::SoundSource::LocalSource::getRenderer()
{
  return mSoundRenderer;
}




