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


class TimedFEMVisualizationUpdate(avango.script.Script):
    TimeIn = avango.SFFloat()
    
    is_first_frame = True
    last_timestamp = 0.0 

    reference_node = 0

    arrow_geometry_list = 0

    def set_reference_node(self, ref_node):
      self.reference_node = ref_node

    def set_arrow_node_list(self, arrow_geometry_list):
      self.arrow_geometry_list = arrow_geometry_list

    @field_has_changed(TimeIn)
    def update(self):
      if True == self.is_first_frame:
        self.is_first_frame = False
        self.last_timestamp = self.TimeIn.value
        return

      current_timestamp = self.TimeIn.value
      elapsed_seconds = current_timestamp - self.last_timestamp 
      self.last_timestamp = current_timestamp

      #ask one of the nodes for the current simulation positions (train axes)
      if 0 != self.reference_node:
        #the cursor position is updates using milliseconds (for auto playback)
        self.reference_node.update_cursor_position(elapsed_seconds * 1000.0)
        current_positions = self.reference_node.get_current_simulation_positions()

        #update all the train geometry by getting the multi field of axes translation
        if 0 != self.arrow_geometry_list:
          for arrow_index in range(self.reference_node.get_number_of_simulation_positions().value):
            current_arrow_translation_matrix = avango.gua.make_trans_mat(current_positions.value[arrow_index])
            self.arrow_geometry_list[arrow_index].Transform.value = current_arrow_translation_matrix

def start():

  graph  = avango.gua.nodes.SceneGraph(Name = "scene")
  mesh_loader = avango.gua.nodes.TriMeshLoader()
  
  #configure lod backend
  lod_loader = avango.gua.lod.nodes.LodLoader()
  lod_loader.UploadBudget.value = 64
  lod_loader.RenderBudget.value = 2048
  lod_loader.OutOfCoreBudget.value = 4096
  
  fallback_mat = avango.gua.create_material(avango.gua.MaterialCapabilities.COLOR_VALUE)





  #add (registered) fem model as reference coordinate system at normalized position and scale
  fem_model_geode = mesh_loader.create_geometry_from_file("fem_model", "/mnt/pitoti/AISTec/FEM_simulation/Scherkondetal_Time_Series_20190822/FEM_OBJS/2020_01_06/flipped_normals_Scherkonde_Geom_registered_2020_01_06.obj", 
                                                            avango.gua.LoaderFlags.NORMALIZE_POSITION | avango.gua.LoaderFlags.NORMALIZE_SCALE | avango.gua.LoaderFlags.LOAD_MATERIALS);

  #fem_model_geode.Transform.value = avango.gua.make_rot_mat(90.0, 1.0, 0.0, 0.0) * fem_model_geode.Transform.value
  fem_model_geode.RenderToGBuffer.value = False
  graph.Root.value.Children.value.append(fem_model_geode)



  #the vis file loader functions return MFNodes instead of a single Node
  plod_nodes = lod_loader.load_lod_pointclouds_from_vis_file("/mnt/pitoti/AISTec/FEM_simulation/Scherkondetal_Time_Series_20190822/Scherkondetal_Pointclouds/2020_01_06/vis_files/fixed_simulation_train_test.vis",
                                                              avango.gua.lod.LoaderFlags.MAKE_PICKABLE)

  #set the parameters for every node (I put the nodes under the fem_model to be in a meaningful reference coordinate system)
  for node in plod_nodes.value:
    fem_model_geode.Children.value.append(node)
    #node.Transform.value = avango.gua.make_trans_mat(0, 0.0, 1.5) * node.Transform.value
    node.ShadowMode.value = 1

    node.EnableTimeSeriesColoring.value = True
    node.EnableTimeSeriesDeformation.value = True

    #sets the mixin factor between fe coloring and original color (0.0 = full point cloud color, 1.0 = full FEM color)
    node.AttributeColorToMixInFactor.value = 0.7
    #exaggeration factor for the deformation
    node.TimeSeriesDeformFactor.value = 1000.0
    #playback speed for the time series (1.0 = real-time). setting the playback speed affects the FE simulation, so all nodes refer to the same model
    node.TimeSeriesPlaybackSpeed.value = 1.0
    
    #if this is set, the bridge keeps on playing back if the update_time_cursor function is called (see timer script)
    node.EnableAutomaticPlayback.value = True
    
    #index of the FE attribute to visualize. At least 4 are available, usually 10
    node.AttributeToVisualizeIndex.value = 3


  #ask one of the nodes (we just assume that we have at least one mode in the vis file) for the number of simulation positions (= train axes)
  arrow_geometry_list = []
  reference_node = plod_nodes.value[0]
  number_of_train_axes = reference_node.get_number_of_simulation_positions().value

  #load the right amount of arrows
  for arrow_index in range(number_of_train_axes):

    new_arrow_geometry_node = mesh_loader.create_geometry_from_file(
        "arrow_geometry_" + str(arrow_index), "/opt/3d_models/symbols/arrow.obj",
        avango.gua.LoaderFlags.LOAD_MATERIALS)

    arrow_geometry_list.append(new_arrow_geometry_node)
    fem_model_geode.Children.value.append(new_arrow_geometry_node)




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
  timed_fem_updater = TimedFEMVisualizationUpdate()
  timed_fem_updater.TimeIn.connect_from(timer.Time)
  timed_fem_updater.set_reference_node(plod_nodes.value[0])
  timed_fem_updater.set_arrow_node_list(arrow_geometry_list)

  guaVE = GuaVE()
  guaVE.start(locals(), globals())


  viewer.run()


if __name__ == '__main__':
  start()

