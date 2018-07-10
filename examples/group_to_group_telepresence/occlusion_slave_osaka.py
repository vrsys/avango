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
from avango.script import field_has_changed

from examples_common.GuaVE import GuaVE

#avango.enable_logging(4, "client.log")


class NetInit(avango.script.Script):
    NetChildren = avango.gua.MFNode()
    WindowName = avango.SFString()
    Viewer = avango.gua.nodes.Viewer()
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


nettrans = avango.gua.nodes.NetTransform(Name="net",
                                         # specify role, ip, and port
                                         Groupname="AVCLIENT|141.54.147.52|7432")

loader = avango.gua.nodes.TriMeshLoader()

graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
graph.Root.value.Children.value = [nettrans]

size = avango.gua.Vec2ui(800, 600)

window = avango.gua.nodes.GlfwWindow(Size=size,
                                     LeftResolution=size,
                                     Title="client_window_weimar")
avango.gua.register_window("client_window_weimar", window)

logger = avango.gua.nodes.Logger(EnableWarning=False)

viewer = avango.gua.nodes.Viewer()
viewer.SceneGraphs.value = [graph]
viewer.Windows.value = [window]

init = NetInit()
init.WindowName.value = "client_window_weimar"
init.Viewer = viewer
init.NetChildren.connect_from(nettrans.Children)

guaVE = GuaVE()
guaVE.start(locals(), globals())



MemoryController = avango.gua.nodes.NamedSharedMemoryController()
MemoryController.add_read_only_memory_segment("DEPTH_FEEDBACK_SEGMENT")

MemoryController.register_remotely_constructed_object_on_segment("DEPTH_FEEDBACK_SEGMENT", "DEPTH_FEEDBACK_SEMAPHOR")


curr_val_to_set = 0

# get signal
while True:
  #curr_val_to_set+=1
  #MemoryController.get_value_from_named_object("atomic_semaphor") 
  #print(MemoryController.get_value_from_named_object("DEPTH_FEEDBACK_SEMAPHOR"))

  #print("SHARED VALUE: " + str(MemoryController.get_value_from_named_object("DEPTH_FEEDBACK_SEMAPHOR")) )
  #if MemoryController.get_value_from_named_object("DEPTH_FEEDBACK_SEMAPHOR") < 1:
  #  print("Occlusion Slave Waiting For Signal\n")
  #else:
  #  print("Starting Occlusion Slave!!")
  #  break
  current_signal_value = MemoryController.get_value_from_named_object("DEPTH_FEEDBACK_SEMAPHOR")
  if 0 == current_signal_value:
    print("Rendering Frame!")
    MemoryController.set_value_for_named_object("DEPTH_FEEDBACK_SEMAPHOR", 1)
    viewer.frame()
  viewer.frame()
#print("::::::")
  #if MemoryController.get_value_from_named_object("DEPTH_FEEDBACK_SEMAPHOR") == 0:
  #  print("Rendering One Frame!")
  #  MemoryController.set_value_for_named_object("DEPTH_FEEDBACK_SEMAPHOR", 1)
    #viewer.frame()
  #else:
  #  print("Resetting counter to 0!")
  #  MemoryController.set_value_for_named_object("DEPTH_FEEDBACK_SEMAPHOR", 0)


# avango frame:[
  #while true
    # if signal == 0 (request state)
      #-> set signal to 1 (wait state)
      #-> trigger frame rendering ()
#]

# guacamole frame:[
  #render:
    # for each camera: 
      #write depth buffer to shared memory
      #set signal to 2 (g-buffer written)
#]

# recon server frame:[
  # set signal to 0 (== request)
  # wait til signal == 2 (request answered)
  # use gbuffer to perform reconstruction
#]

#viewer.run()
