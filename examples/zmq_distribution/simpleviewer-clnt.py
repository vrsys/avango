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

'''
A distributed viewer setup: This Python scripts starts an avango.osg.simpleviewer
and connects to the distribution group "testgroup". If there is another instance
loading some geometry under this node this model should also appear in the client.
(see also simpleviewer-srv.py)
'''

import avango
import avango.script
import avango.gua
import time
import math
import sys

from avango import enable_logging
#avango.enable_logging(5)
# specify role, ip, and port
nettrans = avango.gua.nodes.NetMatrixTransform(Groupname = "AVCLIENT|127.0.0.1|7432")

graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")
graph.Root.value.Children.value = [nettrans]

#setup viewer
viewer = avango.gua.nodes.Viewer()
viewer.Pipelines.value = [pipe]
viewer.SceneGraphs.value = [graph]

viewer.run()
