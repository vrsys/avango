#!/usr/bin/python

import avango
import avango.daemon
import avango.gua
import avango.oculus
from examples_common.GuaVE import GuaVE

def create_monkeys(PARENT_NODE):
  loader = avango.gua.nodes.TriMeshLoader()

  num_monkeys = 6

  # create NUM monkeys around the user's head position
  for i in range(num_monkeys):
    monkey_geometry = loader.create_geometry_from_file("monkey_geometry" + str(i), "data/objects/monkey.obj", "data/materials/Stones.gmd", avango.gua.LoaderFlags.DEFAULTS)
    monkey_geometry.Transform.value = avango.gua.make_rot_mat(360 / num_monkeys * i, 0, 1, 0) * avango.gua.make_trans_mat(0, 0, -3)
    PARENT_NODE.Children.value.append(monkey_geometry)


def create_lights(PARENT_NODE):
  red_light = avango.gua.nodes.PointLightNode(Name = "red_light", Color = avango.gua.Color(1, 0, 0))
  red_light.Transform.value = avango.gua.make_trans_mat(0, 1, 2) * avango.gua.make_scale_mat(5, 5, 5)
  PARENT_NODE.Children.value.append(red_light)

  green_light = avango.gua.nodes.PointLightNode(Name = "green_light", Color = avango.gua.Color(0, 1, 0))
  green_light.Transform.value = avango.gua.make_rot_mat(120, 0, 1, 0) * avango.gua.make_trans_mat(0, 1, 2) * avango.gua.make_scale_mat(5, 5, 5)
  PARENT_NODE.Children.value.append(green_light)

  blue_light = avango.gua.nodes.PointLightNode(Name = "blue_light", Color = avango.gua.Color(0, 0, 1))
  blue_light.Transform.value = avango.gua.make_rot_mat(240, 0, 1, 0) * avango.gua.make_trans_mat(0, 1, 2) * avango.gua.make_scale_mat(5, 5, 5)
  PARENT_NODE.Children.value.append(blue_light)
  


def start():
  # oculus data (for the development kit)
  eye_distance = 0.064  # in meters
  oculus_resolution = avango.gua.Vec2ui(1280, 800)  # in pixels
  oculus_screensize = avango.gua.Vec2(0.16, 0.1)    # in meters

  # setup scenegraph and setup scene
  graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")
  create_monkeys(graph.Root.value)
  create_lights(graph.Root.value)

  # viewing setup for the OculusRift
  head = avango.gua.nodes.TransformNode(Name = "head")

  eye_left = avango.gua.nodes.TransformNode(Name = "eye_left")
  eye_left.Transform.value = avango.gua.make_trans_mat(-0.5 * eye_distance, 0.0, 0.0)

  eye_right = avango.gua.nodes.TransformNode(Name = "eye_right")
  eye_right.Transform.value = avango.gua.make_trans_mat(0.5 * eye_distance, 0.0, 0.0)

  screen_left = avango.gua.nodes.ScreenNode(Name = "screen_left")
  screen_left.Width.value = 0.5 * oculus_screensize.x
  screen_left.Height.value = oculus_screensize.y
  screen_left.Transform.value = avango.gua.make_trans_mat(-0.04, 0.0, -0.05)

  screen_right = avango.gua.nodes.ScreenNode(Name = "screen_right")
  screen_right.Width.value = 0.5 * oculus_screensize.x
  screen_right.Height.value = oculus_screensize.y
  screen_right.Transform.value = avango.gua.make_trans_mat(0.04, 0.0, -0.05)

  head.Children.value = [eye_left, eye_right, screen_left, screen_right]

  graph.Root.value.Children.value.append(head)

  # setup a stereo camera
  camera = avango.gua.nodes.Camera()
  camera.LeftEye.value = "/head/eye_left"
  camera.RightEye.value = "/head/eye_right"
  camera.LeftScreen.value = "/head/screen_left"
  camera.RightScreen.value = "/head/screen_right"
  camera.SceneGraph.value = "scenegraph"

  # create an OculusRift window with the barrel distortion
  window = avango.oculus.nodes.OculusWindow()
  window.Size.value = oculus_resolution
  window.LeftResolution.value = avango.gua.Vec2ui(oculus_resolution.x / 2, oculus_resolution.y)
  window.RightResolution.value = avango.gua.Vec2ui(oculus_resolution.x / 2, oculus_resolution.y)

  # setup the pipe
  pipe = avango.gua.nodes.Pipeline()
  pipe.Camera.value = camera
  pipe.Window.value = window
  pipe.LeftResolution.value = window.LeftResolution.value
  pipe.RightResolution.value = window.RightResolution.value
  pipe.EnableStereo.value = True

  # create a viewer
  viewer = avango.gua.nodes.Viewer()
  viewer.Pipelines.value = [pipe]
  viewer.SceneGraphs.value = [graph]

  # connect oculus rotation to head node
  oculus_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
  oculus_sensor.Station.value = "oculus-0"
  head.Transform.connect_from(oculus_sensor.Matrix)

  # create interactive shell and run the viewer
  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()

