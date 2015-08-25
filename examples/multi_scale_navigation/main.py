#basic
import sys

#avango gua
import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE

#multiscale
import navigator
import scenes
import csv_logger

class ClippingUpdater(avango.script.Script):
  MinDistance = avango.SFFloat()
  Near = avango.SFFloat()
  Far = avango.SFFloat()

  @field_has_changed(MinDistance)
  def update(self):
    # print(self.MinDistance.value)
    if self.MinDistance.value == -1.0:
      self.Near.value *= 1.5
    else:  
      if self.MinDistance.value < (self.Near.value*3):
        self.Near.value *= 0.6
      if self.MinDistance.value > (self.Near.value*10):
        self.Near.value *= 1.5
    
    self.Near.value = min(self.Near.value, 10)
    self.Near.value = max(self.Near.value, 0.0001)
    self.Far.value = self.Near.value * 1000.0


class MarkerUpdater(avango.script.Script):
  DistanceMapNode = avango.gua.SFDepthMapNode()
  OutTransform = avango.gua.SFMatrix4()

  def __init__(self):
    self.super(MarkerUpdater).__init__()
    self.DistanceMapNode.value = None
    self.always_evaluate(True)
  
  def evaluate(self):
    pos = self.DistanceMapNode.value.MinDistanceWorldPosition.value
    distance = self.DistanceMapNode.value.MinDistance.value
    self.OutTransform.value = avango.gua.make_trans_mat(pos) * avango.gua.make_scale_mat(distance/100)


