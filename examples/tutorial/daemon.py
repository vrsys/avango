import avango.daemon
import os
import sys

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


device_list = []

init_mouse()
init_keyboard()

avango.daemon.run(device_list)
