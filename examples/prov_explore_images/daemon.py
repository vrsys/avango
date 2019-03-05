#!/usr/bin/python

## @file
# avango daemon to initialize tracking and device stations.

# import avango-guacamole libraries
import avango.daemon

# import python libraries
import os
import sys



## Initializes AR Track on LCD wall.
def init_lcd_wall_tracking():

    # create instance of DTrack
    _dtrack = avango.daemon.DTrack()
    _dtrack.port = "5000" # ART port at LCD wall

    #_dtrack.stations[5] = avango.daemon.Station('tracking-lcd-glasses-1')
    _dtrack.stations[2] = avango.daemon.Station('tracking-lcd-glasses-1') # circular polarisation glasses
    _dtrack.stations[5] = avango.daemon.Station('tracking-lcd-glasses-2') # linear polarisation glasses
    _dtrack.stations[10] = avango.daemon.Station('tracking-lcd-glasses-3') # Mitsubishi 3D-TV glasses


    ## devices
    _dtrack.stations[19] = avango.daemon.Station('tracking-lcd-cam') # new camera shutter

    #_dtrack.stations[6] = avango.daemon.Station('tracking-old-spheron') # old spheron device
        
    _dtrack.stations[1] = avango.daemon.Station('tracking-lcd-pointer-1') # AUGUST pointer
    _dtrack.stations[11] = avango.daemon.Station('tracking-lcd-pointer-2') # pointer orange
    _dtrack.stations[27] = avango.daemon.Station('tracking-lcd-pointer-3') # has pointer
    _dtrack.stations[23] = avango.daemon.Station('tracking-lcd-pointer-4') # gunpointer


    _dtrack.stations[25] = avango.daemon.Station('tracking-lcd-latency-rotator')

    _dtrack.stations[17] = avango.daemon.Station('tracking-lcd-calibration') # calibration tool

    _dtrack.stations[28] = avango.daemon.Station('tracking-lcd-portal-camera-31')
    _dtrack.stations[29] = avango.daemon.Station('tracking-lcd-portal-camera-32')


    device_list.append(_dtrack)
    print("ART Tracking started at LCD WALL")



## Initializes AR Track on DLP wall.
def init_dlp_wall_tracking():

    # create instance of DTrack
    _dtrack = avango.daemon.DTrack()
    _dtrack.port = "5002" # ART port at LED wall

    # glasses
    _dtrack.stations[31] = avango.daemon.Station('tracking-dlp-glasses-n_1')
    _dtrack.stations[32] = avango.daemon.Station('tracking-dlp-glasses-2_1')
    _dtrack.stations[33] = avango.daemon.Station('tracking-dlp-glasses-3_1')

    _dtrack.stations[1] = avango.daemon.Station('tracking-dlp-glasses-3_2')
    _dtrack.stations[2] = avango.daemon.Station('tracking-dlp-glasses-4_2')
    _dtrack.stations[5] = avango.daemon.Station('tracking-dlp-glasses-6_2')
    
    _dtrack.stations[36] = avango.daemon.Station('tracking-dlp-glasses-1_3')


    _dtrack.stations[40] = avango.daemon.Station('tracking-dlp-glasses-A')
    _dtrack.stations[41] = avango.daemon.Station('tracking-dlp-glasses-B')
    _dtrack.stations[42] = avango.daemon.Station('tracking-dlp-glasses-C')
    _dtrack.stations[43] = avango.daemon.Station('tracking-dlp-glasses-D')
    _dtrack.stations[44] = avango.daemon.Station('tracking-dlp-glasses-E')
    _dtrack.stations[45] = avango.daemon.Station('tracking-dlp-glasses-F')
    _dtrack.stations[46] = avango.daemon.Station('tracking-dlp-glasses-G')
    _dtrack.stations[47] = avango.daemon.Station('tracking-dlp-glasses-H')
    _dtrack.stations[48] = avango.daemon.Station('tracking-dlp-glasses-I')
    _dtrack.stations[49] = avango.daemon.Station('tracking-dlp-glasses-J')


    ## devices
    _dtrack.stations[19] = avango.daemon.Station('tracking-new-spheron')       # new spheron device

    #_dtrack.stations[23] = avango.daemon.Station('tracking-dlp-pointer-1')      # AUGUST pointer (red)
    _dtrack.stations[7] = avango.daemon.Station('tracking-dbl-pointer-1') # gunpointer2
    _dtrack.stations[16] = avango.daemon.Station('tracking-dlp-pointer-2')      # Speedlink (yellow)
    _dtrack.stations[24] = avango.daemon.Station('tracking-dlp-pointer-3')      # HAS pointer (blue)
    _dtrack.stations[30] = avango.daemon.Station('tracking-dlp-pointer-4')      # BUW pointer
    _dtrack.stations[37] = avango.daemon.Station('tracking-dlp-pointer-5')      # gunpointer (green)
    _dtrack.stations[24] = avango.daemon.Station('tracking-dlp-pointer-6')      # Canon pointer
    
    _dtrack.stations[20] = avango.daemon.Station('tracking-dlp-calibration') # calibration target

    _dtrack.stations[25] = avango.daemon.Station('tracking-dlp-portal-camera-31')  # portal camera 3.1
    _dtrack.stations[26] = avango.daemon.Station('tracking-dlp-portal-camera-32')  # portal camera 3.2
    

    _dtrack.stations[35] = avango.daemon.Station('tracking-dlp-video-camera')     # new video camera target

    _dtrack.stations[13] = avango.daemon.Station('tracking-dlp-lht-1') # LHT1
    #_dtrack.stations[14] = avango.daemon.Station('tracking-dlp-lht-2') # LHT2

    _dtrack.stations[38] = avango.daemon.Station('tracking-dlp-latency-rotator')

    _dtrack.stations[8] = avango.daemon.Station('tracking-dlp-prop-1') # wooden prop (phone-shape)

    device_list.append(_dtrack)
    print("ART Tracking started at DLP WALL")


