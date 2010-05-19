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

import socket
import avango.connect
import avango.display
import sys


argv = avango.display.init(sys.argv)
view = avango.display.make_view()
view.EnableTrackball.value = True

# 1. simple scene
# ===============
scene_root = avango.osg.nodes.MatrixTransform(Matrix=avango.osg.make_trans_mat(0,1.7,-1))

text = avango.osg.nodes.Text(String = "",
                             Size = 0.022,
                             Alignment = 4)
geode = avango.osg.nodes.Geode(Drawables = [text])
scene_root.Children.value = [geode]

view.Root.value = scene_root


# 2. distribution setup
# =====================

# a node that receives data (from a server)
class Input(avango.connect.InputNode):
    Text = avango.SFString()

# a node that sends data (to a server)
class Output(avango.connect.OutputNode):
    Text = avango.SFString()

try:
    client = avango.connect.Client(host="localhost", port=19901)

    input_node = Input()
    output_node = Output()
    input_node.set_stream(client)
    output_node.set_stream(client)

    # forward data to local instance
    text.String.connect_from(input_node.Text)

except socket.error, msg:
    print "Error: " + str(msg)
    print "Not connected."

# enter render loop
avango.display.run()
