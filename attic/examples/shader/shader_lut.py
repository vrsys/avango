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
import avango.display
import sys


def build_quad():
    geometry = avango.osg.nodes.Geometry(Mode = 7, NormalBinding = 1, ColorBinding = 1)

    vertex_coords = []
    tex_coords = []
    uvs = [ (1., 1.), (0., 1.), (0., 0.), (1., 0.) ]
    for u, v in uvs:
        vertex_coords.append(avango.osg.Vec3(0.1*(2*u-1.), 0.1*(2.*v-1.), 0.))
        tex_coords.append(avango.osg.Vec2(u, v))

    geometry.VertexArray.value = vertex_coords
    geometry.TexCoordArray.value = tex_coords
    geometry.PrimitiveIndices.value = [0,1,2,3]
    geometry.PrimitiveLengths.value = [4]

    return avango.osg.nodes.Geode(Drawables=[geometry])


argv = avango.display.init(sys.argv)
view = avango.display.make_view()
view.EnableTrackball.value = True


###############################################################################
# an example shader program
vshaderfile = "lut.vert"
fshaderfile = "lut.frag"

vshader = avango.osg.nodes.Shader(Name="VertexShader",
                                  Type=avango.osg.shadertype.VERTEX,
                                  FileName=vshaderfile)

fshader = avango.osg.nodes.Shader(Name="FragmentShader",
                                  Type=avango.osg.shadertype.FRAGMENT,
                                  FileName=fshaderfile)

prog = avango.osg.nodes.Program(ShaderList=[vshader,fshader])

image_filename = "seismic.png"
image = avango.osg.read_image_file(image_filename)
texture = avango.osg.nodes.Texture2D(Image=image)

image_filename2 = "lut.png"
image2 = avango.osg.read_image_file(image_filename2)
texture2 = avango.osg.nodes.Texture2D(Image=image2)

ss = avango.osg.nodes.StateSet(Program = prog, Texture0 = texture, Texture1 = texture2)

# Important: Values have to be specified first !!!
ss.Uniforms.value = [avango.osg.nodes.Uniform(
                                              Values=[0],
                                              Type=avango.osg.uniformtype.INT,
                                              UniformName="seismic"
                                              ),
                     avango.osg.nodes.Uniform(
                                              Values=[1],
                                              Type=avango.osg.uniformtype.INT,
                                              UniformName="lut"
                                              )
                     ]



model1 = build_quad()




###############################################################################
# build the scenegraph
modelGroup = avango.osg.nodes.MatrixTransform(StateSet=ss)
modelGroup.Children.value.append(model1)

root = avango.osg.nodes.Group()
root.Children.value = [modelGroup]



###############################################################################
# run the example
view.Root.value = root

avango.display.run()
