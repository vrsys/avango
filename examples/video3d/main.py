import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE
import sys

class TimedRotate(avango.script.Script):
  TimeIn = avango.SFFloat()
  MatrixOut = avango.gua.SFMatrix4()

  @field_has_changed(TimeIn)
  def update(self):
    self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value*2.0, 0.0, 1.0, 0.0)

def start(filename):

  # setup scenegraph
  graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")

  videoloader = avango.gua.nodes.Video3DLoader()
  video_geode = videoloader.load("kinect", filename)

  light = avango.gua.nodes.LightNode(
    Type=avango.gua.LightType.POINT,
    Name = "light", Color = avango.gua.Color(1.0, 1.0, 1.0))
  light.Transform.value = avango.gua.make_trans_mat(1, 1, 2) * avango.gua.make_scale_mat(15, 15, 15)

  eye = avango.gua.nodes.TransformNode(Name = "eye")
  eye.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 3.5)

  screen = avango.gua.nodes.ScreenNode(Name = "screen", Width = 4, Height = 3)
  screen.Children.value = [eye]

  graph.Root.value.Children.value = [video_geode, light, screen]

  # setup viewing
  size = avango.gua.Vec2ui(1024, 768)
  pipe = avango.gua.nodes.Pipeline(Camera = avango.gua.nodes.Camera(LeftEye = "/screen/eye",
                                                                    RightEye = "/screen/eye",
                                                                    LeftScreen = "/screen",
                                                                    RightScreen = "/screen",
                                                                    SceneGraph = "scenegraph"),
                                   Window = avango.gua.nodes.Window(Size = size,
                                                                    LeftResolution = size),
                                   LeftResolution = size)
  pipe.EnableBackfaceCulling.value = False

  #setup viewer
  viewer = avango.gua.nodes.Viewer()
  viewer.Pipelines.value = [pipe]
  viewer.SceneGraphs.value = [graph]

  timer = avango.nodes.TimeSensor()

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start(sys.argv[1])

