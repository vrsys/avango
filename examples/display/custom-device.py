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
A simple application using a custom device provided by Monitor: a SpaceMouse.
'''

import avango.script
import avango.display
import sys


class Accumulator(avango.script.Script):
    MatrixIn = avango.osg.SFMatrix()
    MatrixOrig = avango.osg.SFMatrix()
    MatrixOut = avango.osg.SFMatrix()

    def evaluate(self):
        self.MatrixOut.value = self.MatrixIn.value * self.MatrixOrig.value

class Trigger(avango.script.Script):
    Input = avango.SFBool()
    Text = avango.SFString()

    def evaluate(self):
        if self.Input.value:
            print self.Text.value


avango.display.init(sys.argv)

# create a simple scene
sphere = avango.osg.nodes.Sphere(Radius = 0.15)
transform = avango.osg.nodes.MatrixTransform()
transform.Children.value.append(sphere)
transform.Matrix.value = avango.osg.make_trans_mat(0., 1.7, -2.)

spacemouse = avango.display.make_device("SpaceMouse", "Relative6DOF")
spacemouse.TranslationScale.value = 1.
spacemouse.RotationScale.value = 0.1

accumulator = Accumulator()
accumulator.MatrixIn.connect_from(spacemouse.MatrixOut)
accumulator.MatrixOrig.connect_weak_from(accumulator.MatrixOut)

trigger0 = Trigger(Text = "Button 0 was pressed")
trigger0.Input.connect_from(spacemouse.Button0)
trigger1 = Trigger(Text = "Button 1 was pressed")
trigger1.Input.connect_from(spacemouse.Button1)

# enter render loop
view = avango.display.make_view()
view.Root.value = transform
view.Camera.connect_from(accumulator.MatrixOut)

avango.display.run()