## Initializes AR Track in the DBL.
def init_dbl_tracking():

    # create instance of DTrack
    _dtrack = avango.daemon.DTrack()
    _dtrack.port = "5005" # ART port at LCD wall

    #_dtrack.stations[11] = avango.daemon.Station('tracking-dbl-glasses-3_1')
    #_dtrack.stations[10] = avango.daemon.Station('tracking-dbl-glasses-4_1')
    
    _dtrack.stations[5] = avango.daemon.Station('tracking-dbl-glasses-3_2')
    _dtrack.stations[4] = avango.daemon.Station('tracking-dbl-glasses-4_2') 
    _dtrack.stations[6] = avango.daemon.Station('tracking-dbl-glasses-6_2')

    _dtrack.stations[11] = avango.daemon.Station('tracking-dbl-glasses-3_1')
    _dtrack.stations[1] = avango.daemon.Station('tracking-dbl-glasses-1_3')

    _dtrack.stations[22] = avango.daemon.Station('tracking-dbl-glasses-A')
    _dtrack.stations[23] = avango.daemon.Station('tracking-dbl-glasses-B')
    _dtrack.stations[24] = avango.daemon.Station('tracking-dbl-glasses-C')
    _dtrack.stations[25] = avango.daemon.Station('tracking-dbl-glasses-D')
    _dtrack.stations[26] = avango.daemon.Station('tracking-dbl-glasses-E')
    _dtrack.stations[27] = avango.daemon.Station('tracking-dbl-glasses-F')
    _dtrack.stations[28] = avango.daemon.Station('tracking-dbl-glasses-G')
    _dtrack.stations[29] = avango.daemon.Station('tracking-dbl-glasses-H')
    _dtrack.stations[30] = avango.daemon.Station('tracking-dbl-glasses-I')
    _dtrack.stations[31] = avango.daemon.Station('tracking-dbl-glasses-J')


    _dtrack.stations[18] = avango.daemon.Station('tracking-dbl-video-camera') # new video camera target

    ## devices
    _dtrack.stations[7] = avango.daemon.Station('tracking-dbl-pointer-1') # August pointer
    _dtrack.stations[8] = avango.daemon.Station('tracking-dbl-pointer-2') # HID-compliant Mouse HID-compliant Mouse
    _dtrack.stations[13] = avango.daemon.Station('tracking-dbl-pointer-3') # # HAS pointer (blue)
    _dtrack.stations[14] = avango.daemon.Station('tracking-dbl-pointer-4') # BUW pointer
    _dtrack.stations[20] = avango.daemon.Station('tracking-dbl-pointer-5') # gunpointer (green)
    
    _dtrack.stations[21] = avango.daemon.Station('tracking-dbl-calibration')
    
    _dtrack.stations[3] = avango.daemon.Station('tracking-dbl-lht-1') # LHT1
    _dtrack.stations[2] = avango.daemon.Station('tracking-dbl-prop-1') # wooden prop (phone-shape)

    _dtrack.stations[19] = avango.daemon.Station('tracking-dbl-portal-camera-31')
    _dtrack.stations[9] = avango.daemon.Station('tracking-dbl-portal-camera-32')

    _dtrack.stations[12] = avango.daemon.Station('tracking-new-spheron') # new spheron device



    device_list.append(_dtrack)
    print("ART tracking started at DBL wall")



'''
def init_pst_tracking():

    # create instance of DTrack
    _pst = avango.daemon.DTrack()
    _pst.port = "5020" # PST port

    _pst.stations[2] = avango.daemon.Station('tracking-pst-head')

    device_list.append(_pst)

    print("PST Tracking started!")
'''


'''
## Initizialies Oculus Rift sensors.
def init_oculus():
  _oculus = avango.daemon.Oculus()

  _oculus.stations[0] = avango.daemon.Station('oculus-0')
  _oculus.stations[1] = avango.daemon.Station('oculus-1')
  _oculus.stations[2] = avango.daemon.Station('oculus-2')

  print("Initialized 3 Oculus Rifts")
  device_list.append(_oculus)
'''  

## Initializes touch input at the table.
def init_tuio_input():

  _tuio = avango.daemon.TUIOInput()
  _tuio.port = "3333" # tuio port

  for _i in range(0, 20):
    _tuio.stations[_i] = avango.daemon.Station("gua-finger{0}#finger".format(str(_i)))
  

  _offset = 20
  for _i in range(0, 4):
    _tuio.stations[_i + _offset] = avango.daemon.Station("gua-finger{0}#hand".format(str(_i)))

  device_list.append(_tuio)





'''
## Initializes an old spheron for navigation.
def init_old_spheron():

    ## search Spheron sensor
    _string = get_event_string(1, "BUWEIMAR RAPID DEVEL DEVICE")

    if _string is None:
        print("Old Spheron NOT found!")

        return

    # create a station to propagate the input events
    _spheron = avango.daemon.HIDInput()
    _spheron.station = avango.daemon.Station("device-old-spheron")
    _spheron.device = _string
    _spheron.timeout = '30'
    
    # map incoming spheron events to station values
    _spheron.values[0] = "EV_ABS::ABS_X"   # trans X    
    _spheron.values[1] = "EV_ABS::ABS_Y"   # trans Y
    _spheron.values[2] = "EV_ABS::ABS_Z"   # trans Z
    _spheron.values[3] = "EV_ABS::ABS_RX"  # rotate X
    _spheron.values[4] = "EV_ABS::ABS_RY"  # rotate Y
    _spheron.values[5] = "EV_ABS::ABS_RZ"  # rotate Z
    
    device_list.append(_spheron)
    print("Old Spheron at event:", _spheron.device, "started as station:", _spheron.station.name)


    ## search Spheron button sensor
    _string = get_event_string(1, "PIXART USB OPTICAL MOUSE")

    if _string is None:
        print("Old Spheron Buttons NOT found!")

        return

    # create a station to propagate the input events
    _spheron_buttons = avango.daemon.HIDInput()
    _spheron_buttons.station = avango.daemon.Station("device-old-spheron-buttons")
    _spheron_buttons.device = _string
    
    # map buttons
    _spheron_buttons.buttons[0] = "EV_KEY::BTN_LEFT"   # left button
    _spheron_buttons.buttons[1] = "EV_KEY::BTN_MIDDLE" # middle button 
    _spheron_buttons.buttons[2] = "EV_KEY::BTN_RIGHT"  # right button
    
    device_list.append(_spheron_buttons)
    print("Old Spheron Buttons at event:", _spheron_buttons.device, "started as station:", _spheron_buttons.station.name)
'''


