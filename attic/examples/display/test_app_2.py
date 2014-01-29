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

import avango
import avango.display
import sys

argv = avango.display.init(sys.argv)

xsplit = 0.5
ysplit = 0.5

def setup_view(viewport, color, background):
    view = avango.display.make_view()
    view.EnableTrackball.value = True
    scene = avango.osg.nodes.Sphere(Radius=0.1, Matrix=avango.osg.make_trans_mat(0, 1.7, -3), Color=color)
    view.Root.value = scene
    view.Viewport.value = viewport
    view.BackgroundColor.value = background

setup_view(avango.osg.Vec4(0., 0., xsplit, ysplit), avango.osg.Vec4(1, 0, 0, 0), avango.osg.Vec4(0.2, 0.2, 0.2, 1.))
setup_view(avango.osg.Vec4(xsplit, ysplit, 1., 1.), avango.osg.Vec4(0, 1, 0, 0), avango.osg.Vec4(0.4, 0.4, 0.4, 1.))
setup_view(avango.osg.Vec4(xsplit, 0., 1, ysplit), avango.osg.Vec4(0, 0, 1, 0), avango.osg.Vec4(0.6, 0.6, 0.6, 1.))
setup_view(avango.osg.Vec4(0., ysplit, xsplit, 1.), avango.osg.Vec4(0, 1, 1, 0), avango.osg.Vec4(0.8, 0.8, 0.8, 1.))

avango.display.run()
