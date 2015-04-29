import avango
import avango.script
import avango.gua.skelanim
from examples_common.GuaVE import GuaVE

from avango.gua.skelanim.GroundFollowing import *
from avango.gua.skelanim.CharacterControl import *
from avango.gua.skelanim.CameraControl import *
from avango.gua.skelanim.AnimationControl import AnimationConfig
from CharacterSettings import *

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

  #XBOX Controller
  device_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
  device_sensor.Station.value = "device-xbox-1"
  

  #mixamo character
  skel_mesh_loader = avango.gua.skelanim.nodes.SkeletalAnimationLoader()

  #scenegraph setup:
  #root    ->   kachujin_ground    ->   kachujin_nav    ->   kachujin
  #             ground following        character control    geometry
  kachujin_ground = avango.gua.nodes.TransformNode(Name = "kachujin_ground")
  graph.Root.value.Children.value.append(kachujin_ground)
  kachujin_nav = avango.gua.nodes.TransformNode(Name = "kachujin_nav")
  #set initial position of character here:
  #kachujin_nav.Transform.value =  avango.gua.make_trans_mat(0.0, 0.0, 0.0)

  kachujin = skel_mesh_loader.create_geometry_from_file("kachujin", "/opt/project_animation/Assets/Mixamo/Kachujin/Kachujin_G_Rosales.FBX" ,
         avango.gua.LoaderFlags.LOAD_MATERIALS
         |avango.gua.LoaderFlags.NORMALIZE_SCALE)

  kachujin.Transform.value = kachujin.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0) *  avango.gua.make_scale_mat(0.02,0.02,0.02)

  #load animations from character settings
  load_mixamo_animations_kachujin(kachujin)

  kachujin_ground.Children.value = [kachujin_nav]
  kachujin_nav.Children.value = [kachujin]

  #character control
  kachujin_character_control = CharacterControl()
  kachujin_character_control.my_constructor(kachujin,kachujin_nav,AnimationConfig("idle"), window)
  #override keyboard events with xbox controller events:
  #apply_character_control_settings_mixamo(kachujin_character_control, device_sensor)
  apply_character_control_settings_mixamo(kachujin_character_control)
  #wall detection
  kachujin_character_control.activate_wall_detection(0.0075, 0.009, "idle", graph)

  #camera control
  kachujin_camera_control = CameraControl()
  kachujin_camera_control.my_constructor(kachujin,window,-17.0)
  screen.Transform.connect_from(kachujin_camera_control.OutTransform)
  #camera_control_xbox_connect(kachujin_camera_control, device_sensor)

  #ground following
  kachujin_ground_following = GroundFollowing(
    SceneGraph = graph,
    OffsetToGround = 0.01,
    MaxDistanceToGround = 100.0
  )
  kachujin_ground_following.my_constructor(gravity = -0.00005)
  kachujin_ground_following.InTransform.connect_from(kachujin.WorldTransform)
  kachujin_ground.Transform.connect_from(kachujin_ground_following.OutTransform)


  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()

if __name__ == '__main__':
  start()

