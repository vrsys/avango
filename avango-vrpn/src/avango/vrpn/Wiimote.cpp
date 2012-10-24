#include <iostream>

#include <avango/Logger.h>
#include <avango/osg/Types.h>

#include <avango/vrpn/Wiimote.h>

#include <boost/bind.hpp>

namespace {
av::Logger& logger(av::getLogger("av::vrpn::Wiimote"));
}

AV_FC_DEFINE( av::vrpn::Wiimote);

AV_FIELD_DEFINE( av::vrpn::SFWiimote);
AV_FIELD_DEFINE( av::vrpn::MFWiimote);

av::vrpn::Wiimote::Wiimote() :
  Device()
{
  mRumbling = false;

  AV_FC_ADD_FIELD(Button0, false);
  AV_FC_ADD_FIELD(Button1, false);
  AV_FC_ADD_FIELD(Button2, false);
  AV_FC_ADD_FIELD(Button3, false);
  AV_FC_ADD_FIELD(Button4, false);
  AV_FC_ADD_FIELD(Button5, false);
  AV_FC_ADD_FIELD(Button6, false);
  AV_FC_ADD_FIELD(Button7, false);
  AV_FC_ADD_FIELD(Button8, false);
  AV_FC_ADD_FIELD(Button9, false);
  AV_FC_ADD_FIELD(Button10, false);
  AV_FC_ADD_FIELD(Button11, false);
  AV_FC_ADD_FIELD(Button12, false);
  AV_FC_ADD_FIELD(Button13, false);
  AV_FC_ADD_FIELD(Button14, false);
  AV_FC_ADD_FIELD(Button15, false);

  AV_FC_ADD_FIELD(Channel0, false);
  AV_FC_ADD_FIELD(Channel1, false);
  AV_FC_ADD_FIELD(Channel2, false);
  AV_FC_ADD_FIELD(Channel3, false);
  AV_FC_ADD_FIELD(Channel4, false);
  AV_FC_ADD_FIELD(Channel5, false);
  AV_FC_ADD_FIELD(Channel6, false);
  AV_FC_ADD_FIELD(Channel7, false);
  AV_FC_ADD_FIELD(Channel8, false);
  AV_FC_ADD_FIELD(Channel9, false);
  AV_FC_ADD_FIELD(Channel10, false);
  AV_FC_ADD_FIELD(Channel11, false);
  AV_FC_ADD_FIELD(Channel12, false);
  AV_FC_ADD_FIELD(Channel13, false);
  AV_FC_ADD_FIELD(Channel14, false);
  AV_FC_ADD_FIELD(Channel15, false);

  AV_FC_ADD_ADAPTOR_FIELD(Rumble, boost::bind(&av::vrpn::Wiimote::getRumbleCB,
      this, _1), boost::bind(&av::vrpn::Wiimote::setRumbleCB, this, _1));

  AV_FC_ADD_FIELD(LED0, false);
  AV_FC_ADD_FIELD(LED1, false);
  AV_FC_ADD_FIELD(LED2, false);
  //AV_FC_ADD_FIELD(LED3, false);
  AV_FC_ADD_ADAPTOR_FIELD(LED3, boost::bind(&av::vrpn::Wiimote::getLED3CB,
        this, _1), boost::bind(&av::vrpn::Wiimote::setLED3CB, this, _1));
}

av::vrpn::Wiimote::~Wiimote()
{
}

void av::vrpn::Wiimote::initClass()
{
  if (!isTypeInitialized())
  {
    ::av::vrpn::Device::initClass();

    AV_FC_INIT(av::vrpn::Device, av::vrpn::Wiimote, true);

    SFWiimote::initClass("av::vrpn::SFWiimote", "av::Field");
    MFWiimote::initClass("av::vrpn::MFWiimote", "av::Field");
  }
}

void av::vrpn::Wiimote::handleAnalogTemplFunc(const vrpn_ANALOGCB a)
{
  //seems VRPN only gives you all the channels, not only the changed ones
  for (int i = 0; i < a.num_channel; i++)
  {
    switch (i)
    {
      case 0:
        Channel0.setValue(a.channel[i]);
        break;
      case 1:
        Channel1.setValue(a.channel[i]);
        break;
      case 2:
        Channel2.setValue(a.channel[i]);
        break;
      case 3:
        Channel3.setValue(a.channel[i]);
        break;
      case 4:
        Channel4.setValue(a.channel[i]);
        break;
      case 5:
        Channel5.setValue(a.channel[i]);
        break;
      case 6:
        Channel6.setValue(a.channel[i]);
        break;
      case 7:
        Channel7.setValue(a.channel[i]);
        break;
      case 8:
        Channel8.setValue(a.channel[i]);
        break;
      case 9:
        Channel9.setValue(a.channel[i]);
        break;
      case 10:
        Channel10.setValue(a.channel[i]);
        break;
      case 11:
        Channel11.setValue(a.channel[i]);
        break;
      case 12:
        Channel12.setValue(a.channel[i]);
        break;
      case 13:
        Channel13.setValue(a.channel[i]);
        break;
      case 14:
        Channel14.setValue(a.channel[i]);
        break;
      case 15:
        Channel15.setValue(a.channel[i]);
        break;
    }
  }

}

