import avango
import avango.script
from avango.script import field_has_changed
import avango.gua

import examples_common.navigator
from examples_common.GuaVE import GuaVE

# create a simple scene
avango.gua.load_shading_models_from("data/materials")
avango.gua.load_materials_from("data/materials")

CUBE_COUNT_X = 15
CUBE_COUNT_Y = 15
CUBE_COUNT_Z = 15

class Picker(avango.script.Script):
  SceneGraph = avango.gua.SFSceneGraph()
  Ray        = avango.gua.SFRayNode()
  Options    = avango.SFInt()
  Mask       = avango.SFString()
  Results    = avango.gua.MFPickResult()

  def __init__(self):
    self.super(Picker).__init__()
    self.always_evaluate(True)

    self.SceneGraph.value = avango.gua.nodes.SceneGraph()
    self.Ray.value  = avango.gua.nodes.RayNode()
    self.Options.value = avango.gua.PickingOptions.PICK_ONLY_FIRST_OBJECT \
                         | avango.gua.PickingOptions.GET_TEXTURE_COORDS \
                         | avango.gua.PickingOptions.GET_WORLD_NORMALS \
                         | avango.gua.PickingOptions.INTERPOLATE_NORMALS \
                         | avango.gua.PickingOptions.PICK_ONLY_FIRST_FACE
    self.Mask.value = ""

  def evaluate(self):
    results = self.SceneGraph.value.ray_test(self.Ray.value,
                                             self.Options.value,
                                             self.Mask.value)
    self.Results.value = results.value

class MaterialUpdater(avango.script.Script):
  PickedNodes     = avango.gua.MFPickResult()
  OldNodes        = avango.gua.MFPickResult()
  DefaultMaterial = avango.SFString()
  TargetMaterial  = avango.SFString()

  @field_has_changed(PickedNodes)
  def update_materials(self):

    for i in range(0, len(self.OldNodes.value)):
      if isinstance(self.OldNodes.value[i].Object.value, avango.gua.GeometryNode):
        self.OldNodes.value[i].Object.value.Material.value = self.DefaultMaterial.value

    for i in range(0, len(self.PickedNodes.value)):
      if isinstance(self.PickedNodes.value[i].Object.value, avango.gua.GeometryNode):
        self.PickedNodes.value[i].Object.value.Material.value = self.TargetMaterial.value
        avango.gua.set_material_uniform(self.TargetMaterial.value, "pointer_pos",
                                        self.PickedNodes.value[i].TextureCoords.value)
        avango.gua.set_material_uniform(self.TargetMaterial.value, "color",
                                        self.PickedNodes.value[i].WorldNormal.value)

    self.OldNodes.value = self.PickedNodes.value

def setup_pipe():
  camera = avango.gua.nodes.Camera(LeftEye = "/eye",
                                   RightEye = "/eye",
                                   LeftScreen = "/eye/screen",
                                   RightScreen = "/eye/screen",
                                   SceneGraph = "scene")

  width = 1920;
  height = int(width * 9.0 / 16.0)
  size = avango.gua.Vec2ui(width, height)

  window = avango.gua.nodes.Window(Size = size,
                                   Title = "weimar",
                                   LeftResolution = size)

  pipe = avango.gua.nodes.Pipeline(Camera = camera,
                                   Window = window,
                                   LeftResolution = size)


  avango.gua.create_texture("data/textures/skymap.jpg")

  pipe.OutputTextureName.value = "weimar_pipe"
  pipe.EnableSsao.value = True
  pipe.SsaoRadius.value = 5
  pipe.SsaoIntensity.value = 0.7

  pipe.EnableFog.value = True
  pipe.FogTexture.value = "data/textures/skymap.jpg"
  pipe.FogStart.value = 11.0
  pipe.FogEnd.value = 25.0

  pipe.BackgroundTexture.value = "data/textures/skymap.jpg"

  pipe.EnableVignette.value = True
  pipe.VignetteColor.value = avango.gua.Color()
  pipe.VignetteCoverage.value = 0.5
  pipe.VignetteSoftness.value = 0.6

  pipe.EnableFXAA.value = True
  pipe.EnableFrustumCulling.value = True

  pipe.EnableHDR.value = True
  pipe.HDRKey.value = 3.0

  pipe.EnableBloom.value = True
  pipe.BloomRadius.value = 1.9
  pipe.BloomThreshold.value = 0.9
  pipe.BloomIntensity.value = 0.3

  pipe.EnableFPSDisplay.value = True
  pipe.EnableRayDisplay.value = True

  pipe.BackgroundMode.value = avango.gua.BackgroundMode.SKYMAP_TEXTURE

  return pipe

def start():

  avango.gua.load_material("data/materials/Bright.gmd")

  graph  = avango.gua.nodes.SceneGraph(Name = "scene")
  loader = avango.gua.nodes.TriMeshLoader()

  for x in range(0, CUBE_COUNT_X):
    for y in range(0, CUBE_COUNT_Y):
      for z in range(0, CUBE_COUNT_Z):

        new_cube = loader.create_geometry_from_file("cube" + str(x) + str(y) + str(z),
                  "data/objects/sphere.obj",
                  "data/materials/Stone.gmd",
                  avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.MAKE_PICKABLE)

        new_cube.Transform.value = avango.gua.make_trans_mat(x, y, z) * \
                                   avango.gua.make_scale_mat(0.3, 0.3, 0.3)
        graph.Root.value.Children.value.append(new_cube)

  light = avango.gua.nodes.SunLightNode(
    Name = "sun",
    Color = avango.gua.Color(1.0, 1.0, 0.7),
    EnableGodrays = True
  )

  light.Transform.value = avango.gua.make_rot_mat(120, 0, 1, 0) * avango.gua.make_rot_mat(-20.0, 1.0, 1.0, 0.0)


  pick_ray = avango.gua.nodes.RayNode(Name = "pick_ray")
  pick_ray.Transform.value = avango.gua.make_trans_mat(0.0, -1.0, 0.0) * \
                             avango.gua.make_scale_mat(1.0, 1.0, 50.0)

  screen = avango.gua.nodes.ScreenNode(Name = "screen",
                                       Width = 4.8,
                                       Height = 2.7)


  screen.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -2.5)

  eye = avango.gua.nodes.TransformNode(Name = "eye")

  eye.Transform.value = avango.gua.make_trans_mat(0.0, 2.8, 14.0)

  eye.Children.value = [screen, pick_ray]

  graph.Root.value.Children.value.append(light)
  graph.Root.value.Children.value.append(eye)


  picker = Picker()
  picker.SceneGraph.value = graph
  picker.Ray.value = pick_ray

  material_updater = MaterialUpdater()
  material_updater.DefaultMaterial.value = "data/materials/Stone.gmd"
  material_updater.TargetMaterial.value = "data/materials/Bright.gmd"
  material_updater.PickedNodes.connect_from(picker.Results)

  pipe = setup_pipe()

  navigator = examples_common.navigator.Navigator()
  navigator.StartLocation.value = eye.Transform.value.get_translate()
  navigator.OutTransform.connect_from(eye.Transform)

  navigator.RotationSpeed.value = 0.2
  navigator.MotionSpeed.value = 0.04

  eye.Transform.connect_from(navigator.OutTransform)

  viewer = avango.gua.nodes.Viewer()
  viewer.Pipelines.value = [pipe]
  viewer.SceneGraphs.value = [graph]

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()

