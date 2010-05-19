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

import avango.osg
import avango.inspector
import avango.display

import math
import sys

argv = avango.display.init(sys.argv)
view = avango.display.make_view()

time_sensor = avango.nodes.TimeSensor()

root = avango.osg.nodes.MatrixTransform(Name="Root",Matrix=avango.osg.make_trans_mat(0,1.7,-20))
sphere1 = avango.osg.nodes.Sphere(Name="RedSphere", Color=avango.osg.Vec4(1., 0., 0., 1), DetailRatio = 0.8)
sphere1.Matrix.value = avango.osg.make_trans_mat(1, 0, 0)
sphere2 = avango.osg.nodes.Sphere(Name="GreenSphere", Color=avango.osg.Vec4(0., 1., 0., 1), DetailRatio = 0.8)
sphere2.Matrix.value = avango.osg.make_trans_mat(-1, 0, 0)
root.Children.value = [ sphere1, sphere2 ]


class Wobble(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.osg.SFMatrix()
    Offset = avango.SFFloat()

    def evaluate(self):
        self.MatrixOut.value = avango.osg.make_trans_mat( self.Offset.value + math.sin(self.TimeIn.value), 0., 0.)

wobble = Wobble(Name='Wobble')
sphere1.Matrix.connect_from(wobble.MatrixOut)
wobble.TimeIn.connect_from(time_sensor.Time)

inspector = avango.inspector.nodes.Inspector(Children=[root])

view.Root.value = root

avango.display.run()