## Initializes a new spheron for navigation.
def init_spheron():
    
    ## search Spheron right sensor
    _string = get_event_string(1, "BUW Spheron")

    if _string is None:
        print("Spheron (right sensor) NOT found!")

    else:
        # create a station to propagate the input events
        _spheron_right = avango.daemon.HIDInput()
        _spheron_right.station = avango.daemon.Station("device-new-spheron-right")
        _spheron_right.device = _string
        _spheron_right.timeout = '20'        

        # map incoming events to station values
        _spheron_right.values[0] = "EV_ABS::ABS_X"            # joystick trans y
        _spheron_right.values[1] = "EV_ABS::ABS_THROTTLE"     # joystick trans y  
        _spheron_right.values[2] = "EV_ABS::ABS_Y"            # joystick trans z    

        _spheron_right.values[3] = "EV_REL::REL_RX"
        _spheron_right.values[4] = "EV_REL::REL_RY"
        _spheron_right.values[5] = "EV_REL::REL_RZ"

        _spheron_right.values[6] = "EV_ABS::ABS_Z"            # joystick throttle
        
        # buttons
        _spheron_right.buttons[2] = "EV_KEY::BTN_B"           # left button
        _spheron_right.buttons[1] = "EV_KEY::BTN_A"           # right button
        _spheron_right.buttons[0] = "EV_KEY::BTN_C"           # middle button
        _spheron_right.buttons[3] = "EV_KEY::BTN_X"           # left joystick button
        _spheron_right.buttons[4] = "EV_KEY::BTN_Y"           # right joystick button

        device_list.append(_spheron_right)
        print("Spheron (right sensor) at event:", _spheron_right.device, "started as station:", _spheron_right.station.name)



    ## search Spheron left sensor
    _string = get_event_string(2, "BUW Spheron")

    if _string is None:
        print("Spheron (left sensor) NOT found!")

    else:
        # create a station to propagate the input events
        _spheron_left = avango.daemon.HIDInput()
        _spheron_left.station = avango.daemon.Station("device-new-spheron-left")
        _spheron_left.device = _string
        _spheron_left.timeout = '20'

        # map incoming events to station values
        _spheron_left.values[0] = "EV_ABS::ABS_X"            # joystick trans x
        _spheron_left.values[1] = "EV_ABS::ABS_THROTTLE"     # joystick trans y
        _spheron_left.values[2] = "EV_ABS::ABS_Y"            # joystick trans z

        _spheron_left.values[3] = "EV_REL::REL_RX" 
        _spheron_left.values[4] = "EV_REL::REL_RY"
        _spheron_left.values[5] = "EV_REL::REL_RZ"

        _spheron_left.values[6] = "EV_ABS::ABS_Z"            # joystick throttle

        # buttons
        _spheron_left.buttons[0] = "EV_KEY::BTN_B"           # left button
        _spheron_left.buttons[1] = "EV_KEY::BTN_A"           # right button
        _spheron_left.buttons[2] = "EV_KEY::BTN_C"           # middle button
        _spheron_left.buttons[3] = "EV_KEY::BTN_X"           # left joystick button
        _spheron_left.buttons[4] = "EV_KEY::BTN_Y"           # right joystick button    

        device_list.append(_spheron_left)
        print("Spheron (left sensor) at event:", _spheron_left.device, "started as station:", _spheron_left.station.name)



'''
## Initializes a new spheron for navigation.
def init_new_globefish():

  _string = os.popen("python find_device.py 1 BUW Spheron").read()
  _string = _string.split()

  if len(_string) > 0:
    
    _string1 = _string[0]

    # create a station to propagate the input events
    _globefish = avango.daemon.HIDInput()
    _globefish.station = avango.daemon.Station("device-new-globefish")
    _globefish.device = _string1
    _globefish.timeout = '30'
    
    # map incoming events to station values
    _globefish.values[0] = "EV_ABS::ABS_THROTTLE" # X
    _globefish.values[1] = "EV_ABS::ABS_Z" # Y    
    _globefish.values[2] = "EV_ABS::ABS_X" # Z
    
    _globefish.values[3] = "EV_REL::REL_RY" # PITCH
    _globefish.values[4] = "EV_REL::REL_RX" # HEAD 
    _globefish.values[5] = "EV_REL::REL_RZ" # ROLL
    
    # buttons
    # ...
        
    device_list.append(_globefish)

    print("New Globefish found at:", _string1)

  else:
    print("New Globefish NOT found !")
'''  

def init_griffin(EVENT_STRING, STATION_STRING):

    if EVENT_STRING is None:
        print("Griffin NOT found!")        
        return

    _button_mappings = []
    _button_mappings.append( (0, "EV_KEY::BTN_0") )

    init_hid_device(EVENT_STRING = EVENT_STRING, STATION_STRING = STATION_STRING, BUTTON_MAPPINGS = _button_mappings)



def init_spacemouse(EVENT_STRING, STATION_STRING, REL_FLAG = True):

    if EVENT_STRING is None:
        print("SpaceMouse NOT found!")        
        return

    _value_mappings = []
    _timeout = None

    if REL_FLAG == True: # spacemouse with relative input (blue LED)
        _value_mappings.append( (0, "EV_REL::REL_X") )
        _value_mappings.append( (1, "EV_REL::REL_Z") )
        _value_mappings.append( (2, "EV_REL::REL_Y") )
        _value_mappings.append( (3, "EV_REL::REL_RX") )
        _value_mappings.append( (4, "EV_REL::REL_RZ") )
        _value_mappings.append( (5, "EV_REL::REL_RY") )

        _timeout = '14' # better !

    else: # spacemouse with absolute input (old ones)
        _value_mappings.append( (0, "EV_ABS::ABS_X") )
        _value_mappings.append( (1, "EV_ABS::ABS_Z") )
        _value_mappings.append( (2, "EV_ABS::ABS_Y") )
        _value_mappings.append( (3, "EV_ABS::ABS_RX") )
        _value_mappings.append( (4, "EV_ABS::ABS_RZ") )
        _value_mappings.append( (5, "EV_ABS::ABS_RY") )



    _button_mappings = []

    _button_mappings.append( (1, "EV_KEY::BTN_0") ) # left button
    _button_mappings.append( (2, "EV_KEY::BTN_1") ) # right button


    init_hid_device(EVENT_STRING = EVENT_STRING, STATION_STRING = STATION_STRING, VALUE_MAPPINGS = _value_mappings, BUTTON_MAPPINGS = _button_mappings, TIMEOUT = _timeout)

    


