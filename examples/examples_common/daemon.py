import avango.daemon
import os
import sys


# functions
def init_pst_tracking():
    # create instance of DTrack
    pst = avango.daemon.DTrack()
    pst.port = "5000"  # PST port
    # pst.port = "5020" # PST port

    #pst.stations[1] = avango.daemon.Station('head')
    #pst.stations[3] = avango.daemon.Station('torch')
    #pst.stations[2] = avango.daemon.Station('prop')
    pst.stations[3] = avango.daemon.Station('head1')
    pst.stations[4] = avango.daemon.Station('head0')
    pst.stations[1] = avango.daemon.Station('pen')
    pst.stations[13] = avango.daemon.Station('lens')

    device_list.append(pst)
    print("PST Tracking started!")


# functions
def init_tuio_input():

    tuio = avango.daemon.TUIOInput()
    tuio.port = "3333"  # tuio port

    tuio.stations[0] = avango.daemon.Station('gua-finger0')
    tuio.stations[1] = avango.daemon.Station('gua-finger1')
    tuio.stations[2] = avango.daemon.Station('gua-finger2')
    tuio.stations[3] = avango.daemon.Station('gua-finger3')
    tuio.stations[4] = avango.daemon.Station('gua-finger4')
    tuio.stations[5] = avango.daemon.Station('gua-finger5')
    tuio.stations[6] = avango.daemon.Station('gua-finger6')
    tuio.stations[7] = avango.daemon.Station('gua-finger7')
    tuio.stations[8] = avango.daemon.Station('gua-finger8')
    tuio.stations[9] = avango.daemon.Station('gua-finger9')
    tuio.stations[10] = avango.daemon.Station('gua-finger10')
    tuio.stations[11] = avango.daemon.Station('gua-finger11')
    tuio.stations[12] = avango.daemon.Station('gua-finger12')
    tuio.stations[13] = avango.daemon.Station('gua-finger13')
    tuio.stations[14] = avango.daemon.Station('gua-finger14')
    tuio.stations[15] = avango.daemon.Station('gua-finger15')
    tuio.stations[16] = avango.daemon.Station('gua-finger16')
    tuio.stations[17] = avango.daemon.Station('gua-finger17')
    tuio.stations[18] = avango.daemon.Station('gua-finger18')
    tuio.stations[19] = avango.daemon.Station('gua-finger19')

    device_list.append(tuio)
    print("TUIO Input started!")


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
        print("Mouse started at:", mouse_name)

    else:
        print("Mouse NOT found !")


def init_keyboard():

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


def init_spheron():
    name = os.popen(
        "/opt/avango/vr_application_lib/tools/list-ev -s | grep \"BUWEIMAR RAPID DEVEL DEVICE\" | sed -e \'s/\"//g\'  | cut -d\" \" -f4").read(
        )

    name = name.split()
    if len(name) > 0:

        name = name[0]

        spheron = avango.daemon.HIDInput()
        spheron.station = avango.daemon.Station("gua-device-spheron")  # create a station to propagate the input events
        spheron.device = name

        # map incoming spheron events to station values
        spheron.values[0] = "EV_ABS::ABS_X"  # trans X
        spheron.values[1] = "EV_ABS::ABS_Y"  # trans Y
        spheron.values[2] = "EV_ABS::ABS_Z"  # trans Z
        spheron.values[3] = "EV_ABS::ABS_RX"  # rotate X
        spheron.values[4] = "EV_ABS::ABS_RY"  # rotate Y
        spheron.values[5] = "EV_ABS::ABS_RZ"  # rotate Z

        device_list.append(spheron)
        print('Spheron started at:', name)

    else:
        print("Spheron NOT found !")


def init_xbox_controllers():

    # xbox receiver gives us up to four controllers, init each
    for num in range(4):
        _query = "/opt/avango/vr_application_lib/tools/list-ev -s " + \
				"| grep \"Xbox 360 Wireless Receiver\" | sed -e \'s/\"//g\' " + \
				"| cut -d\" \" -f4" + num * " | sed -n \'1!p\'"

        string = os.popen(_query).read()
        string = string.split()

        # more than one xbox receiver found, use the first one
        if len(string) > 0:
            string = string[0]

            # create a station to propagate the input events
            xbox = avango.daemon.HIDInput()
            xbox.station = avango.daemon.Station('gua-device-xbox-' + str(num))
            xbox.device = string

            xbox.values[0] = "EV_ABS::ABS_X"  # left joystick
            xbox.values[1] = "EV_ABS::ABS_Y"  # left joystick
            xbox.values[2] = "EV_ABS::ABS_RX"  # right joystick
            xbox.values[3] = "EV_ABS::ABS_RY"  # right joystick

            xbox.values[4] = "EV_ABS::ABS_Z"  # left bumper
            xbox.values[5] = "EV_ABS::ABS_RZ"  # right bumper

            xbox.buttons[0] = "EV_KEY::BTN_X"  # Button X
            xbox.buttons[1] = "EV_KEY::BTN_B"  # Button B
            xbox.buttons[2] = "EV_KEY::BTN_A"  # Button A
            xbox.buttons[3] = "EV_KEY::BTN_Y"  # Button Y

            xbox.buttons[4] = "EV_KEY::BTN_START"  # Start button
            xbox.buttons[5] = "EV_KEY::BTN_SELECT"  # Select button

            xbox.buttons[6] = "EV_KEY::BTN_TL"  # left shoulder button
            xbox.buttons[7] = "EV_KEY::BTN_TR"  # right shoulder button

            xbox.buttons[8] = "EV_KEY::BTN_THUMBL"  # left joystick button
            xbox.buttons[9] = "EV_KEY::BTN_THUMBR"  # right joystick button

            xbox.buttons[10] = "EV_KEY::BTN_TRIGGER_HAPPY1"  # left key
            xbox.buttons[11] = "EV_KEY::BTN_TRIGGER_HAPPY2"  # right key
            xbox.buttons[12] = "EV_KEY::BTN_TRIGGER_HAPPY3"  # up key
            xbox.buttons[13] = "EV_KEY::BTN_TRIGGER_HAPPY4"  # down key

            device_list.append(xbox)
            print("XBox Controller started at:", string)

        else:
            print("XBox Controllers NOT found!")
            return


device_list = []

init_pst_tracking()
init_tuio_input()
init_mouse()
init_keyboard()
init_spheron()
init_xbox_controllers()

avango.daemon.run(device_list)
