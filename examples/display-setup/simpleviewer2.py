# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of Avango.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# Avango is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# Avango is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with Avango. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
# Avango is a trademark owned by FhG.                                    #
#                                                                        #
##########################################################################

'''
This example shows the usage of avango.osg.simpleviewer to load
a given geometry file. A class RotateTransform is used to animate
the module.
'''

import avango.osg.simpleviewer
import avango.script
import math
import sys

if len(sys.argv) != 2:
    print "Usage: "+sys.argv[0]+" <modelname>"
    sys.exit(1)

obj = avango.osg.nodes.LoadFile(Filename=sys.argv[1])
trans = avango.osg.nodes.MatrixTransform()
trans.Children.value.append(obj)

class RotateTransform(avango.script.Script):
    'Build a time-variant matrix performing a rotation'

    TimeIn = avango.SFDouble()
    Matrix = avango.osg.SFMatrix()

    def evaluate(self):
        cur_rot = math.sin(self.TimeIn.value/2.)
        self.Matrix.value = avango.osg.make_rot_mat(cur_rot, 1., 0., 0.)

time = avango.nodes.TimeSensor()
rt = RotateTransform()

rt.TimeIn.connect_from(time.Time)
trans.Matrix.connect_from(rt.Matrix)

avango.osg.simpleviewer.run(trans)
