import avango.daemon
import os
import sys


## Initializes a X-Box controller for navigation.
# @param PLAYER_NUMBER A number from 1 to 4 indicating which of the four possible inputs to use.
def xbox_controller(PLAYER_NUMBER):

 _query = "/opt/avango/avango-applications/list-ev -s | grep \"Xbox 360 Wireless Receiver\" | sed -e \'s/\"//g\'  | cut -d\" \" -f4"
 
 # Grab one of the four controllers according to PLAYER_NUMBER  
 for i in range(1, PLAYER_NUMBER):
   _query = _query + " | sed -n \'1!p\'"

 _string = os.popen(_query).read()
 _string = _string.split()

 if len(_string) > 0:
   _string = _string[0]
   
   # create a station to propagate the input events
   _xbox = avango.daemon.HIDInput()
   _xbox.station = avango.daemon.Station('device-xbox-' + str(PLAYER_NUMBER))
   _xbox.device = _string
   
   _xbox.values[0] = "EV_ABS::ABS_X"         # left joystick
   _xbox.values[1] = "EV_ABS::ABS_Y"         # left joystick
   _xbox.values[2] = "EV_ABS::ABS_RX"        # right joystick
   _xbox.values[3] = "EV_ABS::ABS_RY"        # right joystick

   _xbox.values[4] = "EV_ABS::ABS_Z"         # left bumper
   _xbox.values[5] = "EV_ABS::ABS_RZ"        # right bumper

   _xbox.buttons[0] = "EV_KEY::BTN_X"        # Button X
   _xbox.buttons[1] = "EV_KEY::BTN_B"        # Button B
   _xbox.buttons[2] = "EV_KEY::BTN_A"        # Button A
   _xbox.buttons[3] = "EV_KEY::BTN_Y"        # Button Y

   _xbox.buttons[4] = "EV_KEY::BTN_START"    # Start button
   _xbox.buttons[5] = "EV_KEY::BTN_SELECT"   # Select button

   device_list.append(_xbox)
   
   print("XBox Controller " + str(PLAYER_NUMBER) + " started at:", _string)
   
 else:
   print("XBox Controller NOT found !")

device_list = []
xbox_controller(1)
xbox_controller(2)
avango.daemon.run(device_list)