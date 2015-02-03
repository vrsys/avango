import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE

bob = None
bob_nav = None
LAST_ANIMATION = ""
light2 = None

class TimedRun(avango.script.Script):
  TimeIn = avango.SFFloat()
  MatrixOut = avango.gua.SFMatrix4()

  @field_has_changed(TimeIn)
  def update(self):
    global bob
    global bob_nav
    global LAST_ANIMATION
    #self.MatrixOut.value = avango.gua.make_trans_mat(0.0, 0.0,-self.TimeIn.value*2.0,)
    if bob != None:
      blendFact = bob.BlendingFactor.value
      if bob.Animation.value == "data/objects/marine/run.md5anim":
        bob_nav.Transform.value = bob_nav.Transform.value * avango.gua.make_trans_mat(0.0, 0.0,blendFact * 0.05)
      if LAST_ANIMATION == "data/objects/marine/run.md5anim":
        bob_nav.Transform.value = bob_nav.Transform.value * avango.gua.make_trans_mat(0.0, 0.0,(1-blendFact) * 0.05)

class GroundFollowing(avango.script.Script):
  InTransform = avango.gua.SFMatrix4()
  OutTransform = avango.gua.SFMatrix4()
  OutTransform.value = avango.gua.make_identity_mat()

  OffsetToGround = avango.SFFloat()
  MaxDistanceToGround = avango.SFFloat()
  SceneGraph = avango.gua.nodes.SceneGraph()

  def __init__(self):
    self.super(GroundFollowing).__init__()

    self.OffsetToGround.value = 0.0
    self.MaxDistanceToGround.value = 1000.0
    self.__ray = avango.gua.nodes.RayNode()


  # @field_has_changed(InTransform)
  def evaluate(self):
    global light2
    in_translation = self.InTransform.value.get_translate()
    #temp_trans = avango.gua.Vec3(in_translation.x,in_translation.y,in_translation.z)
    #temp_trans.y += 0.51
    in_translation.y += self.OffsetToGround.value * 2.0

    self.__ray.Transform.value = avango.gua.make_trans_mat(in_translation) *\
                                 avango.gua.make_rot_mat(-90, 1, 0, 0) *\
                                 avango.gua.make_scale_mat(1.0, 1.0, self.MaxDistanceToGround.value + self.OffsetToGround.value * 2.0)

    results = self.SceneGraph.value.ray_test(
                                     self.__ray,
                                     avango.gua.PickingOptions.PICK_ONLY_FIRST_OBJECT |
                                     #avango.gua.PickingOptions.GET_POSITIONS)
                                     avango.gua.PickingOptions.GET_WORLD_POSITIONS)

    #result = self.InTransform.value
    result = avango.gua.Vec3(0.0,0.0,0.0)

    if len(results.value) > 0:

      first_hit = results.value[0]

      #hit_world = avango.gua.make_trans_mat(first_hit.WorldPosition.value) * first_hit.Object.value.Transform.value
      if light2 != None:
        #light2.Transform.value = avango.gua.make_trans_mat(temp_trans)
        light2.Transform.value = avango.gua.make_trans_mat(first_hit.WorldPosition.value)
        #light2.Transform.value = hit_world

      new_pos = first_hit.WorldPosition.value
      #new_pos = hit_world.get_translate()
      new_pos.y += self.OffsetToGround.value

      result = (new_pos - in_translation)#/100

    self.OutTransform.value = self.OutTransform.value * avango.gua.make_trans_mat(result)


