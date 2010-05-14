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
import avango.osg
import avango.script

class View(avango.script.Script):

    Root = avango.osg.SFNode()
    Camera = avango.osg.SFMatrix()
    Viewport = avango.osg.SFVec4()
    Depth = avango.SFFloat()
    UserSelector = avango.SFInt()
    Near = avango.SFFloat()
    Far = avango.SFFloat()
    BackgroundColor = avango.osg.SFVec4()

    def __init__(self):
        self.super(View).__init__()
        self.Viewport.value = avango.osg.Vec4(0, 0, 1, 1)
        self.Depth.value = 0
        self.Near.value = 0.1
        self.Far.value = 500.
        self.BackgroundColor.value = avango.osg.Vec4(0., 0., 0., 1.)
        self.UserSelector.value = 0

    def evaluate(self):
        pass
