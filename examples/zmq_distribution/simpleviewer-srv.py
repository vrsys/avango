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

import avango.osg.simpleviewer
import avango.script
import time
import math
import sys
from avango import enable_logging
#avango.enable_logging(5)
if len(sys.argv) < 2:
    print "Usage: "+sys.argv[0]+" <modelname>"
    sys.exit(1)



nettrans = avango.osg.nodes.NetMatrixTransform(Groupname = "AVSERVER|127.0.0.1|7432")



###############################################################################
# an example shader program
vshaderfile = "vshader.vert"
fshaderfile = "fshader.frag"



vshader = avango.osg.nodes.Shader(Name="VertexShader",
                                  Type=avango.osg.shadertype.VERTEX,
                                  FileName=vshaderfile)
nettrans.distribute_object(vshader)

fshader = avango.osg.nodes.Shader(Name="FragmentShader",
                                  Type=avango.osg.shadertype.FRAGMENT,
                                  FileName=fshaderfile)
nettrans.distribute_object(fshader)

prog = avango.osg.nodes.Program(ShaderList=[vshader,fshader])
nettrans.distribute_object(prog)
ss = avango.osg.nodes.StateSet(Program = prog)
nettrans.distribute_object(ss)

u1 = avango.osg.nodes.Uniform(
                                              Values=[2.0],
                                              Type=avango.osg.uniformtype.FLOAT,
                                              UniformName="NumLights"
                                              )
nettrans.distribute_object(u1)

ss.Uniforms.value = [u1]


###############################################################################










obj = avango.osg.nodes.LoadFile(Filename=sys.argv[1])

nettrans.distribute_object(obj)
trans = avango.osg.nodes.MatrixTransform(StateSet = ss)
nettrans.distribute_object(trans)
trans.Children.value.append(obj)







light0 = avango.osg.nodes.Light(Name="Fucking Light",LightNum=1,
                               Ambient=avango.osg.Vec4(0.2,0.2,0.2,0.2),
                               Diffuse=avango.osg.Vec4(0.7,0.0,0.0,1.0),
                               Specular=avango.osg.Vec4(1.0,0.0,0.0,1.0),
                               Position=avango.osg.Vec4(3.0,3.0,3.0,1.0))
nettrans.distribute_object(light0)
lightsource0 = avango.osg.nodes.LightSource(Light=light0)
lightsource0.Children.value.append(trans)
nettrans.distribute_object(lightsource0)

nettrans.Children.value.append(lightsource0)








class RotateTransform(object):
    def __init__(self, trans, u):
        self.trans = trans
        self.time = time.time()
        self.u = u
    def callback(self):

        # test matrix

        cur_rot = math.sin((time.time()-self.time)/2.)
        cur_mat = avango.osg.make_rot_mat(cur_rot, 1., 0., 0.)
        self.trans.Matrix.value = cur_mat

        # test uniform

        myvalue = math.sin(time.time()*5.0)
        if myvalue > 0.0:
            self.u.Values.value = [2.0]
        else:
            self.u.Values.value = [1.0]



rt = RotateTransform(trans,u1)
update = avango.script.nodes.Update(Callback = rt.callback, Active = True)

avango.osg.simpleviewer.run(nettrans)


