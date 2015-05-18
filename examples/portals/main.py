import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import avango.gua.gui
import examples_common.navigator
from examples_common.GuaVE import GuaVE

CUBE_COUNT_X = 15
CUBE_COUNT_Y = 3
CUBE_COUNT_Z = 15

class FPSUpdater(avango.script.Script):
  TimeIn = avango.SFFloat()
  FPSResource = avango.gua.gui.SFGuiResourceNode()
  Window = avango.gua.SFWindowBase()
  Viewer = avango.gua.SFViewer()

  @field_has_changed(TimeIn)
  def update_fps(self):
    application_string = "{:5.2f}".format(self.Viewer.value.ApplicationFPS.value)
    rendering_string = "{:5.2f}".format(self.Window.value.RenderingFPS.value)
    fps_string = "FPS: " + application_string + " " + rendering_string
    self.FPSResource.value.call_javascript("set_fps_text", [fps_string])

def start():

  width = 1920;
  height = int(width * 9.0 / 16.0)
  size = avango.gua.Vec2ui(width, height)

  main_scene  = avango.gua.nodes.SceneGraph(Name = "main_scene")
  portal_scene  = avango.gua.nodes.SceneGraph(Name = "portal_scene")
  loader = avango.gua.nodes.TriMeshLoader()

  # main scene -----------------------------------------------------------------
  desc = avango.gua.nodes.MaterialShaderDescription()
  desc.load_from_file("data/materials/portal.gmd")
  avango.gua.register_material_shader(desc, "portal_mat")

  portal_transform = avango.gua.nodes.TransformNode(
    Name = "portal_transform",
    Transform = avango.gua.make_trans_mat(3, 0.6, 6) * avango.gua.make_scale_mat(3)
  )

  sphere = loader.create_geometry_from_file(
    "sphere",
    "data/objects/sphere.obj",
  )
  sphere.Material.value.ShaderName.value = "portal_mat"
  sphere.Material.value.EnableBackfaceCulling.value = False

  main_scene.Root.value.Children.value.append(portal_transform)
  portal_transform.Children.value.append(sphere)

  screen = avango.gua.nodes.ScreenNode(
    Name = "screen",
    Width = 4.8,
    Height = 2.7,
    Transform = avango.gua.make_trans_mat(0.0, 0.0, -2.5)
  )

  camera = avango.gua.nodes.CameraNode(
    Name = "cam",
    LeftScreenPath = "/cam/screen",
    SceneGraph = "main_scene",
    Resolution = size,
    OutputWindowName = "window",
    Children = [screen],
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 7.0)
  )

  res_pass = avango.gua.nodes.ResolvePassDescription(
    BackgroundMode = 1,
    BackgroundTexture = "/opt/guacamole/resources/skymaps/water_painted_noon.jpg",
    EnableSSAO = True,
    SSAOIntensity = 3.0,
    SSAOFalloff = 20.0,
    SSAORadius = 10.0
  )

  pipeline_description = avango.gua.nodes.PipelineDescription(
    EnableABuffer = True,
    Passes = [
      avango.gua.nodes.TriMeshPassDescription(),
      avango.gua.nodes.LightVisibilityPassDescription(),
      res_pass,
      avango.gua.nodes.TexturedScreenSpaceQuadPassDescription()
    ]
  )
  camera.PipelineDescription.value = pipeline_description

  geometry = loader.create_geometry_from_file(
    "geometry",
    "/opt/3d_models/architecture/weimar_geometry/weimar_stadtmodell_latest_version/weimar_stadtmodell_final.obj",
    avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY
  )
  geometry.Material.value.set_uniform("Roughness", 0.6)
  geometry.Material.value.set_uniform("Metalness", 0.0)
  geometry.Transform.value = avango.gua.make_trans_mat(0, -0.99, 0) * avango.gua.make_scale_mat(0.1)
  main_scene.Root.value.Children.value.append(geometry)

  sun_light = avango.gua.nodes.LightNode(
    Name = "sun_light",
    Type = avango.gua.LightType.SUN,
    Color = avango.gua.Color(1.0, 1.0, 0.7),
    EnableShadows = True,
    ShadowMapSize = 1024,
    ShadowOffset = 0.003,
    ShadowCascadedSplits = [0.1, 1.5, 4, 15],
    ShadowNearClippingInSunDirection = 100,
    Brightness = 2,
    Transform = avango.gua.make_rot_mat(210, 0, 1, 0) * avango.gua.make_rot_mat(-50.0, 1.0, 1.0, 0.0)
  )

  top_light = avango.gua.nodes.LightNode(
    Name = "top_light",
    Type = avango.gua.LightType.SUN,
    Color = avango.gua.Color(0.1, 0.2, 0.4),
    EnableSpecularShading = False,
    Brightness = 1,
    Transform = avango.gua.make_rot_mat(90.0, 1.0, 0.0, 0.0)
  )

  main_scene.Root.value.Children.value.append(top_light)
  main_scene.Root.value.Children.value.append(sun_light)
  main_scene.Root.value.Children.value.append(camera)


  fps_size = avango.gua.Vec2(170, 55)

  fps = avango.gua.gui.nodes.GuiResourceNode()
  fps.TextureName.value = "fps"
  fps.URL.value = "asset://gua/data/html/fps.html"
  fps.Size.value = fps_size

  fps_quad = avango.gua.nodes.TexturedScreenSpaceQuadNode(
    Name = "fps_quad",
    Texture = "fps",
    Width = int(fps_size.x),
    Height = int(fps_size.y),
    Anchor = avango.gua.Vec2(1.0, 1.0)
  )
  main_scene.Root.value.Children.value.append(fps_quad)

  # portal scene ---------------------------------------------------------------
  portal_origin = avango.gua.nodes.TransformNode(
    Name = "portal_origin"
  )
  portal_origin.Transform.connect_from(portal_transform.Transform)
  portal_scene.Root.value.Children.value.append(portal_origin)

  sphere = loader.create_geometry_from_file(
    "sphere",
    "data/objects/sphere.obj",
  )
  sphere.RenderToStencilBuffer.value = True
  sphere.RenderToGBuffer.value = False
  sphere.Material.value.EnableBackfaceCulling.value = False
  portal_origin.Children.value.append(sphere)

  fallback_mat = avango.gua.create_material(avango.gua.MaterialCapabilities.COLOR_VALUE)

  for x in range(0, CUBE_COUNT_X):
    for y in range(0, CUBE_COUNT_Y):
      for z in range(0, CUBE_COUNT_Z):
        new_cube = loader.create_geometry_from_file(
          "cube" + str(x) + str(y) + str(z),
          "data/objects/monkey.obj",
          fallback_mat,
          avango.gua.LoaderFlags.DEFAULTS
        )
        new_cube.Transform.value = avango.gua.make_trans_mat(x-CUBE_COUNT_X/2, y-CUBE_COUNT_Y/2, z-CUBE_COUNT_Z/2) * \
                                   avango.gua.make_scale_mat(0.1)
        portal_origin.Children.value.append(new_cube)
        new_cube.ShadowMode.value = 1


  light = avango.gua.nodes.LightNode(
    Type = avango.gua.LightType.SUN,
    Name = "sun",
    Color = avango.gua.Color(1.0, 1.0, 0.5),
    Brightness = 3,
    Transform = avango.gua.make_rot_mat(-150, 1, 0, 0)
  )
  portal_scene.Root.value.Children.value.append(light)

  screen = avango.gua.nodes.ScreenNode(
    Name = "screen",
    Width = 4.8,
    Height = 2.7,
    Transform = avango.gua.make_trans_mat(0.0, 0.0, -2.5)
  )

  portal_culling_screen = avango.gua.nodes.ScreenNode(
    Name = "portal_culling_screen",
    Width = 1,
    Height = 1
  )
  portal_origin.Children.value.append(portal_culling_screen)

  frame = loader.create_geometry_from_file(
    "frame",
    "data/objects/plane.obj",
  )
  frame.Transform.value = avango.gua.make_rot_mat(90, 1, 0, 0)
  frame.Tags.value = ["invisible"]
  portal_culling_screen.Children.value.append(frame)

  global_clipping_plane = avango.gua.nodes.ClippingPlaneNode(
    Tags = ["invisible"]
  )
  portal_culling_screen.Children.value.append(global_clipping_plane);

  portal_camera = avango.gua.nodes.CameraNode(
    Name = "portal_cam",
    LeftScreenPath = "/portal_cam/screen",
    AlternativeFrustumCullingScreenPath = "/portal_origin/portal_culling_screen",
    SceneGraph = "portal_scene",
    Resolution = size,
    OutputTextureName = "portal",
    Children = [screen],
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 7.0),
    BlackList = ["invisible"]
  )
  portal_scene.Root.value.Children.value.append(portal_camera)

  print (portal_camera.OutputWindowName.value)

  portal_res_pass = avango.gua.nodes.ResolvePassDescription()
  portal_res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.2,0.2,0.1)
  portal_res_pass.BackgroundMode.value = 1
  portal_res_pass.EnableSSAO.value = True
  portal_res_pass.SSAORadius.value = 5
  portal_res_pass.SSAOIntensity.value = 2
  portal_res_pass.BackgroundTexture.value = "/opt/guacamole/resources/skymaps/checker.png"
  portal_pipeline_description_stencil = avango.gua.nodes.PipelineDescription(
    Passes = [
      avango.gua.nodes.StencilPassDescription(),
      avango.gua.nodes.TriMeshPassDescription(),
      avango.gua.nodes.LightVisibilityPassDescription(),
      portal_res_pass
    ]
  )
  portal_camera.PipelineDescription.value = portal_pipeline_description_stencil

  portal_pipeline_description_no_stencil = avango.gua.nodes.PipelineDescription(
    Passes = [
      avango.gua.nodes.TriMeshPassDescription(),
      avango.gua.nodes.LightVisibilityPassDescription(),
      portal_res_pass
    ]
  )

  portal_camera.PipelineDescription.value.EnableABuffer.value = True

  portal_scene.Root.value.Children.value.append(portal_camera)

  # window setup ---------------------------------------------------------------
  window = avango.gua.nodes.GlfwWindow(
    Size = size,
    Title = "picking",
    LeftResolution = size
  )


  def disable_global_clipping_plane():
    nonlocal portal_camera
    portal_camera.BlackList.value = ["invisible"]
    print("Disable global clippping plane.")

  def enable_global_clipping_plane():
    nonlocal portal_camera
    portal_camera.BlackList.value = []
    print("Enable global clippping plane.")

  def disable_portal_culling():
    nonlocal portal_camera
    portal_camera.AlternativeFrustumCullingScreenPath.value = ""
    print("Disable portal culling.")

  def enable_portal_culling():
    nonlocal portal_camera
    portal_camera.AlternativeFrustumCullingScreenPath.value = "/portal_origin/portal_culling_screen"
    print("Enable portal culling.")

  class TurnTo(avango.script.Script):
    TransformIn = avango.gua.SFMatrix4()
    CullingScreen = avango.gua.nodes.ScreenNode()
    Outside = True

    def __init__(self):
      self.super(TurnTo).__init__()

    @field_has_changed(TransformIn)
    def update(self):
      if self.CullingScreen.Parent.value != None:
        target = self.TransformIn.value.get_translate()
        pos = self.CullingScreen.Parent.value.WorldTransform.value.get_translate()

        dir = target-pos
        dist = dir.normalize()

        if dist < 1.51:
          if self.Outside:
            self.Outside = False
            disable_portal_culling()
            disable_global_clipping_plane()
        else:
          if not self.Outside:
            self.Outside = True
            enable_portal_culling()
            enable_global_clipping_plane()

          self.CullingScreen.Tags.value = []
          self.CullingScreen.Transform.value = avango.gua.make_look_at_mat_inv(
            dir*0.5,
            pos-target,
            avango.gua.Vec3(0, 1, 0)
          )

  turn_to = TurnTo()
  turn_to.TransformIn.connect_from(camera.Transform)
  turn_to.CullingScreen = portal_culling_screen

  def handle_key(key, scancode, action, mods):
    nonlocal frame
    nonlocal camera
    nonlocal portal_camera
    nonlocal portal_pipeline_description_stencil
    nonlocal portal_pipeline_description_no_stencil

    if action == 0:
      if key is 49: # key 1
        if portal_camera.OutputWindowName.value == "window":
          camera.OutputWindowName.value = "window"
          portal_camera.OutputWindowName.value = ""
          print("Showing both scenes.")
        else:
          portal_camera.OutputWindowName.value = "window"
          camera.OutputWindowName.value = ""
          print("Showing only portal scene.")

      elif key is 50: # key 2
        if len(portal_camera.PipelineDescription.value.Passes.value) == 4:
          portal_camera.PipelineDescription.value = portal_pipeline_description_no_stencil
          print("Disable stencil clipping.")
        else:
          portal_camera.PipelineDescription.value = portal_pipeline_description_stencil
          print("Enable stencil clipping.")

      elif key is 51: # key 3
        if portal_camera.AlternativeFrustumCullingScreenPath.value == "":
          enable_portal_culling()
        else:
          disable_portal_culling()

      elif key is 52: # key 4
        if len(portal_camera.BlackList.value) == 0:
          disable_global_clipping_plane()
        else:
          enable_global_clipping_plane()

      elif key is 53: # key 5
        if len(frame.Tags.value) == 0:
          frame.Tags.value = ["invisible"]
          print("Hide frame.")
        else:
          frame.Tags.value = []
          print("Show frame.")


  window.on_key_press(handle_key)

  camera.PreRenderCameras.value.append(portal_camera)

  avango.gua.register_window("window", window)

  navigator = examples_common.navigator.Navigator()
  navigator.StartLocation.value = camera.Transform.value.get_translate()
  navigator.OutTransform.connect_from(camera.Transform)

  navigator.RotationSpeed.value = 0.2
  navigator.MotionSpeed.value = 0.04

  camera.Transform.connect_from(navigator.OutTransform)
  portal_camera.Transform.connect_from(navigator.OutTransform)

  viewer = avango.gua.nodes.Viewer()
  viewer.SceneGraphs.value = [main_scene, portal_scene]
  viewer.Windows.value = [window]

  timer = avango.nodes.TimeSensor()

  fps_updater = FPSUpdater(
    FPSResource=fps,
    Window=window,
    Viewer=viewer
  )
  fps_updater.TimeIn.connect_from(timer.Time)

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  print("")
  print("Hotkeys:")
  print("  1: toggle scene")
  print("  2: toggle stencil clipping of portal scene")
  print("  3: toggle alternative culling of portal scene")
  print("  4: toggle global clipping planes in portal scene")
  print("  5: toggle visibility of culling screen frame")
  print("")
  print("")

  viewer.run()


if __name__ == '__main__':
  start()

