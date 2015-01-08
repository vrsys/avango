import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE

def start():

  # setup scenegraph
  graph  = avango.gua.nodes.SceneGraph(Name = "scenegraph")

  gui = avango.gua.nodes.GuiResource()
  gui.init("google", "https://www.google.com", avango.gua.Vec2(1024, 1024))

  size = avango.gua.Vec2ui(800, 600) * 2

  quad = avango.gua.nodes.TexturedQuadNode(
    Name = "gui",
    Texture = "google",
    Width = 1,
    Height = 1,
    DisplayBoundingBox = True
  )

  light = avango.gua.nodes.PointLightNode(
    Name = "light",
    Color = avango.gua.Color(1.0, 1.0, 1.0),
    Brightness = 10.0
  )
  light.Transform.value = avango.gua.make_trans_mat(1, 1, 5) * avango.gua.make_scale_mat(15, 15, 15)

  cam = avango.gua.nodes.CameraNode(
    LeftScreenPath = "/screen",
    SceneGraph = "scenegraph",
    Resolution = size,
    OutputWindowName = "window",
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 1.0)
  )

  screen = avango.gua.nodes.ScreenNode(
    Name = "screen",
    Width = 0.5,
    Height = 0.375,
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 2.0),
    Children = [cam]
  )

  loader = avango.gua.nodes.TriMeshLoader()
  cube = loader.create_geometry_from_file("cube", "data/objects/cube.obj", avango.gua.LoaderFlags.NORMALIZE_SCALE | avango.gua.LoaderFlags.MAKE_PICKABLE)

  graph.Root.value.Children.value = [light, screen, quad]


  window = avango.gua.nodes.GlfwWindow(
    Size = size,
    LeftResolution = size
  )

  avango.gua.register_window("window", window)

  def handle_char(c):
    gui.inject_char_event(c)

  def handle_key(key, scancode, action, mods):
    gui.inject_keyboard_event(key, scancode, action, mods)

  def handle_button(button, action, mods):
    gui.inject_mouse_button(button, action, mods)

  def handle_cursor(pos):
    resolution = avango.gua.Vec2(size.x, size.y)
    screen_space_pos = pos/resolution - 0.5

    origin = screen.ScaledWorldTransform.value * avango.gua.Vec4(screen_space_pos.x, screen_space_pos.y, 0, 1)
    direction = origin - cam.WorldTransform.value * avango.gua.Vec4(0,0,0,1)

    ray = avango.gua.nodes.Ray(
      Origin = avango.gua.Vec3(origin.x, origin.y, origin.z),
      Direction = avango.gua.Vec3(direction.x, direction.y, direction.z),
      TMax = 1.0
    )

    result = graph.ray_test(ray, avango.gua.PickingOptions.PICK_ONLY_FIRST_OBJECT
                                 | avango.gua.PickingOptions.PICK_ONLY_FIRST_FACE
                                 | avango.gua.PickingOptions.GET_TEXTURE_COORDS)

    if result != None:
      for i in range(0, len(result.value)):
        if result.value[i].Object.value == quad:
          gui.inject_mouse_position_relative(result.value[i].TextureCoords.value);

  window.on_char(handle_char)
  window.on_key_press(handle_key)
  window.on_button_press(handle_button)
  window.on_move_cursor(handle_cursor)


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

