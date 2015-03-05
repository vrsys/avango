import avango
import avango.script
from avango.script import field_has_changed
import avango.gua

import examples_common.navigator
from examples_common.GuaVE import GuaVE

def start():

  width = 1920;
  height = int(width * 9.0 / 16.0)
  size = avango.gua.Vec2ui(width, height)

  main_scene  = avango.gua.nodes.SceneGraph(Name = "main_scene")
  portal_scene  = avango.gua.nodes.SceneGraph(Name = "portal_scene")
  
  loader = avango.gua.nodes.TriMeshLoader()

  # main scene -----------------------------------------------------------------
  desc = avango.gua.nodes.MaterialShaderDescription()
  desc.load_from_file("data/materials/portal.gmd")
  avango.gua.register_material_shader(desc, "portal_mat")

  # portals
  cylinder = loader.create_geometry_from_file(
    "cylinder",
    "data/objects/cylinder.obj",
  )
  cylinder.Material.value.ShaderName.value = "portal_mat"
  cylinder.Transform.value = avango.gua.make_trans_mat(2.5, 0, 0)

  plane = loader.create_geometry_from_file(
    "plane",
    "data/objects/plane.obj",
  )
  plane.Material.value.ShaderName.value = "portal_mat"
  plane.Transform.value = avango.gua.make_trans_mat(-2.5, 0, 0) * avango.gua.make_rot_mat(90, 1, 0, 0)

  cube = loader.create_geometry_from_file(
    "cube",
    "data/objects/cube.obj",
  )
  cube.Material.value.ShaderName.value = "portal_mat"

  main_scene.Root.value.Children.value.append(cube)
  main_scene.Root.value.Children.value.append(cylinder)
  main_scene.Root.value.Children.value.append(plane)

  light = avango.gua.nodes.PointLightNode(
    Name = "sun",
    Color = avango.gua.Color(1.0, 1.0, 0.7),
    Brightness = 10,
    Transform = avango.gua.make_trans_mat(0, 3, 0) * avango.gua.make_scale_mat(100)
  )
  main_scene.Root.value.Children.value.append(light)

  screen = avango.gua.nodes.ScreenNode(
    Name = "screen",
    Width = 4.8,
    Height = 2.7,
    Transform = avango.gua.make_trans_mat(0.0, 0.0, -2.5)
  )

  camera = avango.gua.nodes.CameraNode(
    Name = "cam",
    LeftScreenPath = "/cam/screen",
    SceneGraph = "main_scene",
    Resolution = size,
    OutputWindowName = "window",
    Children = [screen],
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 7.0)
  )

  main_scene.Root.value.Children.value.append(camera)

  floor = loader.create_geometry_from_file(
    "floor",
    "data/objects/plane.obj",
    avango.gua.LoaderFlags.DEFAULTS
  )
  floor.Transform.value = avango.gua.make_scale_mat(6, 1, 2) * avango.gua.make_trans_mat(0, -1, 0)
  main_scene.Root.value.Children.value.append(floor)

  # portal scene -----------------------------------------------------------------
  sponza = loader.create_geometry_from_file(
    "sponza",
    "/opt/3d_models/Sponza/sponza.obj",
    avango.gua.LoaderFlags.LOAD_MATERIALS
  )
  sponza.Transform.value = avango.gua.make_scale_mat(0.05) * avango.gua.make_trans_mat(0, -50, 0)

  portal_scene.Root.value.Children.value.append(sponza)

  light = avango.gua.nodes.PointLightNode(
    Name = "sun",
    Color = avango.gua.Color(1.0, 1.0, 0.7),
    Brightness = 10,
    Transform = avango.gua.make_trans_mat(0, 3, 0) * avango.gua.make_scale_mat(100)
  )
  portal_scene.Root.value.Children.value.append(light)

  screen = avango.gua.nodes.ScreenNode(
    Name = "screen",
    Width = 4.8,
    Height = 2.7,
    Transform = avango.gua.make_trans_mat(0.0, 0.0, -2.5)
  )

  portal_camera = avango.gua.nodes.CameraNode(
    Name = "cam",
    LeftScreenPath = "/cam/screen",
    SceneGraph = "portal_scene",
    Resolution = size,
    OutputTextureName = "portal",
    Children = [screen],
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 7.0)
  )

  portal_scene.Root.value.Children.value.append(camera)

  # window setup ---------------------------------------------------------------
  window = avango.gua.nodes.Window(
    Size = size,
    Title = "picking",
    LeftResolution = size
  )

  camera.PreRenderCameras.value.append(portal_camera)

  avango.gua.register_window("window", window)

  navigator = examples_common.navigator.Navigator()
  navigator.StartLocation.value = camera.Transform.value.get_translate()
  navigator.OutTransform.connect_from(camera.Transform)

  navigator.RotationSpeed.value = 0.2
  navigator.MotionSpeed.value = 0.04

  camera.Transform.connect_from(navigator.OutTransform)
  portal_camera.Transform.connect_from(navigator.OutTransform)

  viewer = avango.gua.nodes.Viewer()
  viewer.CameraNodes.value = [camera]
  viewer.SceneGraphs.value = [main_scene, portal_scene]
  viewer.Windows.value = [window]

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()

