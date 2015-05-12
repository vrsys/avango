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
avango.osg.simpleviewer to load a given geometry. Any client connected to the
group "testgroup" should receive this model. (see also simpleviewer-clnt.py)
'''

import avango
import avango.script
import avango.gua
import avango.gua.gui
from avango.script import field_has_changed
import time

from examples_common.GuaVE import GuaVE

class FPSUpdater(avango.script.Script):
  TimeIn = avango.SFFloat()
  FPSResource = avango.gua.gui.SFGuiResource()
  Window = avango.gua.SFWindowBase()
  Viewer = avango.gua.SFViewer()

  @field_has_changed(TimeIn)
  def update_fps(self):
    application_string = "{:5.2f}".format(self.Viewer.value.ApplicationFPS.value)
    rendering_string = "{:5.2f}".format(self.Window.value.RenderingFPS.value)
    fps_string = "FPS: " + application_string + " " + rendering_string
    self.FPSResource.value.call_javascript("set_fps_text", [fps_string])

nettrans = avango.gua.nodes.NetTransform(
  Name = "net",
  # specify role, ip, and port
  Groupname = "AVSERVER|127.0.0.1|7432"
)

def make_node_distributable(node):
  for child in node.Children.value:
    make_node_distributable(child)
  nettrans.distribute_object(node)

def make_material_distributable(mat):
  nettrans.distribute_object(mat)

def start():
  # setup scenegraph
  graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")

  web = avango.gua.gui.nodes.GuiResource(
    TextureName = "google",
    URL = "https://www.google.com",
    Size = avango.gua.Vec2(1024, 1024)
  )

  focused_element = web

  web_mat = avango.gua.nodes.Material()
  web_mat.set_uniform("ColorMap", "google")
  web_mat.set_uniform("Emissivity", 0.0)

  transform = avango.gua.nodes.TransformNode()
  transform.Transform.value = avango.gua.make_scale_mat(0.5) * avango.gua.make_rot_mat(40, 1, 0, 0)

  resolution = avango.gua.Vec2ui(1920, 1080)

  loader = avango.gua.nodes.TriMeshLoader()
  google_geom = loader.create_geometry_from_file("google_geom", "data/objects/curved_plane.obj", web_mat,
                                                 avango.gua.LoaderFlags.NORMALIZE_POSITION |
                                                 avango.gua.LoaderFlags.NORMALIZE_SCALE |
                                                 avango.gua.LoaderFlags.MAKE_PICKABLE )

  transform.Children.value.append(google_geom)

  fps_size = avango.gua.Vec2(170, 55)

  fps = avango.gua.gui.nodes.GuiResource(
    TextureName = "fps",
    URL = "asset://gua/data/html/fps.html",
    Size = fps_size
  )

  fps_quad = avango.gua.nodes.TexturedScreenSpaceQuadNode(
    Name = "fps_quad",
    Texture = "fps",
    Width = int(fps_size.x),
    Height = int(fps_size.y),
    Anchor = avango.gua.Vec2(1.0, 1.0),
    Children = [fps]
  )

  address_bar_size = avango.gua.Vec2(340, 55)

  address_bar = avango.gua.gui.nodes.GuiResource(
    TextureName = "address_bar",
    URL = "asset://gua/data/html/address_bar.html",
    Size = address_bar_size
  )

  def address_bar_loaded():
    address_bar.add_javascript_callback("update_address")
    address_bar.add_javascript_callback("address_back")
    address_bar.add_javascript_callback("address_forward")

  def address_bar_handle_callback(callback, params):
    if callback == "update_address":
      web.URL.value = params[0]
    elif callback == "address_back":
      web.go_back()
    elif callback == "address_forward":
      web.go_forward()

  address_bar.on_loaded(address_bar_loaded)
  address_bar.on_javascript_callback(address_bar_handle_callback)

  address_bar_quad = avango.gua.nodes.TexturedScreenSpaceQuadNode(
    Name = "address_bar_quad",
    Texture = "address_bar",
    Width = int(address_bar_size.x),
    Height = int(address_bar_size.y),
    Anchor = avango.gua.Vec2(-1.0, 1.0),
    Children = [address_bar]
  )

  google_geom.Children.value.append(address_bar_quad)
  google_geom.Children.value.append(fps_quad)


  light = avango.gua.nodes.LightNode(
    Type=avango.gua.LightType.POINT,
    Name = "light",
    Color = avango.gua.Color(1.0, 1.0, 1.0),
    Brightness = 80,
    Transform = avango.gua.make_trans_mat(-2, 3, 5) * avango.gua.make_scale_mat(50)
  )

  server_cam = avango.gua.nodes.CameraNode(
    ViewID = 1,
    LeftScreenPath = "/net/screen",
    SceneGraph = "scenegraph",
    Resolution = resolution,
    OutputWindowName = "server_window",
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 1.5)
  )

  client_cam = avango.gua.nodes.CameraNode(
    ViewID = 2,
    LeftScreenPath = "/net/screen",
    SceneGraph = "scenegraph",
    Resolution = resolution,
    OutputWindowName = "client_window",
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 1.5)
  )

  screen = avango.gua.nodes.ScreenNode(
    Name = "screen",
    Width = 1.92 * 0.25,
    Height = 1.08 * 0.25,
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 2.0),
    Children = [server_cam, client_cam]
  )

  graph.Root.value.Children.value = [nettrans]

  make_node_distributable(transform)
  make_node_distributable(light)
  make_node_distributable(screen)

  nettrans.Children.value = [transform, light, screen]

  window = avango.gua.nodes.GlfwWindow(
    Size = resolution,
    LeftResolution = resolution,
    Title = "server_window"
  )

  avango.gua.register_window("server_window", window)

  def handle_char(c):
    nonlocal focused_element
    focused_element.inject_char_event(c)

  def handle_key(key, scancode, action, mods):
    nonlocal focused_element
    focused_element.inject_keyboard_event(key, scancode, action, mods)

  def handle_button(button, action, mods):
    nonlocal focused_element
    focused_element.inject_mouse_button(button, action, mods)

  def handle_cursor(pos):
    nonlocal focused_element
    hit_pos = avango.gua.Vec2()

    if (address_bar_quad.pixel_to_texcoords(pos, resolution, hit_pos)):
      address_bar.inject_mouse_position_relative(hit_pos)
      focused_element = address_bar

    else:
      res = avango.gua.Vec2(resolution.x, resolution.y)
      screen_space_pos = pos/res - 0.5

      origin = screen.ScaledWorldTransform.value * avango.gua.Vec4(screen_space_pos.x, screen_space_pos.y, 0, 1)
      direction = origin - server_cam.WorldTransform.value * avango.gua.Vec4(0,0,0,1)

      ray = avango.gua.nodes.Ray(
        Origin = avango.gua.Vec3(origin.x, origin.y, origin.z),
        Direction = avango.gua.Vec3(direction.x, direction.y, direction.z) * 100.0,
        TMax = 1.0
      )

      result = graph.ray_test(ray, avango.gua.PickingOptions.PICK_ONLY_FIRST_OBJECT
                                   | avango.gua.PickingOptions.PICK_ONLY_FIRST_FACE
                                   | avango.gua.PickingOptions.GET_TEXTURE_COORDS)

      for i in range(0, len(result.value)):
        if result.value[i].Object.value == google_geom:
          web.inject_mouse_position_relative(result.value[i].TextureCoords.value);
          focused_element = web

  def handle_scroll(scroll):
    web.inject_mouse_wheel(scroll)

  window.on_char(handle_char)
  window.on_key_press(handle_key)
  window.on_button_press(handle_button)
  window.on_move_cursor(handle_cursor)
  window.on_scroll(handle_scroll)


  #setup viewer
  viewer = avango.gua.nodes.Viewer()
  viewer.SceneGraphs.value = [graph]
  viewer.Windows.value = [window]

  timer = avango.nodes.TimeSensor()

  fps_updater = FPSUpdater(
    FPSResource=fps,
    Window=window,
    Viewer=viewer
  )
  fps_updater.TimeIn.connect_from(timer.Time)

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()

if __name__ == '__main__':
  start()