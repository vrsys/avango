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
A distributed viewer setup: This Python scripts starts an avango.osg.simpleviewer
and connects to the distribution group "testgroup". If there is another instance
loading some geometry under this node this model should also appear in the client.
(see also simpleviewer-srv.py)
'''

import avango
import avango.script
import avango.gua
import time
import math
import sys

from avango import enable_logging
# avango.enable_logging(6,"client-log.txt")
avango.enable_logging()
# specify role, ip, and port
nettrans = avango.gua.nodes.NetMatrixTransform(Name = "net", Groupname = "AVCLIENT|127.0.0.1|7432")

avango.gua.load_shading_models_from("data/materials")
avango.gua.load_materials_from("data/materials")

graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")

loader = avango.gua.nodes.GeometryLoader()
monkey = loader.create_geometry_from_file("monkey", "data/objects/monkey.obj", "Stones", avango.gua.LoaderFlags.DEFAULTS)

eye = avango.gua.nodes.TransformNode(Name = "eye")
eye.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 3.5)

screen = avango.gua.nodes.ScreenNode(Name = "screen", Width = 4, Height = 3)
screen.Children.value = [eye]

graph.Root.value.Children.value = [nettrans, screen]

# setup viewing
#size = avango.gua.Vec2ui(1024, 768)
size = avango.gua.Vec2ui(800, 600)
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

viewer.run()
