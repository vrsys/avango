# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       #
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

import avango.osg.viewer
import avango.moving
import avango.tools
import avango.display
import sys

argv = avango.display.init(sys.argv)
view = avango.display.make_view()
view.EnableTrackball.value = False

# set up scene graph
obj_trans = avango.osg.nodes.MatrixTransform(Matrix=avango.osg.make_trans_mat(0,1.7,-1))

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
view.Root.value = root_group

#create mouse device
mouse = avango.display.make_dominant_user_device(interface="Mouse")

# setup tool interaction
pick_selector = avango.tools.nodes.PickSelector()
pick_selector.PickTrigger.connect_from(mouse.MouseButtonLeft)
pick_selector.PickRayTransform.connect_from(mouse.MouseNearTransform)
pick_selector.RootNode.connect_from(view.Root)

drag_tool = avango.tools.nodes.DragTool()
drag_tool.DragTransform.connect_from(mouse.MouseTransform)
drag_tool.Targets.connect_from(pick_selector.SelectedTargets)

# run evaluation and render loop
avango.display.run()
