import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE
from examples_common.navigator import Navigator

from projector import Projector

class TimedRotate(avango.script.Script):
  TimeIn = avango.SFFloat()
  MatrixOut = avango.gua.SFMatrix4()

  @field_has_changed(TimeIn)
  def update(self):
    time = self.TimeIn.value
    self.MatrixOut.value = avango.gua.make_rot_mat(time*5.0, 0.0, 1.0, 0.0) * avango.gua.make_scale_mat(0.5)


def start():
  # setup scenegraph
  graph  = avango.gua.nodes.SceneGraph(Name = "scenegraph")
  loader = avango.gua.nodes.TriMeshLoader()

  projector = Projector()
  projector.Graph.value = graph
  projector.Texture.value = "data/textures/smiley.jpg"

  monkey = loader.create_geometry_from_file("monkey", "data/objects/monkey.obj", avango.gua.LoaderFlags.NORMALIZE_SCALE)
  monkey.Material.connect_from(projector.Material)

  light = avango.gua.nodes.PointLightNode(
    Name = "light",
    Color = avango.gua.Color(1.0, 1.0, 1.0),
    Brightness = 50.0,
    Transform = avango.gua.make_trans_mat(1, 1, 5) * avango.gua.make_scale_mat(15, 15, 15)
  )

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

  pipeline_description = avango.gua.nodes.PipelineDescription(
      Passes = [
            avango.gua.nodes.TriMeshPassDescription(),
            avango.gua.nodes.LightVisibilityPassDescription(),
            avango.gua.nodes.ResolvePassDescription()
          ])

  cam.PipelineDescription.value = pipeline_description

  screen = avango.gua.nodes.ScreenNode(
    Name = "screen",
    Width = 2,
    Height = 1.5,
    Children = [cam]
  )

  graph.Root.value.Children.value = [monkey, light, screen, projector.group_node]

  #setup viewer
  viewer = avango.gua.nodes.Viewer()
  viewer.SceneGraphs.value = [graph]
  viewer.Windows.value = [window]

  #monkey rotation
  monkey_updater = TimedRotate()
  timer = avango.nodes.TimeSensor()
  monkey_updater.TimeIn.connect_from(timer.Time)
  monkey.Transform.connect_from(monkey_updater.MatrixOut)

  #projector movement
  navigator = Navigator()
  navigator.OutTransform.value = avango.gua.make_identity_mat()
  navigator.StartLocation.value = avango.gua.Vec3(0.0, 0.0, 1.0)

  navigator.RotationSpeed.value = 0.2
  navigator.MotionSpeed.value = 0.04

  projector.Transform.connect_from(navigator.OutTransform)


  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()





if __name__ == '__main__':
  start()

