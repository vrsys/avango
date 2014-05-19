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
Learn how to setup and use DeviceDaemon and DeviceService to readout input data
from a HIDInput device.

This example manipulates the position of a displayed monkey
based on the output the device specified in mouse-daemon.py generates.
'''

import avango.daemon
import avango.script
import avango.gua

# create a simple scene
avango.gua.load_shading_models_from("data/materials")
avango.gua.load_materials_from("data/materials")

# setup scenegraph
graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")

loader = avango.gua.nodes.TriMeshLoader()
monkey = loader.create_geometry_from_file("monkey", "data/objects/monkey.obj", "Stones", avango.gua.LoaderFlags.DEFAULTS)

light = avango.gua.nodes.PointLightNode(Name = "light", Color = avango.gua.Color(1.0, 1.0, 1.0))
light.Transform.value = avango.gua.make_trans_mat(1, 1, 2) * avango.gua.make_scale_mat(15, 15, 15)

eye = avango.gua.nodes.TransformNode(Name = "eye")
eye.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 3.5)

screen = avango.gua.nodes.ScreenNode(Name = "screen", Width = 4, Height = 3)
screen.Children.value = [eye]

# ------------------------------
transform = avango.gua.nodes.TransformNode()
transform.Children.value.append(monkey)

graph.Root.value.Children.value = [transform, light, screen]

# setup viewing
size = avango.gua.Vec2ui(1024, 768)
pipe = avango.gua.nodes.Pipeline(Camera = avango.gua.nodes.Camera(LeftEye = "/screen/eye",
                                                                  RightEye = "/screen/eye",
                                                                  LeftScreen = "/screen",
                                                                  RightScreen = "/screen",
                                                                  SceneGraph = "scenegraph"),
                                 Window = avango.gua.nodes.Window(Size = size,
                                                                  LeftResolution = size),
                                 LeftResolution = size)

#setup viewer
viewer = avango.gua.nodes.Viewer()
viewer.Pipelines.value = [pipe]
viewer.SceneGraphs.value = [graph]

# device service setup
service = avango.daemon.DeviceService()

# a sensor to a specific station
sensor = avango.daemon.nodes.DeviceSensor(DeviceService = service,
                                          Station = "mousestation")

# process output from sensor
class Move(avango.script.Script) :
    SensorX = avango.SFFloat()
    SensorY = avango.SFFloat()
    MatrixOut = avango.gua.SFMatrix4()

    def evaluate(self):
        values = self.get_values()
        trans_x = values.SensorX
        trans_y = values.SensorY
        trans_x /= -350.
        trans_y /= -350.
        old_pos = values.MatrixOut.get_translate()
        matrix = avango.gua.make_trans_mat(old_pos.x + trans_x, old_pos.y, old_pos.z + trans_y)
        values.MatrixOut = matrix

move = Move()

# manipulate the position of the monkey
move.SensorX.connect_from(sensor.Value0)
move.SensorY.connect_from(sensor.Value1)
#transform.Matrix.connect_from(move.MatrixOut)
transform.Transform.connect_from(move.MatrixOut)

viewer.run()
