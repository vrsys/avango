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

VR16      = "141.54.147.16"
PAN       = "141.54.147.52"
LOCALHOST = "127.0.0.1"
DAEDALOS  = "141.54.147.34"

SPACEMONSTER = "141.54.147.101"

KERBEROS = "141.54.147.20"

CURRENTLY_USED_SERVER = KERBEROS


CLIENT_MODE = "MEASUREMENT_ANAGLYPH"
#CLIENT_MODE = "VIDEO_POWERWALL"
#CLIENT_MODE = "SCREENSHOT_DESKTOP"
##CLIENT_MODE = "DEBUG_3_USERS_WEAK_PC"

DEBUG_MODE = "NONE"
#DEBUG_MODE = "OCCLUSION_SLAVE_DEBUG"
#DEBUG_MODE = "CENTRAL_USER"

STEREO_MODE = 0
WINDOW_RESOLUTION = 0
RENDERING_RESOLUTION = 0
LEFT_VIEWPORT_START  = 0
RIGHT_VIEWPORT_START = 0

if "MEASUREMENT_ANAGLYPH" == CLIENT_MODE:
  STEREO_MODE = avango.gua.StereoMode.ANAGLYPH_RED_CYAN

  if "OCCLUSION_SLAVE_DEBUG" == DEBUG_MODE:
    #WINDOW_RESOLUTION    = avango.gua.Vec2ui(3840, 2160)
    #RENDERING_RESOLUTION = avango.gua.Vec2ui(3840, 2160)
    #RENDERING_RESOLUTION = avango.gua.Vec2ui(4096, 2160)
    if True:
      WINDOW_RESOLUTION    = avango.gua.Vec2ui(3840, 2160)
      RENDERING_RESOLUTION = avango.gua.Vec2ui(3840, 2160)
      LEFT_VIEWPORT_START  = avango.gua.Vec2ui(0, 0)
      RIGHT_VIEWPORT_START = avango.gua.Vec2ui(0, 0)
    else:
      WINDOW_RESOLUTION    = avango.gua.Vec2ui(1280, 720)
      RENDERING_RESOLUTION = avango.gua.Vec2ui(1280, 720)
      LEFT_VIEWPORT_START  = avango.gua.Vec2ui(0, 0)
      RIGHT_VIEWPORT_START = avango.gua.Vec2ui(0, 0)

  else:
    if False:
      WINDOW_RESOLUTION    = avango.gua.Vec2ui(4096, 2160)
      RENDERING_RESOLUTION = avango.gua.Vec2ui(4096 - 400 - 425, 2160)
      #RENDERING_RESOLUTION = avango.gua.Vec2ui(4096, 2160)
      LEFT_VIEWPORT_START  = avango.gua.Vec2ui(400, 0)
      RIGHT_VIEWPORT_START = avango.gua.Vec2ui(400, 0)
    else:
      WINDOW_RESOLUTION    = avango.gua.Vec2ui(4096, 2160)
      RENDERING_RESOLUTION = avango.gua.Vec2ui(4096, 2160)
      #RENDERING_RESOLUTION = avango.gua.Vec2ui(4096, 2160)
      LEFT_VIEWPORT_START  = avango.gua.Vec2ui(0, 0)
      RIGHT_VIEWPORT_START = avango.gua.Vec2ui(4096, 0)
  
  DISPLAY_VARIABLE_LEFT   = ":0.1"
  DISPLAY_VARIABLE_CENTER = ":0.1"
  DISPLAY_VARIABLE_RIGHT  = ":0.1" # for the occlusion slave, one GPU is rendering everything
