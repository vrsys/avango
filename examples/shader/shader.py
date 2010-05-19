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

import math

import avango.osg
import avango.inspector
import avango.display
import sys

argv = avango.display.init(sys.argv)
view = avango.display.make_view()
view.EnableTrackball.value = True


###############################################################################
# some light sources -> see also below the NumLights Uniform for the shader
light0 = avango.osg.nodes.Light(LightNum=0,
                               Ambient=avango.osg.Vec4(0.2,0.2,0.2,0.2),
                               Diffuse=avango.osg.Vec4(0.7,0.7,0.7,1.0),
                               Specular=avango.osg.Vec4(1.0,1.0,1.0,1.0),
                               Position=avango.osg.Vec4(3.0,3.0,3.0,1.0))
lightsource0 = avango.osg.nodes.LightSource(Light=light0)


light1 = avango.osg.nodes.Light(LightNum=1,
                               Ambient=avango.osg.Vec4(0.2,0.2,0.2,0.2),
                               Diffuse=avango.osg.Vec4(0.7,0.7,0.7,1.0),
                               Specular=avango.osg.Vec4(1.0,1.0,1.0,1.0),
                               Position=avango.osg.Vec4(-3.0,3.0,3.0,1.0))
lightsource1 = avango.osg.nodes.LightSource(Light=light1)


light2 = avango.osg.nodes.Light(LightNum=2,
                               Ambient=avango.osg.Vec4(0.2,0.2,0.2,0.2),
                               Diffuse=avango.osg.Vec4(0.7,0.7,0.7,1.0),
                               Specular=avango.osg.Vec4(1.0,1.0,1.0,1.0),
                               Position=avango.osg.Vec4(0.0,-3.0,3.0,1.0))
lightsource2 = avango.osg.nodes.LightSource(Light=light2)

light3 = avango.osg.nodes.Light(LightNum=3,
                               Ambient=avango.osg.Vec4(0.2,0.2,0.2,0.2),
                               Diffuse=avango.osg.Vec4(0.7,0.7,0.7,1.0),
                               Specular=avango.osg.Vec4(1.0,1.0,1.0,1.0),
                               Position=avango.osg.Vec4(0.0,0.0,-3.0,1.0))
lightsource3 = avango.osg.nodes.LightSource(Light=light3)


###############################################################################
# an example shader program
vshaderfile = "vshader.vert"
fshaderfile = "fshader.frag"



vshader = avango.osg.nodes.Shader(Name="VertexShader",
                                  Type=avango.osg.shadertype.VERTEX,
                                  FileName=vshaderfile)

fshader = avango.osg.nodes.Shader(Name="FragmentShader",
                                  Type=avango.osg.shadertype.FRAGMENT,
                                  FileName=fshaderfile)

prog = avango.osg.nodes.Program(ShaderList=[vshader,fshader])

ss = avango.osg.nodes.StateSet(Program = prog)

# Important: Values have to be specified first !!!
ss.Uniforms.value = [avango.osg.nodes.Uniform(
                                              Values=[4.0],
                                              Type=avango.osg.uniformtype.FLOAT,
                                              UniformName="NumLights"
                                              )
                     ]


###############################################################################
# some example models
model1 = avango.osg.nodes.Sphere(StateSet = avango.osg.nodes.StateSet(Uniforms = [avango.osg.nodes.Uniform( Values=[0.2,0.0,0.0,1.0],
                                                                                                            Type=avango.osg.uniformtype.FLOAT_VEC4,
                                                                                                            UniformName="ambient"),
                                                                                  avango.osg.nodes.Uniform( Values=[0.7,0.0,0.0,1.0],
                                                                                                            Type=avango.osg.uniformtype.FLOAT_VEC4,
                                                                                                            UniformName="diffuse"),
                                                                                  avango.osg.nodes.Uniform( Values=[1.0,1.0,1.0,1.0],
                                                                                                            Type=avango.osg.uniformtype.FLOAT_VEC4,
                                                                                                            UniformName="specular"),
                                                                                  avango.osg.nodes.Uniform( Values=[256.0],
                                                                                                            Type=avango.osg.uniformtype.FLOAT,
                                                                                                            UniformName="shininess"),

                                                                                  ]))

