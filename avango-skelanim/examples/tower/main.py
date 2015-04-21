import avango
import avango.script
from avango.script import field_has_changed
import avango.gua.skelanim
from examples_common.GuaVE import GuaVE

from avango.gua.skelanim.GroundFollowing import *
from avango.gua.skelanim.CharacterControl import *
from avango.gua.skelanim.CameraControl import *
from avango.gua.skelanim.DistanceEvents import *
from avango.gua.skelanim.AnimationControl import AnimationConfig
from CharacterSettings import *

camera_controls = []
character_controls = []
current_character = 0
device_sensor = None
screen = None

class XBOX_Listener(avango.script.Script):

  XBOX_BTN_B = avango.SFFloat()
  XBOX_BTN_Y = avango.SFFloat()

  def __init__(self):

    self.super(XBOX_Listener).__init__()

  @field_has_changed(XBOX_BTN_B)
  def xbox_button_b_changed(self):
    if self.XBOX_BTN_B.value:
      next_character()

  @field_has_changed(XBOX_BTN_Y)
  def xbox_button_y_changed(self):
    if self.XBOX_BTN_Y.value:
      next_character(-1)

def next_character(add = 1):

  global current_character
  global camera_controls
  global camera_controls
  global device_sensor
  global screen

  if device_sensor != None and screen != None and len(character_controls)>0 and len(camera_controls)>0:
    character_controls[current_character].listen_keyboard(False)
    camera_controls[current_character].listen_mouse(False)
    camera_control_xbox_disconnect(camera_controls[current_character], device_sensor)
    current_character = (current_character + add) % len(camera_controls)
    character_controls[current_character].listen_keyboard(True)
    camera_controls[current_character].listen_mouse(True)
    camera_control_xbox_connect(camera_controls[current_character], device_sensor)
    screen.Transform.disconnect()
    screen.Transform.connect_from(camera_controls[current_character].OutTransform) 

