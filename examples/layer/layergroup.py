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

# this script demonstrates the use of a LayerGroup. Replace LayerGroup with
# Group to see the difference. Also you can see one of LayerGroups issues
# with wrong inheritance of Transformations.
#
# See avango/osg/include/LayerGroup.h for more information on bugs and
# missing features.

import avango.display
import avango.osg

import sys
import math

argv = avango.display.init(sys.argv)
view = avango.display.make_view()

panel1 = avango.osg.nodes.Panel(PanelColor = avango.osg.Vec4(1,0,0,0.5),
                                Width = 2,
                                Height = 1,
                                Name = "panel1",
                                )

panel2 = avango.osg.nodes.Panel(PanelColor = avango.osg.Vec4(0.8, 0.8, 0.8, 0.35),
                                Height = 1.8,
                                Name = "panel2",
                                )

panel3 = avango.osg.nodes.Panel(PanelColor = avango.osg.Vec4(0.2, 0.2, 1, 0.25),
                                Width = 1.5,
                                Height = 1.5,
                                Name = "panel3",
                                )

text = avango.osg.nodes.Text(String = "Layering is easy!",
                             Size = 0.2,
                             Alignment = 4,
                             Fontname = "VeraBI.ttf",
                             Position = avango.osg.Vec3(0,-0.2,0),
                             Color = avango.osg.Vec4(1,1,0,0.5),
                             Name = "text",
                             )

logo = avango.osg.nodes.TexturedQuad(Filename = "AvangoLogo.png",
                                     Width = 2.008,
                                     Height = 0.648,
                                     MaxAnisotropy = 16,
                                     Position = avango.osg.Vec3(-1.004, 0.42, 0),
                                     Name = "logo",
                                     )

icon = avango.osg.nodes.TexturedQuad(Filename = "delete.png",
                                     Width = 0.3,
                                     Height = 0.3,
                                     MaxAnisotropy = 16,
                                     Position = avango.osg.Vec3(-0.7, 0.12, 0),
                                     Name = "icon",
                                     )

sphere = avango.osg.nodes.Sphere(Matrix = avango.osg.make_trans_mat(1.7,0.3,1),
                                 Radius = 0.17,
                                 Color = avango.osg.Vec4(0.2,1,0.2,1),
                                 Name = "sphere",
                                 )

state = avango.osg.nodes.StateSet(BlendMode = 1,
                                  LightingMode = 0,
                                  RenderingHint = 2,
                                  )

panelgeode = avango.osg.nodes.Geode(Drawables = [panel1,panel2,panel3],
                                    StateSet = state,
                                    )

logogeode = avango.osg.nodes.Geode(Drawables = [logo,text],
                                   StateSet = state,
                                   )

icongeode = avango.osg.nodes.Geode(Drawables = [icon],
                                   StateSet = state,
                                   )

midgroup = avango.osg.nodes.MatrixTransform(Matrix = avango.osg.make_trans_mat(1,0,1),
                                            Children = [panelgeode,logogeode],
                                            )

layergroup = avango.osg.nodes.LayerGroup(Children=[midgroup,icongeode])

trans1 = avango.osg.nodes.MatrixTransform(Children=[layergroup, sphere])

root = avango.osg.nodes.MatrixTransform(Children=[trans1],
                                        Matrix = avango.osg.make_trans_mat(0,1.7,-20))


view.Root.value = root
avango.display.run()
