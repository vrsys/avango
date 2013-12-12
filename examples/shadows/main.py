#!/usr/bin/python

import avango
import avango.script
from avango.script import field_has_changed
import avango.gua

import examples_common.navigator
from examples_common.GuaVE import GuaVE

CUBE_COUNT_X = 30
CUBE_COUNT_Y = 1
CUBE_COUNT_Z = 30

def setup_pipe():
  camera = avango.gua.nodes.Camera(LeftEye = "/eye",
                                   RightEye = "/eye",
                                   LeftScreen = "/eye/screen",
                                   RightScreen = "/eye/screen",
                                   SceneGraph = "scene")

  width = 1920;
  height = int(width * 9.0 / 16.0)
  size = avango.gua.Vec2ui(width, height)

  window = avango.gua.nodes.Window(Size = size,
                                   Title = "weimar",
                                   LeftResolution = size)

  pipe = avango.gua.nodes.Pipeline(Camera = camera,
                                   Window = window,
                                   LeftResolution = size)


  avango.gua.create_texture("data/textures/skymap.jpg")

  pipe.OutputTextureName.value = "weimar_pipe"
  pipe.EnableSsao.value = True
  pipe.SsaoRadius.value = 5
  pipe.SsaoIntensity.value = 0.7

  pipe.EnableFog.value = True
  pipe.FogTexture.value = "data/textures/skymap.jpg"
  pipe.FogStart.value = 80.0
  pipe.FogEnd.value = 100.0

  pipe.BackgroundTexture.value = "data/textures/skymap.jpg"

  pipe.EnableVignette.value = True
  pipe.VignetteColor.value = avango.gua.Color()
  pipe.VignetteCoverage.value = 0.5
  pipe.VignetteSoftness.value = 0.6

  pipe.EnableFXAA.value = True
  pipe.EnableFrustumCulling.value = True

  pipe.EnableFPSDisplay.value = True
  pipe.EnableRayDisplay.value = True


  return pipe

def start():

  avango.gua.load_shading_models_from("data/materials")
  avango.gua.load_materials_from("data/materials")

  graph  = avango.gua.nodes.SceneGraph(Name = "scene")
  loader = avango.gua.nodes.GeometryLoader()

  for x in range(0, CUBE_COUNT_X):
    for y in range(0, CUBE_COUNT_Y):
      for z in range(0, CUBE_COUNT_Z):

        new_cube = loader.create_geometry_from_file("cube" + str(x) + str(y) + str(z),
                  "data/objects/monkey.obj",
                  "White",
                  avango.gua.LoaderFlags.DEFAULTS)

        new_cube.Transform.value = avango.gua.make_trans_mat(x*2, y*2, z*2) * \
                                   avango.gua.make_scale_mat(0.3, 0.3, 0.3)
        graph.Root.value.Children.value.append(new_cube)

  light = avango.gua.nodes.SunLightNode(Name = "spot_light",
                                         Color = avango.gua.Color(1.0, 1.0, 1.0),
                                         EnableGodrays = True,
                                         EnableShadows = True,
                                         ShadowMapSize = 4096,
                                         ShadowOffset = 0.0005)

  light.Transform.value = avango.gua.make_rot_mat(-45.0, 1.0, 0.0, 0.0)

  floor = loader.create_geometry_from_file("floor",
                  "data/objects/plane.obj",
                  "White",
                  avango.gua.LoaderFlags.DEFAULTS)
  floor.Transform.value = avango.gua.make_scale_mat(100, 1, 100) * avango.gua.make_trans_mat(0, -0.5, 0)
  graph.Root.value.Children.value.append(floor)

  screen = avango.gua.nodes.ScreenNode(Name = "screen",
                                       Width = 4.8,
                                       Height = 2.7)


  screen.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -2.5)

  eye = avango.gua.nodes.TransformNode(Name = "eye")
  eye.Transform.value = avango.gua.make_trans_mat(0.0, 2.8, 14.0)
  eye.Children.value = [screen]

  graph.Root.value.Children.value.append(light)
  graph.Root.value.Children.value.append(eye)

  pipe = setup_pipe()

  navigator = examples_common.navigator.Navigator()
  navigator.StartLocation.value = eye.Transform.value.get_translate()
  navigator.OutTransform.connect_from(eye.Transform)

  navigator.RotationSpeed.value = 0.2
  navigator.MotionSpeed.value = 0.04

  eye.Transform.connect_from(navigator.OutTransform)

  viewer = avango.gua.nodes.Viewer()
  viewer.Pipelines.value = [pipe]
  viewer.SceneGraphs.value = [graph]

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()