def search_and_init_mouse(STATION_STRING):

    _device_list = ["Logitech USB-PS/2 Optical Mouse", "Logitech Wireless Mouse M560", "Logitech USB-PS/2 Optical Mouse", "Logitech USB Optical Mouse", "Dell Dell USB Optical Mouse", "USB Optical Mouse", "PixArt USB Optical Mouse"]

    for _device in _device_list:
        _string = get_event_string(1, _device)

        if _string is not None:
            init_mouse(_string, STATION_STRING)
            break


def search_and_init_two_mice():

    _device_list = ["Logitech USB-PS/2 Optical Mouse", "Logitech USB Optical Mouse", "Dell Dell USB Optical Mouse", "USB Optical Mouse", "PixArt USB Optical Mouse"]

    _counter = 0
    for _device in _device_list:
        _string = get_event_string(1, _device)

        if _string is not None:
            init_mouse(_string, "device-mouse" + str(_counter))
            _counter += 1

            if _counter == 2:
                break
    


## Initalizes a mouse.
def init_mouse(EVENT_STRING, STATION_STRING):    
    if EVENT_STRING is None: # guard
        print("Mouse NOT found!")
        return

    _value_mappings = []
    _value_mappings.append( (0, "EV_REL::REL_X") )
    _value_mappings.append( (1, "EV_REL::REL_Y") )
    _value_mappings.append( (2, "EV_REL::REL_WHEEL") )

    _button_mappings = []
    _button_mappings.append( (0, "EV_KEY::BTN_LEFT") )
    _button_mappings.append( (1, "EV_KEY::BTN_RIGHT") )
    _button_mappings.append( (2, "EV_KEY::BTN_MIDDLE") )
    
    init_hid_device(EVENT_STRING = EVENT_STRING, STATION_STRING = STATION_STRING, VALUE_MAPPINGS = _value_mappings, BUTTON_MAPPINGS = _button_mappings, TIMEOUT = '10')



def search_and_init_keyboard(STATION_STRING):
    _device_list = ["Cherry GmbH", "HID 046a:0011", "MOSART Semi. 2.4G Keyboard Mouse", "Logitech USB Keyboard", "DELL Dell QuietKey Keyboard"]

    for _device in _device_list:
        _string = get_event_string(1, _device)

        if _string is not None:
            #init_keyboard(_string, STATION_STRING)
            init_full_keyboard(_string, STATION_STRING)
            
            break # break loop


## Initalizes a keyboard.
def init_full_keyboard(EVENT_STRING, STATION_STRING, ):
    if EVENT_STRING is None: # guard
        print("Keyboard NOT found!")
        return

    _map = []
    _map.append( (0, "EV_KEY::KEY_A") )
    _map.append( (1, "EV_KEY::KEY_B") )
    _map.append( (2, "EV_KEY::KEY_C") )
    _map.append( (3, "EV_KEY::KEY_D") )
    _map.append( (4, "EV_KEY::KEY_E") )
    _map.append( (5, "EV_KEY::KEY_F") )
    _map.append( (6, "EV_KEY::KEY_G") )
    _map.append( (7, "EV_KEY::KEY_H") )
    _map.append( (8, "EV_KEY::KEY_I") )
    _map.append( (9, "EV_KEY::KEY_J") )
    _map.append( (10, "EV_KEY::KEY_K") )
    _map.append( (11, "EV_KEY::KEY_L") )
    _map.append( (12, "EV_KEY::KEY_M") )
    _map.append( (13, "EV_KEY::KEY_N") )
    _map.append( (14, "EV_KEY::KEY_O") )
    _map.append( (15, "EV_KEY::KEY_P") )
    _map.append( (16, "EV_KEY::KEY_Q") )
    _map.append( (17, "EV_KEY::KEY_R") )
    _map.append( (18, "EV_KEY::KEY_S") )
    _map.append( (19, "EV_KEY::KEY_T") )
    _map.append( (20, "EV_KEY::KEY_U") )
    _map.append( (21, "EV_KEY::KEY_V") )
    _map.append( (22, "EV_KEY::KEY_W") )
    _map.append( (23, "EV_KEY::KEY_X") )
    _map.append( (24, "EV_KEY::KEY_Y") )
    _map.append( (25, "EV_KEY::KEY_Z") )
    _map.append( (26, "EV_KEY::KEY_SPACE") )
    _map.append( (27, "EV_KEY::KEY_ENTER") )
    _map.append( (28, "EV_KEY::KEY_LEFT") )
    _map.append( (29, "EV_KEY::KEY_RIGHT") )
    _map.append( (30, "EV_KEY::KEY_UP") )
    _map.append( (31, "EV_KEY::KEY_DOWN") )

    init_hid_device(EVENT_STRING = EVENT_STRING, STATION_STRING = STATION_STRING + "-part0", BUTTON_MAPPINGS = _map)

    _map = []
    _map.append( (0, "EV_KEY::KEY_0") )
    _map.append( (1, "EV_KEY::KEY_1") )
    _map.append( (2, "EV_KEY::KEY_2") )
    _map.append( (3, "EV_KEY::KEY_3") )
    _map.append( (4, "EV_KEY::KEY_4") )
    _map.append( (5, "EV_KEY::KEY_5") )
    _map.append( (6, "EV_KEY::KEY_6") )
    _map.append( (7, "EV_KEY::KEY_7") )
    _map.append( (8, "EV_KEY::KEY_8") )
    _map.append( (9, "EV_KEY::KEY_9") )
    _map.append( (10, "EV_KEY::KEY_F1") )
    _map.append( (11, "EV_KEY::KEY_F2") )
    _map.append( (12, "EV_KEY::KEY_F3") )
    _map.append( (13, "EV_KEY::KEY_F4") )
    _map.append( (14, "EV_KEY::KEY_F5") )
    _map.append( (15, "EV_KEY::KEY_F6") )
    _map.append( (16, "EV_KEY::KEY_F7") )
    _map.append( (17, "EV_KEY::KEY_F8") )
    _map.append( (18, "EV_KEY::KEY_F9") )
    _map.append( (19, "EV_KEY::KEY_F10") )
    _map.append( (20, "EV_KEY::KEY_F11") )
    _map.append( (21, "EV_KEY::KEY_F12") )
    _map.append( (22, "EV_KEY::KEY_INSERT") )
    _map.append( (23, "EV_KEY::KEY_DELETE") )
    _map.append( (24, "EV_KEY::KEY_HOME") )
    _map.append( (25, "EV_KEY::KEY_END") )
    _map.append( (26, "EV_KEY::KEY_PAGEUP") )
    _map.append( (27, "EV_KEY::KEY_PAGEDOWN") )

    init_hid_device(EVENT_STRING = EVENT_STRING, STATION_STRING = STATION_STRING + "-part1", BUTTON_MAPPINGS = _map)


