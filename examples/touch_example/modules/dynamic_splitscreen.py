import avango
import avango.gua
from avango.script import field_has_changed
from examples_common.device import TouchDevice
from examples_common.device import TouchCursor
import modules.voronoi_helpers

class SplitScreen(avango.script.Script):

  Location = avango.gua.SFVec2()
  Pipe = avango.gua.SFPipeline()
  POISize = avango.SFFloat()

  ID = avango.SFInt()

  def __init__(self):
    self.super(SplitScreen).__init__()


    self.__proxy = None
    self.__poi = None

  def init(self, pipe_id, split_id, graph, screen_size):

    self.__screen_size = screen_size
    self.__proxy_sphere_size = screen_size.length()*2
    self.POISize.value = 0.2

    loader = avango.gua.nodes.TriMeshLoader()

    # create proxy sphere ----------------------------------------------------
    self.__proxy = loader.create_geometry_from_file("proxy" + str(split_id), "data/objects/sphere.obj", "data/materials/Splitscreen" + str(pipe_id) + ".gmd", avango.gua.LoaderFlags.DEFAULTS)


    graph.Root.value.Children.value.append(self.__proxy)
    avango.gua.set_material_uniform("data/materials/Splitscreen" + str(pipe_id) + ".gmd", "in_texture", "splitscreen_pipe" + str(pipe_id))

    # create POI marker ------------------------------------------------------
    self.__poi = loader.create_geometry_from_file("poi" + str(split_id), "data/objects/poi.obj", "data/materials/White.gmd", avango.gua.LoaderFlags.LOAD_MATERIALS)

    graph["/screen"].Children.value.append(self.__poi)

    self.update_position()

  @field_has_changed(Location)
  def update_position(self):
    if self.__proxy != None and self.__poi != None:
      self.__proxy.Transform.value = avango.gua.make_trans_mat(self.Location.value.x * self.__screen_size.x, 0, self.Location.value.y * self.__screen_size.y) * \
                          avango.gua.make_scale_mat(self.__proxy_sphere_size/2)
      self.__poi.Transform.value = avango.gua.make_trans_mat(self.Location.value.x * self.__screen_size.x, -self.Location.value.y * self.__screen_size.y, self.__proxy_sphere_size*0.51) * \
                          avango.gua.make_rot_mat(90, 1, 0, 0) * \
                          avango.gua.make_scale_mat(self.POISize.value)

  def get_screen_size(self):
    return self.__screen_size


class DynamicSplitScreens(avango.script.Script):

  SplitScreens = []

  GraphIn = avango.gua.SFSceneGraph()
  WindowSize = avango.gua.SFVec2ui()

  GraphOut = avango.gua.SFSceneGraph()
  PipelineOut = avango.gua.SFPipeline()

  def __init__(self):
    self.super(DynamicSplitScreens).__init__()

    self.__screen_size = avango.gua.Vec2(16, 9)
    self.__last_id = 0
    self.__pipelines = []

  def init(self, count, create_pipeline_func):

    self.GraphOut.value = avango.gua.nodes.SceneGraph(Name = "proxy_scenegraph")

    eye = avango.gua.nodes.TransformNode(Name = "eye")
    eye.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, self.__screen_size.length()*2)

    screen = avango.gua.nodes.ScreenNode(
      Name = "screen",
      Width = self.__screen_size.x,
      Height = self.__screen_size.y,
      Transform = avango.gua.make_rot_mat(-90, 1, 0, 0)
    )
    screen.Children.value = [eye]
    self.GraphOut.value.Root.value.Children.value = [screen]

    self.PipelineOut.value = avango.gua.nodes.Pipeline(
      Camera = avango.gua.nodes.Camera(
        LeftEye = "/screen/eye",
        LeftScreen = "/screen",
        SceneGraph = "proxy_scenegraph",
        RenderMask = "!invisible",
        Mode = avango.gua.ProjectionMode.ORTHOGRAPHIC
      ),
      Window = avango.gua.nodes.Window(
        Size = self.WindowSize.value,
        LeftResolution = self.WindowSize.value
      ),
      LeftResolution = self.WindowSize.value,
      EnableFPSDisplay = True,
      AmbientColor = avango.gua.Color(1, 1, 1)
    )


    for i in range(0, count):
      pipe = create_pipeline_func()
      pipe.Enabled.value = False
      pipe.OutputTextureName.value = "splitscreen_pipe" + str(i)
      self.PipelineOut.value.PreRenderPipelines.value.append(pipe)
      self.__pipelines.append(pipe)

    self.PipelineOut.value.OutputTextureName.value = "final_pipe"

  def get_next_id(self):
    return self.__last_id

  def get_closest(self, position):
    current_split_locations = []

    for split in self.SplitScreens:
      current_split_locations.append(split.Location.value)

    closest, distance = modules.voronoi_helpers.get_closest(position, current_split_locations)

    return self.SplitScreens[closest], distance

  def add_split_screen(self, camera, location):

    split_id = self.__last_id
    self.__last_id += 1

    target_pipe = None

    for pipe_id in range(0, len(self.__pipelines)):
      if self.__pipelines[pipe_id].Enabled.value == False:
        target_pipe = self.__pipelines[pipe_id]
        break

    if target_pipe:
      target_pipe.Camera.value = camera

      new_split_screen = SplitScreen(
        Location = location,
        Pipe = target_pipe,
        ID = split_id
      )

      new_split_screen.init(pipe_id, split_id, self.GraphOut.value, self.__screen_size)
      self.SplitScreens.append(new_split_screen)

      target_pipe.Enabled.value = True

      return new_split_screen

    else:
      print("Trying to add too many split screens!")

  def remove_split_screen(self, target_split_Screen):

    for split_screen in self.SplitScreens:
      if split_screen.ID == target_split_Screen.ID:
        split_id = split_screen.ID.value
        split_screen.Pipe.value.Enabled.value = False
        self.GraphOut.value["/screen"].Children.value.remove(self.GraphOut.value["/screen/poi" + str(split_id)])
        self.GraphOut.value["/"].Children.value.remove(self.GraphOut.value["/proxy" + str(split_id)])
        self.SplitScreens.remove(target_split_Screen)
        break
