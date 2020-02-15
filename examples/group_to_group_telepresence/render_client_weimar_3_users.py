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

import sys
import subprocess


import avango
import avango.script
import avango.gua
import avango.gua.lod
from avango.script import field_has_changed

from examples_common.GuaVE import GuaVE

import time
#avango.enable_logging(4, "client.log")

#CLIENT_MODE = "MEASUREMENT_ANAGLYPH"
CLIENT_MODE = "VIDEO_POWERWALL"
#CLIENT_MODE = "SCREENSHOT_DESKTOP"
#CLIENT_MODE = "DEBUG_3_USERS_WEAK_PC"

DEBUG_MODE = "NONE"
#DEBUG_MODE = "OCCLUSION_SLAVE_DEBUG"
#DEBUG_MODE = "CENTRAL_USER"

#LOGGING_MODE = "DISABLED"
LOGGING_MODE = "ENABLED"

STEREO_MODE = 0
WINDOW_RESOLUTION = 0
RENDERING_RESOLUTION = 0
LEFT_VIEWPORT_START  = 0
RIGHT_VIEWPORT_START = 0

VR16      = "141.54.147.16"
PAN       = "141.54.147.52"
LOCALHOST = "127.0.0.1"
DAEDALOS  = "141.54.147.34"
LADON     = "141.54.147.47"

ARGOS     = "141.54.147.26"


SPACEMONSTER = "141.54.147.101"

ARACHNE   = "141.54.147.27"

KERBEROS = "141.54.147.20"

CURRENTLY_USED_SERVER = KERBEROS

#CURRENTLY_USED_SERVER = ARGOS

RES_MODE = "POWERWALL"
#RES_MODE = "HMD_LIKE"

DISPLAY_VARIABLE_LEFT   = ":0.0"
DISPLAY_VARIABLE_CENTER = ":0.1"
DISPLAY_VARIABLE_RIGHT  = ":0.2"

DISPLAY_VARIABLE_CAM_SLOT_1  = DISPLAY_VARIABLE_RIGHT
DISPLAY_VARIABLE_CAM_SLOT_2  = ":0.3"

if "MEASUREMENT_ANAGLYPH" == CLIENT_MODE:
  STEREO_MODE = avango.gua.StereoMode.ANAGLYPH_RED_CYAN
  #WINDOW_RESOLUTION    = avango.gua.Vec2ui(1400, 1600)
  WINDOW_RESOLUTION    = avango.gua.Vec2ui(3840, 2160)


  #WINDOW_RESOLUTION    = avango.gua.Vec2ui(1920, 1080)
  RENDERING_RESOLUTION = WINDOW_RESOLUTION
  LEFT_VIEWPORT_START  = avango.gua.Vec2ui(0, 0)
  RIGHT_VIEWPORT_START = avango.gua.Vec2ui(0, 0)
  DISPLAY_VARIABLE_LEFT   = ":0.2"
  DISPLAY_VARIABLE_CENTER = ":0.1"
  DISPLAY_VARIABLE_RIGHT  = ":0.0"
elif "VIDEO_POWERWALL" == CLIENT_MODE:
  STEREO_MODE = avango.gua.StereoMode.SIDE_BY_SIDE

  if "OCCLUSION_SLAVE_DEBUG" == DEBUG_MODE:
    if True:
      WINDOW_RESOLUTION    = avango.gua.Vec2ui(2*3840, 2160)
      RENDERING_RESOLUTION = avango.gua.Vec2ui(3840, 2160)
      #RENDERING_RESOLUTION = avango.gua.Vec2ui(4096, 2160)
      LEFT_VIEWPORT_START  = avango.gua.Vec2ui(0, 0)
      RIGHT_VIEWPORT_START = avango.gua.Vec2ui(3840, 0)
    else:
      WINDOW_RESOLUTION    = avango.gua.Vec2ui(2*1280, 720)
      RENDERING_RESOLUTION = avango.gua.Vec2ui(1280, 720)
      #RENDERING_RESOLUTION = avango.gua.Vec2ui(4096, 2160)
      LEFT_VIEWPORT_START  = avango.gua.Vec2ui(0, 0)
      RIGHT_VIEWPORT_START = avango.gua.Vec2ui(1280, 0)
      
  else:
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