model1.Matrix.value = avango.osg.make_trans_mat(-5.0, 0.0, 0.0) * avango.osg.make_scale_mat(0.1, 0.1, 0.1)


model2 = avango.osg.nodes.Sphere(StateSet = avango.osg.nodes.StateSet(Uniforms = [avango.osg.nodes.Uniform( Values=[0.0,0.2,0.0,1.0],
                                                                                                            Type=avango.osg.uniformtype.FLOAT_VEC4,
                                                                                                            UniformName="ambient"),
                                                                                  avango.osg.nodes.Uniform( Values=[0.0,0.7,0.0,1.0],
                                                                                                            Type=avango.osg.uniformtype.FLOAT_VEC4,
                                                                                                            UniformName="diffuse"),
                                                                                  avango.osg.nodes.Uniform( Values=[1.0,1.0,1.0,1.0],
                                                                                                            Type=avango.osg.uniformtype.FLOAT_VEC4,
                                                                                                            UniformName="specular"),
                                                                                  avango.osg.nodes.Uniform( Values=[256.0],
                                                                                                            Type=avango.osg.uniformtype.FLOAT,
                                                                                                            UniformName="shininess"),

                                                                                  ]))

model2.Matrix.value = avango.osg.make_trans_mat(2.0, 0.0, 0.0) * avango.osg.make_scale_mat(0.2, 0.2, 0.2)




###############################################################################
# build the scenegraph
modelGroup = avango.osg.nodes.Group(StateSet=ss)
modelGroup.Children.value.append(model1)
modelGroup.Children.value.append(model2)



lightsource3.Children.value = [modelGroup]
lightsource2.Children.value = [lightsource3]
lightsource1.Children.value = [lightsource2]
lightsource0.Children.value = [lightsource1]


root = avango.osg.nodes.Group()
root.Children.value = [lightsource0]





###############################################################################
# visualize the Lightsources in Wireframe

wirestate = avango.osg.nodes.StateSet(WireframeMode = 1,
                                      Program = avango.osg.nodes.Program(ShaderList=[
            avango.osg.nodes.Shader(Name="VertexShader",Type=avango.osg.shadertype.VERTEX,ShaderSource="void main() { gl_Position = ftransform(); }"),
            avango.osg.nodes.Shader(Name="VertexShader",Type=avango.osg.shadertype.FRAGMENT,ShaderSource="void main() { gl_FragColor = gl_FrontMaterial.diffuse; }")
            ]))
lightcolor = avango.osg.Vec4(1.0,0.8,0.0,1.0)

root.Children.value.append(avango.osg.nodes.Sphere(StateSet = wirestate,
                                                   Color  = lightcolor,
                                                   Matrix = avango.osg.make_scale_mat(0.1,0.1,0.1) *
                                                   avango.osg.make_trans_mat(light0.Position.value.x,
                                                                             light0.Position.value.y,
                                                                             light0.Position.value.z,)))
root.Children.value.append(avango.osg.nodes.Sphere(StateSet = wirestate,
                                                   Color  = lightcolor,
                                                   Matrix = avango.osg.make_scale_mat(0.1,0.1,0.1) *
                                                   avango.osg.make_trans_mat(light1.Position.value.x,
                                                                             light1.Position.value.y,
                                                                             light1.Position.value.z,)))
root.Children.value.append(avango.osg.nodes.Sphere(StateSet = wirestate,
                                                   Color  = lightcolor,
                                                   Matrix = avango.osg.make_scale_mat(0.1,0.1,0.1) *
                                                   avango.osg.make_trans_mat(light2.Position.value.x,
                                                                             light2.Position.value.y,
                                                                             light2.Position.value.z,)))
root.Children.value.append(avango.osg.nodes.Sphere(StateSet = wirestate,
                                                   Color  = lightcolor,
                                                   Matrix = avango.osg.make_scale_mat(0.1,0.1,0.1) *
                                                   avango.osg.make_trans_mat(light3.Position.value.x,
                                                                             light3.Position.value.y,
                                                                             light3.Position.value.z,)))






###############################################################################
# run the example
view.Root.value = root

avango.display.run()