//void
//av::daemon::DeviceSensor::updateLEDs()
//{
//  for (int i=0; i<sMaxLEDs; ++i)
//  {
//    std::stringstream fieldname;
//    fieldname << "LED" << i;
//    SingleField<bool>* sf = dynamic_cast<SingleField<bool>*>(getField(fieldname.str()));
//
//    if ( (sf != 0) && (getLED(i) != sf->getValue()) )
//      sf->setValue(getLED(i));
//  }
//}
void av::vrpn::Wiimote::handleButtonTemplFunc(const vrpn_BUTTONCB b)
{
  int buttonNumber = b.button;
  switch (buttonNumber)
  {
    case 0:
      Button0.setValue(b.state);
      break;
    case 1:
      Button1.setValue(b.state);
      break;
    case 2:
      Button2.setValue(b.state);
      break;
    case 3:
      Button3.setValue(b.state);
      break;
    case 4:
      Button4.setValue(b.state);
      break;
    case 5:
      Button5.setValue(b.state);
      break;
    case 6:
      Button6.setValue(b.state);
      break;
    case 7:
      Button7.setValue(b.state);
      break;
    case 8:
      Button8.setValue(b.state);
      break;
    case 9:
      Button9.setValue(b.state);
      break;
    case 10:
      Button10.setValue(b.state);
      break;
    case 11:
      Button11.setValue(b.state);
      break;
    case 12:
      Button12.setValue(b.state);
      break;
    case 13:
      Button13.setValue(b.state);
      break;
    case 14:
      Button14.setValue(b.state);
      break;
    case 15:
      Button15.setValue(b.state);
      break;

  }

}

void av::vrpn::Wiimote::openVRPNConnectionsTemplateFunc()
{
  mVRPNAnalogOutput = boost::shared_ptr<vrpn_Analog_Output_Remote>(
      new vrpn_Analog_Output_Remote(mVRPNID.c_str()));
}

void av::vrpn::Wiimote::closeVRPNConnectionsTemplateFunc()
{
}

void av::vrpn::Wiimote::triggerVRPNMainLoopTemplateFunc()
{
  mVRPNAnalogOutput->mainloop();
}

void av::vrpn::Wiimote::getRumbleCB(const av::SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = mRumbling;
}

void av::vrpn::Wiimote::setRumbleCB(const av::SFBool::SetValueEvent& event)
{
  const bool rumbling = (event.getValue());
//  std::cout << "Rumbling: " << rumbling << std::endl;

  update_channel(RUMBLE_CHANNEL, rumbling);
}

void av::vrpn::Wiimote::getLED3CB(const av::SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = true;
}

void av::vrpn::Wiimote::setLED3CB(const av::SFBool::SetValueEvent& event)
{
  const bool ledOn = (event.getValue());
//  std::cout << "LedOn: " << ledOn << std::endl;
  setLeds(4,ledOn);
}

bool av::vrpn::Wiimote::update_channel(const vrpn_int32 channel,
    const bool value)
{
  vrpn_float64 channel_val = (value ? 1.0 : 0.0);
  return mVRPNAnalogOutput->request_change_channel_value(channel, channel_val);
}

bool av::vrpn::Wiimote::setLeds(int leds, bool on)
{
  vrpn_int32 channel = 0;
  switch (leds)
  {
    case 1:
      channel = VRPN_WIIMOTE_CHANNEL_LED_1;
      break;
    case 2:
      channel = VRPN_WIIMOTE_CHANNEL_LED_2;
      break;
    case 3:
      channel = VRPN_WIIMOTE_CHANNEL_LED_3;
      break;
    case 4:
      channel = VRPN_WIIMOTE_CHANNEL_LED_4;
      break;
    default:
      return false;
  }
  std::cout << "LED: " << leds << " (channel=" << channel << ")\n";
  return update_channel(channel, on);
}

bool av::vrpn::Wiimote::set_mode(const int mode, bool value)
{
  switch (mode)
  {
    case VRPN_WIIMOTE_CHANNEL_MODE_IR:
    case VRPN_WIIMOTE_CHANNEL_MODE_MOTION_SENSE:
      return update_channel(mode, value);
    default:
      return false;
  }
}

