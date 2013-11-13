#!/usr/bin/python

import avango
import avango.script
from avango.script import field_has_changed
import avango.gua

import examples_common.navigator
from examples_common.GuaVE import GuaVE

def setup_pipe():
  camera = avango.gua.nodes.Camera(LeftEye = "/eye",
                                   RightEye = "/eye",
                                   LeftScreen = "/eye/screen",
                                   RightScreen = "/eye/screen",
                                   SceneGraph = "weimar")

  width = 1920;
  height = int(width * 9.0 / 16.0)
  size = avango.gua.Vec2ui(width, height)

  window = avango.gua.nodes.Window(Size = size,
                                   Title = "weimar",
                                   LeftResolution = size)

  pipe = avango.gua.nodes.Pipeline(Camera = camera,
                                   Window = window,
                                   LeftResolution = size)


  avango.gua.create_texture("data/textures/sky.jpg")

  pipe.OutputTextureName.value = "weimar_pipe"
  pipe.EnableSsao.value = True
  pipe.SsaoRadius.value = 5
  pipe.SsaoIntensity.value = 0.7

  pipe.EnableFog.value = True
  pipe.FogTexture.value = "data/textures/sky.jpg"
  pipe.FogStart.value = 11.0
  pipe.FogEnd.value = 25.0

  pipe.BackgroundTexture.value = "data/textures/sky.jpg"

  pipe.EnableVignette.value = True
  pipe.VignetteColor.value = avango.gua.Color()
  pipe.VignetteCoverage.value = 0.5
  pipe.VignetteSoftness.value = 0.6

  pipe.EnableFXAA.value = True
  pipe.EnableFrustumCulling.value = True

  pipe.EnableHDR.value = True
  pipe.HDRKey.value = 3.0

  pipe.EnableBloom.value = True
  pipe.BloomRadius.value = 1.9
  pipe.BloomThreshold.value = 0.9
  pipe.BloomIntensity.value = 0.3

  return pipe

def start():

  avango.gua.load_shading_models_from("data/materials")
  avango.gua.load_materials_from("data/materials")

  graph  = avango.gua.nodes.SceneGraph(Name = "weimar")
  loader = avango.gua.nodes.GeometryLoader()

  weimar = loader.create_geometry_from_file("weimar",
          "/opt/3d_models/architecture/weimar_geometry/"
          "weimar_stadtmodell_latest_version/weimar_stadtmodell_final.obj",
          "Bone",
          avango.gua.LoaderFlags.DEFAULTS)

  weimar.Transform.value = avango.gua.make_rot_mat(90.0, 0.0, 1.0, 0.0) * \
                           avango.gua.make_scale_mat(0.05, 0.05, 0.05)

  light = avango.gua.nodes.SpotLightNode(Name = "spot_light",
                                         Color = avango.gua.Color(1.0, 1.0, 1.0),
                                         Falloff = 0.009,
                                         Softness = 0.003,
                                         EnableShadows = True,
                                         ShadowMapSize = 4096,
                                         ShadowOffset = 0.0005)

  light.Transform.value = avango.gua.make_trans_mat(0.0, 40.0, 40.0) * \
                          avango.gua.make_rot_mat(-45.0, 1.0, 0.0, 0.0) * \
                          avango.gua.make_scale_mat(100, 100, 160)

  screen = avango.gua.nodes.ScreenNode(Name = "screen",
                                       Width = 4.8,
                                       Height = 2.7)


  screen.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -2.5)

  textured_node = avango.gua.nodes.TexturedQuadNode(Name = "quad",
                                                    Width = 2.4,
                                                    Height = 1.35,
                                                    Texture = "weimar_pipe")

  textured_node.Transform.value = avango.gua.make_trans_mat(0.0, 1.5, 0.0)


  eye = avango.gua.nodes.TransformNode(Name = "eye")

  eye.Transform.value = avango.gua.make_trans_mat(0.0, 2.8, 14.0)

  eye.Children.value = [screen]

  graph.Root.value.Children.value = [weimar, light, eye, textured_node]

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