## Initializes a keypad.
def init_keypad(EVENT_STRING, STATION_STRING):
    if EVENT_STRING is None:
        print("Keypad NOT found!")
        return

    _button_mappings = []
    _button_mappings.append( (10, "EV_KEY::KEY_KP1") ) # scene 1
    _button_mappings.append( (11, "EV_KEY::KEY_KP2") ) # scene 2
    _button_mappings.append( (12, "EV_KEY::KEY_KP3") ) # scene 3
    _button_mappings.append( (13, "EV_KEY::KEY_KP4") ) # scene 4

    _button_mappings.append( (18, "EV_KEY::KEY_ENTER") ) # reset navigation matrix
    _button_mappings.append( (8, "EV_KEY::KEY_KPDOT") ) # video avatar visibility
    _button_mappings.append( (3, "EV_KEY::KEY_KPSLASH") ) # normal visualization
    _button_mappings.append( (1, "EV_KEY::KEY_KPASTERISK") ) # node highlight
    _button_mappings.append( (0, "EV_KEY::KEY_BACKSPACE") ) # default color

    _button_mappings.append( (20, "EV_KEY::KEY_KP0") ) # toggle shading model
    _button_mappings.append( (14, "EV_KEY::KEY_KPMINUS") ) # decrease PLOD radius scale
    _button_mappings.append( (15, "EV_KEY::KEY_KPPLUS") ) # increase PLOD radius scale
    _button_mappings.append( (16, "EV_KEY::KEY_KP9") ) # decrease PLOD error threshold
    _button_mappings.append( (17, "EV_KEY::KEY_KP6") ) # increase PLOD error threshold
    _button_mappings.append( (6, "EV_KEY::KEY_KP5") ) # toggle plod rendering mode: 1-pass vs. 2-pass

    '''    
    _button_mappings.append( (25, "EV_KEY::KEY_KP7") )
    _button_mappings.append( (26, "EV_KEY::KEY_KP8") )
    '''
        
    init_hid_device(EVENT_STRING = EVENT_STRING, STATION_STRING = STATION_STRING + "-part1", BUTTON_MAPPINGS = _button_mappings)



'''
## Initalizes a keyboard.
def init_keyboard(EVENT_STRING, STATION_STRING):    
    if EVENT_STRING is None: # guard
        print("Keyboard NOT found!")
        return

    _button_mappings = []

    _button_mappings.append( (0, "EV_KEY::KEY_W") )
    _button_mappings.append( (1, "EV_KEY::KEY_A") )
    _button_mappings.append( (2, "EV_KEY::KEY_S") )
    #_button_mappings.append( (3, "EV_KEY::KEY_D") )
    _button_mappings.append( (3, "EV_KEY::KEY_DELETE") )    
    _button_mappings.append( (4, "EV_KEY::KEY_R") )
    _button_mappings.append( (5, "EV_KEY::KEY_Q") )
    _button_mappings.append( (6, "EV_KEY::KEY_L") )
    _button_mappings.append( (7, "EV_KEY::KEY_UP") )
    _button_mappings.append( (8, "EV_KEY::KEY_DOWN") )
    _button_mappings.append( (9, "EV_KEY::KEY_0") )
    _button_mappings.append( (10, "EV_KEY::KEY_1") )
    _button_mappings.append( (11, "EV_KEY::KEY_2") )
    _button_mappings.append( (12, "EV_KEY::KEY_3") )
    _button_mappings.append( (13, "EV_KEY::KEY_4") )
    _button_mappings.append( (14, "EV_KEY::KEY_5") )
    _button_mappings.append( (15, "EV_KEY::KEY_6") )
    _button_mappings.append( (16, "EV_KEY::KEY_7") )
    _button_mappings.append( (17, "EV_KEY::KEY_8") )
    _button_mappings.append( (18, "EV_KEY::KEY_9") )
    _button_mappings.append( (19, "EV_KEY::KEY_F1") )
    _button_mappings.append( (20, "EV_KEY::KEY_F2") )
    _button_mappings.append( (21, "EV_KEY::KEY_F3") )
    _button_mappings.append( (22, "EV_KEY::KEY_F4") )
    _button_mappings.append( (23, "EV_KEY::KEY_F5") )
    _button_mappings.append( (24, "EV_KEY::KEY_F6") )
    _button_mappings.append( (25, "EV_KEY::KEY_F7") )
    _button_mappings.append( (26, "EV_KEY::KEY_F8") )
    _button_mappings.append( (27, "EV_KEY::KEY_F9") )
    _button_mappings.append( (28, "EV_KEY::KEY_F10") )
    _button_mappings.append( (29, "EV_KEY::KEY_F11") )
    _button_mappings.append( (30, "EV_KEY::KEY_F12") )
    _button_mappings.append( (31, "EV_KEY::KEY_SPACE") )

    init_hid_device(EVENT_STRING = EVENT_STRING, STATION_STRING = STATION_STRING, BUTTON_MAPPINGS = _button_mappings)

'''    


