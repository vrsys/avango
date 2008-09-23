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
Rendering objects in wireframe mode. Exemplary usage of the corresponding mode in StateSet.
'''

import avango.osg
import avango.osg.simpleviewer


root = avango.osg.nodes.Group()

# default (filled model)
root.Children.value.append(avango.osg.nodes.Sphere(StateSet = avango.osg.nodes.StateSet(WireframeMode = 0),
                                                   Color = avango.osg.Vec4(0.3,0.3,0.9,0.0),
                                                   Matrix = avango.osg.make_scale_mat(0.25,0.25,0.25) * avango.osg.make_trans_mat(0.5,0,0)))
# wireframe mode with lines
root.Children.value.append(avango.osg.nodes.Sphere(StateSet = avango.osg.nodes.StateSet(WireframeMode = 1),
                                                   Color = avango.osg.Vec4(0.3,0.3,0.9,0.0),
                                                   Matrix = avango.osg.make_scale_mat(0.25,0.25,0.25) * avango.osg.make_trans_mat(0,0,0)))
# wireframe mode with points
root.Children.value.append(avango.osg.nodes.Sphere(StateSet = avango.osg.nodes.StateSet(WireframeMode = 2),
                                                   Color = avango.osg.Vec4(0.3,0.3,0.9,0.0),
                                                   Matrix = avango.osg.make_scale_mat(0.25,0.25,0.25) * avango.osg.make_trans_mat(-0.5,0,0)))

avango.osg.simpleviewer.run(root)