def start():

  # setup scenegraph
  graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")

  loader = avango.gua.nodes.SkeletalAnimationLoader()

  global bob
  global bob_nav
  global LAST_ANIMATION

  bob_nav = avango.gua.nodes.TransformNode(Name = "bob_nav")
  bob_ground = avango.gua.nodes.TransformNode(Name = "bob_ground")

  bob = loader.create_geometry_from_file("bob", "data/objects/marine/mpplayer.md5mesh" ,
         avango.gua.LoaderFlags.LOAD_MATERIALS
         |avango.gua.LoaderFlags.NORMALIZE_SCALE)

  loader.load_animation(bob,
          "data/objects/marine/fists_idle.md5anim", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob, "data/objects/marine/run.md5anim",
          avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/objects/marine/fists_idle.md5anim", avango.gua.LoaderFlags.DEFAULTS)
  loader.load_animation(bob,
          "data/objects/marine/crouch.md5anim", avango.gua.LoaderFlags.DEFAULTS)

  LAST_ANIMATION = bob.Animation.value

  bob_nav.Transform.value =  bob_nav.Transform.value * avango.gua.make_trans_mat(0.0,0.05,0.0) * avango.gua.make_scale_mat(0.04,0.04,0.04)

  bob.AnimationMode.value = 1

  bob_ground.Children.value = [bob_nav]
  bob_nav.Children.value = [bob]


  #environment:
  tri_mesh_loader = avango.gua.nodes.TriMeshLoader()

  medieval_harbour = tri_mesh_loader.create_geometry_from_file("medieval_harbour", "data/medieval_harbour/town.obj",
                                            avango.gua.LoaderFlags.NORMALIZE_POSITION
                                            | avango.gua.LoaderFlags.NORMALIZE_SCALE
                                            | avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY
                                            | avango.gua.LoaderFlags.MAKE_PICKABLE)

  medieval_harbour.Transform.value = medieval_harbour.Transform.value * avango.gua.make_trans_mat(0,0.57, -5)

  plane = tri_mesh_loader.create_geometry_from_file("cube",
                                            "data/objects/cube.obj",
                                            avango.gua.LoaderFlags.NORMALIZE_POSITION
                                            | avango.gua.LoaderFlags.NORMALIZE_SCALE
                                            | avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY
                                            | avango.gua.LoaderFlags.MAKE_PICKABLE)

  plane.Transform.value *= avango.gua.make_scale_mat(3.0,0.01,10.0) *  avango.gua.make_trans_mat(0, -3, 0)

  plane2 = tri_mesh_loader.create_geometry_from_file("cube2",
                                            "data/objects/cube.obj",
                                            avango.gua.LoaderFlags.NORMALIZE_POSITION
                                            | avango.gua.LoaderFlags.NORMALIZE_SCALE
                                            | avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY
                                            | avango.gua.LoaderFlags.MAKE_PICKABLE)

  plane.Transform.value *= avango.gua.make_scale_mat(1.0,1.0,1.0) *  avango.gua.make_trans_mat(0, 0, 0)

  light = avango.gua.nodes.PointLightNode(
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
                EnableShadows = True)


  size = avango.gua.Vec2ui(2560, 1440)

  window = avango.gua.nodes.GlfwWindow(
    Size = size,
    LeftResolution = size
  )


  def handle_char(c):
      global LAST_ANIMATION
      if c == 119 and bob.Animation.value != "data/objects/marine/run.md5anim":
        LAST_ANIMATION = bob.Animation.value
        bob.Animation.value = "data/objects/marine/run.md5anim"
      if c == 115 and bob.Animation.value != "data/objects/marine/fists_idle.md5anim":
        LAST_ANIMATION = bob.Animation.value
        bob.Animation.value = "data/objects/marine/fists_idle.md5anim"
      if c == 100:
        bob_nav.Transform.value = bob_nav.Transform.value * avango.gua.make_rot_mat(-2.0, 0.0, 1.0,0.0) 
      if c == 97:
        bob_nav.Transform.value = bob_nav.Transform.value * avango.gua.make_rot_mat(2.0, 0.0, 1.0,0.0) 
      if c == 99 and bob.Animation.value != "data/objects/marine/crouch.md5anim":
        LAST_ANIMATION = bob.Animation.value
        bob.Animation.value = "data/objects/marine/crouch.md5anim"
      if c == 99:
        bob_nav.Transform.value = bob_nav.Transform.value * avango.gua.make_trans_mat(0.0,0.5,0.0)
  window.on_char(handle_char)

  cam = avango.gua.nodes.CameraNode(LeftScreenPath = "/screen",
                                    SceneGraph = "scenegraph",
                                    Resolution = size,
                                    OutputWindowName = "window")

  cam.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 5.0)
  cam.FarClip.value = 10000

  screen = avango.gua.nodes.ScreenNode(Name = "screen", Width = 1.6, Height = 0.9)
  screen.Children.value = [cam, light]
  screen.Transform.value = avango.gua.make_trans_mat(0, 0.1, -2)

  graph.Root.value.Children.value = [bob_ground, medieval_harbour , plane ,screen,light2]

  avango.gua.register_window("window", window)

  #setup viewer
  viewer = avango.gua.nodes.Viewer()
  viewer.CameraNodes.value = [cam]
  viewer.SceneGraphs.value = [graph]
  viewer.Windows.value = [window]

  ground_following = GroundFollowing(
    SceneGraph = graph,
    OffsetToGround = 0.0,
    MaxDistanceToGround = 10.3
  )

  ground_following.InTransform.connect_from(bob.WorldTransform)

  bob_ground.Transform.connect_from(ground_following.OutTransform)

  run_updater = TimedRun()

  timer = avango.nodes.TimeSensor()
  run_updater.TimeIn.connect_from(timer.Time)

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()

if __name__ == '__main__':
  start()

