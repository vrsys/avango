#!/usr/bin/python
# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       #
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

'''
Learn how to setup and use DeviceDaemon and DeviceService to readout data
from a VRPN client running in the AvangoDaemon.

This example manipulates position and color of a displayed sphere
based on button press and analog events sent by the VRPN server.
'''

import avango.daemon
import avango.script
import avango.osg.simpleviewer

# create a simple scene
sphere = avango.osg.nodes.Sphere(Radius = 0.25)
transform = avango.osg.nodes.MatrixTransform()
transform.Children.value.append(sphere)

# device service setup
service = avango.daemon.DeviceService()

# a sensor that listens to a specific device station (referenced by name)
sensor = avango.daemon.nodes.DeviceSensor(DeviceService = service,
                                          Station = "vrpn-client")

# process input from sensor
class Change(avango.script.Script) :
    Button1 = avango.SFInt()
    Button2 = avango.SFInt()
    Button3 = avango.SFInt()
    Button4 = avango.SFInt()
    ValueX = avango.SFFloat()
    ValueY = avango.SFFloat()
    ValueZ = avango.SFFloat()
    ColorOut = avango.osg.SFVec4()
    MatrixOut = avango.osg.SFMatrix()

    def evaluate(self):
        values = self.get_values()
        # set transformation
        values.MatrixOut = avango.osg.make_trans_mat(values.ValueX * -10,
                                                     values.ValueY * -10,
                                                     values.ValueZ * -10)
        # set color
        if (values.Button4 == 1) :
            values.ColorOut = avango.osg.Vec4(1,1,1,1)
        else :
            values.ColorOut = avango.osg.Vec4(values.Button1, values.Button2, values.Button3, 1)

change = Change()

# connect inputs of 'change' with outputs of 'sensor'
change.Button1.connect_from(sensor.Button1)
change.Button2.connect_from(sensor.Button2)
change.Button3.connect_from(sensor.Button3)
change.Button4.connect_from(sensor.Button4)
change.ValueX.connect_from(sensor.Value0)
change.ValueY.connect_from(sensor.Value1)
change.ValueZ.connect_from(sensor.Value2)

# provide result
sphere.Color.connect_from(change.ColorOut)
transform.Matrix.connect_from(change.MatrixOut)

# enter render loop
avango.osg.simpleviewer.run(transform)