elif "VIDEO_POWERWALL" == CLIENT_MODE:
  DISPLAY_VARIABLE_LEFT   = ":0.1"
  DISPLAY_VARIABLE_CENTER = ":0.1"
  DISPLAY_VARIABLE_RIGHT  = ":0.1" 
  STEREO_MODE = avango.gua.StereoMode.SIDE_BY_SIDE
  
  
  if True:
    WINDOW_RESOLUTION    = avango.gua.Vec2ui(2*4096, 2160)
    RENDERING_RESOLUTION = avango.gua.Vec2ui(4096 - 400 - 425, 2160)
    #RENDERING_RESOLUTION = avango.gua.Vec2ui(4096, 2160)
    LEFT_VIEWPORT_START  = avango.gua.Vec2ui(400, 0)
    RIGHT_VIEWPORT_START = avango.gua.Vec2ui(4096 + 400, 0)
  else:
    WINDOW_RESOLUTION    = avango.gua.Vec2ui(2*4096, 2160)
    RENDERING_RESOLUTION = avango.gua.Vec2ui(4096, 2160)
    #RENDERING_RESOLUTION = avango.gua.Vec2ui(4096, 2160)
    LEFT_VIEWPORT_START  = avango.gua.Vec2ui(0, 0)
    RIGHT_VIEWPORT_START = avango.gua.Vec2ui(4096, 0)    
  

  if False:
    WINDOW_RESOLUTION    = avango.gua.Vec2ui(2*4096, 2160)
    RENDERING_RESOLUTION = avango.gua.Vec2ui(4096 - 400 - 425, 2160)
    #RENDERING_RESOLUTION = avango.gua.Vec2ui(4096, 2160)
    LEFT_VIEWPORT_START  = avango.gua.Vec2ui(400, 0)
    RIGHT_VIEWPORT_START = avango.gua.Vec2ui(4096 + 400, 0)
  else:
    WINDOW_RESOLUTION    = avango.gua.Vec2ui(2*1024, 512)
    RENDERING_RESOLUTION = avango.gua.Vec2ui(1024, 512)
    #RENDERING_RESOLUTION = avango.gua.Vec2ui(4096, 2160)
    LEFT_VIEWPORT_START  = avango.gua.Vec2ui(0, 0)
    RIGHT_VIEWPORT_START = avango.gua.Vec2ui(1024, 0) 


elif "SCREENSHOT_DESKTOP" == CLIENT_MODE:
  STEREO_MODE          = avango.gua.StereoMode.MONO
  WINDOW_RESOLUTION    = avango.gua.Vec2ui(128, 72)
  RENDERING_RESOLUTION = WINDOW_RESOLUTION
  LEFT_VIEWPORT_START  = avango.gua.Vec2ui(0, 0)
  RIGHT_VIEWPORT_START = avango.gua.Vec2ui(0, 0)
  DISPLAY_VARIABLE_LEFT   = ":0.4"
  DISPLAY_VARIABLE_CENTER = ":0.2"
  DISPLAY_VARIABLE_RIGHT  = ":0.1"
elif "DEBUG_3_USERS_WEAK_PC" == CLIENT_MODE:
  STEREO_MODE = avango.gua.StereoMode.ANAGLYPH_RED_CYAN
  #WINDOW_RESOLUTION    = avango.gua.Vec2ui(1400, 1600)
  #WINDOW_RESOLUTION    = avango.gua.Vec2ui(3840, 2160)
  WINDOW_RESOLUTION    = avango.gua.Vec2ui(128, 72)
  RENDERING_RESOLUTION    = WINDOW_RESOLUTION
  LEFT_VIEWPORT_START     = avango.gua.Vec2ui(0, 0)
  RIGHT_VIEWPORT_START    = avango.gua.Vec2ui(0, 0)
  DISPLAY_VARIABLE_LEFT   = ":0"
  DISPLAY_VARIABLE_CENTER = ":0"
  DISPLAY_VARIABLE_RIGHT  = ":0"

