import avango
import avango.script
from avango.script import field_has_changed
# import avango.gua
import avango.gua.skelanim
from examples_common.GuaVE import GuaVE
import math

from CharacterControl import *
from CameraControl import *

bob = None
#light2 = None
cubes = []
character_control = None



class GroundFollowing(avango.script.Script):
  InTransform = avango.gua.SFMatrix4()
  OutTransform = avango.gua.SFMatrix4()
  OutTransform.value = avango.gua.make_identity_mat()

  OffsetToGround = avango.SFFloat()
  MaxDistanceToGround = avango.SFFloat()
  SceneGraph = avango.gua.nodes.SceneGraph()

  _velocity_y = 0.0
  _gravity = -0.00005

  def __init__(self):
    self.super(GroundFollowing).__init__()

    self.OffsetToGround.value = 0.0
    self.MaxDistanceToGround.value = 1000.0
    self.__ray = avango.gua.nodes.RayNode()


  def evaluate(self):
    global bob
    #global light2
    global cubes
    global character_control

    in_translation = self.InTransform.value.get_translate()
    in_translation.y += self.OffsetToGround.value

    self.__ray.Transform.value = avango.gua.make_trans_mat(in_translation) *\
                                 avango.gua.make_rot_mat(-90, 1, 0, 0) *\
                                 avango.gua.make_scale_mat(1.0, 1.0, self.MaxDistanceToGround.value)

    results = self.SceneGraph.value.ray_test(
                                     self.__ray,
                                     avango.gua.PickingOptions.PICK_ONLY_FIRST_OBJECT |
                                     avango.gua.PickingOptions.GET_POSITIONS)


    delta_trans = avango.gua.Vec3(0.0,0.0,0.0)

    if len(results.value) > 0:


      if bob and character_control.get_current_animation() == "swim":
       character_control._switch_animation("run_fwd")

      first_hit = results.value[0]

      hit_world = first_hit.Object.value.Transform.value * avango.gua.make_trans_mat(first_hit.Position.value)
      
      hit_world_trans = hit_world.get_translate()
      #hit_world_trans = first_hit.WorldPosition.value

      new_pos = avango.gua.make_trans_mat(hit_world_trans)# * avango.gua.make_trans_mat(0.0,self.OffsetToGround.value, 0.0)

      '''if light2 != None:
        light2.Transform.value = new_pos * avango.gua.make_trans_mat(0.0,0.2,0.0)'''

      delta_trans = (new_pos.get_translate() - self.InTransform.value.get_translate())

    else:
      if bob and character_control.get_current_animation() == "run_fwd":
       character_control._switch_animation("swim")

    delta_norm = avango.gua.Vec3(delta_trans.x,delta_trans.y,delta_trans.z)
    length = delta_norm.normalize()

    if length > self._velocity_y and delta_trans.y < 0.0:

      self._velocity_y -= self._gravity

      #self.OutTransform.value = self.OutTransform.value * avango.gua.make_trans_mat(delta_norm*0.01)
      self.OutTransform.value = self.OutTransform.value * avango.gua.make_trans_mat(delta_norm*self._velocity_y)
    else:
      if delta_trans.y < 0.0:
        self.OutTransform.value = self.OutTransform.value * avango.gua.make_trans_mat(delta_trans)
      else:
        self.OutTransform.value = self.OutTransform.value * avango.gua.make_trans_mat(delta_trans/4.0)#smooth walking stairs

      self._velocity_y = 0.0

      if character_control.get_current_animation() == "jump_loop":
        character_control._switch_animation("jump_land",0.01, False)
        character_control.queue_animation("idle", 0.25)

      if character_control.get_current_animation() == "jump_fwd_preland":
        character_control._switch_animation("jump_fwd_land", 0.1, False)
        character_control.queue_animation("idle", 0.25)

    if math.fabs(delta_trans.y)<0.09 and character_control != None:
      if character_control.get_current_animation() == "jump_fwd_loop":
        character_control._switch_animation("jump_fwd_preland",0.01, False)



