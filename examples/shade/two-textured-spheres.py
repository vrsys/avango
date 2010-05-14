# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       #
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
import avango.osg.simpleviewer
import avango.shade

def make_sphere():
    dependencies = []
    image_loader = avango.osg.nodes.LoadImage(Filename="pattern.dds")
    dependencies.append(image_loader)
    texture = avango.osg.nodes.Texture2D()
    texture.Image.connect_from(image_loader.Image)
    xycoord = avango.shade.nodes.XYCoord()
    sampler = avango.shade.nodes.Texture2D(texture_unit=texture, uv=xycoord)
    constant = avango.shade.nodes.Constant(color=sampler)
    surface = avango.shade.nodes.Surface(material=constant)
    state = avango.shade.nodes.State(Shader = surface)

    sphere = avango.osg.nodes.Sphere(Radius=1.)
    stategroup = avango.shade.nodes.StateGroup(State=state, Children=[sphere])
    root = avango.osg.nodes.MatrixTransform(Children=[stategroup])

    return root, dependencies

sphere1, dep1 = make_sphere()
sphere1.Matrix.value = avango.osg.make_trans_mat(1., 0., 0.)
sphere2, dep2 = make_sphere()
sphere2.Matrix.value = avango.osg.make_trans_mat(-1., 0., 0.)

root = avango.osg.nodes.Group()
root.Children.value = [ sphere1, sphere2 ]

avango.osg.simpleviewer.run(root)
