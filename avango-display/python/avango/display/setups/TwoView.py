# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# AVANGO is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# AVANGO is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
##########################################################################

import avango.display

import math
import re

class StatsViewer(avango.script.Script):
    Views = avango.osg.viewer.MFView()
    StatsNum = avango.SFInt()

    def __init__(self):
        self.super(StatsViewer).__init__()

    def evaluate(self):
        for view in self.Views.value:
            view.StatsMode.value = self.StatsNum.value

class TwoView(avango.display.Display):

    def __init__(self, inspector, options):
        super(TwoView, self).__init__("TwoView", inspector)

        if "twopipe" in options:
            two_view_walls = [":0.0", ":0.1"]
            twopipe = True
        else:
            two_view_walls = [":0.0", ":0.0"]
            twopipe = False

        # Viewer 1
        window1 = self.make_window(0, 0, 1280, 1024, 6, 2.4, True, two_view_walls[0])
        window1.Name.value = "1"
        self.add_window(window1, avango.osg.make_trans_mat(0., 1.2, -2.4), 0)

        # Viewer 2
        if not twopipe:
            xpos = 1280
        else:
            xpos = 0
        window2 = self.make_window(xpos, 0, 1280, 1024, 6, 2.4, True, two_view_walls[1])
        window2.Name.value = "2"
        self.add_window(window2, avango.osg.make_trans_mat(0., 1.2, -2.4), 1)

        #store the created views. Needed for the stats display
        self._views = []

        #create a device service
        self._device_service = avango.daemon.DeviceService()

        # Users
        user1 = avango.display.nodes.User()
        self.add_user(user1)
        user2 = avango.display.nodes.User()
        self.add_user(user2)

        # Connect head-tracking
        #view1_yellow_glasses = self.make_glasses("ve-dtrack-head4", avango.osg.Vec3(-0.074, -0.018, 0.025))
        view1_yellow_glasses = self.make_glasses("ve-dtrack-xpand1", avango.osg.Vec3(-0.0825, 0.0, -0.045))
        
        user1.Matrix.connect_from(view1_yellow_glasses.Matrix)
        self.keep_alive(view1_yellow_glasses)
        #blue glasses do not work properly. Use purple glasses instead
        #view2_blue_glasses = self.make_glasses("ve-dtrack-head3", avango.osg.Vec3(-0.073, -0.016, 0.025))
        #user2.Matrix.connect_from(view2_blue_glasses.Matrix)
        #self.keep_alive(view2_blue_glasses)
        #view2_purple_glasses = self.make_glasses("ve-dtrack-head1", avango.osg.Vec3(-0.089, 0.015, -0.040))
        view2_purple_glasses = self.make_glasses("ve-dtrack-head1", avango.osg.Vec3(0.12, 0.043, 0.0 ))
        
        user2.Matrix.connect_from(view2_purple_glasses.Matrix)
        self.keep_alive(view2_purple_glasses)

        self._wiimote_config = {}
        self._wiimote_config["wiimote1"] = ["ve-dtrack-atek",     avango.osg.Vec3( 0.045, 0.0,  0.022)]
        self._wiimote_config["wiimote2"] = ["ve-dtrack-reddot",   avango.osg.Vec3(-0.05 , 0.0,  0.031)]
        self._wiimote_config["wiimote3"] = ["ve-dtrack-logitech", avango.osg.Vec3(-0.04 , 0.0, -0.025)]
        self._wiimote_config["wiimote4"] = ["ve-dtrack-raytac",   avango.osg.Vec3( 0.068, 0.02, 0.035)]

    def view_created(self, camera, view,subdisplay=""):
        self._views.append(view)

    def make_dominant_user_device(self, user, interface, subdisplay):
        pda_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = self.device_service,
                                                      Station = "ve-dtrack-pda2")
        pda_sensor.TransmitterOffset.value = self._perf2osg
        pda_sensor.ReceiverOffset.value = avango.osg.make_trans_mat(0.076, -0.016, 0.025)
        self.keep_alive(pda_sensor)

        device = avango.display.nodes.Device()
        device.Matrix.connect_from(pda_sensor.Matrix)
        return device


    def make_device(self, device, interface):
        #check for wiimote
        if "wiimote" in device:
            pattern = re.compile("wiimote(\d+)$")
            result = pattern.match(device)
            if not result:
                print "Wiimote device <" + device +"> does not exist."
                return None
            wiimote_num = int(result.group(1))
            if wiimote_num < 1 or wiimote_num > 4:
                print "Wiimote device <" + device +"> does not exist. Currently wiimote 1-4 exists."
                return None

            osg2perf = avango.osg.make_rot_mat(math.radians(90), 1, 0, 0)
            perf2osg = avango.osg.make_rot_mat(math.radians(-90), 1, 0, 0)

            wiimote_station_name = device
            wiimote_dtrack_station_name = self._wiimote_config[wiimote_station_name][0]
            wiimote_offset = self._wiimote_config[wiimote_station_name][1]

            wiimote_offset.y -= 0.1
            wiimote_device = avango.display.make_wiimote_device(wiimote_station_name,
                                                                wiimote_dtrack_station_name,
                                                                self._device_service,
                                                                osg2perf * avango.osg.make_trans_mat(wiimote_offset),
                                                                perf2osg)


            if not wiimote_device:
                print "Could not create device <" + str(device) + ">"
                return None

            #activate the LED according to the num (-1, since the numbering of the leds starts with 0) of the wiimote
            led_field = getattr(wiimote_device,"LED"+str(wiimote_num-1))
            led_field.value = True

            return wiimote_device

        elif device == "StatsViewer":
            stats_viewer = StatsViewer()
            stats_viewer.Views.value = self._views
            return stats_viewer
