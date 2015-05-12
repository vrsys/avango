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
A distributed viewer setup: This Python scripts starts an
avango.osg.simpleviewer and connects to the distribution group "testgroup". If
there is another instance loading some geometry under this node this model
should also appear in the client.  (see also simpleviewer-srv.py)
'''

import avango
import avango.script
import avango.gua
import avango.gua.gui
from avango.script import field_has_changed

from examples_common.GuaVE import GuaVE

#avango.enable_logging(4, "client.log")

class NetInit(avango.script.Script):
  NetChildren = avango.gua.MFNode()
  WindowName  = avango.SFString()
  Viewer      = avango.gua.nodes.Viewer()

  """
  @field_has_changed(NetChildren)
  def update(self):
    for tmp in self.NetChildren.value:
      for child in tmp.Children.value:
        if isinstance(child, avango.gua.CameraNode):
          if child.OutputWindowName.value == self.WindowName.value:
            # self.Viewer.CameraNodes.value = [child]
            pass
  """

nettrans = avango.gua.nodes.NetTransform(
  Name = "net",
  # specify role, ip, and port
  Groupname = "AVCLIENT|127.0.0.1|7432"
)

loader = avango.gua.nodes.TriMeshLoader()

graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")
graph.Root.value.Children.value = [nettrans]

resolution = avango.gua.Vec2ui(1920, 1080)

window = avango.gua.nodes.GlfwWindow(
  Size = resolution,
  LeftResolution = resolution,
  Title = "client_window"
)
avango.gua.register_window("client_window", window)

logger = avango.gua.nodes.Logger(
  EnableWarning = False
)

viewer = avango.gua.nodes.Viewer()
viewer.SceneGraphs.value = [graph]
viewer.Windows.value = [window]

init = NetInit()
init.WindowName.value = "client_window"
init.Viewer = viewer
init.NetChildren.connect_from(nettrans.Children)

guaVE = GuaVE()
guaVE.start(locals(), globals())

viewer.run()
