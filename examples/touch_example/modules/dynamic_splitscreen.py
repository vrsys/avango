import avango
import avango.gua
from examples_common.device import TouchDevice
from examples_common.device import TouchCursor

class FingerUpdater(avango.script.Script):
  Cursor = TouchCursor()
  MatrixOut = avango.gua.SFMatrix4()
  FingerEnabled = avango.SFBool()
  FingerGroupNames = avango.MFString()

  def __init__(self):
    self.super(FingerUpdater).__init__()
    self.always_evaluate(True)

    self.FingerGroupNames.value = ["invisible"]
    self.FingerEnabled.value = False

  def evaluate(self):
    self.MatrixOut.value = avango.gua.make_trans_mat((self.Cursor.PosX.value-0.5)*16, 0, (self.Cursor.PosY.value-0.5)*9) * avango.gua.make_scale_mat(20)

    enabled = self.Cursor.State.value != 4

    if (self.FingerEnabled.value != enabled):
      self.FingerEnabled.value = enabled

      if (self.FingerEnabled.value):
        self.FingerGroupNames.value = [""]
      else:
        self.FingerGroupNames.value = ["invisible"]


def add_split_screen(id, proxy_graph):
  loader = avango.gua.nodes.GeometryLoader()
  proxy = loader.create_geometry_from_file("proxy" + str(id), "data/objects/sphere.obj", "Splitscreen" + str(id), avango.gua.LoaderFlags.DEFAULTS)
  proxy.Transform.value = avango.gua.make_scale_mat(20)
  proxy_graph.Root.value.Children.value.append(proxy)
  avango.gua.set_material_uniform("Splitscreen" + str(id), "in_texture", "splitscreen_pipe" + str(id))
  return proxy

def make_split_screen_dynamic(proxy, touch_cursor, pipe):
  updater = FingerUpdater()
  updater.Cursor = touch_cursor
  proxy.Transform.connect_from(updater.MatrixOut)
  proxy.GroupNames.connect_from(updater.FingerGroupNames)
  pipe.Enabled.connect_from(updater.FingerEnabled)

def add_dynamic_split_screens(count, create_pipeline_func, graph, size):
  touch_device = TouchDevice()

  proxy_graph = avango.gua.nodes.SceneGraph(Name = "proxy_scenegraph")

  eye = avango.gua.nodes.TransformNode(Name = "eye")
  eye.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 25)

  screen = avango.gua.nodes.ScreenNode(
    Name = "screen",
    Width = 16,
    Height = 9,
    Transform = avango.gua.make_rot_mat(-90, 1, 0, 0)
  )
  screen.Children.value = [eye]
  proxy_graph.Root.value.Children.value = [screen]

  proxy_pipe = avango.gua.nodes.Pipeline(
    Camera = avango.gua.nodes.Camera(
      LeftEye = "/screen/eye",
      LeftScreen = "/screen",
      SceneGraph = "proxy_scenegraph",
      RenderMask = "!invisible",
      Mode = avango.gua.ProjectionMode.ORTHOGRAPHIC
    ),
    Window = avango.gua.nodes.Window(
      Size = size,
      LeftResolution = size
    ),
    LeftResolution = size,
    EnableFPSDisplay = True
  )

  for i in range(0, count):
    proxy = add_split_screen(i, proxy_graph)

    pipe = create_pipeline_func(i, graph)
    pipe.OutputTextureName.value = "splitscreen_pipe" + str(i)
    proxy_pipe.PreRenderPipelines.value.append(pipe)

    if (i != 0):
      make_split_screen_dynamic(proxy, touch_device.TouchCursors[i-1], pipe)


  proxy_pipe.OutputTextureName.value = "final_pipe"


  return proxy_graph, proxy_pipe
