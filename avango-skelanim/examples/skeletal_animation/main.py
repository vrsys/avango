import avango
import avango.script
from avango.script import field_has_changed
import avango.gua.skelanim
from examples_common.GuaVE import GuaVE

from avango.gua.skelanim.GroundFollowing import *
from avango.gua.skelanim.CharacterControl import *
from avango.gua.skelanim.CameraControl import *
from avango.gua.skelanim.DistanceEvents import *

def start():
  
  # setup scenegraph
  graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")


  loader = avango.gua.skelanim.nodes.SkeletalAnimationLoader()

  bob_nav = avango.gua.nodes.TransformNode(Name = "bob_nav")
  bob_ground = avango.gua.nodes.TransformNode(Name = "bob_ground")

  bob = loader.create_geometry_from_file("bob", "data/objects/Necris/necris_male_ut4_SKELMESH.FBX" ,
         avango.gua.LoaderFlags.LOAD_MATERIALS
         |avango.gua.LoaderFlags.NORMALIZE_SCALE)

  bob.Transform.value = bob.Transform.value * avango.gua.make_rot_mat(-90.0,1.0,0.0,0.0)

  # idle:
  loader.load_animation(bob,
          "data/animations/Idle_Ready_Rif.FBX","idle", avango.gua.LoaderFlags.DEFAULTS)
  
  # run:
  loader.load_animation(bob,
         "data/animations/Run_Fwd_Rif.FBX","run_fwd", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
         "data/animations/Run_Bwd_Rif.FBX","run_bwd", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
         "data/animations/Run_Lt_Rif.FBX","run_lt", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
         "data/animations/Run_Rt_Rif.FBX","run_rt", avango.gua.LoaderFlags.DEFAULTS)
  
  # taunts:
  loader.load_animation(bob,
          "data/animations/Taunt_BringItOn.FBX","taunt1", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Taunt_ComeHear.FBX","taunt2", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Taunt_Hoolahoop.FBX","taunt3", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Taunt_NoNo.FBX","taunt4", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Taunt_PelvicThrust.FBX","taunt5", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Taunt_SlitThroat.FBX","taunt6", avango.gua.LoaderFlags.DEFAULTS)
  
  # swim forward: 
  loader.load_animation(bob,
          "data/animations/Swim_Fwd_Rif.FBX","swim", avango.gua.LoaderFlags.DEFAULTS)
  
  # crouch:
  loader.load_animation(bob,
          "data/animations/Crouch_Idle_Ready_Rif.FBX","crouch", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Crouch_Fwd_Rif.FBX","crouch_fwd", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Crouch_Bwd_Rif.FBX","crouch_bwd", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Crouch_Lt_Rif.FBX","crouch_lt", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Crouch_Rt_Rif.FBX","crouch_rt", avango.gua.LoaderFlags.DEFAULTS)
  
  # jump idle:
  loader.load_animation(bob,
          "data/animations/Jump_Idle_Rif_Start.FBX","jump", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Idle_Rif_Loop.FBX","jump_loop", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Idle_Rif_Preland.FBX","jump_preland", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Idle_Rif_Land.FBX","jump_land", avango.gua.LoaderFlags.DEFAULTS)
  
  # jump forward: 
  loader.load_animation(bob,
          "data/animations/Jump_Fwd_Rif_Start.FBX","jump_fwd", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Fwd_Rif_Loop.FBX","jump_fwd_loop", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Fwd_Rif_PreLand.FBX","jump_fwd_preland", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Fwd_Rif_Land.FBX","jump_fwd_land", avango.gua.LoaderFlags.DEFAULTS)

  # jump backward: 
  loader.load_animation(bob,
          "data/animations/Jump_Bwd_Rif_Start.FBX","jump_bwd", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Bwd_Rif_Loop.FBX","jump_bwd_loop", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Bwd_Rif_PreLand.FBX","jump_bwd_preland", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Bwd_Rif_Land.FBX","jump_bwd_land", avango.gua.LoaderFlags.DEFAULTS)

  # jump left: 
  loader.load_animation(bob,
          "data/animations/Jump_Lt_Rif_Start.FBX","jump_lt", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Lt_Rif_Loop.FBX","jump_lt_loop", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Lt_Rif_PreLand.FBX","jump_lt_preland", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Lt_Rif_Land.FBX","jump_lt_land", avango.gua.LoaderFlags.DEFAULTS)

  # jump right: 
  loader.load_animation(bob,
          "data/animations/Jump_Rt_Rif_Start.FBX","jump_rt", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Rt_Rif_Loop.FBX","jump_rt_loop", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Rt_Rif_PreLand.FBX","jump_rt_preland", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Rt_Rif_Land.FBX","jump_rt_land", avango.gua.LoaderFlags.DEFAULTS)


  bob_nav.Transform.value =  bob_nav.Transform.value * avango.gua.make_trans_mat(0.0,0.05,0.0) * avango.gua.make_scale_mat(0.02,0.02,0.02)

  bob_ground.Children.value = [bob_nav]
  bob_nav.Children.value = [bob]


  #environment:
  tri_mesh_loader = avango.gua.nodes.TriMeshLoader()

  #medieval_harbour = tri_mesh_loader.create_geometry_from_file("medieval_harbour", "data/objects/highrise/highrise_from_dae.fbx",
  #medieval_harbour = tri_mesh_loader.create_geometry_from_file("medieval_harbour", "/opt/3d_models/architecture/medieval_harbour/town.obj",
  #medieval_harbour = tri_mesh_loader.create_geometry_from_file("medieval_harbour", "data/objects/highrise/highrise_from_obj2.fbx",
  medieval_harbour = tri_mesh_loader.create_geometry_from_file("medieval_harbour", "data/objects/highrise/tower.fbx",
                                            avango.gua.LoaderFlags.MAKE_PICKABLE|
                                            avango.gua.LoaderFlags.LOAD_MATERIALS)

  environment = tri_mesh_loader.create_geometry_from_file("environment", "data/objects/highrise/environment.fbx",
                                            avango.gua.LoaderFlags.MAKE_PICKABLE|
                                            avango.gua.LoaderFlags.LOAD_MATERIALS)
  environment.Transform.value = environment.Transform.value * avango.gua.make_scale_mat(0.1)

  '''for child in medieval_harbour.Children.value:
    child.Material.value.EnableBackfaceCulling.value = False
    child.Material.value.set_uniform("Roughness",0.8)'''

  #medieval_harbour.Transform.value = medieval_harbour.Transform.value * avango.gua.make_trans_mat(0,0.57, -5)
  #medieval_harbour.Transform.value = medieval_harbour.Transform.value * avango.gua.make_trans_mat(0.0,0.0, -5.0)

  plane = tri_mesh_loader.create_geometry_from_file("cube",
                                            "data/objects/cube2.obj",
                                            avango.gua.LoaderFlags.NORMALIZE_POSITION
                                            | avango.gua.LoaderFlags.NORMALIZE_SCALE
                                            | avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY
                                            | avango.gua.LoaderFlags.MAKE_PICKABLE
                                            | avango.gua.LoaderFlags.LOAD_MATERIALS)
  plane.Transform.value *= avango.gua.make_scale_mat(10.0,0.01,10.0) *  avango.gua.make_trans_mat(0, -3, 0)

  plane.Material.value.set_uniform("NormalMap","data/objects/glass_2_3_nm.TGA")

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

  #graph.Root.value.Children.value = [bob_ground, medieval_harbour ,screen]
  graph.Root.value.Children.value = [bob_ground,screen, medieval_harbour, environment, sunlight]

  avango.gua.register_window("window", window)

  #setup viewer
  viewer = avango.gua.nodes.Viewer()
  viewer.SceneGraphs.value = [graph]
  viewer.Windows.value = [window]
  #window.CursorMode.value = avango.gua.CursorMode.DISABLED


  # setup character control
  character_control = CharacterControl()
  character_control.my_constructor(bob,bob_nav,window)

  # A
  character_control.bind_transformation(65, "run_fwd", avango.gua.make_rot_mat(4.0, 0.0, 1.0,0.0))
  character_control.bind_transformation(65, "run_bwd", avango.gua.make_rot_mat(4.0, 0.0, 1.0,0.0))
  character_control.bind_transformation(65, "crouch_fwd", avango.gua.make_rot_mat(2.0, 0.0, 1.0,0.0))
  character_control.bind_transformation(65, "crouch_bwd", avango.gua.make_rot_mat(2.0, 0.0, 1.0,0.0))
  character_control.bind_transformation(65, "swim", avango.gua.make_rot_mat(2.0, 0.0, 1.0,0.0))
  character_control.key_down(65, "idle","run_lt")
  character_control.key_up(65, "run_lt","idle")
  character_control.key_down(65, "crouch","crouch_lt")
  character_control.key_up(65, "crouch_lt","crouch")

  # D
  character_control.bind_transformation(68, "run_fwd", avango.gua.make_rot_mat(-4.0, 0.0, 1.0,0.0))
  character_control.bind_transformation(68, "run_bwd", avango.gua.make_rot_mat(-4.0, 0.0, 1.0,0.0))
  character_control.bind_transformation(68, "crouch_fwd", avango.gua.make_rot_mat(-2.0, 0.0, 1.0,0.0))
  character_control.bind_transformation(68, "crouch_bwd", avango.gua.make_rot_mat(-2.0, 0.0, 1.0,0.0))
  character_control.bind_transformation(68, "swim", avango.gua.make_rot_mat(-2.0, 0.0, 1.0,0.0))
  character_control.key_down(68, "idle","run_rt")
  character_control.key_up(68, "run_rt","idle")
  character_control.key_down(68, "crouch","crouch_rt")
  character_control.key_up(68, "crouch_rt","crouch")


  # W
  character_control.key_down(87, "idle","run_fwd")
  character_control.key_up(87, "run_fwd","idle")
  character_control.key_down(87, "crouch","crouch_fwd")
  character_control.key_up(87, "crouch_fwd","crouch")
  character_control.bind_transformation(87, "run_rt", avango.gua.make_trans_mat(0.0,0.0,0.025))
  character_control.bind_transformation(87, "run_lt", avango.gua.make_trans_mat(0.0,0.0,0.025))
  character_control.bind_transformation(87, "crouch_rt", avango.gua.make_trans_mat(0.0,0.0,0.0175))
  character_control.bind_transformation(87, "crouch_lt", avango.gua.make_trans_mat(0.0,0.0,0.0175))

  # S
  character_control.key_down(83, "idle","run_bwd")
  character_control.key_up(83, "run_bwd","idle")
  character_control.key_down(83, "crouch","crouch_bwd")
  character_control.key_up(83, "crouch_bwd","crouch")
  character_control.bind_transformation(83, "run_rt", avango.gua.make_trans_mat(0.0,0.0,-0.025))
  character_control.bind_transformation(83, "run_lt", avango.gua.make_trans_mat(0.0,0.0,-0.025))
  character_control.bind_transformation(83, "crouch_rt", avango.gua.make_trans_mat(0.0,0.0,-0.0175))
  character_control.bind_transformation(83, "crouch_lt", avango.gua.make_trans_mat(0.0,0.0,-0.0175))

  # C
  character_control.key_down(67, "idle","crouch",0.2)
  character_control.key_up(67, "crouch","idle",0.4)
  character_control.key_down(67, "run_fwd","crouch_fwd")
  character_control.key_up(67, "crouch_fwd","run_fwd")
  character_control.key_down(67, "run_bwd","crouch_bwd")
  character_control.key_up(67, "crouch_bwd","run_bwd")
  character_control.key_down(67, "run_lt","crouch_lt")
  character_control.key_up(67, "crouch_lt","run_lt")
  character_control.key_down(67, "run_rt","crouch_rt")
  character_control.key_up(67, "crouch_rt","run_rt")

  # F
  character_control.key_down(70, "idle","taunt1")
  character_control.key_up(70, "taunt1","idle")

  # G
  character_control.key_down(71, "idle","taunt2")
  character_control.key_up(71, "taunt2","idle")

  # H
  character_control.key_down(72, "idle","taunt3")
  character_control.key_up(72, "taunt3","idle")

  # J
  character_control.key_down(74, "idle","taunt4")
  character_control.key_up(74, "taunt4","idle")

  # K
  character_control.key_down(75, "idle","taunt5")
  character_control.key_up(75, "taunt5","idle")

  # L
  character_control.key_down(76, "idle","taunt6")
  character_control.key_up(76, "taunt6","idle")


  # SPACE BAR
  character_control.key_down(32, "idle","jump",0.1)
  character_control.key_down(32, "run_fwd","jump_fwd",0.25)
  character_control.key_down(32, "run_bwd","jump_bwd",0.25)
  character_control.key_down(32, "run_lt","jump_lt",0.25)
  character_control.key_down(32, "run_rt","jump_rt",0.25)

  # animation dependend translations:
  character_control.bind_translation("run_fwd",avango.gua.Vec3(0.0,0.0,0.05))
  character_control.bind_translation("run_bwd",avango.gua.Vec3(0.0,0.0,-0.05))
  character_control.bind_translation("run_lt",avango.gua.Vec3(0.05,0.0,0.0))
  character_control.bind_translation("run_rt",avango.gua.Vec3(-0.05,0.0,0.0))
  character_control.bind_translation("swim",avango.gua.Vec3(0.0,0.0,0.05))
  character_control.bind_translation("crouch_fwd",avango.gua.Vec3(0.0,0.0,0.035))
  character_control.bind_translation("crouch_bwd",avango.gua.Vec3(0.0,0.0,-0.035))
  character_control.bind_translation("crouch_lt",avango.gua.Vec3(0.035,0.0,0.0))
  character_control.bind_translation("crouch_rt",avango.gua.Vec3(-0.035,0.0,0.0))
  character_control.bind_translation("jump",avango.gua.Vec3(0.0, 0.08, 0.0))
  character_control.bind_translation("jump_loop",avango.gua.Vec3(0.0, 0.08, 0.0))
  character_control.bind_translation("jump_preland",avango.gua.Vec3(0.0, 0.08, 0.0))
  character_control.bind_translation("jump_fwd",avango.gua.Vec3(0.0, 0.08, 0.04))
  character_control.bind_translation("jump_fwd_loop",avango.gua.Vec3(0.0, 0.08, 0.04))
  character_control.bind_translation("jump_fwd_preland",avango.gua.Vec3(0.0, 0.08, 0.04))
  character_control.bind_translation("jump_fwd_land",avango.gua.Vec3(0.0, 0.0, 0.04))
  character_control.bind_translation("jump_bwd",avango.gua.Vec3(0.0, 0.08, -0.04))
  character_control.bind_translation("jump_bwd_loop",avango.gua.Vec3(0.0, 0.08, -0.04))
  character_control.bind_translation("jump_bwd_preland",avango.gua.Vec3(0.0, 0.08, -0.04))
  character_control.bind_translation("jump_bwd_land",avango.gua.Vec3(0.0, 0.0, -0.04))
  character_control.bind_translation("jump_lt",avango.gua.Vec3(0.04, 0.08, 0.0))
  character_control.bind_translation("jump_lt_loop",avango.gua.Vec3(0.04, 0.08, 0.0))
  character_control.bind_translation("jump_lt_preland",avango.gua.Vec3(0.04, 0.08, 0.0))
  character_control.bind_translation("jump_lt_land",avango.gua.Vec3(0.04, 0.0, 0.0))
  character_control.bind_translation("jump_rt",avango.gua.Vec3(-0.04, 0.08, 0.0))
  character_control.bind_translation("jump_rt_loop",avango.gua.Vec3(-0.04, 0.08, 0.0))
  character_control.bind_translation("jump_rt_preland",avango.gua.Vec3(-0.04, 0.08, 0.0))
  character_control.bind_translation("jump_rt_land",avango.gua.Vec3(-0.04, 0.0, 0.0))

  # animations only played once and followed by another one
  character_control.play_once("jump","jump_loop",0.1)
  character_control.play_once("jump_fwd","jump_fwd_loop",0.01)
  character_control.play_once("jump_bwd","jump_bwd_loop",0.01)
  character_control.play_once("jump_lt","jump_lt_loop",0.01)
  character_control.play_once("jump_rt","jump_rt_loop",0.01)
  character_control.play_once("jump_land","idle",0.25)
  character_control.play_once("jump_fwd_land","idle",0.25)
  character_control.play_once("jump_bwd_land","idle",0.25)
  character_control.play_once("jump_lt_land","idle",0.25)
  character_control.play_once("jump_rt_land","idle",0.25)

  #wall detection:
  character_control.activate_wall_detection(0.0075,0.013,"idle",graph)

  # setup camera control
  camera_control = CameraControl()
  camera_control.my_constructor(bob,window)
  screen.Transform.connect_from(camera_control.OutTransform)

  # setup ground following
  ground_following = GroundFollowing(
    SceneGraph = graph,
    OffsetToGround = 0.01,
    MaxDistanceToGround = 1.0
  )
  ground_following.InTransform.connect_from(bob.WorldTransform)

  bob_ground.Transform.connect_from(ground_following.OutTransform)

  distance_events = DistanceEvents()
  distance_events.my_constructor(character_control)
  distance_events.DistanceToGround.connect_from(ground_following.DistanceToGround)
  distance_events.smaller_than(0.09, "jump_fwd_loop", "jump_fwd_preland", 0.01, False)
  distance_events.smaller_than(0.09, "jump_bwd_loop", "jump_bwd_preland", 0.01, False)
  distance_events.smaller_than(0.09, "jump_lt_loop", "jump_lt_preland", 0.01, False)
  distance_events.smaller_than(0.09, "jump_rt_loop", "jump_rt_preland", 0.01, False)
  distance_events.smaller_than(0.09, "jump_loop", "jump_preland", 0.1, False)
  distance_events.smaller_than(0.01, "jump_preland", "jump_land", 0.1, False)
  distance_events.smaller_than(0.01, "jump_fwd_preland", "jump_fwd_land", 0.1, False)
  distance_events.smaller_than(0.01, "jump_bwd_preland", "jump_bwd_land", 0.1, False)
  distance_events.smaller_than(0.01, "jump_lt_preland", "jump_lt_land", 0.1, False)
  distance_events.smaller_than(0.01, "jump_rt_preland", "jump_rt_land", 0.1, False)
  distance_events.smaller_than(ground_following.MaxDistanceToGround.value, "swim", "run_fwd", 0.5, True)
  distance_events.bigger_than(ground_following.MaxDistanceToGround.value, "run_fwd", "swim", 0.5, True)

  timer = avango.nodes.TimeSensor()

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()

if __name__ == '__main__':
  start()

