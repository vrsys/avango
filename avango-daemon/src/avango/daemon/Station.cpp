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

#include <avango/daemon/Station.h>

#include <gua/math.hpp>
#include <cstring>

AV_BASE_DEFINE(av::daemon::Station);

av::daemon::Station::Station()
  : mName(),
    mMatrix(gua::math::mat4::identity()),
    mButton(),
    mValue(),
    mLed(),
    mMatrixUsed(false),
    mButtonsUsed(0),
    mValuesUsed(0),
    mLedsUsed(0)
{
  mName[sMaxNameLength - 1] = '\0';

  for (int i=0; i<sMaxButtons; ++i) {
    mButton[i] = 0;
  }
  for (int i=0; i<sMaxValues; ++i) {
    mValue[i] = 0;
  }
  for (int i=0; i<sMaxLeds; ++i) {
    mLed[i] = false;
  }
}

void
av::daemon::Station::initClass()
{
  if (!isTypeInitialized())
  {
    av::Base::initClass();
    AV_BASE_INIT(av::Base, av::daemon::Station, true);
  }
}

const char*
av::daemon::Station::getName() const
{
  return mName;
}

const ::gua::math::mat4&
av::daemon::Station::getMatrix() const
{
  return mMatrix;
}

int
av::daemon::Station::getButton(int which) const
{
  if (sMaxButtons <= which)
    return 0;

  return mButton[which];
}

const std::vector<int>
av::daemon::Station::getButtons() const
{
  return std::vector<int> (mButton, mButton + sMaxButtons);
}

float
av::daemon::Station::getValue(int which) const
{
  if (sMaxValues <= which)
    return 0.0f;

  return mValue[which];
}

const std::vector<float>
av::daemon::Station::getValues() const
{
  return std::vector<float> (mValue, mValue + sMaxValues);
}

bool
av::daemon::Station::getLED(int which) const
{
  if (sMaxLeds <= which)
    return false;

  return mLed[which];
}

const std::vector<bool>
av::daemon::Station::getLEDs() const
{
  return std::vector<bool> (mLed, mLed + sMaxLeds);
}

void
av::daemon::Station::setName(const char* name)
{
  ::strncpy(mName, name, sMaxNameLength - 2);

  mName[sMaxNameLength - 1] = '\0';
}

void
av::daemon::Station::setMatrix(const ::gua::math::mat4& matrix)
{
  mMatrix = matrix;
}

void
av::daemon::Station::setButton(int which, int on)
{
  if (sMaxButtons <= which)
    return;

  mButton[which] = on;
}

void
av::daemon::Station::setValue(int which, float val)
{
  if (sMaxValues <= which)
    return;

  mValue[which] = val;
}

void
av::daemon::Station::setLED(int which, bool on)
{
  if (sMaxLeds <= which)
    return;

  mLed[which] = on;
}

bool
av::daemon::Station::getMatrixUsed() const
{
  return mMatrixUsed;
}

int
av::daemon::Station::getButtonsUsed() const
{
  return mButtonsUsed;
}

int
av::daemon::Station::getValuesUsed() const
{
  return mValuesUsed;
}

int
av::daemon::Station::getLEDsUsed() const
{
  return mLedsUsed;
}

void
av::daemon::Station::setUsage(bool matrix, int buttons, int values, int leds)
{
  mMatrixUsed = matrix;
  mButtonsUsed = buttons;
  mValuesUsed = values;
  mLedsUsed = leds;
}
