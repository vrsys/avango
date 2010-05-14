#!/usr/bin/python
# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
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
Learn how to setup and use DeviceSensor and DeviceService to readout input
data from a Nintendo Wiimote device.

This example manipulates position and color of a displayed sphere
based on the cursor output that the device in wiimote-daemon.py generates.
The usage of the WiimoteActuator to switch on LEDs or to make use
of the rumble capability of the Wiimote device is shown as well.
'''

import atexit
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
# used to readout data provided by the Wiimote device
sensor = avango.daemon.nodes.DeviceSensor(DeviceService = service,
                                          Station = "wiimotestation")

# an actuator used to switch on/off LEDs or trigger rumbling on the Wiimote device
actuator = avango.daemon.nodes.WiimoteActuator(DeviceService = service,
                                               Station = "wiimotestation")
# switch on all LEDs
actuator.LED0.value = True
actuator.LED1.value = True
actuator.LED2.value = True
actuator.LED3.value = True

# process output from sensor
class Change(avango.script.Script) :
    Button1 = avango.SFInt()
    Button2 = avango.SFInt()
    Button3 = avango.SFInt()
    Button4 = avango.SFInt()
    ColorOut = avango.osg.SFVec4()
    MatrixOut = avango.osg.SFMatrix()

    def evaluate(self):
        values = self.get_values()
        # set transformation
        values.MatrixOut = avango.osg.make_trans_mat(0.5 * (values.Button3 - values.Button4),
                                                     0,
                                                     0.5 * (values.Button1 - values.Button2))
        # set color
        if (values.Button4 == 1) :
            values.ColorOut = avango.osg.Vec4(1,1,1,1)
        else :
            values.ColorOut = avango.osg.Vec4(values.Button1, values.Button2, values.Button3, 1)

change = Change()

# connect the inputs of 'change' with the outputs of the 'sensor'
change.Button1.connect_from(sensor.Button1)
change.Button2.connect_from(sensor.Button2)
change.Button3.connect_from(sensor.Button3)
change.Button4.connect_from(sensor.Button4)
sphere.Color.connect_from(change.ColorOut)
transform.Matrix.connect_from(change.MatrixOut)

# enable rumbling via +/- buttons on the Wiimote
actuator.Rumble0.connect_from(sensor.Button9)
actuator.Rumble0.connect_from(sensor.Button10)

# reset LED and rumble states on exit
def onexit() :
    actuator.reset()

atexit.register(onexit)

# enter render loop
avango.osg.simpleviewer.run(transform)