elif "SCREENSHOT_DESKTOP" == CLIENT_MODE:
  STEREO_MODE          = avango.gua.StereoMode.MONO
  WINDOW_RESOLUTION    = avango.gua.Vec2ui(3840, 2160)
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
  WINDOW_RESOLUTION       = avango.gua.Vec2ui(1964, 1112)
  RENDERING_RESOLUTION    = WINDOW_RESOLUTION
  LEFT_VIEWPORT_START     = avango.gua.Vec2ui(0, 0)
  RIGHT_VIEWPORT_START    = avango.gua.Vec2ui(0, 0)
  DISPLAY_VARIABLE_LEFT   = ":0"
  DISPLAY_VARIABLE_CENTER = ":0"
  DISPLAY_VARIABLE_RIGHT  = ":0"

class TimedFPSPrinter(avango.script.Script):
  TimeIn = avango.SFFloat()
  WindowCenter = 0
  WindowLeft = 0
  WindowRight = 0

  AvatarNode = 0
  
  LoggingIndicatorNode = 0



  draw_time_left_dict = dict()
  draw_time_right_dict = dict()
  draw_time_center_dict = dict()

  recon_time_dict    = dict()
  reply_time_dict    = dict()
  tri_count_dict     = dict()
  payload_count_dict = dict()

  entry_count_dict = dict()

  num_entries = 0
  num_files_logged = 0

  frame_counter = 0

  logged_valid_frames = 0

  have_one_valid_timestamp = 0
  last_timestamp = 0

  stop_logging = 0

  already_logged_seconds = 0.0

  num_seconds_to_log = 0 

  def set_window_left(self, window_left):
    self.WindowLeft = window_left

  def set_window_right(self, window_right):
    self.WindowRight = window_right

  def set_window_center(self, window_center):
    self.WindowCenter = window_center

  def set_avatar_node(self, avatar_node):
    self.AvatarNode = avatar_node

  def set_logging_indicator_node(self, logging_indicator_node):
    self.LoggingIndicatorNode = logging_indicator_node

  @field_has_changed(TimeIn)
  def update(self):
    #return

    if len(sys.argv) == 2:
      self.num_seconds_to_log = int(sys.argv[1])

    self.frame_counter += 1

    if(self.WindowCenter != 0):
      if(0 != self.WindowCenter.RenderingFPS.value):
        #if(self.frame_counter % 100 == 0):
        elapsed_seconds = 1.0 / self.WindowCenter.RenderingFPS.value


        if elapsed_seconds != 1.0:
          #print("Center FPS: " + str(elapsed_seconds) )
          self.logged_valid_frames += 1

          #if self.logged_valid_frames > 100:


    #if self.LoggingIndicatorNode == 0:
    #  return

    if not "ENABLED" == LOGGING_MODE:
      return
    else:
      pass
      #return


    if self.logged_valid_frames > 100:
      print("Started Logging")

      if 1 == self.have_one_valid_timestamp:
        current_timestamp =  time.time()

        elapsed_time_since_last_evaluate_call = current_timestamp - self.last_timestamp
        print(elapsed_time_since_last_evaluate_call)
        self.last_timestamp = current_timestamp
        self.already_logged_seconds += elapsed_time_since_last_evaluate_call

        if self.already_logged_seconds > self.num_seconds_to_log:
          self.stop_logging = 1
      else:
         self.last_timestamp = time.time()
         self.have_one_valid_timestamp = 1
      #pass
    else:
      return
      #exit(-1)

    if (self.AvatarNode != 0) or (self.WindowCenter != 0) or (self.WindowLeft != 0) or (self.WindowRight != 0):
      #if len(self.LoggingIndicatorNode.Tags.value) > 0:

      if self.stop_logging == 0:
        self.AvatarNode.fetch_stats()

        current_stream_timestamp      = round(self.AvatarNode.get_stats_timestamp(), 5)
        current_tri_count             = self.AvatarNode.get_stats_num_triangles()
        current_recon_time            = self.AvatarNode.get_stats_reconstruction_time()
        current_request_reply_latency = self.AvatarNode.get_stats_request_reply_latency()
        current_total_message_payload = self.AvatarNode.get_stats_total_message_payload_in_byte()

        if (0 == self.WindowCenter.RenderingFPS.value) or (0 == self.WindowLeft.RenderingFPS.value) or (0 == self.WindowRight.RenderingFPS.value):
          return

        current_draw_time_center      = 1.0 / self.WindowCenter.RenderingFPS.value
        current_draw_time_left        = 1.0 / self.WindowLeft.RenderingFPS.value
        current_draw_time_right       = 1.0 / self.WindowRight.RenderingFPS.value

        if not current_stream_timestamp in self.entry_count_dict:
          self.draw_time_left_dict[current_stream_timestamp] = 0.0
          self.draw_time_right_dict[current_stream_timestamp] = 0.0
          self.draw_time_center_dict[current_stream_timestamp] = 0.0
          self.recon_time_dict[current_stream_timestamp] = 0.0
          self.reply_time_dict[current_stream_timestamp] = 0.0
          self.tri_count_dict[current_stream_timestamp] = 0
          self.payload_count_dict[current_stream_timestamp] = 0
          self.entry_count_dict[current_stream_timestamp] = 0

        self.draw_time_left_dict[current_stream_timestamp] += current_draw_time_left
        self.draw_time_right_dict[current_stream_timestamp] += current_draw_time_right
        self.draw_time_center_dict[current_stream_timestamp] += current_draw_time_center
        self.recon_time_dict[current_stream_timestamp] += current_recon_time
        self.reply_time_dict[current_stream_timestamp] += current_request_reply_latency
        self.tri_count_dict[current_stream_timestamp] += current_tri_count
        self.payload_count_dict[current_stream_timestamp] += current_total_message_payload
        self.entry_count_dict[current_stream_timestamp] += 1

        self.num_entries = self.num_entries + 1
      else:
        if self.num_entries > 0:
          log_tri_count = open("log_tri_count", 'w')
          for key, value in sorted(self.entry_count_dict.items()):
            log_tri_count.write(str(key) + " " + str(int(self.tri_count_dict[key] / value) ) + "\n" )
          log_tri_count.close()

          log_payload_count = open("log_payload_count", 'w')
          for key, value in sorted(self.entry_count_dict.items()):
            log_payload_count.write(str(key) + " " + str(int(self.payload_count_dict[key] / value) ) + "\n" )
          log_payload_count.close()

          log_recon_times = open("log_recon_times", 'w')
          for key, value in sorted(self.entry_count_dict.items()):
            log_recon_times.write(str(key) + " " + str(self.recon_time_dict[key] / value) + "\n" )
          log_recon_times.close()

          log_reply_times = open("log_reply_times", 'w')
          for key, value in sorted(self.entry_count_dict.items()):
            log_reply_times.write(str(key) + " " + str(self.reply_time_dict[key] / value) + "\n" )
          log_reply_times.close()

          log_draw_times_left = open("log_draw_times_left", 'w')
          for key, value in sorted(self.entry_count_dict.items()):
            log_draw_times_left.write(str(key) + " " + str(self.draw_time_left_dict[key] / value) + "\n" )
          log_draw_times_left.close()

          log_draw_times_right = open("log_draw_times_right", 'w')
          for key, value in sorted(self.entry_count_dict.items()):
            log_draw_times_right.write(str(key) + " " + str(self.draw_time_right_dict[key] / value) + "\n" )
          log_draw_times_right.close()

          log_draw_times_center = open("log_draw_times_center", 'w')
          for key, value in sorted(self.entry_count_dict.items()):
            log_draw_times_center.write(str(key) + " " + str(self.draw_time_center_dict[key] / value) + "\n" )
          log_draw_times_center.close()

          self.draw_time_left_dict.clear()
          self.draw_time_right_dict.clear()
          self.draw_time_center_dict.clear()

          self.recon_time_dict.clear()
          self.reply_time_dict.clear()
          self.tri_count_dict.clear()
          self.payload_count_dict.clear()
          self.entry_count_dict.clear()
          self.num_entries = 0

          self.num_files_logged += 1
            #print(key, value)


          subprocess.check_output(['killall','python3'])
          sys.exit()
          #perform logging
    #if self.LoggingIndicatorNode != 0:
    #  if len(self.LoggingIndicatorNode.Tags.value) > 0:
    #    print("LOGGING ENABLED")
    #  else:
    #    print("LOGGING DISABLED")

