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


monkey1 = loader.create_geometry_from_file("monkey1", "data/objects/monkey.obj",
                                      "data/materials/Stones.gmd", avango.gua.LoaderFlags.DEFAULTS)

monkey2 = loader.create_geometry_from_file("monkey2", "data/objects/monkey.obj",
                                      "data/materials/Stones.gmd", avango.gua.LoaderFlags.DEFAULTS)

light = avango.gua.nodes.PointLightNode(
  Name = "light",
  Color = avango.gua.Color(1.0, 1.0, 1.0)
)
light.Transform.value = avango.gua.make_trans_mat(1, 1, 2) * avango.gua.make_scale_mat(15, 15, 15)

monkey_transform1 = avango.gua.nodes.TransformNode(Name = "monkey_transform1")
monkey_transform1.Transform.value = avango.gua.make_trans_mat(1.0, 0.0, 0.0)
monkey_transform1.Children.value = [monkey1]

monkey_transform2 = avango.gua.nodes.TransformNode(Name = "monkey_transform2")
monkey_transform2.Transform.value = avango.gua.make_trans_mat(-1.0, 0.0, 0.0)
monkey_transform2.Children.value = [monkey2]

group = avango.gua.nodes.TransformNode(Name = "group")
group.Children.value = [monkey_transform1, monkey_transform2, light]

eye = avango.gua.nodes.TransformNode(Name = "eye")
eye.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 3.5)

screen = avango.gua.nodes.ScreenNode(Name = "screen", Width = 4, Height = 3)
screen.Children.value = [eye]

nettrans.Children.value = [group, screen]

graph.Root.value.Children.value = [nettrans]

make_distributable(group)
make_distributable(screen)

# setup viewing
#size = avango.gua.Vec2ui(1024, 768)
size = avango.gua.Vec2ui(800, 600)
pipe = avango.gua.nodes.Pipeline(
  Camera = avango.gua.nodes.Camera(
    LeftEye = "/net/screen/eye",
    LeftScreen = "/net/screen",
    SceneGraph = "scenegraph"
  ),
  Window = avango.gua.nodes.Window(Size = size,
                                   LeftResolution = size),
  LeftResolution = size,
  BackgroundMode = avango.gua.BackgroundMode.COLOR
)

pipe.Enabled.value = True

#setup viewer
viewer = avango.gua.nodes.Viewer()
viewer.Pipelines.value = [pipe]
viewer.SceneGraphs.value = [graph]

monkey_updater = TimedRotate()

timer = avango.nodes.TimeSensor()
monkey_updater.TimeIn.connect_from(timer.Time)

monkey1.Transform.connect_from(monkey_updater.MatrixOut)
monkey2.Transform.connect_from(monkey_updater.MatrixOut)

guaVE = GuaVE()
guaVE.start(locals(), globals())

viewer.run()

