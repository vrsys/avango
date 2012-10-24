#if !defined(AVANGO_WIIMOTE_H)
#define AVANGO_WIIMOTE_H

#define VRPN_WIIMOTE_NUM_CHANNELS (7)
#define VRPN_WIIMOTE_CHANNEL_RUMBLE (0)
#define VRPN_WIIMOTE_CHANNEL_LED_1 (1)
#define VRPN_WIIMOTE_CHANNEL_LED_2 (2)
#define VRPN_WIIMOTE_CHANNEL_LED_3 (3)
#define VRPN_WIIMOTE_CHANNEL_LED_4 (4)
#define VRPN_WIIMOTE_CHANNEL_MODE_IR (5)
#define VRPN_WIIMOTE_CHANNEL_MODE_MOTION_SENSE (6)

#include <avango/FieldContainer.h>
#include "Device.h"

#include <vrpn_Analog_Output.h>

#include "windows_specific_vrpn.h"

namespace av {
namespace vrpn {

class AV_VRPN_DLL Wiimote : public Device
{
  AV_FC_DECLARE();

public:

  Wiimote();

protected:

  /**
   * Destructor made protected to prevent allocation on stack.
   */
  virtual ~Wiimote();

public:
  //Buttons
  //    button[0] = Home
  //    button[1] = "1"
  //    button[2] = "2"
  //    button[3] = "A"
  //    button[4] = "B"
  //    button[5] = "-"
  //    button[6] = "+"
  //    button[7] = direction pad: left
  //    button[8] = direction pad: right
  //    button[9] = direction pad: down
  //    button[10] = direction pad: up
  //    button[11] = WIIMOTE_BUTTON_ZACCEL_BIT4
  //    button[12] = WIIMOTE_BUTTON_ZACCEL_BIT5
  //    button[13] = WIIMOTE_BUTTON_ZACCEL_BIT6
  //    button[14] = WIIMOTE_BUTTON_ZACCEL_BIT7
  //    button[15] = WIIMOTE_BUTTON_UNKNOWN
  SFBool Button0;
  SFBool Button1;
  SFBool Button2;
  SFBool Button3;
  SFBool Button4;
  SFBool Button5;
  SFBool Button6;
  SFBool Button7;
  SFBool Button8;
  SFBool Button9;
  SFBool Button10;
  SFBool Button11;
  SFBool Button12;
  SFBool Button13;
  SFBool Button14;
  SFBool Button15;

  // The Analogs are in an even more random order, both from the primary controller:
  //    channel[0] = battery level (0-1)
  //    channel[1] = gravity X vector calculation (1 = Earth gravity)
  //    channel[2] = gravity Y vector calculation (1 = Earth gravity)
  //    channel[3] = gravity Z vector calculation (1 = Earth gravity)
  //    channel[4] = X of first sensor spot (0-1023, -1 if not seen)
  //    channel[5] = Y of first sensor spot (0-767, -1 if not seen)
  //    channel[6] = size of first sensor spot (0-15, -1 if not seen)
  //    channel[7] = X of second sensor spot (0-1023, -1 if not seen)
  //    channel[9] = Y of second sensor spot (0-767, -1 if not seen)
  //    channel[9] = size of second sensor spot (0-15, -1 if not seen)
  //    channel[10] = X of third sensor spot (0-1023, -1 if not seen)
  //    channel[11] = Y of third sensor spot (0-767, -1 if not seen)
  //    channel[12] = size of third sensor spot (0-15, -1 if not seen)
  //    channel[13] = X of fourth sensor spot (0-1023, -1 if not seen)
  //    channel[14] = Y of fourth sensor spot (0-767, -1 if not seen)
  //    channel[15] = size of fourth sensor spot (0-15, -1 if not seen)
  SFFloat Channel0;
  SFFloat Channel1;
  SFFloat Channel2;
  SFFloat Channel3;
  SFFloat Channel4;
  SFFloat Channel5;
  SFFloat Channel6;
  SFFloat Channel7;
  SFFloat Channel8;
  SFFloat Channel9;
  SFFloat Channel10;
  SFFloat Channel11;
  SFFloat Channel12;
  SFFloat Channel13;
  SFFloat Channel14;
  SFFloat Channel15;

  /**
   * Set LEDs of the connected Wiimote.
   */
  SFBool LED0;
  SFBool LED1;
  SFBool LED2;
  SFBool LED3;

  //use this input field to control the rumbling of the wiimote
  SFBool Rumble;

protected:

  virtual void triggerVRPNMainLoopTemplateFunc();
  virtual void openVRPNConnectionsTemplateFunc();
  virtual void closeVRPNConnectionsTemplateFunc();

  virtual void handleButtonTemplFunc(const vrpn_BUTTONCB b);
  virtual void handleAnalogTemplFunc(const vrpn_ANALOGCB a);

  virtual void getRumbleCB(const av::SFBool::GetValueEvent& event);
  virtual void setRumbleCB(const av::SFBool::SetValueEvent& event);

  virtual void getLED3CB(const av::SFBool::GetValueEvent& event);
  virtual void setLED3CB(const av::SFBool::SetValueEvent& event);

private:

  bool setLeds(int leds, bool on);
  bool set_mode(const int mode, bool value);
  bool update_channel(const vrpn_int32 channel, const bool value);

  boost::shared_ptr<vrpn_Analog_Output_Remote> mVRPNAnalogOutput;

  bool mRumbling;

  const static int RUMBLE_CHANNEL = 0;
  const static float RUMBLE_ON_VAL = 1.0f;
  const static float RUMBLE_OFF_VAL = 0.0f;

};

typedef SingleField<Link<Wiimote> > SFWiimote;
typedef MultiField<Link<Wiimote> > MFWiimote;
}

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_VRPN_DLL SingleField<Link<av:vrpn::Wiimote> >;
template class AV_VRPN_DLL MultiField<Link<av:vrpn::Wiimote> >;
#endif

}

#endif
