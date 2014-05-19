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

import avango
import avango.gua
import avango.sound
import avango.sound.openal
from examples_common.GuaVE import GuaVE

soundtraverser = avango.sound.nodes.SoundTraverser()
openal_renderer = avango.sound.openal.nodes.OpenALSoundRenderer()
openal_renderer.Device.value = ""
soundtraverser.Renderers.value = [openal_renderer]

# set up scene graph
avango.gua.load_shading_models_from("data/materials")
avango.gua.load_materials_from("data/materials")

graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")
loader = avango.gua.nodes.TriMeshLoader()
obj = loader.create_geometry_from_file("monkey",
                                      "data/objects/monkey.obj",
                                      "Stones",
                                      avango.gua.LoaderFlags.DEFAULTS)
obj_trans = avango.gua.nodes.TransformNode()
obj_trans.Children.value = [obj]

#set up sound
stereosound = avango.sound.nodes.SoundSource()
obj_trans.Children.value.append(stereosound)
stereosound.URL.value = "applause.ogg"
stereosound.Loop.value = False

monosound = avango.sound.nodes.SoundSource()
obj_trans.Children.value.append(monosound)
monosound.URL.value = "applause_mono.ogg"
monosound.Loop.value = False

root_group = avango.gua.nodes.TransformNode(Name="sound")

light = avango.gua.nodes.PointLightNode(Name = "light",
                                        Color = avango.gua.Color(1.0, 1.0, 1.0))
light.Transform.value = avango.gua.make_trans_mat(1, 1, 2) * avango.gua.make_scale_mat(15, 15, 15)

eye = avango.gua.nodes.TransformNode(Name = "eye")
eye.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 3.5)

screen = avango.gua.nodes.ScreenNode(Name = "screen", Width = 4, Height = 3)
screen.Children.value = [eye]

root_group.Children.value = [obj_trans, light, screen]
graph.Root.value.Children.value = [root_group]

# set up viewing
size = avango.gua.Vec2ui(1024, 768)
pipe = avango.gua.nodes.Pipeline(
          Camera = avango.gua.nodes.Camera(
            LeftEye = "/sound/screen/eye",
            RightEye = "/sound/screen/eye",
            LeftScreen = "/sound/screen",
            RightScreen = "/sound/screen",
            SceneGraph = "scenegraph"),
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

soundtraverser.RootNode.value = root_group
soundtraverser.Traverse.value = True

openal_renderer.ListenerPosition.connect_from(eye.Transform)

stereosound.Play.value = True
monosound.Play.value = True

#viewer.frame()

guaVE = GuaVE()
guaVE.start(locals(), globals())
viewer.run()
