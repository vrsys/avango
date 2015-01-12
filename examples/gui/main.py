import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE

def start():

  # setup scenegraph
  graph  = avango.gua.nodes.SceneGraph(Name = "scenegraph")

  google_res = avango.gua.nodes.GuiResource()
  google_res.init("google", "https://www.google.com", avango.gua.Vec2(1024, 1024))

  google_mat_desc = avango.gua.nodes.MaterialShaderDescription(
    FileName = "data/materials/SimpleMaterial.gmd"
  )
  google_mat = avango.gua.create_material_from_description(google_mat_desc, "google_mat");
  google_mat.set_uniform("tex", "google")

  transform = avango.gua.nodes.TransformNode()

  resolution = avango.gua.Vec2ui(1920, 1080)

  loader = avango.gua.nodes.TriMeshLoader()
  google_geom = loader.create_geometry_from_file("google_geom", "data/objects/curved_plane.obj", google_mat,
                                                 avango.gua.LoaderFlags.NORMALIZE_POSITION |
                                                 avango.gua.LoaderFlags.NORMALIZE_SCALE |
                                                 avango.gua.LoaderFlags.MAKE_PICKABLE )

  transform.Children.value.append(google_geom)

  fps_size = avango.gua.Vec2(170, 55)

  fps = avango.gua.nodes.GuiResource()
  fps.init("fps", "asset://gua/data/html/fps.html", fps_size)

  fps_quad = avango.gua.nodes.TexturedScreenSpaceQuadNode(
    Name = "fps_quad",
    Width = int(fps_size.x),
    Height = int(fps_size.y),
    Anchor = avango.gua.Vec2(1.0, 1.0)
  )

  light = avango.gua.nodes.PointLightNode(
    Name = "light",
    Color = avango.gua.Color(1.0, 1.0, 1.0),
    Transform = avango.gua.make_trans_mat(-2, 3, 5) * avango.gua.make_scale_mat(10)
  )

  cam = avango.gua.nodes.CameraNode(
    LeftScreenPath = "/screen",
    SceneGraph = "scenegraph",
    Resolution = resolution,
    OutputWindowName = "window",
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 1.0)
  )

  screen = avango.gua.nodes.ScreenNode(
    Name = "screen",
    Width = 1.92 * 0.25,
    Height = 1.08 * 0.25,
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 2.0),
    Children = [cam]
  )

  graph.Root.value.Children.value = [transform, light, screen, fps_quad]


  window = avango.gua.nodes.GlfwWindow(
    Size = resolution,
    LeftResolution = resolution
  )

  avango.gua.register_window("window", window)

  def handle_char(c):
    google_res.inject_char_event(c)

  def handle_key(key, scancode, action, mods):
    google_res.inject_keyboard_event(key, scancode, action, mods)

  def handle_button(button, action, mods):
    google_res.inject_mouse_button(button, action, mods)

  def handle_cursor(pos):
    res = avango.gua.Vec2(resolution.x, resolution.y)
    screen_space_pos = pos/res - 0.5

    origin = screen.ScaledWorldTransform.value * avango.gua.Vec4(screen_space_pos.x, screen_space_pos.y, 0, 1)
    direction = origin - cam.WorldTransform.value * avango.gua.Vec4(0,0,0,1)

    ray = avango.gua.nodes.Ray(
      Origin = avango.gua.Vec3(origin.x, origin.y, origin.z),
      Direction = avango.gua.Vec3(direction.x, direction.y, direction.z) * 100.0,
      TMax = 1.0
    )

    result = graph.ray_test(ray, avango.gua.PickingOptions.PICK_ONLY_FIRST_OBJECT
                                 | avango.gua.PickingOptions.PICK_ONLY_FIRST_FACE
                                 | avango.gua.PickingOptions.GET_TEXTURE_COORDS)

    for i in range(0, len(result.value)):
      if result.value[i].Object.value == google_geom:
        google_res.inject_mouse_position_relative(result.value[i].TextureCoords.value);

  def handle_scroll(scroll):
    google_res.inject_mouse_wheel(scroll)

  window.on_char(handle_char)
  window.on_key_press(handle_key)
  window.on_button_press(handle_button)
  window.on_move_cursor(handle_cursor)
  window.on_scroll(handle_scroll)


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

