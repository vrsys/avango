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
Learn how to configure a desktop-flyer to manipulate the viewpoint.
Use arrow keys to control the position of the viewer.
'''

import avango.osg.viewer
import avango.moving
import sys
import avango.script
from avango.script import field_has_changed


# set up scene graph

obj = avango.osg.nodes.LoadFile(Filename="Models/lzsnow.osg")
obj_trans = avango.osg.nodes.MatrixTransform()
obj_trans.Children.value = [obj]
obj_trans.Matrix.value = avango.osg.make_rot_mat(-1.571, 1, 0, 0) * avango.osg.make_trans_mat(0,-93,-170)

root_group = avango.osg.nodes.Group()
root_group.Children.value = [obj_trans]


# set up viewing

window = avango.osg.viewer.nodes.GraphicsWindow()

camera = avango.osg.viewer.nodes.Camera()
camera.Window.value = window
camera.Far.value = 5000

viewer = avango.osg.viewer.nodes.Viewer()
viewer.MasterCamera.value = camera
viewer.Scene.value = root_group


# set up desktop flyer mover

time = avango.nodes.TimeSensor()

desktopflyer = avango.moving.nodes.DesktopFlyer()
desktopflyer.MouseIn.connect_from(window.MouseMovementNorm)
desktopflyer.Matrix.value = camera.ViewerTransform.value
desktopflyer.TimeIn.connect_from(time.Time)
window.MouseFixed.connect_from(desktopflyer.RelativeMouseTrigger)
camera.ViewerTransform.connect_from(desktopflyer.Matrix)

# typical mouse/keyboard setup:
# control xy view rotation with mouse right button and mouse movement
desktopflyer.RotateXYTrigger.connect_from(window.MouseButtons_OnlyRight)

# control move xz with cursor keys
desktopflyer.MoveForward.connect_from(window.KeyUp)
desktopflyer.MoveBackward.connect_from(window.KeyDown)
desktopflyer.MoveLeft.connect_from(window.KeyLeft)
desktopflyer.MoveRight.connect_from(window.KeyRight)

# control move up down and z rotation with additional keys
desktopflyer.MoveUp.connect_from(window.KeyHome)
desktopflyer.MoveDown.connect_from(window.KeyEnd)
desktopflyer.RotateCW.connect_from(window.KeyDelete)
desktopflyer.RotateCCW.connect_from(window.KeyPageDown)

# controlling xy rotation is also possible using keys
#desktopflyer.RotateLeft.connect_from(window.KeySpace)
#desktopflyer.RotateRight.connect_from(window.KeyEnter)
#desktopflyer.RotateUp.connect_from(window.KeyShift)
#desktopflyer.RotateDown.connect_from(window.KeyCtrl)

# it's possible to control all 6 dof with mouse only
#desktopflyer.MoveXZTrigger.connect_from(window.MouseButtons_OnlyMiddle)
#desktopflyer.MoveYRotateZTrigger.connect_from(window.MouseButtons_MiddleAndRight)

# uncomment and play around with custom properties
#desktopflyer.MoveSpeed.value = 100
#desktopflyer.MaxSpeed.value = 500
#desktopflyer.MoveAcceleration.value = 200
#desktopflyer.MoveBreak.value = 100
#desktopflyer.RotationSpeed.value = 5.0
#desktopflyer.HaltOnClutch.value = False


# run evaluation and render loop

viewer.run()
