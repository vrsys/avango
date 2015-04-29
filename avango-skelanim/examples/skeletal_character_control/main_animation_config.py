import avango
import avango.script
import avango.gua.skelanim
from examples_common.GuaVE import GuaVE
import examples_common.navigator
from avango.gua.skelanim.CharacterControl import *
from avango.gua.skelanim.AnimationControl import AnimationConfig


### CAMERA CONTROL VIA XBOX CONTROLLER:

def camera_control_xbox_connect(camera_control, device_sensor):
  # optional / additional xbox controller settings:
  camera_control.XBOX_X.connect_from(device_sensor.Value2)
  camera_control.XBOX_Y.connect_from(device_sensor.Value3)
  camera_control.XBOX_LZ.connect_from(device_sensor.Value4)
  camera_control.XBOX_RZ.connect_from(device_sensor.Value5)

def camera_control_xbox_disconnect(camera_control, device_sensor):
  # optional / additional xbox controller settings:
  camera_control.XBOX_X.disconnect()
  camera_control.XBOX_Y.disconnect()
  camera_control.XBOX_LZ.disconnect()
  camera_control.XBOX_RZ.disconnect()

def start():
  
  # setup scenegraph
  graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")

  #environment:
  tri_mesh_loader = avango.gua.nodes.TriMeshLoader()

  plane = tri_mesh_loader.create_geometry_from_file("cube",
                                            "/opt/project_animation/demo/data/objects/cube2.obj",
                                            avango.gua.LoaderFlags.NORMALIZE_POSITION
                                            | avango.gua.LoaderFlags.NORMALIZE_SCALE
                                            | avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY
                                            | avango.gua.LoaderFlags.MAKE_PICKABLE
                                            | avango.gua.LoaderFlags.LOAD_MATERIALS)
  plane.Transform.value *= avango.gua.make_scale_mat(10.0,0.01,10.0) *  avango.gua.make_trans_mat(0, -3, 0)

  plane.Material.value.set_uniform("NormalMap","/opt/project_animation/demo/data/objects/glass_2_3_nm.TGA")

  sunlight = avango.gua.nodes.LightNode(
      Type=avango.gua.LightType.SUN,
      Name="light",
      Color=avango.gua.Color(245.0/255.0 , 246.0/255.0, 178.0/255.0),
      Brightness=5.0,
      Transform=(avango.gua.make_rot_mat(119.5, 0.0, 1.0, 0.0) * avango.gua.make_rot_mat(-10, 1.0, 0.0, 0.0))
  )

  #view setup:
  size = avango.gua.Vec2ui(2560, 1440)

  window = avango.gua.nodes.GlfwWindow(
    Size = size,
    LeftResolution = size
  )
  window.CursorMode.value = 2
  window.EnableFullscreen.value = True

  cam = avango.gua.nodes.CameraNode(LeftScreenPath = "/screen",
                                    SceneGraph = "scenegraph",
                                    Resolution = size,
                                    OutputWindowName = "window")

  #render pipeline
  pipeline_description = avango.gua.nodes.PipelineDescription(
      Passes = [
            avango.gua.nodes.TriMeshPassDescription(),
            avango.gua.nodes.LightVisibilityPassDescription(),
            avango.gua.skelanim.nodes.SkeletalAnimationPassDescription(),
            avango.gua.nodes.ResolvePassDescription(),
            avango.gua.nodes.SSAAPassDescription(),
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

  screen = avango.gua.nodes.ScreenNode(Name = "screen", Width = 0.8, Height = 0.45)
  screen.Children.value = [cam]
  screen.Transform.value = avango.gua.make_trans_mat(0, 0.1, -2)

  graph.Root.value.Children.value = [screen, plane, sunlight]

  avango.gua.register_window("window", window)

  #setup viewer
  viewer = avango.gua.nodes.Viewer()
  viewer.SceneGraphs.value = [graph]
  viewer.Windows.value = [window]
  window.CursorMode.value = avango.gua.CursorMode.DISABLED

  #navigation
  navigator = examples_common.navigator.Navigator()
  #navigator.StartLocation.value = screen.Transform.value.get_translate()
  navigator.StartRotation.value = avango.gua.Vec2(0, 790)
  navigator.OutTransform.connect_from(screen.Transform)
  navigator.RotationSpeed.value = 0.2
  navigator.MotionSpeed.value = 0.004
  # enable navigation
  screen.Transform.connect_from(navigator.OutTransform)

  #XBOX Controller
  device_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
  device_sensor.Station.value = "device-xbox-1"


  #unreal tournament characters
  skel_mesh_loader = avango.gua.skelanim.nodes.SkeletalAnimationLoader()


  #LOOP MODE DEMO BOB
  bob_loop = skel_mesh_loader.create_geometry_from_file("bob_loop", "/opt/project_animation/Assets/UnrealTournament/Characters/Necris_Male/necris_male_ut4_SKELMESH.FBX" ,
         avango.gua.LoaderFlags.LOAD_MATERIALS
         |avango.gua.LoaderFlags.NORMALIZE_SCALE)

  bob_loop.Transform.value = avango.gua.make_trans_mat(0.0,-0.01,0.0) * bob_loop.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) *  avango.gua.make_scale_mat(0.2,0.2,0.2)

  graph.Root.value.Children.value.append(bob_loop)

  #load animations
  bob_loop.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Idle_Ready_Rif.FBX","idle")
  bob_loop.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Idle_Ready_Rif.FBX","idle2")
  bob_loop.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Run_Fwd_Rif.FBX","run_fwd")
  bob_loop.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Run_Fwd_Rif.FBX","run_fwd2")

  #character control
  character_control_loop = CharacterControl()
  character_control_loop.my_constructor(bob_loop,bob_loop,AnimationConfig("idle"),window)
  character_control_loop.on_animation_end("idle", AnimationConfig("run_fwd", loop=True, speed=1.0, duration=3.0))
  character_control_loop.on_animation_end("run_fwd", AnimationConfig("idle2", loop=False, speed=1.0, duration=3.0))
  character_control_loop.on_animation_end("idle2", AnimationConfig("run_fwd2", loop=False, speed=1.0, duration=3.0))
  character_control_loop.on_animation_end("run_fwd2", AnimationConfig("idle", loop=True, speed=1.0, duration=3.0))


  #SPEED DEMO BOB
  bob_speed = skel_mesh_loader.create_geometry_from_file("bob_speed", "/opt/project_animation/Assets/UnrealTournament/Characters/Necris_Male/necris_male_ut4_SKELMESH.FBX" ,
         avango.gua.LoaderFlags.LOAD_MATERIALS
         |avango.gua.LoaderFlags.NORMALIZE_SCALE)

  bob_speed.Transform.value = avango.gua.make_trans_mat(0.1,-0.01,0.0) * bob_speed.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) *  avango.gua.make_scale_mat(0.2,0.2,0.2)

  graph.Root.value.Children.value.append(bob_speed)

  #load animations
  bob_speed.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Idle_Ready_Rif.FBX","idle")
  bob_speed.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Idle_Ready_Rif.FBX","idle2")
  bob_speed.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Run_Fwd_Rif.FBX","run_fwd")
  bob_speed.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Run_Fwd_Rif.FBX","run_fwd2")

  #character control
  character_control_speed = CharacterControl()
  character_control_speed.my_constructor(bob_speed,bob_speed,AnimationConfig("idle"),window)
  character_control_speed.on_animation_end("idle", AnimationConfig("run_fwd", loop=True, speed=1.0, duration=3.0))
  character_control_speed.on_animation_end("run_fwd", AnimationConfig("idle2", loop=True, speed=1.5, duration=3.0))
  character_control_speed.on_animation_end("idle2", AnimationConfig("run_fwd2", loop=True, speed=1.5, duration=3.0))
  character_control_speed.on_animation_end("run_fwd2", AnimationConfig("idle", loop=True, speed=1.0, duration=3.0))


  #DURATION DEMO BOB
  bob_duration = skel_mesh_loader.create_geometry_from_file("bob_duration", "/opt/project_animation/Assets/UnrealTournament/Characters/Necris_Male/necris_male_ut4_SKELMESH.FBX" ,
         avango.gua.LoaderFlags.LOAD_MATERIALS
         |avango.gua.LoaderFlags.NORMALIZE_SCALE)

  bob_duration.Transform.value = avango.gua.make_trans_mat(0.2,-0.01,0.0) * bob_duration.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) *  avango.gua.make_scale_mat(0.2,0.2,0.2)

  graph.Root.value.Children.value.append(bob_duration)

  #load animations
  bob_duration.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Idle_Ready_Rif.FBX","idle")
  bob_duration.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Idle_Ready_Rif.FBX","idle2")
  bob_duration.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Run_Fwd_Rif.FBX","run_fwd")
  bob_duration.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Run_Fwd_Rif.FBX","run_fwd2")

  #character control
  character_control_duration = CharacterControl()
  character_control_duration.my_constructor(bob_duration,bob_duration,AnimationConfig("idle"),window)
  character_control_duration.on_animation_end("idle", AnimationConfig("run_fwd", loop=True, speed=1.0, duration=1.0))
  character_control_duration.on_animation_end("run_fwd", AnimationConfig("idle2", loop=True, speed=1.0, duration=2.0))
  character_control_duration.on_animation_end("idle2", AnimationConfig("run_fwd2", loop=True, speed=1.0, duration=2.0))
  character_control_duration.on_animation_end("run_fwd2", AnimationConfig("idle", loop=True, speed=1.0, duration=1.0))


  #start animation sequence
  character_control_loop.switch_animation(AnimationConfig("idle", loop=False))
  character_control_speed.switch_animation(AnimationConfig("idle", loop=False))
  character_control_duration.switch_animation(AnimationConfig("idle", loop=False))



  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()

if __name__ == '__main__':
  start()

