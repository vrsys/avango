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

import avango.script
from avango.script import field_has_changed
import avango.osg

class NodeOptimizer(avango.script.Script):
    Node = avango.osg.SFNode()
    Trigger = avango.SFBool()

    def __init__(self):
        self.super(NodeOptimizer).__init__()
        
        self.__triggered = False

        self.Name.value = "NodeOptimizer"

    @field_has_changed(Trigger)
    def loading_finished(self):
        self.__triggered = self.Trigger.value

    def evaluate(self):
        if self.__triggered and self.Node.value:
            avango.osg.optimize_scene_graph(self.Node.value)

