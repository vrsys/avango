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
This example shows the usage of avango.osg.viewer.
'''

import sys
import avango.moving
import avango.osg.viewer

if len(sys.argv) != 2:
    print "Usage: " + sys.argv[0] + " <modelname>"
    sys.exit(1)


# set up scene graph

obj = avango.osg.nodes.LoadFile(Filename = sys.argv[1])
obj_trans = avango.osg.nodes.MatrixTransform(Children = [obj])
root_group = avango.osg.nodes.Group(Children = [obj_trans])


# set up viewing

window = avango.osg.viewer.nodes.GraphicsWindow()
camera = avango.osg.viewer.nodes.Camera(Window = window)
viewer = avango.osg.viewer.nodes.Viewer(MasterCamera = camera, Scene = root_group)


# set up trackball mover

trackball = avango.moving.nodes.Trackball(Matrix = camera.ViewerTransform.value)
trackball.Direction.connect_from(window.MousePositionNorm)
trackball.RotateTrigger.connect_from(window.MouseButtons_OnlyLeft)
trackball.ZoomTrigger.connect_from(window.MouseButtons_LeftAndRight)
trackball.PanTrigger.connect_from(window.MouseButtons_OnlyRight)
trackball.CenterTransform.value = \
  avango.osg.make_scale_mat(0.1, 0.1, 0.1) * \
  avango.osg.make_trans_mat(0, 0, -0.6)

camera.ViewerTransform.connect_from(trackball.Matrix)


# render a frame to update bounding spheres and scale model to fit in window

viewer.frame()
scale = 0.08 / obj.get_bounding_sphere().radius()
obj_trans.Matrix.value = avango.osg.make_scale_mat(scale, scale, scale)


# run evaluation and render loop

viewer.run()
