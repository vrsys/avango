import avango
import avango.script
from avango.script import field_has_changed
import avango.gua

import examples_common.navigator
from examples_common.GuaVE import GuaVE

class ClippingControl(avango.script.Script):
  CameraNode = avango.gua.SFCameraNode()
  ClippingPlaneNode = avango.gua.SFClippingPlaneNode()

  def __init__(self):
    self.super(ClippingControl).__init__()
    self.always_evaluate(True)

  def evaluate(self):
    direction = self.CameraNode.value.WorldTransform.value.get_translate() - self.ClippingPlaneNode.value.get_center()
    if self.ClippingPlaneNode.value.get_normal().dot(direction) > 0:
      self.ClippingPlaneNode.value.Tags.value = ["invisible"]
    else:
      self.ClippingPlaneNode.value.Tags.value = []

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

  cube = loader.create_geometry_from_file(
    "cube",
    "data/objects/cube.obj",
  )
  cube.Transform.value = avango.gua.make_scale_mat(2)
  cube.Material.value.ShaderName.value = "portal_mat"
  cube.Material.value.EnableBackfaceCulling.value = False

  main_scene.Root.value.Children.value.append(cube)

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

  res_pass = avango.gua.nodes.ResolvePassDescription()
  res_pass.BackgroundMode.value = 1
  res_pass.BackgroundTexture.value = "/opt/guacamole/resources/skymaps/water_painted_noon.jpg"
  pipeline_description = avango.gua.nodes.PipelineDescription(
    Passes = [
      avango.gua.nodes.TriMeshPassDescription(),
      avango.gua.nodes.LightVisibilityPassDescription(),
      res_pass
    ]
  )
  camera.PipelineDescription.value = pipeline_description

  main_scene.Root.value.Children.value.append(camera)

  # portal scene -----------------------------------------------------------------
  geometry = loader.create_geometry_from_file(
    "geometry",
    "/opt/3d_models/architecture/weimar_geometry/weimar_stadtmodell_latest_version/weimar_stadtmodell_final.obj",
    avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY
  )
  geometry.Material.value.set_uniform("Roughness", 0.6)
  geometry.Material.value.set_uniform("Metalness", 0.0)
  geometry.Transform.value = avango.gua.make_trans_mat(0, -0.99, 0) * avango.gua.make_scale_mat(0.03)
  portal_scene.Root.value.Children.value.append(geometry)

  box = loader.create_geometry_from_file(
    "box",
    "data/objects/box.obj",
    avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY
  )
  box.Material.value.EnableBackfaceCulling.value = False
  box.Material.value.set_uniform("Color", avango.gua.Vec4(1, 0, 0, 0.5))
  box.Material.value.set_uniform("Roughness", 0.6)
  box.Material.value.set_uniform("Metalness", 0.0)
  box.Transform.value = avango.gua.make_scale_mat(0.999)
  portal_scene.Root.value.Children.value.append(box)


  light = avango.gua.nodes.SunLightNode(
    Name = "sun",
    Color = avango.gua.Color(1.0, 1.0, 0.5),
    Brightness = 3,
    Transform = avango.gua.make_rot_mat(-150, 1, 0, 0)
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
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 7.0),
    BlackList = ["invisible"]
  )

  portal_res_pass = avango.gua.nodes.ResolvePassDescription()
  portal_res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.2,0.2,0.1)
  portal_res_pass.BackgroundMode.value = 1
  portal_res_pass.EnableSSAO.value = True
  portal_res_pass.SSAORadius.value = 5
  portal_res_pass.SSAOIntensity.value = 2
  portal_res_pass.BackgroundTexture.value = "/opt/guacamole/resources/skymaps/sunrise.jpg"
  portal_pipeline_description = avango.gua.nodes.PipelineDescription(
    Passes = [
      avango.gua.nodes.TriMeshPassDescription(),
      avango.gua.nodes.LightVisibilityPassDescription(),
      portal_res_pass
    ]
  )
  portal_camera.PipelineDescription.value = portal_pipeline_description
  portal_camera.PipelineDescription.value.EnableABuffer.value = True

  def add_clipping_plane(translation, rotation):
    n = avango.gua.nodes.ClippingPlaneNode(
      Transform = avango.gua.make_trans_mat(translation) * avango.gua.make_rot_mat(rotation)
    )
    portal_scene.Root.value.Children.value.append(n);
    c = ClippingControl(
      CameraNode = portal_camera,
      ClippingPlaneNode = n
    )

  add_clipping_plane(avango.gua.Vec3(0.0, 0.0, 1.0), avango.gua.Vec4(0, 0, 0, 0))
  add_clipping_plane(avango.gua.Vec3(0.0, 0.0, -1.0), avango.gua.Vec4(180, 0, 1, 0))
  add_clipping_plane(avango.gua.Vec3(1.0, 0.0, 0.0), avango.gua.Vec4(90, 0, 1, 0))
  add_clipping_plane(avango.gua.Vec3(-1.0, 0.0, 0.0), avango.gua.Vec4(-90, 0, 1, 0))

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
  viewer.SceneGraphs.value = [main_scene, portal_scene]
  viewer.Windows.value = [window]

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()