def start(scenename, stereo=False):
    print("loading:", scenename, "Stereo:", stereo) 
    # setup scenegraph
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")

    scene = avango.gua.nodes.TransformNode(
      Transform=avango.gua.make_identity_mat(),
      Name="scene"
      )

    starting_pos = scenes.load_scene(scenename, scene)

    navigation = avango.gua.nodes.TransformNode(
      Transform=avango.gua.make_trans_mat(starting_pos),
      Name="navigation"
      )

    loader = avango.gua.nodes.TriMeshLoader()
    marker = loader.create_geometry_from_file(
        "marker", "data/objects/sphere.obj", avango.gua.LoaderFlags.NORMALIZE_SCALE
    )
    marker.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 0.1, 0.1, 1.0))
    graph.Root.value.Children.value.extend([scene, navigation, marker])

    size = avango.gua.Vec2ui(2560, 1440)

    stereomode = avango.gua.StereoMode.MONO
    if stereo:
      stereomode = avango.gua.StereoMode.ANAGLYPH_RED_CYAN

    window = avango.gua.nodes.GlfwWindow(
      Size = size,
      LeftPosition = avango.gua.Vec2ui(0, 0),
      LeftResolution = size,
      RightPosition = avango.gua.Vec2ui(0, 0),
      RightResolution = size,
      StereoMode = stereomode
    )

    avango.gua.register_window("window", window)

    cam = avango.gua.nodes.CameraNode(
      Name = "cam",
      LeftScreenPath = "/navigation/screen",
      RightScreenPath = "/navigation/screen",
      SceneGraph = "scenegraph",
      Resolution = size,
      EyeDistance = 0.06,
      EnableStereo = stereo,
      OutputWindowName = "window",
      Transform = avango.gua.make_trans_mat(0.0, 0.0, 0.0)
    )
    navigation.Children.value.append(cam)

    res_pass = avango.gua.nodes.ResolvePassDescription()
    res_pass.EnableSSAO.value = False
    res_pass.SSAOIntensity.value = 4.0
    res_pass.SSAOFalloff.value = 10.0
    res_pass.SSAORadius.value = 7.0

    #res_pass.EnableScreenSpaceShadow.value = True

    res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1, 0.1, 0.1)
    res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
    res_pass.Exposure.value = 1.0
    res_pass.BackgroundMode.value = avango.gua.BackgroundMode.CUBEMAP_TEXTURE
    if not stereo:
      res_pass.BackgroundTexture.value = "awesome_skymap"

    anti_aliasing = avango.gua.nodes.SSAAPassDescription()

    pipeline_description = avango.gua.nodes.PipelineDescription(
        Passes=[
            avango.gua.nodes.DepthMapPassDescription(),
            avango.gua.nodes.TriMeshPassDescription(),
            avango.gua.nodes.BBoxPassDescription(),
            avango.gua.nodes.SkyMapPassDescription(
              OutputTextureName="awesome_skymap"
            ),
            avango.gua.nodes.PLODPassDescription(),
            avango.gua.nodes.LightVisibilityPassDescription(),
            res_pass,
            anti_aliasing,
            avango.gua.nodes.TexturedScreenSpaceQuadPassDescription(),
            ])

    cam.PipelineDescription.value = pipeline_description

    screen = avango.gua.nodes.ScreenNode(
        Name="screen",
        Transform=avango.gua.make_trans_mat(0.0, 0.0, -0.45),
        Width=0.444,
        Height=0.25,
        )
    navigation.Children.value.append(screen)

    dcm_transform = avango.gua.make_identity_mat()
    if stereo:
      dcm_transform = avango.gua.make_trans_mat(0.0, 0.0, -0.20)

    distance_cube_map = avango.gua.nodes.DepthMapNode(
      Name="navigation_depth_cube_map",
      NearClip=0.0001,
      FarClip=1000.0,
      Transform=dcm_transform,
    )
    navigation.Children.value.append(distance_cube_map)

    debug_quad = avango.gua.nodes.TexturedScreenSpaceQuadNode(
      Name = "DepthMapDebug",
      Texture = distance_cube_map.TextureName.value,
      Width = int(size.x / 2),
      Height = int(size.x / 12),
      Anchor = avango.gua.Vec2(0.0, -1.0)
      )
    graph.Root.value.Children.value.append(debug_quad)
    
    navi = navigator.MulitScaleNavigator()
    navi.StartLocation.value = navigation.Transform.value.get_translate()
    navi.OutTransform.connect_from(navigation.Transform)

    navi.RotationSpeed.value = 0.2
    navi.MaxMotionSpeed.value = 1000.0
    navi.MaxDistance.value = 1000.0
    navi.DepthMapNode.value = distance_cube_map

    navigation.Transform.connect_from(navi.OutTransform)

    #MarkerUpdate
    marker_updater = MarkerUpdater()
    marker_updater.DistanceMapNode.value = distance_cube_map
    marker.Transform.connect_from(marker_updater.OutTransform)


    #Clipping update
    clip_updater = ClippingUpdater(
      Near=0.1,
      Far=10.0,
    )
    clip_updater.MinDistance.connect_from(navi.MinDistance)

    cam.NearClip.connect_from(clip_updater.Near)
    cam.FarClip.connect_from(clip_updater.Far)
    # distance_cube_map.NearClip.connect_from(clip_updater.Near)
    # distance_cube_map.FarClip.connect_from(clip_updater.Far)

    #setup viewer
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    # def toogle_adaptive_locomotion():

    #logging
    logger = csv_logger.CSV_Logger()
    timer = avango.nodes.TimeSensor()
    logger.Input_Time.connect_from(timer.Time)
    logger.Input_MinDistance.connect_from(navi.MinDistance)
    logger.Input_MotionSpeed.connect_from(navi.CurrentMotionSpeed)
    logger.Input_NearClip.connect_from(clip_updater.Near)
    logger.Input_FarClip.connect_from(clip_updater.Far)
    logger.Input_Window.value = window

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()


if __name__ == '__main__':
  if len(sys.argv) == 2:
    start(sys.argv[1])
  elif len(sys.argv) == 3:
    if sys.argv[2] == "stereo":
      start(sys.argv[1], stereo=  True)
    else:
      start(sys.argv[1])
  else:
    print("please pass a scene name as first argument")
    print("available scenes:")
    print(scenes.get_scene_names())
    print("now loading monkey scene")
    start("monkey")

