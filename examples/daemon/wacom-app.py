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
from a Wacom Intuos3 tablet device.

This example manipulates the position and orientation of 3 displayed spheres
use buttons for changing colours and the airbrush wheel to scale the shape
'''

import avango.daemon
import avango.script
import avango.osg.viewer
import os

# create a simple scene with 3 spheres to form a 3d pointer
sphere1 = avango.osg.nodes.Sphere(Radius = 0.15)
sphere2 = avango.osg.nodes.Sphere(Radius = 0.10)
sphere3 = avango.osg.nodes.Sphere(Radius = 0.05)
sphereTrans1 = avango.osg.nodes.MatrixTransform()
sphereTrans1.Children.value.append(sphere1)
sphereTrans1.Matrix.value = avango.osg.make_trans_mat(0,0,0)
sphereTrans2 = avango.osg.nodes.MatrixTransform()
sphereTrans2.Children.value.append(sphere2)
sphereTrans2.Matrix.value = avango.osg.make_trans_mat(0,0,0.25)
sphereTrans3 = avango.osg.nodes.MatrixTransform()
sphereTrans3.Children.value.append(sphere3)
sphereTrans3.Matrix.value = avango.osg.make_trans_mat(0,0,0.4)

transform = avango.osg.nodes.MatrixTransform()
transform.Children.value = (sphereTrans1,sphereTrans2,sphereTrans3)
transform.Matrix.value = avango.osg.make_trans_mat(0,0,-10)

# device service setup
service = avango.daemon.DeviceService()

# a sensor to a specific station
sensor = avango.daemon.nodes.DeviceSensor(DeviceService = service,
                                          Station = "wacomstation")

# process output from sensor
class Tablet(avango.script.Script) :
    PenX = avango.SFFloat()
    PenY = avango.SFFloat()
    TouchLeft = avango.SFFloat()
    TouchRight = avango.SFFloat()
    UnknownRz = avango.SFFloat()
    UnknownThrottle = avango.SFFloat()
    PenWheel = avango.SFFloat()
    PenPressure = avango.SFFloat()
    PenDistance = avango.SFFloat()
    PenTiltX = avango.SFFloat()
    PenTiltY = avango.SFFloat()
    UnknownRelWheel = avango.SFFloat()
    Button0 = avango.SFBool()
    Button1 = avango.SFBool()
    Button2 = avango.SFBool()
    Button3 = avango.SFBool()
    Button4 = avango.SFBool()
    Button5 = avango.SFBool()
    Button6 = avango.SFBool()
    Button7 = avango.SFBool()
    ButtonLeft = avango.SFBool()
    ButtonRight = avango.SFBool()
    ButtonMiddle = avango.SFBool()
    ButtonSide = avango.SFBool()
    ButtonExtra = avango.SFBool()
    ButtonPen = avango.SFBool()
    ButtonRubber = avango.SFBool()
    ButtonBrush = avango.SFBool()
    ButtonPencil = avango.SFBool()
    ButtonAirbrush = avango.SFBool()
    ButtonFinger = avango.SFBool()
    ButtonMouse = avango.SFBool()
    ButtonLens = avango.SFBool()
    ButtonTouch = avango.SFBool()
    ButtonStylus = avango.SFBool()
    ButtonStylus2 = avango.SFBool()

    def evaluate(self):
        #values = self.get_values()
        if os.name == 'posix':
            os.system('clear')
        print "---Axes----------------------------------------"
        print "PenX: " + str(self.PenX.value)
        print "PenY: " + str(self.PenY.value)
        print "TouchLeft: " + str(self.TouchLeft.value)
        print "TouchRight: " + str(self.TouchRight.value)
        print "UnknownRz: " + str(self.UnknownRz.value)
        print "UnknownThrottle: " + str(self.UnknownThrottle.value)
        print "PenWheel: " + str(self.PenWheel.value)
        print "PenPressure: " + str(self.PenPressure.value)
        print "PenDistance: " + str(self.PenDistance.value)
        print "PenTiltX: " + str(self.PenTiltX.value)
        print "PenTiltY: " + str(self.PenTiltY.value)
        print "UnknownRelWheel: " + str(self.UnknownRelWheel.value)
        print "---Buttons-------------------------------------"
        print "Button0: " + str(self.Button0.value)
        print "Button1: " + str(self.Button1.value)
        print "Button2: " + str(self.Button2.value)
        print "Button3: " + str(self.Button3.value)
        print "Button4: " + str(self.Button4.value)
        print "Button5: " + str(self.Button5.value)
        print "Button6: " + str(self.Button6.value)
        print "Button7: " + str(self.Button7.value)
        print "ButtonLeft: " + str(self.ButtonLeft.value)
        print "ButtonRight: " + str(self.ButtonRight.value)
        print "ButtonMiddle: " + str(self.ButtonMiddle.value)
        print "ButtonSide: " + str(self.ButtonSide.value)
        print "ButtonExtra: " + str(self.ButtonExtra.value)
        print "ButtonPen: " + str(self.ButtonPen.value)
        print "ButtonRubber: " + str(self.ButtonRubber.value)
        print "ButtonBrush: " + str(self.ButtonBrush.value)
        print "ButtonPencil: " + str(self.ButtonPencil.value)
        print "ButtonAirbrush: " + str(self.ButtonAirbrush.value)
        print "ButtonFinger: " + str(self.ButtonFinger.value)
        print "ButtonMouse: " + str(self.ButtonMouse.value)
        print "ButtonLens: " + str(self.ButtonLens.value)
        print "ButtonTouch: " + str(self.ButtonTouch.value)
        print "ButtonStylus: " + str(self.ButtonStylus.value)
        print "ButtonStylus2: " + str(self.ButtonStylus2.value)

        if (self.ButtonAirbrush.value):
            transform.Matrix.value = avango.osg.make_scale_mat(1,1,1-self.PenWheel.value)
        else:
            transform.Matrix.value = avango.osg.make_scale_mat(1,1,1)

        transform.Matrix.value = transform.Matrix.value*\
            avango.osg.make_rot_mat(1.047*self.PenTiltX.value, avango.osg.Vec3(0,1,0))*\
            avango.osg.make_rot_mat(1.047*self.PenTiltY.value,avango.osg.Vec3(1,0,0))*\
            avango.osg.make_trans_mat(1*self.PenX.value,-1*self.PenY.value,-10)
        sphereTrans2.Matrix.value = avango.osg.make_trans_mat(0,0,0.25 - (0.175*self.PenPressure.value))
        sphereTrans3.Matrix.value = avango.osg.make_trans_mat(0,0,0.4 - (0.25*self.PenPressure.value))
        if (self.ButtonTouch.value):
            sphere1.Color.value = avango.osg.Vec4(1,0,0,1)
        else:
            sphere1.Color.value = avango.osg.Vec4(1,1,1,1)
        if (self.ButtonStylus.value):
            sphere2.Color.value = avango.osg.Vec4(0,1,0,1)
        else:
            sphere2.Color.value = avango.osg.Vec4(1,1,1,1)
        if (self.ButtonStylus2.value):
            sphere3.Color.value = avango.osg.Vec4(0,0,1,1)
        else:
            sphere3.Color.value = avango.osg.Vec4(1,1,1,1)

tablet = Tablet()

# manipulate the position of the sphere
tablet.PenX.connect_from(sensor.Value0)
tablet.PenY.connect_from(sensor.Value1)
tablet.TouchLeft.connect_from(sensor.Value2)
tablet.TouchRight.connect_from(sensor.Value3)
tablet.UnknownRz.connect_from(sensor.Value4)
tablet.UnknownThrottle.connect_from(sensor.Value5)
tablet.PenWheel.connect_from(sensor.Value6)
tablet.PenPressure.connect_from(sensor.Value7)
tablet.PenDistance.connect_from(sensor.Value8)
tablet.PenTiltX.connect_from(sensor.Value9)
tablet.PenTiltY.connect_from(sensor.Value10)
tablet.UnknownRelWheel.connect_from(sensor.Value11)
tablet.Button0.connect_from(sensor.Button0)
tablet.Button1.connect_from(sensor.Button1)
tablet.Button2.connect_from(sensor.Button2)
tablet.Button3.connect_from(sensor.Button3)
tablet.Button4.connect_from(sensor.Button4)
tablet.Button5.connect_from(sensor.Button5)
tablet.Button6.connect_from(sensor.Button6)
tablet.Button7.connect_from(sensor.Button7)
tablet.ButtonLeft.connect_from(sensor.Button8)
tablet.ButtonRight.connect_from(sensor.Button9)
tablet.ButtonMiddle.connect_from(sensor.Button10)
tablet.ButtonSide.connect_from(sensor.Button11)
tablet.ButtonExtra.connect_from(sensor.Button12)
tablet.ButtonPen.connect_from(sensor.Button13)
tablet.ButtonRubber.connect_from(sensor.Button14)
tablet.ButtonBrush.connect_from(sensor.Button15)
tablet.ButtonPencil.connect_from(sensor.Button16)
tablet.ButtonAirbrush.connect_from(sensor.Button17)
tablet.ButtonFinger.connect_from(sensor.Button18)
tablet.ButtonMouse.connect_from(sensor.Button19)
tablet.ButtonLens.connect_from(sensor.Button20)
tablet.ButtonTouch.connect_from(sensor.Button21)
tablet.ButtonStylus.connect_from(sensor.Button22)
tablet.ButtonStylus2.connect_from(sensor.Button23)

# set up viewing
window = avango.osg.viewer.nodes.GraphicsWindow()
camera = avango.osg.viewer.nodes.Camera()
camera.Window.value = window
viewer = avango.osg.viewer.nodes.Viewer()
viewer.MasterCamera.value = camera
viewer.Scene.value = transform
# run evaluation and render loop
viewer.run()

