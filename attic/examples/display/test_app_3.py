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

import avango
import avango.osg
import avango.display
import sys

argv = avango.display.init(sys.argv)

wiimote = avango.display.make_dominant_user_device()
sphere_transform = avango.osg.nodes.MatrixTransform()
sphere_transform.Matrix.connect_from(wiimote.Matrix)

sphere = avango.osg.nodes.Sphere(Radius=0.3, Matrix=avango.osg.make_trans_mat(0, 1.2, -3))
sphere_transform.Children.value = [sphere]

view = avango.display.make_view()
view.EnableTrackball.value = True
view.Root.value = sphere_transform

avango.display.run()
