import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import avango.gua.lod
from examples_common.GuaVE import GuaVE
import examples_common.navigator
from examples_common.GuaVE import GuaVE

from time import sleep

OPEN_2_WINDOWS = True


class TimedFEMUpdate(avango.script.Script):
    TimeIn = avango.SFFloat()
    
    is_first_frame = True
    last_timestamp = 0.0 

    reference_node = 0

    def set_reference_node(self, ref_node):
      self.reference_node = ref_node

    @field_has_changed(TimeIn)
    def update(self):
      if True == self.is_first_frame:
        self.is_first_frame = False
        self.last_timestamp = self.TimeIn.value
        return

      current_timestamp = self.TimeIn.value
      elapsed_seconds = current_timestamp - self.last_timestamp 
      self.last_timestamp = current_timestamp

      if 0 != self.reference_node:
        #the cursor position is updates using milliseconds
        self.reference_node.update_cursor_position(elapsed_seconds * 1000.0)


def start():

  graph  = avango.gua.nodes.SceneGraph(Name = "scene")
  mesh_loader = avango.gua.nodes.TriMeshLoader()
  
  #configure lod backend
  lod_loader = avango.gua.lod.nodes.LodLoader()
  lod_loader.UploadBudget.value = 64
  lod_loader.RenderBudget.value = 2048
  lod_loader.OutOfCoreBudget.value = 4096
  
  fallback_mat = avango.gua.create_material(avango.gua.MaterialCapabilities.COLOR_VALUE)


  #the vis file loader functions return MFNodes instead of a single Node
  plod_nodes = lod_loader.load_lod_pointclouds_from_vis_file("/mnt/pitoti/AISTec/FEM_simulation/Scherkondetal_Time_Series_20190822/Scherkondetal_Pointclouds/2019_10_24/vis_files/fe_vis_mat_150kmh.vis",
                                                              avango.gua.lod.LoaderFlags.NORMALIZE_SCALE |
                                                              avango.gua.lod.LoaderFlags.NORMALIZE_POSITION)


  for node in plod_nodes.value:
    graph.Root.value.Children.value.append(node)
    node.Transform.value = avango.gua.make_trans_mat(0, 0.0, 1.5) * node.Transform.value
    node.ShadowMode.value = 1

    node.TimeSeriesDeformFactor.value = 10000.0
    node.TimeSeriesPlaybackSpeed.value = 1.0
    
    node.EnableAutomaticPlayback.value = True

    node.AttributeToVisualizeIndex.value = 3



  spot_light_1 = avango.gua.nodes.LightNode(Name = "spot_light_1",
                                         Type = avango.gua.LightType.SPOT,
                                         Color = avango.gua.Color(1.0, 1.0, 1.0),
                                         EnableShadows = True,
                                         ShadowMapSize = 1024,
                                         ShadowOffset = 0.002,
                                         ShadowMaxDistance = 10,
                                         Falloff = 1.5,
                                         ShadowNearClippingInSunDirection = 0.1,
                                         ShadowFarClippingInSunDirection = 10.0,
                                         Softness = 2,
                                         Brightness = 20)
  spot_light_1.Transform.value = avango.gua.make_trans_mat(0.0, 3.0, 0.0) * avango.gua.make_rot_mat(-90, 1, 0, 0) * avango.gua.make_scale_mat(4)
  graph.Root.value.Children.value.append(spot_light_1)

  point_light1 = avango.gua.nodes.LightNode(
                                         Type = avango.gua.LightType.POINT,
                                         Name = "point_light1",
                                         Color = avango.gua.Color(0.2, 1.0, 1.7),
                                         EnableShadows = True,
                                         ShadowMapSize = 1024,
                                         ShadowMaxDistance = 10,
                                         ShadowOffset = 0.03,
                                         Falloff = 0.5,
                                         ShadowNearClippingInSunDirection = 0.1,
                                         ShadowFarClippingInSunDirection = 10.0,
                                         Brightness = 20)
                                         
  point_light1.Transform.value = avango.gua.make_trans_mat(1.5, 1.0, 1.5) * avango.gua.make_scale_mat(4)
  graph.Root.value.Children.value.append(point_light1)

  sun_light = avango.gua.nodes.LightNode(Name = "sun_light",
                                         Type = avango.gua.LightType.SUN,
                                         Color = avango.gua.Color(1.0, 1.0, 0.7),
                                         EnableShadows = True,
                                         ShadowMapSize = 1024,
                                         ShadowOffset = 0.0005,
                                         ShadowCascadedSplits = [0.1, 4, 7, 20],
                                         ShadowMaxDistance = 30,
                                         ShadowNearClippingInSunDirection = 100,
                                         ShadowFarClippingInSunDirection = 100,
                                         Brightness = 2
                                         )
  sun_light.Transform.value = avango.gua.make_rot_mat(210, 0, 1, 0) * avango.gua.make_rot_mat(-50.0, 1.0, 0.0, 0.0)



  width = 1920;
  height = int(width * 9.0 / 16.0)
  size = avango.gua.Vec2ui(width, height)

  screen = avango.gua.nodes.ScreenNode(Name = "screen",
                                       Width = 4.8,
                                       Height = 2.7)

  screen.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -2.5)

  camera = avango.gua.nodes.CameraNode(
    Name = "cam",
    LeftScreenPath = "/cam/screen",
    RightScreenPath = "/cam/screen",
    SceneGraph = "scene",
    Resolution = size,
    OutputWindowName = "window",
    EyeDistance = 0.06,
    EnableStereo = False,
    Children = [screen],
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 2.0)
  )


  if OPEN_2_WINDOWS:
    screen2 = avango.gua.nodes.ScreenNode(Name = "screen2",
                                         Width = 4.8,
                                         Height = 2.7)

    screen2.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -6.5)

    camera2 = avango.gua.nodes.CameraNode(
      Name = "cam2",
      LeftScreenPath = "/cam2/screen2",
      RightScreenPath = "/cam2/screen2",
      SceneGraph = "scene",
      Resolution = size,
      OutputWindowName = "window2",
      EyeDistance = 0.06,
      EnableStereo = False,
      Children = [screen2],
      Transform = avango.gua.make_trans_mat(0.0, 0.0, 2.0)
    )


  res_pass = avango.gua.nodes.ResolvePassDescription()
  res_pass.EnableSSAO.value = False
  res_pass.SSAOIntensity.value = 3.0
  res_pass.SSAOFalloff.value = 20.0
  res_pass.SSAORadius.value = 10.0
  res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1,0.1,0.1)
  res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
  res_pass.Exposure.value = 1.0
  res_pass.BackgroundMode.value = avango.gua.BackgroundMode.CUBEMAP_TEXTURE
  res_pass.BackgroundTexture.value = "awesome_skymap"
  # res_pass.BackgroundColor.value = avango.gua.Color(0.45, 0.5, 0.6)
  res_pass.VignetteColor.value = avango.gua.Vec4(0, 0, 0, 1)

  plod_pass = avango.gua.lod.nodes.PLodPassDescription()
  plod_pass.SurfelRenderMode.value = avango.gua.lod.RenderFlags.HQ_TWO_PASS
  #plod_pass.SurfelRenderMode.value = avango.gua.lod.RenderFlags.LQ_ONE_PASS

  pipeline_description = avango.gua.nodes.PipelineDescription(
    Passes = [
      avango.gua.nodes.TriMeshPassDescription(),
      plod_pass,
      avango.gua.nodes.SkyMapPassDescription(
        OutputTextureName="awesome_skymap"
      ),
      avango.gua.nodes.LightVisibilityPassDescription(),
      res_pass,
      avango.gua.nodes.DebugViewPassDescription()
    ],
    EnableABuffer = False
  )

  camera.PipelineDescription.value = pipeline_description
  graph.Root.value.Children.value.append(camera)

  if OPEN_2_WINDOWS:
    camera2.PipelineDescription.value = pipeline_description
    graph.Root.value.Children.value.append(camera2)

  window = avango.gua.nodes.Window(
    Size = size,
    Title = "Programmable LOD",
    LeftResolution = size,
    RightResolution = size,
    EnableVsync = False,
    StereoMode = avango.gua.StereoMode.MONO
  )

  if OPEN_2_WINDOWS:
    window2 = avango.gua.nodes.Window(
      Size = size,
      Title = "Programmable LOD",
      LeftResolution = size,
      RightResolution = size,
      EnableVsync = False,
      StereoMode = avango.gua.StereoMode.MONO
      #StereoMode = avango.gua.StereoMode.ANAGLYPH_RED_CYAN
    )

  avango.gua.register_window("window", window)

  if OPEN_2_WINDOWS:
    avango.gua.register_window("window2", window2)

  navigator = examples_common.navigator.Navigator()
  navigator.StartLocation.value = camera.Transform.value.get_translate()
  navigator.OutTransform.connect_from(camera.Transform)

  navigator.RotationSpeed.value = 0.2
  navigator.MotionSpeed.value = 0.04

  camera.Transform.connect_from(navigator.OutTransform)

  viewer = avango.gua.nodes.Viewer()
  # viewer.DesiredFPS.value = 200
  viewer.SceneGraphs.value = [graph]


  if OPEN_2_WINDOWS:
    viewer.Windows.value = [window, window2]
  else:
    viewer.Windows.value = [window]

  timer = avango.nodes.TimeSensor()

  timed_fem_updater = TimedFEMUpdate()
  timed_fem_updater.TimeIn.connect_from(timer.Time)
  timed_fem_updater.set_reference_node(plod_nodes.value[0])

  guaVE = GuaVE()
  guaVE.start(locals(), globals())


  viewer.run()


if __name__ == '__main__':
  start()

