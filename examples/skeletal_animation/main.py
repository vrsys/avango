import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE

def start():

  # setup scenegraph
  graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")

  loader = avango.gua.nodes.TriMeshLoader()

  monkey = loader.create_geometry_from_file("monkey",
                                            "data/objects/monkey.obj",
                                            avango.gua.create_default_material(),
                                            avango.gua.LoaderFlags.NORMALIZE_POSITION
                                            | avango.gua.LoaderFlags.NORMALIZE_SCALE
                                            | avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY)

  monkey.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 0.766, 0.336, 1.0))
  monkey.Material.value.set_uniform("Roughness", 0.3)
  monkey.Material.value.set_uniform("Metalness", 1.0)

  light = avango.gua.nodes.PointLightNode(
                Name = "light",
                Color = avango.gua.Color(1.0, 1.0, 1.0),
                Brightness = 100.0)
  light.Transform.value = avango.gua.make_trans_mat(2, 2, 5) * avango.gua.make_scale_mat(15, 15, 15)

  size = avango.gua.Vec2ui(1024, 768)

  window = avango.gua.nodes.GlfwWindow(
    Size = size,
    LeftResolution = size
  )

  cam = avango.gua.nodes.CameraNode(LeftScreenPath = "/screen",
                                    SceneGraph = "scenegraph",
                                    Resolution = size,
                                    OutputWindowName = "window")

  cam.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 5.0)

  screen = avango.gua.nodes.ScreenNode(Name = "screen", Width = 2, Height = 1.5)
  screen.Children.value = [cam]

  graph.Root.value.Children.value = [monkey, light, screen]


  avango.gua.register_window("window", window)


  #setup viewer
  viewer = avango.gua.nodes.Viewer()
  viewer.CameraNodes.value = [cam]
  viewer.SceneGraphs.value = [graph]
  viewer.Window.value = window

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()

if __name__ == '__main__':
  start()

