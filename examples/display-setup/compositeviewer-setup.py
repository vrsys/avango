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
Learn how to configure a CompositeViewer that opens up two separate windows
showing the given geometry files. For each window an own view, camera and
scene graph exist.
'''

import sys
import avango.moving
import avango.osg.viewer

if len(sys.argv) != 3:
    print "Usage: " + sys.argv[0] + " <modelname1> <modelname2>"
    sys.exit(1)


# set up scene graphs

obj1 = avango.osg.nodes.LoadFile(Filename = sys.argv[1])
obj_scale1 = avango.osg.nodes.MatrixTransform(Children = [obj1])
root_group1 = avango.osg.nodes.Group(Children = [obj_scale1])

obj2 = avango.osg.nodes.LoadFile(Filename = sys.argv[2])
obj_scale2 = avango.osg.nodes.MatrixTransform(Children = [obj2])
root_group2 = avango.osg.nodes.Group(Children = [obj_scale2])


# set up views

window1 = avango.osg.viewer.nodes.GraphicsWindow(WantedPositionX = 100, WantedPositionY = 200)
camera1 = avango.osg.viewer.nodes.Camera(Window = window1)
view1 = avango.osg.viewer.nodes.View(MasterCamera = camera1, Scene = root_group1)

window2 = avango.osg.viewer.nodes.GraphicsWindow(WantedPositionX = 700, WantedPositionY = 200)
camera2 = avango.osg.viewer.nodes.Camera(Window = window2)
view2 = avango.osg.viewer.nodes.View(MasterCamera = camera2, Scene = root_group2)


# set up composite viewer

composite_viewer = avango.osg.viewer.nodes.CompositeViewer(Views = [view1, view2])


# set up trackball mover 1

trackball1 = avango.moving.nodes.Trackball(Matrix = camera1.ViewerTransform.value)
trackball1.Direction.connect_from(window1.MousePositionNorm)
trackball1.RotateTrigger.connect_from(window1.MouseButtons_OnlyLeft)
trackball1.ZoomTrigger.connect_from(window1.MouseButtons_LeftAndRight)
trackball1.PanTrigger.connect_from(window1.MouseButtons_OnlyRight)
trackball1.CenterTransform.value = \
  avango.osg.make_scale_mat(0.1, 0.1, 0.1) * \
  avango.osg.make_trans_mat(0, 0, -0.6)

camera1.ViewerTransform.connect_from(trackball1.Matrix)


# set up trackball mover 2

trackball2 = avango.moving.nodes.Trackball(Matrix = camera2.ViewerTransform.value)
trackball2.Direction.connect_from(window2.MousePositionNorm)
trackball2.RotateTrigger.connect_from(window2.MouseButtons_OnlyLeft)
trackball2.ZoomTrigger.connect_from(window2.MouseButtons_LeftAndRight)
trackball2.PanTrigger.connect_from(window2.MouseButtons_OnlyRight)
trackball2.CenterTransform.value = \
  avango.osg.make_scale_mat(0.1, 0.1, 0.1) * \
  avango.osg.make_trans_mat(0, 0, -0.6)

camera2.ViewerTransform.connect_from(trackball2.Matrix)


# render a frame to update bounding spheres and scale models to fit in windows

composite_viewer.frame()
scale1 = 0.08 / obj1.get_bounding_sphere().radius()
obj_scale1.Matrix.value = avango.osg.make_scale_mat(scale1, scale1, scale1)
scale2 = 0.08 / obj2.get_bounding_sphere().radius()
obj_scale2.Matrix.value = avango.osg.make_scale_mat(scale2, scale2, scale2)


# run evaluation and render loop

composite_viewer.run()
