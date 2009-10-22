# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of Avango.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# Avango is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# Avango is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with Avango. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
# Avango is a trademark owned by FhG.                                    #
#                                                                        #
##########################################################################

import avango.display

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

        # Users
        user1 = avango.display.nodes.User()
        self.add_user(user1)
        user2 = avango.display.nodes.User()
        self.add_user(user2)

        # Connect head-tracking
        view1_yellow_glasses = self.make_glasses("ve-dtrack-head4", avango.osg.Vec3(-0.074, -0.018, 0.025))
        user1.Matrix.connect_from(view1_yellow_glasses.Matrix)
        self.keep_alive(view1_yellow_glasses)
        view2_blue_glasses = self.make_glasses("ve-dtrack-head3", avango.osg.Vec3(-0.073, -0.016, 0.025))
        user2.Matrix.connect_from(view2_blue_glasses.Matrix)
        self.keep_alive(view2_blue_glasses)

    def make_dominant_user_device(self, user, interface, subdisplay):
        pda_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = self.device_service,
                                                      Station = "ve-dtrack-pda2")
        pda_sensor.TransmitterOffset.value = self._perf2osg
        pda_sensor.ReceiverOffset.value = avango.osg.make_trans_mat(0.076, -0.016, 0.025)
        self.keep_alive(pda_sensor)

        device = avango.display.nodes.Device()
        device.Matrix.connect_from(pda_sensor.Matrix)
        return device
