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

#include <avango/daemon/DeviceSensor.h>

#include <avango/SingleField.h>
#include <avango/Logger.h>

#include <scm/core/math.h>
#include <gua/math.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::daemon::DeviceSensor"));
}

AV_FC_DEFINE(av::daemon::DeviceSensor);

av::daemon::DeviceSensor::DeviceSensor()
{
  AV_FC_ADD_FIELD(DeviceService,            0);
  AV_FC_ADD_FIELD(Station,                  ::std::string());
  AV_FC_ADD_FIELD(ResetValuesOnRead,        false);
  AV_FC_ADD_FIELD(Matrix,                   ::gua::math::mat4());
  AV_FC_ADD_FIELD(Button0,                  false);
  AV_FC_ADD_FIELD(Button1,                  false);
  AV_FC_ADD_FIELD(Button2,                  false);
  AV_FC_ADD_FIELD(Button3,                  false);
  AV_FC_ADD_FIELD(Button4,                  false);
  AV_FC_ADD_FIELD(Button5,                  false);
  AV_FC_ADD_FIELD(Button6,                  false);
  AV_FC_ADD_FIELD(Button7,                  false);
  AV_FC_ADD_FIELD(Button8,                  false);
  AV_FC_ADD_FIELD(Button9,                  false);
  AV_FC_ADD_FIELD(Button10,                 false);
  AV_FC_ADD_FIELD(Button11,                 false);
  AV_FC_ADD_FIELD(Button12,                 false);
  AV_FC_ADD_FIELD(Button13,                 false);
  AV_FC_ADD_FIELD(Button14,                 false);
  AV_FC_ADD_FIELD(Button15,                 false);
  AV_FC_ADD_FIELD(Button16,                 false);
  AV_FC_ADD_FIELD(Button17,                 false);
  AV_FC_ADD_FIELD(Button18,                 false);
  AV_FC_ADD_FIELD(Button19,                 false);
  AV_FC_ADD_FIELD(Button20,                 false);
  AV_FC_ADD_FIELD(Button21,                 false);
  AV_FC_ADD_FIELD(Button22,                 false);
  AV_FC_ADD_FIELD(Button23,                 false);
  AV_FC_ADD_FIELD(Button24,                 false);
  AV_FC_ADD_FIELD(Button25,                 false);
  AV_FC_ADD_FIELD(Button26,                 false);
  AV_FC_ADD_FIELD(Button27,                 false);
  AV_FC_ADD_FIELD(Button28,                 false);
  AV_FC_ADD_FIELD(Button29,                 false);
  AV_FC_ADD_FIELD(Button30,                 false);
  AV_FC_ADD_FIELD(Button31,                 false);
  AV_FC_ADD_FIELD(Value0,                   0.0f);
  AV_FC_ADD_FIELD(Value1,                   0.0f);
  AV_FC_ADD_FIELD(Value2,                   0.0f);
  AV_FC_ADD_FIELD(Value3,                   0.0f);
  AV_FC_ADD_FIELD(Value4,                   0.0f);
  AV_FC_ADD_FIELD(Value5,                   0.0f);
  AV_FC_ADD_FIELD(Value6,                   0.0f);
  AV_FC_ADD_FIELD(Value7,                   0.0f);
  AV_FC_ADD_FIELD(Value8,                   0.0f);
  AV_FC_ADD_FIELD(Value9,                   0.0f);
  AV_FC_ADD_FIELD(Value10,                  0.0f);
  AV_FC_ADD_FIELD(Value11,                  0.0f);
  AV_FC_ADD_FIELD(Value12,                  0.0f);
  AV_FC_ADD_FIELD(Value13,                  0.0f);
  AV_FC_ADD_FIELD(Value14,                  0.0f);
  AV_FC_ADD_FIELD(Value15,                  0.0f);
  AV_FC_ADD_FIELD(LED0,                     false);
  AV_FC_ADD_FIELD(LED1,                     false);
  AV_FC_ADD_FIELD(LED2,                     false);
  AV_FC_ADD_FIELD(LED3,                     false);
  AV_FC_ADD_FIELD(LED4,                     false);
  AV_FC_ADD_FIELD(LED5,                     false);
  AV_FC_ADD_FIELD(LED6,                     false);
  AV_FC_ADD_FIELD(LED7,                     false);
  AV_FC_ADD_FIELD(LED8,                     false);
  AV_FC_ADD_FIELD(LED9,                     false);
  AV_FC_ADD_FIELD(LED10,                    false);
  AV_FC_ADD_FIELD(LED11,                    false);
  AV_FC_ADD_FIELD(LED12,                    false);
  AV_FC_ADD_FIELD(LED13,                    false);
  AV_FC_ADD_FIELD(LED14,                    false);
  AV_FC_ADD_FIELD(LED15,                    false);
  AV_FC_ADD_FIELD(TransmitterOffset,        ::gua::math::mat4());
  AV_FC_ADD_FIELD(ReceiverOffset,           ::gua::math::mat4());
  AV_FC_ADD_FIELD(Rotation,                 ::gua::math::quat());
  AV_FC_ADD_FIELD(Translation,              ::gua::math::vec3());

  alwaysEvaluate(true);
}