def init_xbox_controller(EVENT_STRING, STATION_STRING):

    if EVENT_STRING is None:
        print("Xbox-Controller NOT found!")
        return


    _value_mappings = []

    _value_mappings.append( (0, "EV_ABS::ABS_X") ) # left joystick
    _value_mappings.append( (1, "EV_ABS::ABS_Y") ) # left joystick
    _value_mappings.append( (2, "EV_ABS::ABS_RX") ) # right joystick
    _value_mappings.append( (3, "EV_ABS::ABS_RY") ) # right joystick
    _value_mappings.append( (4, "EV_ABS::ABS_Z") ) # left bumper
    _value_mappings.append( (5, "EV_ABS::ABS_RZ") ) # right bumper

    _button_mappings = []

    _button_mappings.append( (0, "EV_KEY::BTN_X") )
    _button_mappings.append( (1, "EV_KEY::BTN_B") )
    _button_mappings.append( (2, "EV_KEY::BTN_A") )
    _button_mappings.append( (3, "EV_KEY::BTN_Y") )
    _button_mappings.append( (4, "EV_KEY::BTN_TL") )
    _button_mappings.append( (5, "EV_KEY::BTN_TR") )
    _button_mappings.append( (6, "EV_KEY::BTN_TRIGGER_HAPPY1") ) # cross left
    _button_mappings.append( (7, "EV_KEY::BTN_TRIGGER_HAPPY2") ) # cross right
    _button_mappings.append( (8, "EV_KEY::BTN_TRIGGER_HAPPY3") ) # cross top
    _button_mappings.append( (9, "EV_KEY::BTN_TRIGGER_HAPPY4") ) # cross bottom


    init_hid_device(EVENT_STRING = EVENT_STRING, STATION_STRING = STATION_STRING, VALUE_MAPPINGS = _value_mappings, BUTTON_MAPPINGS = _button_mappings)


## Initializes a portal camera for portal features.
def init_portal_camera(EVENT_STRING, STATION_STRING):

    if EVENT_STRING is None:
        print("Portal-Camera NOT found!")
        return


    _button_mappings = []

    _button_mappings.append( (0, "EV_KEY::BTN_START") )     # trigger button half step
    _button_mappings.append( (1, "EV_KEY::BTN_MODE") )      # trigger button full step
    _button_mappings.append( (2, "EV_KEY::BTN_X") )         # top left of trigger
    _button_mappings.append( (3, "EV_KEY::BTN_C") )         # top right of trigger
    _button_mappings.append( (4, "EV_KEY::BTN_TL") )        # top left button left
    _button_mappings.append( (5, "EV_KEY::BTN_Y") )         # top left button right
    _button_mappings.append( (6, "EV_KEY::BTN_Z") )         # top left button center
    _button_mappings.append( (7, "EV_KEY::BTN_TR2") )       # left thumb left button
    _button_mappings.append( (8, "EV_KEY::BTN_SELECT") )    # left thumb right button
    _button_mappings.append( (9, "EV_KEY::BTN_B") )         # top right button top
    _button_mappings.append( (10, "EV_KEY::BTN_BUW1") )     # top right button bottom
    _button_mappings.append( (11, "EV_KEY::BTN_A") )        # top right button center
    _button_mappings.append( (12, "EV_KEY::BTN_THUMBR") )   # right thumb left button
    _button_mappings.append( (13, "EV_KEY::BTN_THUMBL") )   # right thumb right button
    _button_mappings.append( (14, "EV_KEY::BTN_TL2") )      # left and right head button
    _button_mappings.append( (15, "EV_KEY::BTN_TR") )       # center head button


    init_hid_device(EVENT_STRING = EVENT_STRING, STATION_STRING = STATION_STRING, BUTTON_MAPPINGS = _button_mappings)

    

def init_hid_device(EVENT_STRING, STATION_STRING, VALUE_MAPPINGS = [], BUTTON_MAPPINGS = [], TIMEOUT = None):

    if EVENT_STRING is None or STATION_STRING is None: # guard
        return

    # create a station to propagate the input events
    _device = avango.daemon.HIDInput()
    _device.station = avango.daemon.Station(STATION_STRING) # create a station to propagate the input events
    _device.device = EVENT_STRING


    for _tupel in VALUE_MAPPINGS:
        _id = _tupel[0]
        _string = _tupel[1]
    
        _device.values[_id] = _string

        #print(_id, _string)


    for _tupel in BUTTON_MAPPINGS:
        _id = _tupel[0]
        _string = _tupel[1]
    
        _device.buttons[_id] = _string

        #print(_id, _string)


    if TIMEOUT is not None:
        _device.timeout = TIMEOUT

    device_list.append(_device)
    print("Device at event:", _device.device, "started as station:", _device.station.name)


def init_mouse2():

    _device_list = ["Logitech USB-PS/2 Optical Mouse", "Logitech Wireless Mouse M560"]

    for _device in _device_list:
        _string = get_event_string(1, _device)

        if _string is not None:
            mouse = avango.daemon.HIDInput()
            mouse.station = avango.daemon.Station("gua-device-mouse")
            mouse.device = _string

            mouse.values[0] = "EV_REL::REL_X"
            mouse.values[1] = "EV_REL::REL_Y"

            mouse.buttons[0] = "EV_KEY::BTN_LEFT"
            mouse.buttons[1] = "EV_KEY::BTN_RIGHT"

            device_list.append(mouse)
            print("Mouse started at:", mouse.device)

            break

    print("Mouse not found")

def init_keyboard2():
    _device_list = ["AT Translated Set 2 keyboard"]


    for _device in _device_list:
        _string = get_event_string(1, _device)

        if _string is not None:
            keyboard = avango.daemon.HIDInput()
            keyboard.station = avango.daemon.Station(
                'gua-device-keyboard' + str(0))
            keyboard.device = _string

            keyboard.buttons[0] = "EV_KEY::KEY_Q"
            keyboard.buttons[1] = "EV_KEY::KEY_W"
            keyboard.buttons[2] = "EV_KEY::KEY_E"
            keyboard.buttons[3] = "EV_KEY::KEY_R"
            keyboard.buttons[4] = "EV_KEY::KEY_T"
            keyboard.buttons[5] = "EV_KEY::KEY_Z"
            keyboard.buttons[6] = "EV_KEY::KEY_U"
            keyboard.buttons[7] = "EV_KEY::KEY_I"
            keyboard.buttons[8] = "EV_KEY::KEY_O"
            keyboard.buttons[9] = "EV_KEY::KEY_P"
            keyboard.buttons[10] = "EV_KEY::KEY_A"
            keyboard.buttons[11] = "EV_KEY::KEY_S"
            keyboard.buttons[12] = "EV_KEY::KEY_D"
            keyboard.buttons[13] = "EV_KEY::KEY_F"
            keyboard.buttons[14] = "EV_KEY::KEY_G"
            keyboard.buttons[15] = "EV_KEY::KEY_H"
            keyboard.buttons[16] = "EV_KEY::KEY_J"
            keyboard.buttons[17] = "EV_KEY::KEY_K"
            keyboard.buttons[18] = "EV_KEY::KEY_L"
            keyboard.buttons[19] = "EV_KEY::KEY_Y"
            keyboard.buttons[20] = "EV_KEY::KEY_X"
            keyboard.buttons[21] = "EV_KEY::KEY_C"
            keyboard.buttons[22] = "EV_KEY::KEY_V"
            keyboard.buttons[23] = "EV_KEY::KEY_B"
            keyboard.buttons[24] = "EV_KEY::KEY_N"
            keyboard.buttons[25] = "EV_KEY::KEY_M"

            keyboard.buttons[26] = "EV_KEY::KEY_PAGEUP"
            keyboard.buttons[27] = "EV_KEY::KEY_PAGEDOWN"

            keyboard.buttons[28] = "EV_KEY::KEY_1"
            keyboard.buttons[29] = "EV_KEY::KEY_2"
            keyboard.buttons[30] = "EV_KEY::KEY_LEFT"
            keyboard.buttons[31] = "EV_KEY::KEY_RIGHT"

            device_list.append(keyboard)
            print("Keyboard started at:", _string)

