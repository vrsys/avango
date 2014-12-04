import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE

class TimedRotate(avango.script.Script):
  TimeIn = avango.SFFloat()
  MatrixOut = avango.gua.SFMatrix4()

  @field_has_changed(TimeIn)
  def update(self):
    self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value*2.0, 0.0, 1.0, 0.0)

def on_resize(size):
  print("on_resize " + str(size))

def on_key_press(key, scancode, action, mods):
  print("on_key_press " + str(key) + " " + str(scancode) + " " + str(action) + " " + str(mods))

def on_char(char):
  print("on_char " + str(char))

def on_button_press(key, action, mods):
  print("on_button_press " + str(key) + " " + str(action) + " " + str(mods))

def on_move_cursor(position):
  print("on_move_cursor " + str(position))

def on_scroll(scroll):
  print("on_scroll " + str(scroll))

def on_enter(enter):
  print("on_enter " + str(enter))

def start():

  # setup scenegraph
  graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")

  loader = avango.gua.nodes.TriMeshLoader()
  # monkey = loader.create_geometry_from_file("monkey", "data/objects/monkey.obj", "data/materials/Stones.gmd", avango.gua.LoaderFlags.DEFAULTS)

  desc = avango.gua.nodes.MaterialShaderDescription(FileName= "data/materials/SimpleMaterial.gmd")
  meth = avango.gua.nodes.MaterialShaderMethod(FileName= "data/materials/FragmentColor.gpd")
  mat  = avango.gua.create_material_from_description(desc, "SimpleMaterial")


  # monkey = loader.create_geometry_from_file("monkey",
  #                                           "/opt/3d_models/OIL_RIG_GUACAMOLE/oilrig.obj",
  #                                           mat,
  #                                           avango.gua.LoaderFlags.NORMALIZE_POSITION
  #                                           | avango.gua.LoaderFlags.NORMALIZE_SCALE
  #                                           | avango.gua.LoaderFlags.LOAD_MATERIALS
  #                                           | avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY)

  monkey = loader.create_geometry_from_file("monkey",
                                            "data/objects/monkey.obj",
                                            mat,
                                            avango.gua.LoaderFlags.NORMALIZE_POSITION
                                            | avango.gua.LoaderFlags.NORMALIZE_SCALE
                                            | avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY)


  transform = avango.gua.nodes.TransformNode(Children = [monkey])

  light = avango.gua.nodes.PointLightNode(Name = "light", Color = avango.gua.Color(1.0, 1.0, 1.0))
  light.Transform.value = avango.gua.make_trans_mat(1, 1, 2) * avango.gua.make_scale_mat(15, 15, 15)

  size = avango.gua.Vec2ui(1024, 768)
  # setup viewing

  window = avango.gua.nodes.GlfwWindow(
    Size = size,
    LeftResolution = size
  )

  window.on_resize(on_resize)
  window.on_key_press(on_key_press)
  window.on_char(on_char)
  window.on_button_press(on_button_press)
  window.on_move_cursor(on_move_cursor)
  window.on_scroll(on_scroll)
  window.on_enter(on_enter)


  cam = avango.gua.nodes.CameraNode(LeftScreenPath = "/screen",
                                    SceneGraph = "scenegraph",
                                    Resolution = size,
                                    OutputWindowName = "window")
  cam.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 3.5)

  screen = avango.gua.nodes.ScreenNode(Name = "screen", Width = 2, Height = 1.5)
  screen.Children.value = [cam]


  graph.Root.value.Children.value = [transform, light, screen]


  avango.gua.register_window("window", window)


  #setup viewer
  viewer = avango.gua.nodes.Viewer()
  viewer.CameraNodes.value = [cam]
  viewer.SceneGraphs.value = [graph]
  viewer.Window.value = window

  monkey_updater = TimedRotate()

  timer = avango.nodes.TimeSensor()
  monkey_updater.TimeIn.connect_from(timer.Time)

  transform.Transform.connect_from(monkey_updater.MatrixOut)

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()