def start():

  global bob
  global character_control
  
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
  
  # jump idle:
  loader.load_animation(bob,
          "data/animations/Jump_Idle_Rif_Start.FBX","jump", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Idle_Rif_Loop.FBX","jump_loop", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Idle_Rif_Land.FBX","jump_land", avango.gua.LoaderFlags.DEFAULTS)
  
  # jump forward: 
  '''loader.load_animation(bob,
          "data/animations/Jump_Fwd_Rif.FBX","jump_fwd", avango.gua.LoaderFlags.DEFAULTS)'''
  loader.load_animation(bob,
          "data/animations/Jump_Fwd_Rif_Start.FBX","jump_fwd", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Fwd_Rif_Loop.FBX","jump_fwd_loop", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Fwd_Rif_PreLand.FBX","jump_fwd_preland", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/animations/Jump_Idle_Rif_Land.FBX","jump_fwd_land", avango.gua.LoaderFlags.DEFAULTS)


  bob_nav.Transform.value =  bob_nav.Transform.value * avango.gua.make_trans_mat(0.0,0.05,0.0) * avango.gua.make_scale_mat(0.02,0.02,0.02)

  ##bob.AnimationMode.value = 1

  bob_ground.Children.value = [bob_nav]
  bob_nav.Children.value = [bob]


  #environment:
  tri_mesh_loader = avango.gua.nodes.TriMeshLoader()

  #medieval_harbour = tri_mesh_loader.create_geometry_from_file("medieval_harbour", "data/objects/highrise/highrise_from_dae.fbx",
  #medieval_harbour = tri_mesh_loader.create_geometry_from_file("medieval_harbour", "/opt/3d_models/architecture/medieval_harbour/town.obj",
  medieval_harbour = tri_mesh_loader.create_geometry_from_file("medieval_harbour", "data/objects/highrise/highrise_from_obj2.fbx",
                                            avango.gua.LoaderFlags.MAKE_PICKABLE|
                                            avango.gua.LoaderFlags.LOAD_MATERIALS)

  '''for child in medieval_harbour.Children.value:
    child.Material.value.EnableBackfaceCulling.value = False
    child.Material.value.set_uniform("Roughness",0.8)'''


  #medieval_harbour.Transform.value = medieval_harbour.Transform.value * avango.gua.make_trans_mat(0,0.57, -5)
  #medieval_harbour.Transform.value = medieval_harbour.Transform.value * avango.gua.make_trans_mat(0.0,0.0, -5.0)

  '''light = avango.gua.nodes.PointLightNode(
                Name = "light",
                Color = avango.gua.Color(0.1, 0.1, 0.1),
                Brightness = 1.0,
                EnableShadows = True)

  light.Transform.value = avango.gua.make_trans_mat(0.0, 2.0, 2.0) * avango.gua.make_scale_mat(2100, 2100, 2100)

  global light2
  light2 = avango.gua.nodes.PointLightNode(
                Name = "light2",
                Color = avango.gua.Color(1.0, 1.0, 1.0),
                Brightness = 1.0,
                EnableShadows = True)'''


  sunlight = avango.gua.nodes.SunLightNode()
  sunlight.Color.value = avango.gua.Color(0.8,0.6,0.45)
  sunlight.Brightness.value = 3
  sunlight.Transform.value = avango.gua.make_rot_mat(-80, 1.0, 0.0, 0.0)
  graph.Root.value.Children.value.append(sunlight)

  size = avango.gua.Vec2ui(2560, 1440)

  window = avango.gua.nodes.GlfwWindow(
    Size = size,
    LeftResolution = size
  )

  cam = avango.gua.nodes.CameraNode(LeftScreenPath = "/screen",
                                    SceneGraph = "scenegraph",
                                    Resolution = size,
                                    OutputWindowName = "window")

  pipeline_description = avango.gua.nodes.PipelineDescription(
      Passes = [
            avango.gua.nodes.TriMeshPassDescription(),
            avango.gua.nodes.LightVisibilityPassDescription(),
            avango.gua.skelanim.nodes.SkeletalAnimationPassDescription(),
            avango.gua.nodes.ResolvePassDescription()
          ])

  pipeline_description.Passes.value[3].EnableSSAO.value = True
  pipeline_description.Passes.value[3].SSAORadius.value = 2.0
  pipeline_description.Passes.value[3].SSAOIntensity.value = 2.0
  pipeline_description.Passes.value[3].BackgroundMode.value = 1
  pipeline_description.Passes.value[3].BackgroundTexture.value = "/opt/avango/master/examples/picking/data/textures/skymap.jpg"
  #pipeline_description.Passes.value[3].Background  Texture.value = "data/objects/highrise/skydome_space.jpg"
  pipeline_description.Passes.value[3].ToneMappingMode.value = 3

  #pipeline_description.EnableABuffer.value = True
  cam.PipelineDescription.value = pipeline_description

  cam.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 0.4)
  #cam.FarClip.value = 10000
  cam.FarClip.value = 300
  cam.NearClip.value = 0.01

  screen = avango.gua.nodes.ScreenNode(Name = "screen", Width = 0.8, Height = 0.45)
  #screen.Children.value = [cam, light]
  screen.Children.value = [cam]
  screen.Transform.value = avango.gua.make_trans_mat(0, 0.1, -2)

  #graph.Root.value.Children.value = [bob_ground, medieval_harbour ,screen,light2]
  graph.Root.value.Children.value = [bob_ground, medieval_harbour ,screen, sunlight]

  avango.gua.register_window("window", window)

  #setup viewer
  viewer = avango.gua.nodes.Viewer()
  #viewer.CameraNodes.value = [cam]
  viewer.SceneGraphs.value = [graph]
  viewer.Windows.value = [window]


  # setup character control
  character_control = CharacterControl()
  character_control.my_constructor(bob,bob_nav,window)

  # A
  character_control.bind_transformation(65, avango.gua.make_rot_mat(4.0, 0.0, 1.0,0.0))
  # D
  character_control.bind_transformation(68, avango.gua.make_rot_mat(-4.0, 0.0, 1.0,0.0))

  # W
  character_control.key_down(87, "idle","run_fwd")
  character_control.key_up(87, "run_fwd","idle")
  character_control.key_down(87, "crouch","crouch_fwd")
  character_control.key_up(87, "crouch_fwd","crouch")
  #character_control.key_up(87, "jump_fwd","idle")

  # S
  character_control.key_down(83, "idle","run_bwd")
  character_control.key_up(83, "run_bwd","idle")
  character_control.key_down(83, "crouch","crouch_bwd")
  character_control.key_up(83, "crouch_bwd","crouch")

  # C
  character_control.key_down(67, "idle","crouch",0.2)
  character_control.key_up(67, "crouch","idle",0.4)
  character_control.key_down(67, "run_fwd","crouch_fwd")
  character_control.key_up(67, "crouch_fwd","run_fwd")
  character_control.key_down(67, "run_bwd","crouch_bwd")
  character_control.key_up(67, "crouch_bwd","run_bwd")

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
  character_control.key_down(32, "idle","jump",0.01)
  character_control.key_down(32, "run_fwd","jump_fwd",0.25)
  #character_control.key_up(32, "jump","jump_loop",0.25)
  #character_control.key_up(32, "jump_fwd","jump_fwd_loop",0.25)
  #character_control.bind_transformation(32, avango.gua.make_trans_mat(0.0, 0.1, 0.0))

  character_control.bind_translation("run_fwd",avango.gua.Vec3(0.0,0.0,0.05))
  character_control.bind_translation("run_bwd",avango.gua.Vec3(0.0,0.0,-0.05))
  character_control.bind_translation("swim",avango.gua.Vec3(0.0,0.0,0.05))
  character_control.bind_translation("crouch_fwd",avango.gua.Vec3(0.0,0.0,0.02))
  character_control.bind_translation("crouch_bwd",avango.gua.Vec3(0.0,0.0,-0.02))
  character_control.bind_translation("jump",avango.gua.Vec3(0.0, 0.08, 0.0))
  character_control.bind_translation("jump_loop",avango.gua.Vec3(0.0, 0.08, 0.0))
  character_control.bind_translation("jump_fwd",avango.gua.Vec3(0.0, 0.08, 0.04))
  character_control.bind_translation("jump_fwd_loop",avango.gua.Vec3(0.0, 0.08, 0.04))
  character_control.bind_translation("jump_fwd_preland",avango.gua.Vec3(0.0, 0.08, 0.04))

  character_control.play_once("jump","jump_loop",0.01)
  character_control.play_once("jump_fwd","jump_fwd_loop",0.01)

  #wall detection:
  character_control.activate_wall_detection(0.0075,0.013,"idle",graph)

  # setup camera control
  camera_control = CameraControl()
  camera_control.my_constructor(screen,bob,window)

  # setup ground following
  ground_following = GroundFollowing(
    SceneGraph = graph,
    OffsetToGround = 0.01,
    MaxDistanceToGround = 1.0
  )

  ground_following.InTransform.connect_from(bob.WorldTransform)

  bob_ground.Transform.connect_from(ground_following.OutTransform)

  timer = avango.nodes.TimeSensor()
  #character_control.TimeIn.connect_from(timer.Time)
  #camera_control.TimeIn.connect_from(timer.Time)

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()

if __name__ == '__main__':
  start()