def init_mouse():

    mouse_name = os.popen(
        "ls /dev/input/by-id | grep \"-event-mouse\" | sed -e \'s/\"//g\'  | cut -d\" \" -f4").read(
        )

    mouse_name = mouse_name.split()
    if len(mouse_name) > 0:

        mouse_name = mouse_name[0]

        mouse = avango.daemon.HIDInput()
        mouse.station = avango.daemon.Station('gua-device-mouse')
        mouse.device = "/dev/input/by-id/" + mouse_name

        mouse.values[0] = "EV_REL::REL_X"
        mouse.values[1] = "EV_REL::REL_Y"

        mouse.buttons[0] = "EV_KEY::BTN_LEFT"
        mouse.buttons[1] = "EV_KEY::BTN_RIGHT"

        device_list.append(mouse)
        print("Mouse started at:", mouse.device)

    else:
        print("Mouse NOT found !")

def init_keyboard():
    print("start KEYBOARD #################################################################")

    keyboard_name = os.popen(
        "ls /dev/input/by-id | grep \"-event-kbd\" | sed -e \'s/\"//g\'  | cut -d\" \" -f4").read(
        )

    keyboard_name = keyboard_name.split()

    for i, name in enumerate(keyboard_name):

        keyboard = avango.daemon.HIDInput()
        keyboard.station = avango.daemon.Station(
            'gua-device-keyboard' + str(i))
        keyboard.device = "/dev/input/by-id/" + name

        keyboard.buttons[0] = "EV_KEY::KEY_Q"
        keyboard.buttons[1] = "EV_KEY::KEY_W"
        keyboard.buttons[2] = "EV_KEY::KEY_E"
        keyboard.buttons[3] = "EV_KEY::KEY_R"
        keyboard.buttons[4] = "EV_KEY::KEY_T"
        keyboard.buttons[5] = "EV_KEY::KEY_Z"
        keyboard.buttons[6] = "EV_KEY::KEY_U"
        keyboard.buttons[7] = "EV_KEY::KEY_I"
        keyboard.buttons[8] = "EV_KEY::KEY_O"
        keyboard.buttons[9] = "EV_KEY::KEY_P"
        keyboard.buttons[10] = "EV_KEY::KEY_A"
        keyboard.buttons[11] = "EV_KEY::KEY_S"
        keyboard.buttons[12] = "EV_KEY::KEY_D"
        keyboard.buttons[13] = "EV_KEY::KEY_F"
        keyboard.buttons[14] = "EV_KEY::KEY_G"
        keyboard.buttons[15] = "EV_KEY::KEY_H"
        keyboard.buttons[16] = "EV_KEY::KEY_J"
        keyboard.buttons[17] = "EV_KEY::KEY_K"
        keyboard.buttons[18] = "EV_KEY::KEY_L"
        keyboard.buttons[19] = "EV_KEY::KEY_Y"
        keyboard.buttons[20] = "EV_KEY::KEY_X"
        keyboard.buttons[21] = "EV_KEY::KEY_C"
        keyboard.buttons[22] = "EV_KEY::KEY_V"
        keyboard.buttons[23] = "EV_KEY::KEY_B"
        keyboard.buttons[24] = "EV_KEY::KEY_N"
        keyboard.buttons[25] = "EV_KEY::KEY_M"

        keyboard.buttons[26] = "EV_KEY::KEY_PAGEUP"
        keyboard.buttons[27] = "EV_KEY::KEY_PAGEDOWN"

        keyboard.buttons[28] = "EV_KEY::KEY_1"
        keyboard.buttons[29] = "EV_KEY::KEY_2"
        keyboard.buttons[30] = "EV_KEY::KEY_LEFT"
        keyboard.buttons[31] = "EV_KEY::KEY_RIGHT"

        device_list.append(keyboard)
        print("Keyboard " + str(i) + " started at:", name)

## Gets the event string of a given input device.
# @param STRING_NUM Integer saying which device occurence should be returned.
# @param DEVICE_NAME Name of the input device to find the event string for.
def get_event_string(STRING_NUM, DEVICE_NAME):

    # file containing all devices with additional information
    _device_file = os.popen("cat /proc/bus/input/devices").read()
    _device_file = _device_file.split("\n")
    
    DEVICE_NAME = '\"' + DEVICE_NAME + '\"'
    
    # lines in the file matching the device name
    _indices = []

    for _i, _line in enumerate(_device_file):
        if DEVICE_NAME in _line:
            _indices.append(_i)

    # if no device was found or the number is too high, return an empty string
    if len(_indices) == 0 or STRING_NUM > len(_indices):
        return None

    # else captue the event number X of one specific device and return /dev/input/eventX
    else:
        _event_string_start_index = _device_file[_indices[STRING_NUM-1]+4].find("event")
                
        return "/dev/input/" + _device_file[_indices[STRING_NUM-1]+4][_event_string_start_index:].split(" ")[0]


## @var device_list
# List of devices to be handled by daemon.
device_list = []