class Initializer(avango.script.Script):
  def __init__(self):
    self.super(Initializer).__init__()

    #scenegraph
    self.nettrans = avango.gua.nodes.NetTransform(Name="net",
                                         # specify role, ip, and port
                                         Groupname="AVCLIENT|"+CURRENTLY_USED_SERVER+"|7432")




    self.graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
    self.graph.Root.value.Children.value = [self.nettrans]

    # viewing setup
    #size = avango.gua.Vec2ui(3840, 1080)

    #rendering_res = avango.gua.Vec2ui(int(size.x/2), size.y)

    #left_pos = avango.gua.Vec2ui(0, 0)
    #right_pos = avango.gua.Vec2ui(rendering_res.x, 0)
         
    if "CENTRAL_USER" != DEBUG_MODE: 
      self.window_left = avango.gua.nodes.Window(Size=WINDOW_RESOLUTION,
                                        Display = DISPLAY_VARIABLE_LEFT,
                                        LeftPosition = LEFT_VIEWPORT_START,
                                        RightPosition = RIGHT_VIEWPORT_START,
                                        LeftResolution=RENDERING_RESOLUTION,
                                        RightResolution=RENDERING_RESOLUTION,
                                        StereoMode = STEREO_MODE,
                                        Title="client_window_weimar_left")
      self.window_left.EnableVsync.value = False
      avango.gua.register_window("client_window_weimar_left", self.window_left)
    

    self.window_center = avango.gua.nodes.Window(Size=WINDOW_RESOLUTION,
                                 Display = DISPLAY_VARIABLE_CENTER,  # ":0.1",
                                 LeftPosition = LEFT_VIEWPORT_START,
                                 RightPosition = RIGHT_VIEWPORT_START,
                                 LeftResolution=RENDERING_RESOLUTION,
                                 RightResolution=RENDERING_RESOLUTION,
                                 StereoMode = STEREO_MODE,
                                 Title="client_window_weimar_center")
    self.window_center.EnableVsync.value = False

    avango.gua.register_window("client_window_weimar_center", self.window_center)
     

    if "CENTRAL_USER" != DEBUG_MODE: 
      self.window_right = avango.gua.nodes.Window(Size=WINDOW_RESOLUTION,
                                        Display = DISPLAY_VARIABLE_RIGHT,  # ":0.1",
                                        LeftPosition = LEFT_VIEWPORT_START,
                                        RightPosition = RIGHT_VIEWPORT_START,
                                        LeftResolution=RENDERING_RESOLUTION,
                                        RightResolution=RENDERING_RESOLUTION,
                                        StereoMode = STEREO_MODE,
                                        Title="client_window_weimar_right")
      self.window_right.EnableVsync.value = False
      avango.gua.register_window("client_window_weimar_right", self.window_right)

    #if "VIDEO_POWERWALL" == CLIENT_MODE:
      # self.window_cam_slot_2 = avango.gua.nodes.Window(Size=WINDOW_RESOLUTION,
      #                                   Display = DISPLAY_VARIABLE_CAM_SLOT_2,  # ":0.1",
      #                                   LeftPosition = LEFT_VIEWPORT_START,
      #                                   RightPosition = RIGHT_VIEWPORT_START,
      #                                   LeftResolution=RENDERING_RESOLUTION,
      #                                   RightResolution=RENDERING_RESOLUTION,
      #                                   StereoMode = STEREO_MODE,
      #                                   Title="client_window_weimar_cam_slot_2")
      # self.window_cam_slot_2.EnableVsync.value = False
      # avango.gua.register_window("client_window_weimar_cam_slot_2", self.window_cam_slot_2)
   
    self.logger = avango.gua.nodes.Logger(EnableWarning=False)

    self.viewer = avango.gua.nodes.Viewer()
    self.viewer.SceneGraphs.value = [self.graph]
    
    self.viewer.DesiredFPS.value = 5000.0

    if "CENTRAL_USER" != DEBUG_MODE:
      # if "VIDEO_POWERWALL" == CLIENT_MODE:
      #   self.viewer.Windows.value = [self.window_center, self.window_left, self.window_right, self.window_cam_slot_2]
      # else:
      self.viewer.Windows.value = [self.window_center, self.window_left, self.window_right]
    else:
      self.viewer.Windows.value = [self.window_center]



    self.timer = avango.nodes.TimeSensor()

    self.fps_printer = TimedFPSPrinter()
    self.fps_printer.TimeIn.connect_from(self.timer.Time)

    self.fps_printer.set_window_center(self.window_center)
    if "CENTRAL_USER" != DEBUG_MODE: 
      self.fps_printer.set_window_left(self.window_left)
      self.fps_printer.set_window_right(self.window_right)

    self.is_initialized = False
    self.always_evaluate(True)
    #self.evaluate()
   
    self.viewer.run()

    # parameters
    #print("Before setting is initialized")

    #print("always evaluate")

    #print("after always evaluate")

  def evaluate(self):
    #print("beginning of evaluation")
    if not self.is_initialized:
      if len(self.nettrans.Children.value) > 0:
        #self.on_arrival()

        if "ENABLED" == LOGGING_MODE:
          self.is_initialized = True

          self.fps_printer.set_avatar_node(self.graph["/net/grouped_view_setups_and_scene/scene_view_transform/scene_transform/avatar_transform/kinect"])
          
          self.fps_printer.set_logging_indicator_node(self.graph["/net/grouped_view_setups_and_scene/logging_indicator"])
          self.always_evaluate(False)

  def on_arrival(self):
    pass



init = Initializer()
