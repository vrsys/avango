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
import avango.osg
import avango.osg.viewer
import avango.tools
import avango.utils
import avango.display
import sys

argv = avango.display.init(sys.argv)
view = avango.display.make_view()

scene = avango.osg.nodes.MatrixTransform(Matrix=avango.osg.make_trans_mat(0,1.7,-30))
#view.EnableTrackball.value = True

# a node that receives data (from a client)
class Input(avango.utils.InterconnectInputNode):

    def __init__(self):
        self.super(Input).__init__()

    def node_was_added(self, node):
        scene.Children.value.append(node)

    def node_was_deleted(self, node):
        scene.Children.value.remove(node)

# a node that sends data (to a client)
class Output(avango.utils.InterconnectOutputNode):
    pass

server = avango.utils.InterconnectServer(Port=19901)

input_node = Input()
output_node = Output()

input_node.set_stream(server)
output_node.set_stream(server)

view.Root.value = scene

avango.display.run()
