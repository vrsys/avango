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
import avango.display
import sys

class Rotation(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.osg.SFMatrix()

    def evaluate(self):
        self.MatrixOut.value = avango.osg.make_rot_mat(self.TimeIn.value, 0, 1, 0)


argv = avango.display.init(sys.argv)

text1 = avango.osg.nodes.Text(String = "LayerGeode",
                            Size = 0.2,
                            Alignment = 4,
                            Color = avango.osg.Vec4(0,0,0,1)
                            )
text2 = avango.osg.nodes.Text(String = "Billboard",
                            Size = 0.2,
                            Alignment = 4,
                            Color = avango.osg.Vec4(0,0,0,1)
                            )
panel1 = avango.osg.nodes.Panel(PanelColor = avango.osg.Vec4(1,1,0,0.5),
                            Width = 0.7,
                            Height = 0.25,
                            )
panel2 = avango.osg.nodes.Panel(PanelColor = avango.osg.Vec4(1,0,0,0.5),
                            Width = 0.7,
                            Height = 0.25,
                            )

layer_geode = avango.osg.nodes.LayerGeode(Drawables = [panel1, text1])

billboard = avango.osg.nodes.Billboard(Drawables = [panel2, text2])
billboard.Mode.value = 2  # AXIAL_ROT
billboard.Axis.value = avango.osg.Vec3(0.,1.,0.)
billboard.Normal.value = avango.osg.Vec3(0.0,0.0,1.0)

translation_layer_geode = avango.osg.nodes.MatrixTransform(
    Children=[layer_geode],
    Matrix =  avango.osg.make_trans_mat(0.8,0.2,0))
translation_billboard = avango.osg.nodes.MatrixTransform(
    Children=[billboard],
    Matrix =  avango.osg.make_trans_mat(0.8,-0.2,0))
rotation_boards = avango.osg.nodes.MatrixTransform(
    Children=[translation_billboard, translation_layer_geode])
translation_world = avango.osg.nodes.MatrixTransform(
    Children=[rotation_boards],
    Matrix = avango.osg.make_trans_mat(0,1.7,-13))

time_sensor = avango.nodes.TimeSensor()

rotation = Rotation()
rotation_boards.Matrix.connect_from(rotation.MatrixOut)
rotation.TimeIn.connect_from(time_sensor.Time)

root = avango.osg.nodes.Group(Name="Root")
root.Children.value = [ translation_world ]
view = avango.display.make_view()
view.Root.value = root

avango.display.run()
