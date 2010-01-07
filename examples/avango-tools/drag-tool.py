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

import avango.osg.viewer
import avango.moving
import avango.tools

# set up scene graph

obj_trans = avango.osg.nodes.MatrixTransform()

def make_sphere(pos):
    sphere = avango.osg.nodes.Sphere(Radius = 0.01, Matrix = avango.osg.make_trans_mat(pos))
    obj_trans.Children.value.append(sphere)

make_sphere(avango.osg.Vec3( 0.00,  0.00,  0.00))
make_sphere(avango.osg.Vec3( 0.05,  0.05,  0.05))
make_sphere(avango.osg.Vec3( 0.05,  0.05, -0.05))
make_sphere(avango.osg.Vec3( 0.05, -0.05,  0.05))
make_sphere(avango.osg.Vec3( 0.05, -0.05, -0.05))
make_sphere(avango.osg.Vec3(-0.05,  0.05,  0.05))
make_sphere(avango.osg.Vec3(-0.05,  0.05, -0.05))
make_sphere(avango.osg.Vec3(-0.05, -0.05,  0.05))
make_sphere(avango.osg.Vec3(-0.05, -0.05, -0.05))

root_group = avango.osg.nodes.Group(Children = [obj_trans])


# set up viewing

window = avango.osg.viewer.nodes.GraphicsWindow()
camera = avango.osg.viewer.nodes.Camera(Window = window)
viewer = avango.osg.viewer.nodes.Viewer(MasterCamera = camera, Scene = root_group)

# set up event handling

eventfields = avango.osg.viewer.nodes.EventFields(View = viewer)
window.ToggleFullScreen.connect_from(eventfields.KeyAltReturn)
window.DragEvent.connect_from(eventfields.DragEvent)
window.MoveEvent.connect_from(eventfields.MoveEvent)


# set up trackball mover

trackball = avango.moving.nodes.Trackball(Matrix = camera.ViewerTransform.value)
trackball.Direction.connect_from(window.MousePositionNorm)
trackball.RotateTrigger.connect_from(eventfields.MouseButtons_OnlyMiddle)
trackball.ZoomTrigger.connect_from(eventfields.MouseButtons_MiddleAndRight)
trackball.PanTrigger.connect_from(eventfields.MouseButtons_OnlyRight)
trackball.CenterTransform.value = \
  avango.osg.make_scale_mat(0.1, 0.1, 0.1) * \
  avango.osg.make_trans_mat(0, 0, -0.6)

camera.ViewerTransform.connect_from(trackball.Matrix)

# setup tool interaction

pick_selector = avango.tools.nodes.PickSelector()
pick_selector.PickTrigger.connect_from(eventfields.MouseButtons_OnlyLeft)
pick_selector.PickRayTransform.connect_from(camera.MouseNearTransform)
pick_selector.RootNode.connect_from(viewer.Scene)

drag_tool = avango.tools.nodes.DragTool()
drag_tool.DragTransform.connect_from(camera.MouseTransform)
drag_tool.Targets.connect_from(pick_selector.SelectedTargets)


# run evaluation and render loop

viewer.run()