class Initializer(avango.script.Script):

  def __init__(self):
    self.super(Initializer).__init__()

    # scenegraph
    self.nettrans = avango.gua.nodes.NetTransform(Name="net",
                                         # specify role, ip, and port
                                         Groupname="AVCLIENT|"+CURRENTLY_USED_SERVER+"|7432") #server
                                         #Groupname="AVCLIENT|141.54.147.54|7432")
    
    self.graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
    self.graph.Root.value.Children.value = [self.nettrans]

    print("Before setting is initialized")

    # viewing setup
    #size = avango.gua.Vec2ui(1600, 1200)
    #size = avango.gua.Vec2ui(1920, 1080)
    #size = avango.gua.Vec2ui(100, 100)
    size = avango.gua.Vec2ui(128, 72)    
    #size = avango.gua.Vec2ui(3840, 2160)
    self.window_center = avango.gua.nodes.GlfwWindow(Size=size,
                                              Display = DISPLAY_VARIABLE_CENTER,  # ":0.1",
                                              LeftResolution=size,
                                              RightResolution=size,
                                              Title="slave_weimar_v0_osaka_center")

    self.window_center.EnableVsync.value = False
    avango.gua.register_window("slave_weimar_v0_osaka_center", self.window_center)
    
    if "CENTRAL_USER" != DEBUG_MODE: 
      self.window_left = avango.gua.nodes.GlfwWindow(Size=size,
                                                Display = DISPLAY_VARIABLE_LEFT,  # ":0.1",
                                                LeftResolution=size,
                                                RightResolution=size,
                                                Title="slave_weimar_v0_osaka_left")

      self.window_left.EnableVsync.value = False
      avango.gua.register_window("slave_weimar_v0_osaka_left", self.window_left)

    if "CENTRAL_USER" != DEBUG_MODE: 
      self.window_right = avango.gua.nodes.GlfwWindow(Size=size,
                                                Display = DISPLAY_VARIABLE_RIGHT,  # ":0.1",
                                                LeftResolution=size,
                                                RightResolution=size,
                                                Title="slave_weimar_v0_osaka_right")

      self.window_right.EnableVsync.value = False
      avango.gua.register_window("slave_weimar_v0_osaka_right", self.window_right)

    logger = avango.gua.nodes.Logger(EnableWarning=False)

    #self.MemoryController = avango.gua.nodes.NamedSharedMemoryController()
    #self.MemoryController.add_read_only_memory_segment("DEPTH_FEEDBACK_SEGMENT")

    #self.MemoryController.register_remotely_constructed_object_on_segment("DEPTH_FEEDBACK_SEGMENT", "DEPTH_FEEDBACK_SEMAPHOR")


    #print("Before adding shared memory segments")
    #add shared depth buffer memory

    self.viewer = avango.gua.nodes.Viewer()
    self.viewer.SceneGraphs.value = [self.graph]
    
    if "CENTRAL_USER" != DEBUG_MODE: 
      self.viewer.Windows.value = [self.window_center, self.window_right, self.window_left]
    else:
      self.viewer.Windows.value = [self.window_center]

    self.viewer.DesiredFPS.value = 1000.0

    self.viewer.run()
    #while True:
    #  depth_feedback_state = self.MemoryController.get_value_from_named_object("DEPTH_FEEDBACK_SEMAPHOR")
      #print("Read State")
      #if 0 == depth_feedback_state:
        #self.MemoryController.set_value_for_named_object("DEPTH_FEEDBACK_SEMAPHOR", 1)
    #  self.viewer.frame()
      #elif 2 == depth_feedback_state:
      #  pass
        #print("Depth buffer was successfully written.")



    # parameters
    print("Before setting is initialized")
    self.is_initialized = False
    print("always evaluate")
    self.always_evaluate(True)
    print("after always evaluate")



  def evaluate(self):
    print("beginning of evaluation")
    if not self.is_initialized:
      if len(self.nettrans.Children.value) > 0:
        self.on_arrival()
        self.is_initialized = True

        #self.always_evaluate(False)


  def on_arrival(self):
    pass
    #print("I HAVE ARRIVED")
    #print(self.graph["/net/screen/cam"].Name.value)

    #occlusion_slave_pipeline_description = avango.gua.nodes.PipelineDescription(
    #    Passes=[
    #        avango.gua.nodes.TriMeshPassDescription(),
    #        avango.gua.nodes.LightVisibilityPassDescription(),
    #        avango.gua.nodes.SPointsPassDescription(),
    #        avango.gua.nodes.OcclusionSlaveResolvePassDescription()
    #    ])
    #self.graph["/net/screen/cam"].PipelineDescription.value = occlusion_slave_pipeline_description

    #print("Reconfigured pipeline")



init = Initializer()