av::daemon::DeviceSensor::~DeviceSensor()
{}

void
av::daemon::DeviceSensor::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();
    AV_FC_INIT(av::FieldContainer, av::daemon::DeviceSensor, true);
  }
}

/* virtual */ void
av::daemon::DeviceSensor::evaluate()
{
  if (DeviceService.getValue().isValid() && !(Station.getValue().empty()) )
  {
    updateMatrix();
    updateButtons();
    updateValues();
    updateLEDs();
  }
}

void
av::daemon::DeviceSensor::updateMatrix()
{
  ::gua::math::mat4 mat;
  getMatrix(mat);

  if (Matrix.getValue() != mat)
  {
    ::gua::math::quat rot = ::scm::math::quat<float>::from_matrix(mat);
    ::gua::math::vec3 pos = ::gua::math::get_translation(mat);

    Rotation.setValue(rot);
    Translation.setValue(pos);
    Matrix.setValue(mat);
  }
}

void
av::daemon::DeviceSensor::updateButtons()
{
  for (int i=0; i<sMaxButtons; ++i)
  {
    std::stringstream fieldname;
    fieldname << "Button" << i;
    SingleField<bool>* sf = dynamic_cast<SingleField<bool>*>(getField(fieldname.str()));

    if ( (sf != 0) && (getButton(i) != sf->getValue()) )
      sf->setValue(getButton(i));
  }
}

void
av::daemon::DeviceSensor::updateValues()
{
  for (int i=0; i<sMaxValues; ++i)
  {
    std::stringstream fieldname;
    fieldname << "Value" << i;
    SingleField<float>* sf = dynamic_cast<SingleField<float>*>(getField(fieldname.str()));

    if ( (sf != 0) && (getValue(i) != sf->getValue()) )
      sf->setValue(getValue(i));
  }
}

void
av::daemon::DeviceSensor::updateLEDs()
{
  for (int i=0; i<sMaxLEDs; ++i)
  {
    std::stringstream fieldname;
    fieldname << "LED" << i;
    SingleField<bool>* sf = dynamic_cast<SingleField<bool>*>(getField(fieldname.str()));

    if ( (sf != 0) && (getLED(i) != sf->getValue()) )
      sf->setValue(getLED(i));
  }
}

void
av::daemon::DeviceSensor::getMatrix(::gua::math::mat4& mat)
{
  const char* stationstr = Station.getValue().c_str();
  ::gua::math::mat4 station = DeviceService.getValue()->getMatrix(stationstr);

  mat = TransmitterOffset.getValue() * station * ReceiverOffset.getValue();
}

float
av::daemon::DeviceSensor::getValue(int v)
{
  if((v >= 0) && (v < sMaxValues))
  {
    const char* stationstr = Station.getValue().c_str();
    float value = DeviceService.getValue()->getValue(stationstr, v);
    if (ResetValuesOnRead.getValue()) DeviceService.getValue()->setValue(stationstr, v, 0.0f);
    return value;
  }

  return 0.0f;
}

bool
av::daemon::DeviceSensor::getButton(int b)
{
  if ((b >= 0) && (b < sMaxButtons))
  {
    const char* stationstr = Station.getValue().c_str();
    return DeviceService.getValue()->getButton(stationstr, b);
  }

  return false;
}

bool
av::daemon::DeviceSensor::getLED(int l)
{
  if ((l >= 0) && (l < sMaxLEDs))
  {
    const char* stationstr = Station.getValue().c_str();
    return DeviceService.getValue()->getLED(stationstr, l);
  }

  return false;
}

void
av::daemon::DeviceSensor::setLED(int l, bool val)
{
  if ((l >= 0) && (l < sMaxLEDs))
  {
    const char* stationstr = Station.getValue().c_str();
    DeviceService.getValue()->setLED(stationstr, l, val);
  }
}
