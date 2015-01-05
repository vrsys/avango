import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE

def start():

  # setup scenegraph
  graph  = avango.gua.nodes.SceneGraph(Name = "scenegraph")

  gui = avango.gua.nodes.GuiResource()
  gui.init("google", "https://www.google.com", avango.gua.Vec2(512, 512))

  quad = avango.gua.nodes.TexturedQuadNode(
    Name = "gui",
    Texture = "google",
    Width = 1,
    Height = 1,
    Transform = avango.gua.make_trans_mat(0, 0, -2)
  )

  light = avango.gua.nodes.PointLightNode(
    Name = "light",
    Color = avango.gua.Color(1.0, 1.0, 1.0),
    Brightness = 50.0
  )
  light.Transform.value = avango.gua.make_trans_mat(1, 1, 5) * avango.gua.make_scale_mat(15, 15, 15)

  size = avango.gua.Vec2ui(1024, 768)

  window = avango.gua.nodes.GlfwWindow(
    Size = size,
    LeftResolution = size
  )

  avango.gua.register_window("window", window)

  cam = avango.gua.nodes.CameraNode(
    LeftScreenPath = "/screen",
    SceneGraph = "scenegraph",
    Resolution = size,
    OutputWindowName = "window",
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 3.5)
  )

  screen = avango.gua.nodes.ScreenNode(
    Name = "screen",
    Width = 2,
    Height = 1.5,
    Children = [cam]
  )

  graph.Root.value.Children.value = [quad, light, screen]

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

