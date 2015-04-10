import avango
import avango.script
import avango.gua.skelanim
from avango.script import field_has_changed
import avango.gua

import examples_common.navigator
from examples_common.GuaVE import GuaVE

from GroundFollowing import *
from CharacterControl import *
from CameraControl import *

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
  skel_loader = avango.gua.skelanim.nodes.SkeletalAnimationLoader()

  # main scene -----------------------------------------------------------------
  desc = avango.gua.nodes.MaterialShaderDescription()
  desc.load_from_file("data/materials/portal.gmd")
  avango.gua.register_material_shader(desc, "portal_mat")

  cube = loader.create_geometry_from_file(
    "cube",
    "data/objects/sphere.obj",
  )
  cube.Transform.value = avango.gua.make_scale_mat(0.02)
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
  camera.FarClip.value = 300
  camera.NearClip.value = 0.01


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
  '''geometry = loader.create_geometry_from_file(
    "geometry",
    "/opt/3d_models/architecture/weimar_geometry/weimar_stadtmodell_latest_version/weimar_stadtmodell_final.obj",
    avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY
  )
  geometry.Material.value.set_uniform("Roughness", 0.6)
  geometry.Material.value.set_uniform("Metalness", 0.0)
  geometry.Transform.value = avango.gua.make_trans_mat(0, -0.99, 0) * avango.gua.make_scale_mat(0.03)'''
  #portal_scene.Root.value.Children.value.append(geometry)

  plane = loader.create_geometry_from_file("cube",
                                            "data/objects/cube2.obj",
                                            avango.gua.LoaderFlags.NORMALIZE_POSITION
                                            | avango.gua.LoaderFlags.NORMALIZE_SCALE
                                            | avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY
                                            | avango.gua.LoaderFlags.MAKE_PICKABLE
                                            | avango.gua.LoaderFlags.LOAD_MATERIALS)
  plane.Transform.value *= avango.gua.make_scale_mat(10.0,0.01,10.0) *  avango.gua.make_trans_mat(0, -3, 0)

  plane.Material.value.set_uniform("NormalMap","data/objects/glass_2_3_nm.TGA")

  portal_scene.Root.value.Children.value.append(plane)

  light = avango.gua.nodes.SunLightNode(
    Name = "sun",
    Color = avango.gua.Color(245.0/255.0 , 246.0/255.0, 178.0/255.0),
    Brightness = 5,
    Transform = avango.gua.make_rot_mat(119.5, 0.0, 1.0, 0.0) * avango.gua.make_rot_mat(-10, 1.0, 0.0, 0.0)
  )
  portal_scene.Root.value.Children.value.append(light)


  bob_nav = avango.gua.nodes.TransformNode(Name = "bob_nav")
  bob_ground = avango.gua.nodes.TransformNode(Name = "bob_ground")

  bob = skel_loader.create_geometry_from_file("bob", "data/objects/Necris/necris_male_ut4_SKELMESH.FBX" ,
         avango.gua.LoaderFlags.LOAD_MATERIALS
         |avango.gua.LoaderFlags.NORMALIZE_SCALE)

  bob.Transform.value = bob.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0)

  bob_nav.Transform.value =  bob_nav.Transform.value * avango.gua.make_trans_mat(0.0,0.05,0.0) * avango.gua.make_scale_mat(0.02,0.02,0.02)

  bob_ground.Children.value = [bob_nav]
  bob_nav.Children.value = [bob]
  portal_scene.Root.value.Children.value.append(bob_ground)

  # idle:
  bob.load_animation("data/animations/Idle_Ready_Rif.FBX","idle", avango.gua.LoaderFlags.DEFAULTS)
  
  # run:
  bob.load_animation("data/animations/Run_Fwd_Rif.FBX","run_fwd", avango.gua.LoaderFlags.DEFAULTS)

  # setup ground following
  ground_following = GroundFollowing(
    SceneGraph = portal_scene,
    OffsetToGround = 0.01,
    MaxDistanceToGround = 1.0
  )
  ground_following.InTransform.connect_from(bob.WorldTransform)

  bob_ground.Transform.connect_from(ground_following.OutTransform)



  #box = loader.create_geometry_from_file(
  #  "box",
  #  "data/objects/box.obj",
  #  avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY
  #)
  #box.Material.value.EnableBackfaceCulling.value = False
  #box.Material.value.set_uniform("Color", avango.gua.Vec4(1, 0, 0, 0.5))
  #box.Material.value.set_uniform("Roughness", 0.6)
  #box.Material.value.set_uniform("Metalness", 0.0)
  #box.Transform.value = avango.gua.make_scale_mat(0.999)
  #portal_scene.Root.value.Children.value.append(box)

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
  portal_camera.FarClip.value = 300
  portal_camera.NearClip.value = 0.01

  portal_res_pass = avango.gua.nodes.ResolvePassDescription()
  portal_res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.2,0.2,0.1)
  portal_res_pass.BackgroundMode.value = 1
  portal_res_pass.EnableSSAO.value = True
  portal_res_pass.SSAORadius.value = 2.0
  portal_res_pass.SSAOIntensity.value = 2.0
  portal_res_pass.BackgroundTexture.value = "/opt/avango/master/examples/picking/data/textures/skymap.jpg"
  portal_res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.LINEAR
  portal_pipeline_description = avango.gua.nodes.PipelineDescription(
    Passes = [
      avango.gua.nodes.TriMeshPassDescription(),
      avango.gua.nodes.LightVisibilityPassDescription(),
      portal_res_pass,
      avango.gua.skelanim.nodes.SkeletalAnimationPassDescription()
    ]
  )
  portal_camera.PipelineDescription.value = portal_pipeline_description
  #portal_camera.PipelineDescription.value.EnableABuffer.value = True

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
  '''window = avango.gua.nodes.Window(
    Size = size,
    Title = "picking",
    LeftResolution = size
  )'''

  window = avango.gua.nodes.GlfwWindow(
    Size = size,
    LeftResolution = size
  )

  # setup character control
  character_control = CharacterControl()
  character_control.my_constructor(bob,bob_nav,window)

  #character_control.bind_translation("run_fwd",avango.gua.Vec3(0.0,0.0,0.05))
  character_control.play_once("idle","run_fwd",0.5)
  character_control.play_once("run_fwd","idle",0.5)
  character_control.switch_animation("idle")


  camera.PreRenderCameras.value.append(portal_camera)

  avango.gua.register_window("window", window)

  navigator = examples_common.navigator.Navigator()
  navigator.StartLocation.value = camera.Transform.value.get_translate()
  navigator.OutTransform.connect_from(camera.Transform)

  navigator.RotationSpeed.value = 0.2
  navigator.MotionSpeed.value = 0.04

  #camera.Transform.connect_from(navigator.OutTransform)
  #portal_camera.Transform.connect_from(navigator.OutTransform)
  
  # setup own camera control
  camera_control = CameraControl()
  camera_control.my_constructor(bob,window)
  camera.Transform.connect_from(camera_control.OutTransform)
  portal_camera.Transform.connect_from(camera_control.OutTransform)


  viewer = avango.gua.nodes.Viewer()
  viewer.SceneGraphs.value = [main_scene, portal_scene]
  viewer.Windows.value = [window]

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()

