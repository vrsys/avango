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
#include <avango/sound/openal/OpenALSoundRenderer.h>
#include <avango/sound/SoundTraverser.h>

#include <iostream>
#include <string>

using namespace av;

namespace {

  /// TESTS
  TEST(TypeCanBeInitialized)
  {
    sound::openal::OpenALSoundRenderer::initClass();
    CHECK(Type::getByName("av::sound::openal::OpenALSoundRenderer") != Type::badType());
  }

  TEST(CanBeDefaultCreated)
  {
    sound::openal::OpenALSoundRenderer::initClass();
    std::auto_ptr<sound::openal::OpenALSoundRenderer> soundRenderer(new sound::openal::OpenALSoundRenderer());

    CHECK(soundRenderer.get() != 0);
  }

  TEST(DeviceCanBeOpenedAndClosed)
  {
    sound::openal::OpenALSoundRenderer::initClass();
    std::auto_ptr<sound::openal::OpenALSoundRenderer> soundRenderer(new sound::openal::OpenALSoundRenderer());

    soundRenderer->openDevice(""); // default device
    CHECK(soundRenderer->deviceOpen() == true);
    soundRenderer->closeDevice();
    CHECK(soundRenderer->deviceOpen() == false);
  }


  TEST(TestDeviceField)
  {
    sound::openal::OpenALSoundRenderer::initClass();
    Link<sound::openal::OpenALSoundRenderer> soundRenderer(new sound::openal::OpenALSoundRenderer());

    CHECK(soundRenderer->Device.getValue() == "none");
    CHECK(soundRenderer->deviceOpen() == false);

    soundRenderer->Device.setValue("");

    CHECK(soundRenderer->deviceOpen() == true);
    CHECK(soundRenderer->hasContext() == true);

    soundRenderer->Device.setValue("none");

    CHECK(soundRenderer->deviceOpen() == false);
    CHECK(soundRenderer->hasContext() == false);

  }




}