def start():
  
  # setup scenegraph
  graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")

  loader = avango.gua.skelanim.nodes.SkeletalAnimationLoader()

  bob_nav = avango.gua.nodes.TransformNode(Name = "bob_nav")
  bob_ground = avango.gua.nodes.TransformNode(Name = "bob_ground")

  bob = loader.create_geometry_from_file("bob", "/opt/project_animation/Assets/UnrealTournament/Characters/Necris_Male/necris_male_ut4_SKELMESH.FBX" ,
         avango.gua.LoaderFlags.LOAD_MATERIALS
         |avango.gua.LoaderFlags.NORMALIZE_SCALE)

  bob.Transform.value = bob.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0)

  load_unreal_animations(bob)

  bob_nav.Transform.value =  bob_nav.Transform.value * avango.gua.make_trans_mat(0.0,0.05,0.0) * avango.gua.make_scale_mat(0.02,0.02,0.02)

  bob_ground.Children.value = [bob_nav]
  bob_nav.Children.value = [bob]

  #environment:
  tri_mesh_loader = avango.gua.nodes.TriMeshLoader()

  tower = tri_mesh_loader.create_geometry_from_file("tower", "/opt/project_animation/highrise/tower.fbx",
                                            avango.gua.LoaderFlags.MAKE_PICKABLE|
                                            avango.gua.LoaderFlags.LOAD_MATERIALS)
  tower.Transform.value = tower.Transform.value * avango.gua.make_rot_mat(235.0, 0.0, 1.0, 0.0) 

  environment = tri_mesh_loader.create_geometry_from_file("environment", "/opt/project_animation/highrise/environment.fbx",
                                            avango.gua.LoaderFlags.MAKE_PICKABLE|
                                            avango.gua.LoaderFlags.LOAD_MATERIALS)
  environment.Transform.value = environment.Transform.value * avango.gua.make_scale_mat(0.1) * avango.gua.make_rot_mat(235.0, 0.0, 1.0, 0.0) 

  # plane = tri_mesh_loader.create_geometry_from_file("cube",
  #                                           "/opt/project_animation/demo/data/objects/cube2.obj",
  #                                           avango.gua.LoaderFlags.NORMALIZE_POSITION
  #                                           | avango.gua.LoaderFlags.NORMALIZE_SCALE
  #                                           | avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY
  #                                           | avango.gua.LoaderFlags.MAKE_PICKABLE
  #                                           | avango.gua.LoaderFlags.LOAD_MATERIALS)
  # plane.Transform.value *= avango.gua.make_scale_mat(10.0,0.01,10.0) *  avango.gua.make_trans_mat(0, -3, 0)

  # plane.Material.value.set_uniform("NormalMap","/opt/project_animation/demo/data/objects/glass_2_3_nm.TGA")

  sunlight = avango.gua.nodes.LightNode(
      Type=avango.gua.LightType.SUN,
      Name="light",
      Color=avango.gua.Color(245.0/255.0 , 246.0/255.0, 178.0/255.0),
      Brightness=5.0,
      Transform=(avango.gua.make_rot_mat(119.5, 0.0, 1.0, 0.0) * avango.gua.make_rot_mat(-10, 1.0, 0.0, 0.0))
  )

  size = avango.gua.Vec2ui(2560, 1440)

  window = avango.gua.nodes.GlfwWindow(
    Size = size,
    LeftResolution = size
  )
  window.CursorMode.value = 2

  def handle_key(ascii, unknown , event , unknown2):

    if ascii == 257 and event == 1:

      next_character()

  window.on_key_press(handle_key)

  cam = avango.gua.nodes.CameraNode(LeftScreenPath = "/screen",
                                    SceneGraph = "scenegraph",
                                    Resolution = size,
                                    OutputWindowName = "window")

  pipeline_description = avango.gua.nodes.PipelineDescription(
      Passes = [
            avango.gua.nodes.TriMeshPassDescription(),
            avango.gua.nodes.LightVisibilityPassDescription(),
            avango.gua.skelanim.nodes.SkeletalAnimationPassDescription(),
            avango.gua.nodes.ResolvePassDescription(),
            avango.gua.nodes.SSAAPassDescription(),
            avango.gua.nodes.DebugViewPassDescription(),
          ])

  pipeline_description.Passes.value[3].EnableSSAO.value = True
  pipeline_description.Passes.value[3].SSAORadius.value = 2.0
  pipeline_description.Passes.value[3].SSAOIntensity.value = 2.0
  pipeline_description.Passes.value[3].BackgroundMode.value = 1
  pipeline_description.Passes.value[3].BackgroundTexture.value = "/opt/avango/master/examples/picking/data/textures/skymap.jpg"
  pipeline_description.Passes.value[3].ToneMappingMode.value = avango.gua.ToneMappingMode.LINEAR
  #pipeline_description.EnableABuffer.value = True

  cam.PipelineDescription.value = pipeline_description
  cam.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 0.4)
  cam.FarClip.value = 300
  cam.NearClip.value = 0.01

  global screen
  screen = avango.gua.nodes.ScreenNode(Name = "screen", Width = 0.8, Height = 0.45)
  screen.Children.value = [cam]
  screen.Transform.value = avango.gua.make_trans_mat(0, 0.1, -2)

  graph.Root.value.Children.value = [bob_ground,screen, tower, environment, sunlight]

  avango.gua.register_window("window", window)

  #setup viewer
  viewer = avango.gua.nodes.Viewer()
  viewer.SceneGraphs.value = [graph]
  viewer.Windows.value = [window]
  #window.CursorMode.value = avango.gua.CursorMode.DISABLED

  # XBOX Controller
  global device_sensor
  device_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
  device_sensor.Station.value = "device-xbox-1"
  # XBOX Controller2
  device_sensor2 = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
  device_sensor2.Station.value = "device-xbox-2"

  xbox_listener = XBOX_Listener()
  xbox_listener.XBOX_BTN_B.connect_from(device_sensor.Button1)
  xbox_listener.XBOX_BTN_Y.connect_from(device_sensor.Button3)

  # setup character control
  character_control = CharacterControl()
  character_control.my_constructor(bob,bob_nav,AnimationConfig("idle"),window)
  apply_character_control_settings1(character_control, device_sensor)
  #wall detection:
  character_control.activate_wall_detection(0.0075,0.009,"idle",graph)
  character_controls.append(character_control)
  # setup camera control
  camera_control = CameraControl()
  camera_control.my_constructor(bob,window)
  camera_controls.append(camera_control)
  # setup ground following
  ground_following = GroundFollowing(
    SceneGraph = graph,
    OffsetToGround = 0.01,
    MaxDistanceToGround = 100.0
  )
  ground_following.my_constructor(gravity = -0.00005)
  ground_following.InTransform.connect_from(bob.WorldTransform)
  bob_ground.Transform.connect_from(ground_following.OutTransform)
  distance_events = DistanceEvents()
  distance_events.my_constructor(character_control)
  apply_distance_events(distance_events, ground_following)

  # adam = loader.create_geometry_from_file("adam", "/opt/project_animation/Assets/Mixamo/Adam/Maximo_Adam.FBX" ,
  #        avango.gua.LoaderFlags.LOAD_MATERIALS
  #        |avango.gua.LoaderFlags.NORMALIZE_SCALE)
  # adam_trans = avango.gua.nodes.TransformNode()
  # adam_trans.Transform.value = avango.gua.make_trans_mat(2.0, 1.0, -0.0*SLIDE_OFFSET) * adam.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) * avango.gua.make_rot_mat(90.0,0.0,0.0,1.0)
  # adam.Transform.connect_from(timed_rotate.MatrixOut)
  # adam_trans.Children.value.append(adam)
  # graph.Root.value.Children.value.append(adam_trans)
  # # idle:
  # adam.load_animation("/opt/project_animation/Assets/Mixamo/Adam/Mixamo_Adam_idle_blender.FBX","idle")
  # # run:
  # adam.load_animation("/opt/project_animation/Assets/Mixamo/Adam/Mixamo_adam_run_blender.FBX","run_fwd")
  # # setup character control
  # adam_character_control = CharacterControl()
  # adam_character_control.my_constructor(adam,adam,AnimationConfig("idle"),window)
  # adam_character_control.on_animation_end("idle", AnimationConfig("run_fwd", False, 1.0, 2.0),0.5)
  # adam_character_control.on_animation_end("run_fwd", AnimationConfig("idle", False, 1.0, 2.0),0.5)
  # adam_character_control.switch_animation(AnimationConfig("idle", False, 1.0, 2.0))


  # alpha = loader.create_geometry_from_file("alpha", "/opt/project_animation/Assets/Mixamo/Alpha/Maximo_Alpha.FBX" ,
  #        avango.gua.LoaderFlags.LOAD_MATERIALS
  #        |avango.gua.LoaderFlags.NORMALIZE_SCALE)
  # alpha_trans = avango.gua.nodes.TransformNode()
  # alpha_trans.Transform.value = avango.gua.make_trans_mat(2.0, 1.0, -1.0*SLIDE_OFFSET) * alpha.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) * avango.gua.make_rot_mat(90.0,0.0,0.0,1.0)
  # alpha.Transform.connect_from(timed_rotate.MatrixOut)
  # alpha_trans.Children.value.append(alpha)
  # graph.Root.value.Children.value.append(alpha_trans)
  # # idle:
  # alpha.load_animation("/opt/project_animation/Assets/Mixamo/Alpha/mixamo_idle_blender.FBX","idle")
  # # run:
  # alpha.load_animation("/opt/project_animation/Assets/Mixamo/Alpha/mixamo_run_blender.FBX","run_fwd")
  # # setup character control
  # alpha_character_control = CharacterControl()
  # alpha_character_control.my_constructor(alpha,alpha,AnimationConfig("idle"),window)
  # alpha_character_control.on_animation_end("idle", AnimationConfig("run_fwd", False, 1.0, 3.0),0.5)
  # alpha_character_control.on_animation_end("run_fwd", AnimationConfig("idle", False, 1.0, 3.0),0.5)
  # alpha_character_control.switch_animation(AnimationConfig("idle", False))


  # # # seg fault!!!
  # # exo = loader.create_geometry_from_file("exo", "/opt/project_animation/Assets/Mixamo/ExoRed/Maximo_ExoRed.FBX" ,
  # #        avango.gua.LoaderFlags.LOAD_MATERIALS
  # #        |avango.gua.LoaderFlags.NORMALIZE_SCALE)
  # # exo.Transform.value = avango.gua.make_trans_mat(2.0, 0.5, -0.0*SLIDE_OFFSET) * exo.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) * avango.gua.make_rot_mat(90.0,0.0,0.0,1.0)
  # # graph.Root.value.Children.value.append(exo)
  # # # idle:
  # # loader.load_animation(exo,
  # #         "/opt/project_animation/Assets/Mixamo/ExoRed/Exo_Idle_Copy_blender.FBX","idle")
  # # # run:
  # # loader.load_animation(exo,
  # #        "/opt/project_animation/Assets/Mixamo/ExoRed/Exo_Run_Copy_blender.FBX","run_fwd")
  # # # setup character control
  # # exo_character_control = CharacterControl()
  # # exo_character_control.my_constructor(exo,exo,AnimationConfig("idle"),window)
  # # exo_character_control.on_animation_end("idle", AnimationConfig("run_fwd", False, 1.0, 3.0),0.5)
  # # exo_character_control.on_animation_end("run_fwd", AnimationConfig("idle", False, 1.0, 3.0),0.5)
  # # exo_character_control.switch_animation(AnimationConfig("idle", False))


  ganfault_nav = avango.gua.nodes.TransformNode(Name = "ganfault_nav")
  ganfault_ground = avango.gua.nodes.TransformNode(Name = "ganfault_ground")
  ganfault = loader.create_geometry_from_file("ganfault", "/opt/project_animation/Assets/Mixamo/Ganfault/Mixamo_Ganfault_Aure.FBX" ,
         avango.gua.LoaderFlags.LOAD_MATERIALS
         |avango.gua.LoaderFlags.NORMALIZE_SCALE)
  ganfault.Transform.value = ganfault.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) *  avango.gua.make_scale_mat(0.02,0.02,0.02)
  load_mixamo_animations_ganfault(ganfault)
  ganfault_nav.Transform.value =  ganfault_nav.Transform.value * avango.gua.make_trans_mat(-0.26,0.012,0.376)
  ganfault_ground.Children.value = [ganfault_nav]
  ganfault_nav.Children.value = [ganfault]
  ganfault_character_control = CharacterControl()
  ganfault_character_control.my_constructor(ganfault,ganfault_nav,AnimationConfig("idle"),window)
  apply_character_control_settings_mixamo(ganfault_character_control, device_sensor)
  character_controls.append(ganfault_character_control)
  ganfault_character_control.listen_keyboard(False)
  ganfault_character_control.activate_wall_detection(0.0075,0.009,"idle",graph)
  ganfault_camera_control = CameraControl()
  ganfault_camera_control.my_constructor(ganfault,window)
  camera_controls.append(ganfault_camera_control)
  ganfault_camera_control.listen_mouse(False)
  ganfault_ground_following = GroundFollowing(
    SceneGraph = graph,
    OffsetToGround = 0.01,
    MaxDistanceToGround = 100.0
  )
  ganfault_ground_following.my_constructor(gravity = -0.00005)
  ganfault_ground_following.InTransform.connect_from(ganfault.WorldTransform)
  ganfault_ground.Transform.connect_from(ganfault_ground_following.OutTransform)
  graph.Root.value.Children.value.append(ganfault_ground)


  # # seg fault:
  # goblin = loader.create_geometry_from_file("goblin", "/opt/project_animation/Assets/Mixamo/Goblin/Mixamo_Goblin.FBX" ,
  #        avango.gua.LoaderFlags.LOAD_MATERIALS
  #        |avango.gua.LoaderFlags.NORMALIZE_SCALE)
  # goblin.Transform.value = avango.gua.make_trans_mat(2.0, 1.0, -0.0*SLIDE_OFFSET) * goblin.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) * avango.gua.make_rot_mat(90.0,0.0,0.0,1.0)
  # graph.Root.value.Children.value.append(goblin)
  # # idle:
  # loader.load_animation(goblin,
  #         "/opt/project_animation/Assets/Mixamo/Goblin/Mixamo_Goblin_idle_blender.FBX","idle")
  # # run:
  # loader.load_animation(goblin,
  #        "/opt/project_animation/Assets/Mixamo/Goblin/Mixamo_Goblin_Run_blender.FBX","run_fwd")
  # # setup character control
  # goblin_character_control = CharacterControl()
  # goblin_character_control.my_constructor(goblin,goblin,AnimationConfig("idle"),window)
  # goblin_character_control.on_animation_end("idle", AnimationConfig("run_fwd", False, 1.0, 3.0),0.5)
  # goblin_character_control.on_animation_end("run_fwd", AnimationConfig("idle", False, 1.0, 3.0),0.5)
  # goblin_character_control.switch_animation(AnimationConfig("idle", False))


  # heraklios = loader.create_geometry_from_file("heraklios", "/opt/project_animation/Assets/Mixamo/Heraklios/Mixamo_Heraklios.FBX" ,
  #        avango.gua.LoaderFlags.LOAD_MATERIALS
  #        |avango.gua.LoaderFlags.NORMALIZE_SCALE)
  # heraklios_trans = avango.gua.nodes.TransformNode()
  # heraklios_trans.Transform.value = avango.gua.make_trans_mat(2.0, 1.0, -3.0*SLIDE_OFFSET) * heraklios.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) * avango.gua.make_rot_mat(90.0,0.0,0.0,1.0)
  # heraklios.Transform.connect_from(timed_rotate.MatrixOut)
  # heraklios_trans.Children.value.append(heraklios)
  # graph.Root.value.Children.value.append(heraklios_trans)
  # # idle:
  # heraklios.load_animation("/opt/project_animation/Assets/Mixamo/Heraklios/Mixamo_Heraklios_idle_blender.FBX","idle")
  # # run:
  # heraklios.load_animation("/opt/project_animation/Assets/Mixamo/Heraklios/Mixamo_Heraklios_Run_blender.FBX","run_fwd")
  # # setup character control
  # heraklios_character_control = CharacterControl()
  # heraklios_character_control.my_constructor(heraklios,heraklios,AnimationConfig("idle"),window)
  # heraklios_character_control.on_animation_end("idle", AnimationConfig("run_fwd", False, 1.0, 3.0),0.5)
  # heraklios_character_control.on_animation_end("run_fwd", AnimationConfig("idle", False, 1.0, 3.0),0.5)
  # heraklios_character_control.switch_animation(AnimationConfig("idle", False))


  # # seg fault:
  # jill = loader.create_geometry_from_file("jill", "/opt/project_animation/Assets/Mixamo/Jill/Jill.FBX" ,
  #        avango.gua.LoaderFlags.LOAD_MATERIALS
  #        |avango.gua.LoaderFlags.NORMALIZE_SCALE)
  # jill.Transform.value = avango.gua.make_trans_mat(2.0, 1.0, -0.0*SLIDE_OFFSET) * jill.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) * avango.gua.make_rot_mat(90.0,0.0,0.0,1.0)
  # graph.Root.value.Children.value.append(jill)
  # # idle:
  # loader.load_animation(jill,
  #         "/opt/project_animation/Assets/Mixamo/Jill/Jill_idle_blender.FBX","idle")
  # # run:
  # loader.load_animation(jill,
  #        "/opt/project_animation/Assets/Mixamo/Jill/Jill_Run_blender.FBX","run_fwd")
  # # setup character control
  # jill_character_control = CharacterControl()
  # jill_character_control.my_constructor(jill,jill,AnimationConfig("idle"),window)
  # jill_character_control.on_animation_end("idle", AnimationConfig("run_fwd", False, 1.0, 3.0),0.5)
  # jill_character_control.on_animation_end("run_fwd", AnimationConfig("idle", False, 1.0, 3.0),0.5)
  # jill_character_control.switch_animation(AnimationConfig("idle", False))


  kachujin_nav = avango.gua.nodes.TransformNode(Name = "kachujin_nav")
  kachujin_ground = avango.gua.nodes.TransformNode(Name = "kachujin_ground")
  kachujin = loader.create_geometry_from_file("kachujin", "/opt/project_animation/Assets/Mixamo/Kachujin/Kachujin_G_Rosales.FBX" ,
         avango.gua.LoaderFlags.LOAD_MATERIALS
         |avango.gua.LoaderFlags.NORMALIZE_SCALE)
  kachujin.Transform.value = kachujin.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) *  avango.gua.make_scale_mat(0.02,0.02,0.02)
  load_mixamo_animations_kachujin(kachujin)
  kachujin_nav.Transform.value =  kachujin_nav.Transform.value * avango.gua.make_trans_mat(-0.576, 0.087, 0.813)
  kachujin_ground.Children.value = [kachujin_nav]
  kachujin_nav.Children.value = [kachujin]
  kachujin_character_control = CharacterControl()
  kachujin_character_control.my_constructor(kachujin,kachujin_nav,AnimationConfig("idle"),window)
  apply_character_control_settings_mixamo(kachujin_character_control, device_sensor)
  character_controls.append(kachujin_character_control)
  kachujin_character_control.listen_keyboard(False)
  kachujin_character_control.activate_wall_detection(0.0075,0.009,"idle",graph)
  kachujin_camera_control = CameraControl()
  kachujin_camera_control.my_constructor(kachujin,window)
  camera_controls.append(kachujin_camera_control)
  kachujin_camera_control.listen_mouse(False)
  kachujin_ground_following = GroundFollowing(
    SceneGraph = graph,
    OffsetToGround = 0.01,
    MaxDistanceToGround = 100.0
  )
  kachujin_ground_following.my_constructor(gravity = -0.00005)
  kachujin_ground_following.InTransform.connect_from(kachujin.WorldTransform)
  kachujin_ground.Transform.connect_from(kachujin_ground_following.OutTransform)
  graph.Root.value.Children.value.append(kachujin_ground)


  maria_nav = avango.gua.nodes.TransformNode(Name = "maria_nav")
  maria_ground = avango.gua.nodes.TransformNode(Name = "maria_ground")
  maria = loader.create_geometry_from_file("maria", "/opt/project_animation/Assets/Mixamo/Maria/Maximo_Maria.FBX" ,
         avango.gua.LoaderFlags.LOAD_MATERIALS
         |avango.gua.LoaderFlags.NORMALIZE_SCALE)
  maria.Transform.value = maria.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) *  avango.gua.make_scale_mat(0.02,0.02,0.02)
  load_mixamo_animations_maria(maria)
  maria_nav.Transform.value =  maria_nav.Transform.value * avango.gua.make_trans_mat(-0.927, 0.0762, 0.127)
  maria_ground.Children.value = [maria_nav]
  maria_nav.Children.value = [maria]
  maria_character_control = CharacterControl()
  maria_character_control.my_constructor(maria,maria_nav,AnimationConfig("idle"),window)
  apply_character_control_settings_mixamo(maria_character_control, device_sensor)
  character_controls.append(maria_character_control)
  maria_character_control.listen_keyboard(False)
  maria_character_control.activate_wall_detection(0.0075,0.009,"idle",graph)
  maria_camera_control = CameraControl()
  maria_camera_control.my_constructor(maria,window)
  camera_controls.append(maria_camera_control)
  maria_camera_control.listen_mouse(False)
  maria_ground_following = GroundFollowing(
    SceneGraph = graph,
    OffsetToGround = 0.01,
    MaxDistanceToGround = 100.0
  )
  maria_ground_following.my_constructor(gravity = -0.00005)
  maria_ground_following.InTransform.connect_from(maria.WorldTransform)
  maria_ground.Transform.connect_from(maria_ground_following.OutTransform)
  graph.Root.value.Children.value.append(maria_ground)


  maw_nav = avango.gua.nodes.TransformNode(Name = "maw_nav")
  maw_ground = avango.gua.nodes.TransformNode(Name = "maw_ground")
  maw = loader.create_geometry_from_file("maw", "/opt/project_animation/Assets/Mixamo/Maw/Maw_J_Laygo.FBX" ,
         avango.gua.LoaderFlags.LOAD_MATERIALS
         |avango.gua.LoaderFlags.NORMALIZE_SCALE)
  maw.Transform.value = maw.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) *  avango.gua.make_scale_mat(0.04,0.04,0.04)
  load_mixamo_animations_maw(maw)
  maw_nav.Transform.value =  maw_nav.Transform.value * avango.gua.make_trans_mat(-0.05, 0.055, 0.081)
  maw_ground.Children.value = [maw_nav]
  maw_nav.Children.value = [maw]
  maw_character_control = CharacterControl()
  maw_character_control.my_constructor(maw,maw_nav,AnimationConfig("idle"),window)
  apply_character_control_settings_mixamo(maw_character_control, device_sensor)
  character_controls.append(maw_character_control)
  maw_character_control.listen_keyboard(False)
  maw_character_control.activate_wall_detection(0.0075,0.009,"idle",graph)
  maw_camera_control = CameraControl()
  maw_camera_control.my_constructor(maw,window)
  camera_controls.append(maw_camera_control)
  maw_camera_control.listen_mouse(False)
  maw_ground_following = GroundFollowing(
    SceneGraph = graph,
    OffsetToGround = 0.01,
    MaxDistanceToGround = 100.0
  )
  maw_ground_following.my_constructor(gravity = -0.00005)
  maw_ground_following.InTransform.connect_from(maw.WorldTransform)
  maw_ground.Transform.connect_from(maw_ground_following.OutTransform)
  graph.Root.value.Children.value.append(maw_ground)


  # # run time seg fault:
  # mortimer = loader.create_geometry_from_file("mortimer", "/opt/project_animation/Assets/Mixamo/Mortimer/Mortimer.FBX" ,
  #        avango.gua.LoaderFlags.LOAD_MATERIALS
  #        |avango.gua.LoaderFlags.NORMALIZE_SCALE)
  # mortimer_trans = avango.gua.nodes.TransformNode()
  # mortimer_trans.Transform.value = avango.gua.make_trans_mat(2.0, 1.0, -7.0*SLIDE_OFFSET) * mortimer.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) * avango.gua.make_rot_mat(90.0,0.0,0.0,1.0)
  # mortimer.Transform.connect_from(timed_rotate.MatrixOut)
  # mortimer_trans.Children.value.append(mortimer)
  # graph.Root.value.Children.value.append(mortimer_trans)
  # # idle:
  # loader.load_animation(mortimer,
  #         "/opt/project_animation/Assets/Mixamo/Mortimer/Mortimer_idle_blender.FBX","idle")
  # # run:
  # loader.load_animation(mortimer,
  #        "/opt/project_animation/Assets/Mixamo/Mortimer/Mortimer_Run_blender.FBX","run_fwd")
  # # setup character control
  # mortimer_character_control = CharacterControl()
  # mortimer_character_control.my_constructor(mortimer,mortimer,AnimationConfig("idle"),window)
  # mortimer_character_control.on_animation_end("idle", AnimationConfig("run_fwd", False, 1.0, 3.0),0.5)
  # mortimer_character_control.on_animation_end("run_fwd", AnimationConfig("idle", False, 1.0, 3.0),0.5)
  # mortimer_character_control.switch_animation(AnimationConfig("idle", False))


  # swat = loader.create_geometry_from_file("swat", "/opt/project_animation/Assets/Mixamo/SWAT/Maximo_SWAT.FBX" ,
  #        avango.gua.LoaderFlags.LOAD_MATERIALS
  #        |avango.gua.LoaderFlags.NORMALIZE_SCALE)
  # swat_trans = avango.gua.nodes.TransformNode()
  # swat_trans.Transform.value = avango.gua.make_trans_mat(2.0, 1.0, -8.0*SLIDE_OFFSET) * swat.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) * avango.gua.make_rot_mat(90.0,0.0,0.0,1.0)
  # swat.Transform.connect_from(timed_rotate.MatrixOut)
  # swat_trans.Children.value.append(swat)
  # graph.Root.value.Children.value.append(swat_trans)
  # # idle:
  # swat.load_animation("/opt/project_animation/Assets/Mixamo/SWAT/Maximo_Swat_Idle_blender.FBX","idle")
  # # run:
  # swat.load_animation("/opt/project_animation/Assets/Mixamo/SWAT/Maximo_Swat_Swat_Run_blender.FBX","run_fwd")
  # # setup character control
  # swat_character_control = CharacterControl()
  # swat_character_control.my_constructor(swat,swat,AnimationConfig("idle"),window)
  # swat_character_control.on_animation_end("idle", AnimationConfig("run_fwd", False, 1.0, 3.0),0.5)
  # swat_character_control.on_animation_end("run_fwd", AnimationConfig("idle", False, 1.0, 3.0),0.5)
  # swat_character_control.switch_animation(AnimationConfig("idle", False))


  vampire_nav = avango.gua.nodes.TransformNode(Name = "vampire_nav")
  vampire_ground = avango.gua.nodes.TransformNode(Name = "vampire_ground")
  vampire = loader.create_geometry_from_file("vampire", "/opt/project_animation/Assets/Mixamo/Vampire/Vampire_A_Lusth.FBX" ,
         avango.gua.LoaderFlags.LOAD_MATERIALS
         |avango.gua.LoaderFlags.NORMALIZE_SCALE)
  vampire.Transform.value = vampire.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) *  avango.gua.make_scale_mat(0.02,0.02,0.02)
  load_mixamo_animations_vampire(vampire)
  vampire_nav.Transform.value =  vampire_nav.Transform.value * avango.gua.make_trans_mat(0.163, 0.077, 0.992)
  vampire_ground.Children.value = [vampire_nav]
  vampire_nav.Children.value = [vampire]
  vampire_character_control = CharacterControl()
  vampire_character_control.my_constructor(vampire,vampire_nav,AnimationConfig("idle"),window)
  apply_character_control_settings_mixamo(vampire_character_control, device_sensor)
  character_controls.append(vampire_character_control)
  vampire_character_control.listen_keyboard(False)
  vampire_character_control.activate_wall_detection(0.0075,0.009,"idle",graph)
  vampire_camera_control = CameraControl()
  vampire_camera_control.my_constructor(vampire,window)
  camera_controls.append(vampire_camera_control)
  vampire_camera_control.listen_mouse(False)
  vampire_ground_following = GroundFollowing(
    SceneGraph = graph,
    OffsetToGround = 0.01,
    MaxDistanceToGround = 100.0
  )
  vampire_ground_following.my_constructor(gravity = -0.00005)
  vampire_ground_following.InTransform.connect_from(vampire.WorldTransform)
  vampire_ground.Transform.connect_from(vampire_ground_following.OutTransform)
  graph.Root.value.Children.value.append(vampire_ground)


  # vanguard = loader.create_geometry_from_file("vanguard", "/opt/project_animation/Assets/Mixamo/Vanguard/Vanguard_by_T__Choonyung.FBX" ,
  #        avango.gua.LoaderFlags.LOAD_MATERIALS
  #        |avango.gua.LoaderFlags.NORMALIZE_SCALE)
  # vanguard_trans = avango.gua.nodes.TransformNode()
  # vanguard_trans.Transform.value = avango.gua.make_trans_mat(2.0, 1.0, -10.0*SLIDE_OFFSET) * vanguard.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) * avango.gua.make_rot_mat(90.0,0.0,0.0,1.0)
  # vanguard.Transform.connect_from(timed_rotate.MatrixOut)
  # vanguard_trans.Children.value.append(vanguard)
  # graph.Root.value.Children.value.append(vanguard_trans)
  # # idle:
  # vanguard.load_animation("/opt/project_animation/Assets/Mixamo/Vanguard/Vanguard_by_T__Choonyung_idle_blender.FBX","idle")
  # # run:
  # vanguard.load_animation("/opt/project_animation/Assets/Mixamo/Vanguard/Vanguard_by_T__Choonyung_Run_blender.FBX","run_fwd")
  # # setup character control
  # vanguard_character_control = CharacterControl()
  # vanguard_character_control.my_constructor(vanguard,vanguard,AnimationConfig("idle"),window)
  # vanguard_character_control.on_animation_end("idle", AnimationConfig("run_fwd", False, 1.0, 3.0),0.5)
  # vanguard_character_control.on_animation_end("run_fwd", AnimationConfig("idle", False, 1.0, 3.0),0.5)
  # vanguard_character_control.switch_animation(AnimationConfig("idle", False))


  # zoe = loader.create_geometry_from_file("zoe", "/opt/project_animation/Assets/Mixamo/Zoe/Mixamo_Zoe.FBX" ,
  #        avango.gua.LoaderFlags.LOAD_MATERIALS
  #        |avango.gua.LoaderFlags.NORMALIZE_SCALE)
  # zoe_trans = avango.gua.nodes.TransformNode()
  # zoe_trans.Transform.value = avango.gua.make_trans_mat(2.0, 1.0, -11.0*SLIDE_OFFSET) * zoe.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) * avango.gua.make_rot_mat(90.0,0.0,0.0,1.0)
  # zoe.Transform.connect_from(timed_rotate.MatrixOut)
  # zoe_trans.Children.value.append(zoe)
  # graph.Root.value.Children.value.append(zoe_trans)
  # # idle:
  # zoe.load_animation("/opt/project_animation/Assets/Mixamo/Zoe/Mixamo_Zoe_idle_blender.FBX","idle")
  # # run:
  # zoe.load_animation("/opt/project_animation/Assets/Mixamo/Zoe/Mixamo_Zoe_Run_blender.FBX","run_fwd")
  # # setup character control
  # zoe_character_control = CharacterControl()
  # zoe_character_control.my_constructor(zoe,zoe,AnimationConfig("idle"),window)
  # zoe_character_control.on_animation_end("idle", AnimationConfig("run_fwd", False, 1.0, 3.0),0.5)
  # zoe_character_control.on_animation_end("run_fwd", AnimationConfig("idle", False, 1.0, 3.0),0.5)
  # zoe_character_control.switch_animation(AnimationConfig("idle", False))


  if len(camera_controls) > 0:
    camera_control_xbox_connect(camera_controls[0], device_sensor)
    screen.Transform.connect_from(camera_controls[0].OutTransform)

  timer = avango.nodes.TimeSensor()

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()

if __name__ == '__main__':
  start()

