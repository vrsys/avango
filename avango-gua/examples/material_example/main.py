#!/usr/bin/python

import avango
import avango.script
from avango.script import field_has_changed
import avango.gua

import examples_common.navigator
from examples_common.GuaVE import GuaVE

loader = avango.gua.nodes.GeometryLoader()

def setup_pipe():
  avango.gua.create_texture("data/textures/sky.jpg")

  width   = 1920;
  height  = int(width * 9.0 / 16.0)
  size    = avango.gua.Vec2ui(width, height)

  camera = avango.gua.nodes.Camera(
    LeftEye = "/eye",
    RightEye = "/eye",
    LeftScreen = "/eye/screen",
    RightScreen = "/eye/screen",
    SceneGraph = "materials"
  )

  window = avango.gua.nodes.Window(
    Size = size,
    Title = "Material Library",
    LeftResolution = size
  )

  return avango.gua.nodes.Pipeline(
    Camera = camera,
    Window = window,
    LeftResolution = size,
    EnableSsao = True,
    SsaoRadius = 5,
    SsaoIntensity = 0.7,

    EnableFog = True,
    FogTexture = "data/textures/sky.jpg",
    FogStart = 11.0,
    FogEnd = 25.0,

    BackgroundTexture = "data/textures/sky.jpg",

    EnableVignette = True,
    VignetteColor = avango.gua.Color(),
    VignetteCoverage = 0.5,
    VignetteSoftness = 0.6,

    EnableFXAA = True,
    EnableFrustumCulling = True,

    AmbientColor = avango.gua.Color(0.1, 0.1, 0.1)
  )


def display_material(parent, row, column, name):
  column_width = 2.0
  row_width = 2.0

  sphere = loader.create_geometry_from_file(
    name, "data/objects/sphere.obj", name,
    avango.gua.LoaderFlags.DEFAULTS
  )

  plane = loader.create_geometry_from_file(
    name, "data/objects/plane.obj", name,
    avango.gua.LoaderFlags.DEFAULTS
  )

  monkey = loader.create_geometry_from_file(
    name, "data/objects/monkey.obj", name,
    avango.gua.LoaderFlags.DEFAULTS
  )

  sphere.Transform.value = avango.gua.make_trans_mat(row*row_width, 0.0, column*column_width) * \
                           avango.gua.make_scale_mat(0.4, 0.4, 0.4)
  plane.Transform.value =  avango.gua.make_trans_mat(row*row_width, -0.4, column*column_width) * \
                           avango.gua.make_scale_mat(2, 1, 2)
  monkey.Transform.value =  avango.gua.make_trans_mat(row*row_width, 0.8, column*column_width) * \
                           avango.gua.make_scale_mat(0.3, 0.3, 0.3)

  parent.Children.value.append(sphere)
  parent.Children.value.append(plane)
  parent.Children.value.append(monkey)


def display_materials(parent):

  mats = {
    "Shadeless" : ["ShadelessBlue", "ShadelessWhite"],
    "EmitPhong" : ["EmitPhongGreen", "EmitPhongOrange"],
    "SimplePhong" : ["SimplePhongGreen", "SimplePhongBlack", "SimplePhongRed", "SimplePhongWhite"],
    "Parallax" : ["ParallaxBricks"],
    "Offset" : ["OffsetBricks"],
    "Chrome" : ["Chrome"],
    "Water" : ["Water"],
    "Stencil" : ["StencilRed"],
    "Planet" : ["PlanetPink", "PlanetYellow", "PlanetOrange", "PlanetGray"],
    "CarPaint" : ["CarPaintBlue", "CarPaintOrange", "CarPaintGreen"],
    "ComplexPhong" : ["ComplexPhongStone", "ComplexPhongWood", "ComplexPhongBricks", "ComplexPhongTiles", "ComplexPhongIvy", "ComplexPhongRock"]
  }

  x = 0;
  y = 0;

  for materials in mats.values():
    for material in materials:
      display_material(parent, x, y, material);
      y += 1
    x += 1
    y = 0


def start():
  avango.gua.load_shading_models_from("data/materials")
  avango.gua.load_materials_from("data/materials")

  graph  = avango.gua.nodes.SceneGraph(
    Name = "materials"
  )

  floor = loader.create_geometry_from_file(
    "floor",
    "data/objects/plane.obj", "SimplePhongWhite",
    avango.gua.LoaderFlags.DEFAULTS
  )
  floor.Transform.value = avango.gua.make_trans_mat(0.0, -0.5, 0.0) * avango.gua.make_scale_mat(100.0, 100.0, 100.0)

  light = avango.gua.nodes.SpotLightNode(
    Name = "spot_light",
    Color = avango.gua.Color(1.0, 1.0, 1.0),
    Softness = 0.003,
    # Falloff = 0.7,
    EnableShadows = True,
    ShadowMapSize = 4096,
    ShadowOffset = 0.001,
    Transform = avango.gua.make_trans_mat(0.0, 2.0, 1.0) * \
                avango.gua.make_rot_mat(-30.0, 1.0, 0.0, 0.0) * \
                avango.gua.make_scale_mat(15, 15, 15)
  )

  light.Falloff.value = 0.7

  sun = avango.gua.nodes.SpotLightNode(
    Name = "sun",
    Color = avango.gua.Color(1.0, 1.0, 1.0),
    Softness = 0.003,
    Falloff = 0.7,
    Transform = avango.gua.make_trans_mat(0.0, 100.0, 0.0) * \
                avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * \
                avango.gua.make_scale_mat(150, 150, 150)
  )

  screen = avango.gua.nodes.ScreenNode(
    Name = "screen",
    Width = 4.8,
    Height = 2.7,
    Transform = avango.gua.make_trans_mat(0.0, 0.0, -2.5)
  )

  eye = avango.gua.nodes.TransformNode(
    Name = "eye",
    Transform = avango.gua.make_trans_mat(0.0, 2.8, 14.0),
    Children = [screen, light]
  )

  graph.Root.value.Children.value = [floor, eye, sun]

  display_materials(graph.Root.value);

  pipe = setup_pipe()


  navigator = examples_common.navigator.Navigator()
  navigator.StartLocation.value = eye.Transform.value.get_translate()
  navigator.OutTransform.connect_from(eye.Transform)

  navigator.RotationSpeed.value = 0.2
  navigator.MotionSpeed.value = 0.04

  eye.Transform.connect_from(navigator.OutTransform)



  guaVE = GuaVE().start(locals(), globals())

  viewer = avango.gua.nodes.Viewer(
    Pipelines = [pipe],
    SceneGraphs = [graph]
  ).run()




if __name__ == '__main__':
  start()

