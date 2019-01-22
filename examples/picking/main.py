import avango
import avango.script
from avango.script import field_has_changed
import avango.gua

import examples_common.navigator
from examples_common.GuaVE import GuaVE

# create a simple scene

CUBE_COUNT_X = 5
CUBE_COUNT_Y = 5
CUBE_COUNT_Z = 5

class Picker(avango.script.Script):
  SceneGraph = avango.gua.SFSceneGraph()
  Ray        = avango.gua.SFRayNode()
  Options    = avango.SFInt()
  BlackList  = []
  WhiteList  = []
  Results    = avango.gua.MFPickResult()

  def __init__(self):
    self.super(Picker).__init__()
    self.always_evaluate(True)

    self.SceneGraph.value = avango.gua.nodes.SceneGraph()
    self.Ray.value  = avango.gua.nodes.RayNode()
    self.Options.value = avango.gua.PickingOptions.GET_TEXTURE_COORDS \
                         | avango.gua.PickingOptions.GET_WORLD_NORMALS \
                         | avango.gua.PickingOptions.INTERPOLATE_NORMALS \
                         | avango.gua.PickingOptions.PICK_ONLY_FIRST_FACE

  def evaluate(self):
    results = self.SceneGraph.value.ray_test(self.Ray.value,
                                             self.Options.value,
                                             self.BlackList,
                                             self.WhiteList)
    self.Results.value = results.value

class MaterialUpdater(avango.script.Script):
  PickedNodes     = avango.gua.MFPickResult()
  OldNodes        = avango.gua.MFPickResult()

  @field_has_changed(PickedNodes)
  def update_materials(self):
    for i in range(0, len(self.OldNodes.value)):
      if isinstance(self.OldNodes.value[i].Object.value, avango.gua.TriMeshNode):
        self.OldNodes.value[i].Object.value.Material.value.set_uniform("Color", avango.gua.Vec4(1,1,1,1))

    for i in range(0, len(self.PickedNodes.value)):
      if isinstance(self.PickedNodes.value[i].Object.value, avango.gua.TriMeshNode):
        self.PickedNodes.value[i].Object.value.Material.value.set_uniform("Color", avango.gua.Vec4(1,0,0,0.5))

    self.OldNodes.value = self.PickedNodes.value

def start():

  graph  = avango.gua.nodes.SceneGraph(Name = "scene")
  loader = avango.gua.nodes.TriMeshLoader()
  for x in range(0, CUBE_COUNT_X):
    for y in range(0, CUBE_COUNT_Y):
      for z in range(0, CUBE_COUNT_Z):

        new_cube = loader.create_geometry_from_file("cube" + str(x) + str(y) + str(z),
                  "data/objects/sphere.obj",
                  avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.MAKE_PICKABLE)

        new_cube.Transform.value = avango.gua.make_trans_mat(x, y, z) * \
                                   avango.gua.make_scale_mat(0.3, 0.3, 0.3)
        graph.Root.value.Children.value.append(new_cube)

  light = avango.gua.nodes.LightNode(
    Type=avango.gua.LightType.POINT,
    Name = "sun",
    Color = avango.gua.Color(1.0, 1.0, 0.7),
    Brightness = 10
  )

  light.Transform.value = avango.gua.make_trans_mat((CUBE_COUNT_X - 1.0) * 0.5, (CUBE_COUNT_Y - 1.0) * 0.5, (CUBE_COUNT_Z - 1.0) * 0.5) * \
                          avango.gua.make_scale_mat(100)



  pick_ray = avango.gua.nodes.RayNode(Name = "pick_ray")
  pick_ray.Transform.value = avango.gua.make_trans_mat(0.0, -1.0, 0.0) * \
                             avango.gua.make_scale_mat(1.0, 1.0, 50.0)

  ray_geom = loader.create_geometry_from_file(
    "ray_geom",
    "data/objects/cylinder.obj",
    avango.gua.LoaderFlags.DEFAULTS
  )
  
  ray_geom.Transform.value = avango.gua.make_scale_mat(0.1, 0.1, 100)
  pick_ray.Children.value.append(ray_geom)

  screen = avango.gua.nodes.ScreenNode(Name = "screen",
                                       Width = 4.8,
                                       Height = 2.7)


  screen.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -2.5)

  graph.Root.value.Children.value.append(light)


  picker = Picker()
  picker.SceneGraph.value = graph
  picker.Ray.value = pick_ray

  material_updater = MaterialUpdater()
  material_updater.PickedNodes.connect_from(picker.Results)

  width = 1920;
  height = int(width * 9.0 / 16.0)
  size = avango.gua.Vec2ui(width, height)

  camera = avango.gua.nodes.CameraNode(
    Name = "cam",
    LeftScreenPath = "/cam/screen",
    SceneGraph = "scene",
    Resolution = size,
    OutputWindowName = "window",
    Children = [screen, pick_ray],
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 7.0)
  )

  camera.PipelineDescription.value.EnableABuffer.value = True

  graph.Root.value.Children.value.append(camera)

  window = avango.gua.nodes.Window(
    Size = size,
    Title = "picking",
    LeftResolution = size
  )

  avango.gua.register_window("window", window)

  navigator = examples_common.navigator.Navigator()
  navigator.StartLocation.value = camera.Transform.value.get_translate()
  navigator.OutTransform.connect_from(camera.Transform)

  navigator.RotationSpeed.value = 0.2
  navigator.MotionSpeed.value = 0.04

  camera.Transform.connect_from(navigator.OutTransform)

  viewer = avango.gua.nodes.Viewer()
  viewer.SceneGraphs.value = [graph]
  viewer.Windows.value = [window]
  guaVE = GuaVE()
  guaVE.start(locals(), globals())
  viewer.run()


if __name__ == '__main__':
  start()

