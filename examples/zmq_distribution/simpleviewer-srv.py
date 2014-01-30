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
to load a given geometry. Any client connected to the group "testgroup" should
receive this model. (see also simpleviewer-clnt.py)
'''

#import avango.osg.simpleviewer
import avango
import avango.script
import avango.gua
import time
import math
import sys
from avango.script import field_has_changed

from avango import enable_logging
avango.enable_logging(5)

# specify role, ip, and port
nettrans = avango.gua.nodes.NetMatrixTransform(Name = "net", Groupname = "AVSERVER|127.0.0.1|7432")

###############################################################################
# an example shader program
#vshaderfile = "vshader.vert"
#fshaderfile = "fshader.frag"

#vshader = avango.osg.nodes.Shader(Name="VertexShader",
#                                  Type=avango.osg.shadertype.VERTEX,
#                                  FileName=vshaderfile)
#nettrans.distribute_object(vshader)

#fshader = avango.osg.nodes.Shader(Name="FragmentShader",
#                                  Type=avango.osg.shadertype.FRAGMENT,
#                                  FileName=fshaderfile)
#nettrans.distribute_object(fshader)

#prog = avango.osg.nodes.Program(ShaderList=[vshader,fshader])
#nettrans.distribute_object(prog)
#ss = avango.osg.nodes.StateSet(Program = prog)
#nettrans.distribute_object(ss)

#u1 = avango.osg.nodes.Uniform(
#                              Values=[2.0],
#                              Type=avango.osg.uniformtype.FLOAT,
#                              UniformName="NumLights"
#                              )
#nettrans.distribute_object(u1)
#
#ss.Uniforms.value = [u1]

###############################################################################

#obj = avango.osg.nodes.LoadFile(Filename=sys.argv[1])

#nettrans.distribute_object(obj)
#trans = avango.osg.nodes.MatrixTransform(StateSet = ss)
#nettrans.distribute_object(trans)
#trans.Children.value.append(obj)

#light0 = avango.osg.nodes.Light(Name="A Light",LightNum=1,
#                               Ambient=avango.osg.Vec4(0.2,0.2,0.2,0.2),
#                               Diffuse=avango.osg.Vec4(0.7,0.0,0.0,1.0),
#                               Specular=avango.osg.Vec4(1.0,0.0,0.0,1.0),
#                               Position=avango.osg.Vec4(3.0,3.0,3.0,1.0))
#nettrans.distribute_object(light0)
#lightsource0 = avango.osg.nodes.LightSource(Light=light0)
#lightsource0.Children.value.append(trans)
#nettrans.distribute_object(lightsource0)
#
#nettrans.Children.value.append(lightsource0)

class TimedRotate(avango.script.Script):
  TimeIn = avango.SFFloat()
  MatrixOut = avango.gua.SFMatrix4()

  @field_has_changed(TimeIn)
  def update(self):
    self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value*2.0, 0.0, 1.0, 0.0)

avango.gua.load_shading_models_from("data/materials")
avango.gua.load_materials_from("data/materials")

# setup scenegraph
graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")

loader = avango.gua.nodes.GeometryLoader()
monkey = loader.create_geometry_from_file("monkey", "data/objects/monkey.obj", "Stones", avango.gua.LoaderFlags.DEFAULTS)

nettrans.distribute_object(monkey)

light = avango.gua.nodes.PointLightNode(Name = "light", Color = avango.gua.Color(1.0, 1.0, 1.0))
light.Transform.value = avango.gua.make_trans_mat(1, 1, 2) * avango.gua.make_scale_mat(15, 15, 15)
nettrans.distribute_object(light)

if False :
  group = avango.gua.nodes.TransformNode(Name = "group")
  group.Children.value = [monkey, light]
  nettrans.distribute_object(group)
  nettrans.Children.value = [group]
else:
  nettrans.Children.value = [monkey, light]

eye = avango.gua.nodes.TransformNode(Name = "eye")
eye.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 3.5)

screen = avango.gua.nodes.ScreenNode(Name = "screen", Width = 4, Height = 3)
screen.Children.value = [eye]

graph.Root.value.Children.value = [nettrans, screen]

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

monkey_updater = TimedRotate()

timer = avango.nodes.TimeSensor()
monkey_updater.TimeIn.connect_from(timer.Time)

monkey.Transform.connect_from(monkey_updater.MatrixOut)

viewer.run()

