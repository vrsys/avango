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

  '''bob2_nav = avango.gua.nodes.TransformNode(Name = "bob2_nav")
  bob2_ground = avango.gua.nodes.TransformNode(Name = "bob2_ground")

  bob2 = loader.create_geometry_from_file("bob2", "/opt/project_animation/Assets/UnrealTournament/Characters/Human/malcolm_ut4_SKELMESH.FBX" ,
         avango.gua.LoaderFlags.LOAD_MATERIALS
         |avango.gua.LoaderFlags.NORMALIZE_SCALE)

  bob2.Transform.value = bob2.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0)

  load_unreal_animations(bob2)

  bob2_nav.Transform.value =  bob2_nav.Transform.value * avango.gua.make_trans_mat(0.0,0.05,0.0) * avango.gua.make_scale_mat(0.02,0.02,0.02)

  bob2_ground.Children.value = [bob2_nav]
  bob2_nav.Children.value = [bob2]'''


  '''mixamo_nav = avango.gua.nodes.TransformNode(Name = "mixamo_nav")
  mixamo_ground = avango.gua.nodes.TransformNode(Name = "mixamo_ground")

  mixamo = loader.create_geometry_from_file("mixamo", "/opt/project_animation/Assets/Mixamo/Maw/Maw_J_Laygo.FBX" ,
         avango.gua.LoaderFlags.LOAD_MATERIALS
         |avango.gua.LoaderFlags.NORMALIZE_SCALE)

  mixamo.Transform.value = mixamo.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) * avango.gua.make_scale_mat(0.04,0.04,0.04)

  load_mixamo_animations_maw(mixamo)

  mixamo_nav.Transform.value =  mixamo_nav.Transform.value * avango.gua.make_trans_mat(0.0,0.05,0.0)

  mixamo_ground.Children.value = [mixamo_nav]
  mixamo_nav.Children.value = [mixamo]'''


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
  device_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
  device_sensor.Station.value = "device-xbox-1"
  # XBOX Controller2
  device_sensor2 = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
  device_sensor2.Station.value = "device-xbox-2"

  # setup character control
  character_control = CharacterControl()
  character_control.my_constructor(bob,bob_nav,AnimationConfig("idle"),window)
  apply_character_control_settings1(character_control)
  #wall detection:
  character_control.activate_wall_detection(0.0075,0.009,"idle",graph)


  '''# setup character control
  character_control2 = CharacterControl()
  character_control2.my_constructor(bob2,bob2_nav,AnimationConfig("idle"),window)
  apply_character_control_settings1(character_control2, device_sensor2)
  #wall detection:
  character_control.activate_wall_detection(0.0075,0.009,"idle",graph)'''


  '''# setup character control
  character_control3 = CharacterControl()
  character_control3.my_constructor(mixamo,mixamo_nav,AnimationConfig("idle"),window)
  #character_control3.blend_animation(AnimationConfig("idle"))
  apply_character_control_settings_mixamo(character_control3)
  #wall detection:
  character_control3.activate_wall_detection(0.0075,0.009,"idle",graph)'''



  # setup camera control
  camera_control = CameraControl()
  camera_control.my_constructor(bob,window)
  # optional / additional xbox controller settings:
  camera_control.XBOX_X.connect_from(device_sensor.Value2)
  camera_control.XBOX_Y.connect_from(device_sensor.Value3)
  camera_control.XBOX_LZ.connect_from(device_sensor.Value4)
  camera_control.XBOX_RZ.connect_from(device_sensor.Value5)
  screen.Transform.connect_from(camera_control.OutTransform)

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


  '''# setup camera control
  camera_control2 = CameraControl()
  camera_control2.my_constructor(bob,window)
  # optional / additional xbox controller settings:
  camera_control2.XBOX_X.connect_from(device_sensor2.Value2)
  camera_control2.XBOX_Y.connect_from(device_sensor2.Value3)
  camera_control2.XBOX_LZ.connect_from(device_sensor2.Value4)
  camera_control2.XBOX_RZ.connect_from(device_sensor2.Value5)
  #screen.Transform.connect_from(camera_control.OutTransform)

  # setup ground following
  ground_following2 = GroundFollowing(
    SceneGraph = graph,
    OffsetToGround = 0.01,
    MaxDistanceToGround = 1.0
  )
  ground_following2.my_constructor(gravity = -0.00005)
  ground_following2.InTransform.connect_from(bob2.WorldTransform)

  bob2_ground.Transform.connect_from(ground_following2.OutTransform)

  distance_events2 = DistanceEvents()
  distance_events2.my_constructor(character_control2)
  apply_distance_events(distance_events2, ground_following2)'''


  '''# setup camera control
  camera_control3 = CameraControl()
  camera_control3.my_constructor(mixamo,window)
  # optional / additional xbox controller settings:
  camera_control3.XBOX_X.connect_from(device_sensor.Value2)
  camera_control3.XBOX_Y.connect_from(device_sensor.Value3)
  camera_control3.XBOX_LZ.connect_from(device_sensor.Value4)
  camera_control3.XBOX_RZ.connect_from(device_sensor.Value5)
  screen.Transform.connect_from(camera_control3.OutTransform)

  # setup ground following
  ground_following3 = GroundFollowing(
    SceneGraph = graph,
    OffsetToGround = 0.01,
    MaxDistanceToGround = 1.0
  )
  ground_following3.my_constructor(gravity = -0.00005)
  ground_following3.InTransform.connect_from(mixamo.WorldTransform)

  mixamo_ground.Transform.connect_from(ground_following3.OutTransform)'''



  timer = avango.nodes.TimeSensor()

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()

if __name__ == '__main__':
  start()

