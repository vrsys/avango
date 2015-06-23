#!/usr/bin/python
# -*- Mode:Python -*-

import os
import avango.daemon


def init_xbox_controllers():

    controllers = []

    # xbox-controller receiver gives us up to four controllers, init each
    for num in range(4):
        query = "/opt/avango/vr_application_lib/tools/list-ev -s |" + \
                "grep \"Xbox 360 Wireless Receiver\" | " + \
                "sed -e \'s/\"//g\' |" + \
                "cut -d\" \" -f4" + \
                num * " | sed -n \'1!p\'"

        name = os.popen(query).read().split()

        # more than one xbox-receiver found, use the first one
        if len(name) > 0:
            name = name[0]

            # create a station to propagate the input events
            controller = avango.daemon.HIDInput()
            controller.station = avango.daemon.Station(
                'gua-device-xbox-' + str(num)
                )
            controller.device = name

            # left stick
            controller.values[0] = "EV_ABS::ABS_X"
            controller.values[1] = "EV_ABS::ABS_Y"

            # right stick
            controller.values[2] = "EV_ABS::ABS_RX"
            controller.values[3] = "EV_ABS::ABS_RY"

            # trigger buttons (back of controller)
            controller.values[4] = "EV_ABS::ABS_Z"
            controller.values[5] = "EV_ABS::ABS_RZ"

            # buttons X, A, B, Y
            controller.buttons[0] = "EV_KEY::BTN_X"
            controller.buttons[1] = "EV_KEY::BTN_B"
            controller.buttons[2] = "EV_KEY::BTN_A"
            controller.buttons[3] = "EV_KEY::BTN_Y"

            # buttons start and select
            controller.buttons[4] = "EV_KEY::BTN_START"
            controller.buttons[5] = "EV_KEY::BTN_SELECT"

            # shoulder buttons
            controller.buttons[6] = "EV_KEY::BTN_TL"
            controller.buttons[7] = "EV_KEY::BTN_TR"

            # thumb buttons (pressing down the sticks)
            controller.buttons[8] = "EV_KEY::BTN_THUMBL"
            controller.buttons[9] = "EV_KEY::BTN_THUMBR"

            # directionial pad not working for unknown reasons
            # controller.buttons[10] = "EV_KEY::BTN_TRIGGER_HAPPY1" # left
            # controller.buttons[11] = "EV_KEY::BTN_TRIGGER_HAPPY2" # right
            # controller.buttons[12] = "EV_KEY::BTN_TRIGGER_HAPPY3" # up
            # controller.buttons[13] = "EV_KEY::BTN_TRIGGER_HAPPY4" # down

            controllers.append(controller)
            print("XBox Controller started at:", name)

        else:
            print("XBox Controllers NOT found!")

    return controllers


def main():
    avango.daemon.run(init_xbox_controllers())


if __name__ == '__main__':
    main()
