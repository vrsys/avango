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
A distributed viewer setup: This Python scripts starts an
avango.osg.simpleviewer to load a given geometry. Any client connected to the
group "testgroup" should receive this model. (see also simpleviewer-clnt.py)
'''

import avango
import avango.script
import avango.gua
from avango.script import field_has_changed

from examples_common.GuaVE import GuaVE

nettrans = avango.gua.nodes.NetTransform(
  Name = "net",
  # specify role, ip, and port
  Groupname = "AVSERVER|127.0.0.1|7432"
)

class TimedRotate(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.gua.SFMatrix4()

    @field_has_changed(TimeIn)
    def update(self):
        self.MatrixOut.value = avango.gua.make_rot_mat(
                                self.TimeIn.value*2.0, 0.0, 1.0, 0.0)

def make_distributable(node):
  for child in node.Children.value:
    make_distributable(child)
  nettrans.distribute_object(node)


# setup scenegraph
graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")
loader = avango.gua.nodes.TriMeshLoader()

monkey1 = loader.create_geometry_from_file("monkey", "../simple_example/data/objects/monkey.obj")
monkey2 = loader.create_geometry_from_file("monkey", "../simple_example/data/objects/monkey.obj")

monkey1.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 0.766, 0.336, 1.0))
monkey1.Material.value.set_uniform("Roughness", 0.3)
monkey1.Material.value.set_uniform("Metalness", 1.0)

monkey2.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 0.266, 0.136, 1.0))
monkey2.Material.value.set_uniform("Roughness", 0.6)
monkey2.Material.value.set_uniform("Metalness", 0.1)

transform1 = avango.gua.nodes.TransformNode(
  Children = [monkey1]
)
transform2 = avango.gua.nodes.TransformNode(
  Transform = avango.gua.make_trans_mat(-0.5, 0.0,0),
  Children = [monkey2]
)

light = avango.gua.nodes.PointLightNode(
  Name = "light",
  Color = avango.gua.Color(1.0, 1.0, 1.0),
  Brightness = 50.0,
  Transform = avango.gua.make_trans_mat(1, 1, 5) * avango.gua.make_scale_mat(15, 15, 15)
)

monkey_transform1 = avango.gua.nodes.TransformNode(Name = "monkey_transform1")
monkey_transform1.Transform.value = avango.gua.make_trans_mat(1.0, 0.0, 0.0)
monkey_transform1.Children.value = [monkey1]

monkey_transform2 = avango.gua.nodes.TransformNode(Name = "monkey_transform2")
monkey_transform2.Transform.value = avango.gua.make_trans_mat(-1.0, 0.0, 0.0)
monkey_transform2.Children.value = [monkey2]

group = avango.gua.nodes.TransformNode(Name = "group")
group.Children.value = [monkey_transform1, monkey_transform2, light]

screen = avango.gua.nodes.ScreenNode(Name = "screen", Width = 4, Height = 3)

size = avango.gua.Vec2ui(800, 600)
server_cam = avango.gua.nodes.CameraNode(
  LeftScreenPath = "/net/screen",
  SceneGraph = "scenegraph",
  Resolution = size,
  OutputWindowName = "server_window",
  Transform = avango.gua.make_trans_mat(0.0, 0.0, 3.5)
)

client_cam = avango.gua.nodes.CameraNode(
  LeftScreenPath = "/net/screen",
  SceneGraph = "scenegraph",
  Resolution = size,
  OutputWindowName = "client_window",
  Transform = avango.gua.make_trans_mat(0.0, 0.0, 3.5)
)

screen.Children.value = [client_cam, server_cam]
nettrans.Children.value = [group, screen]

graph.Root.value.Children.value = [nettrans]

make_distributable(group)
make_distributable(screen)

# setup viewing
window = avango.gua.nodes.GlfwWindow(
  Size = size,
  LeftResolution = size
)

avango.gua.register_window("server_window", window)

#setup viewer
viewer = avango.gua.nodes.Viewer()
viewer.CameraNodes.value = [server_cam]
viewer.SceneGraphs.value = [graph]
viewer.Window.value = window

monkey_updater = TimedRotate()

timer = avango.nodes.TimeSensor()
monkey_updater.TimeIn.connect_from(timer.Time)

monkey1.Transform.connect_from(monkey_updater.MatrixOut)
monkey2.Transform.connect_from(monkey_updater.MatrixOut)

guaVE = GuaVE()
guaVE.start(locals(), globals())

viewer.run()