### initialize tracking systems
#init_pst_tracking()
#init_lcd_wall_tracking()
#init_dlp_wall_tracking()
init_dbl_tracking()


### init navigation devices
#init_old_spheron()
init_spheron()

#init_spacemouse(get_event_string(1, "3Dconnexion SpaceNavigator"), "device-spacemouse0", REL_FLAG = True)
#init_spacemouse(get_event_string(2, "3Dconnexion SpaceNavigator"), "device-spacemouse1", REL_FLAG = True)
#init_spacemouse(get_event_string(1, "3Dconnexion SpaceNavigator REMOTE"), "device-spacemouse0", REL_FLAG = True)
#init_spacemouse(get_event_string(1, "3Dconnexion SpaceNavigator for Notebooks"), "device-spacemouse0", REL_FLAG = True)

#init_griffin(get_event_string(1, "Griffin PowerMate"), "device-griffin")

#init_new_globefish()

#init_xbox_controller(get_event_string(1, "Xbox 360 Wireless Receiver"), "device-xbox0")
#init_xbox_controller(get_event_string(1, "Xbox 360 Wireless Receiver REMOTE"), "device-xbox1")


### init pointers ###

## August
#_button_mapping = [(0, "EV_KEY::KEY_PAGEUP")] # dragging
#_button_mapping = [(0, "EV_KEY::KEY_PAGEDOWN"), (1, "EV_KEY::KEY_PAGEUP")] # dragging & reference
#_button_mapping = [(0, "EV_KEY::KEY_PAGEDOWN"), (2, "EV_KEY::KEY_PAGEUP")] # dragging & lens
#_button_mapping = [(1, "EV_KEY::KEY_PAGEDOWN"), (2, "EV_KEY::KEY_PAGEUP")] # reference & lens
#_button_mapping = [(2, "EV_KEY::KEY_PAGEUP")] # lens
#init_hid_device(EVENT_STRING = get_event_string(1, "MOUSE USB MOUSE"), STATION_STRING = "device-pointer1", BUTTON_MAPPINGS = _button_mapping) # August pointer (red)
#os.system("xinput --set-prop keyboard:'MOUSE USB MOUSE' 'Device Enabled' 0") # disable X-forwarding of events

# gunpointer 2
_button_mapping = [(0, "EV_KEY::BTN_LEFT"), (1, "EV_KEY::BTN_RIGHT")]
init_hid_device(EVENT_STRING = get_event_string(2, "2.4G KB 2.4G Mouse"), STATION_STRING = "device-pointer1", BUTTON_MAPPINGS = _button_mapping)

## Speedlink (yellow)
#_button_mapping = [(0, "EV_KEY::KEY_PAGEUP"), (1, "EV_KEY::KEY_PAGEDOWN")]
#init_hid_device(EVENT_STRING = get_event_string(1, "HID-compliant Mouse HID-compliant Mouse"), STATION_STRING = "device-pointer2", BUTTON_MAPPINGS = _button_mapping)

## HAS pointer (blue)
#_button_mapping = [(0, "EV_KEY::KEY_VOLUMEUP"), (1, "EV_KEY::KEY_PAGEUP"), (2, "EV_KEY::KEY_VOLUMEDOWN")] 
#init_hid_device(EVENT_STRING = get_event_string(1, "HAS   HAS HS304"), STATION_STRING = "device-pointer3", BUTTON_MAPPINGS = _button_mapping)

##
#_button_mapping = [(0, "EV_KEY::KEY_PAGEUP"), (1, "EV_KEY::KEY_B")]
#init_hid_device(EVENT_STRING = get_event_string(1, "MOSART Semi. Input Device"), STATION_STRING = "device-pointer3", BUTTON_MAPPINGS = _button_mapping) # MOSART pointer (orange)

##
#_button_mapping = [(0, "EV_KEY::BTN_A"), (1, "EV_KEY::BTN_B"), (2, "EV_KEY::BTN_BUW1"), (3, "EV_KEY::BTN_X"), (4, "EV_KEY::BTN_Y")]
#init_hid_device(EVENT_STRING = get_event_string(1, "WT32-A"), STATION_STRING = "device-pointer4", BUTTON_MAPPINGS = _button_mapping) # BUW pointer

## gunpointer (green)
#_button_mapping = [(0, "EV_KEY::KEY_ESC"), (1, "EV_KEY::KEY_ENTER")]
#init_hid_device(EVENT_STRING = get_event_string(1, "2.4G KB 2.4G Mouse"), STATION_STRING = "device-pointer5", BUTTON_MAPPINGS = _button_mapping)
#os.system("xinput --set-prop keyboard:'2.4G KB 21.4G Mouse' 'Device Enabled' 0") # disable X-forwarding of events

## Canon pointer for slot control
#_button_mapping = [(0, "EV_KEY::KEY_DOWN"), (1, "EV_KEY::KEY_UP")]
#init_hid_device(EVENT_STRING = get_event_string(1, "2.4G Canon USB Device"), STATION_STRING = "device-pointer6", BUTTON_MAPPINGS = _button_mapping)


### init desktop devices

# search_and_init_keyboard("device-keyboard")


#init_keypad(get_event_string(1, "USB Compliant Keypad"), "device-keypad")
# init_keypad(get_event_string(1, "AT Translated Set 2 keyboard"), "device-keypad")

# search_and_init_mouse("device-mouse0")


#search_and_init_two_mice()
# init_mouse(get_event_string(1, "Dell Dell USB Optical Mouse"), "device-mouse0")
#init_mouse(get_event_string(1, "Logitech USB-PS/2 Optical Mouse"), "device-mouse0")
#init_mouse(get_event_string(1, "Logitech USB-PS/2 Optical Mouse"), "device-mouse1")
# init_mouse(get_event_string(1, "Logitech Wireless Mouse M560"), "device-mouse1")
#init_mouse(get_event_string(1, "Logitech USB Optical Mouse"), "device-mouse1")


# uni
#init_mouse()
init_keyboard()

# laptop
init_mouse2()
init_keyboard2()



### init portal camera
#init_portal_camera(get_event_string(1, "portalCam 3.1"), "device-portal-camera-31")
#init_portal_camera(get_event_string(1, "portalCam 3.2"), "device-portal-camera-32")

### init touch input
#init_tuio_input()

avango.daemon.run(device_list)
