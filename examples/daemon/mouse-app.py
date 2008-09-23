#!/usr/bin/python
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

'''
Learn how to setup and use DeviceDaemon and DeviceService to readout input data
from a HIDInput device.

This example manipulates the position of a displayed sphere
based on the output the device specified in mouse-daemon.py generates.
'''

import avango.daemon
import avango.script
import avango.osg.simpleviewer

# create a simple scene
sphere = avango.osg.nodes.Sphere(Radius = 0.15)
transform = avango.osg.nodes.MatrixTransform()
transform.Children.value.append(sphere)

# device service setup
service = avango.daemon.DeviceService()

# a sensor to a specific station
sensor = avango.daemon.nodes.DeviceSensor(DeviceService = service,
                                          Station = "mousestation")

# process output from sensor
class Move(avango.script.Script) :
    SensorX = avango.SFFloat()
    SensorY = avango.SFFloat()
    MatrixOut = avango.osg.SFMatrix()

    def evaluate(self):
        values = self.get_values()
        trans_x = values.SensorX
        trans_y = values.SensorY
        trans_x /= -350.
        trans_y /= -350.
        old_pos = values.MatrixOut.get_translate()
        matrix = avango.osg.make_trans_mat(old_pos.x + trans_x, old_pos.y, old_pos.z + trans_y)
        values.MatrixOut = matrix

move = Move()

# manipulate the position of the sphere
move.SensorX.connect_from(sensor.Value0)
move.SensorY.connect_from(sensor.Value1)
transform.Matrix.connect_from(move.MatrixOut)

# enter render loop
avango.osg.simpleviewer.run(transform)

