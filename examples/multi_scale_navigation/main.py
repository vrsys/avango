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
  ClosestDistance = avango.SFFloat()
  Near = avango.SFFloat()
  Far = avango.SFFloat()

  @field_has_changed(ClosestDistance)
  def update(self):
    # print(self.ClosestDistance.value)
    if self.ClosestDistance.value == -1.0:
      self.Near.value *= 1.5
    else:  
      if self.ClosestDistance.value < (self.Near.value*3):
        self.Near.value *= 0.6
      if self.ClosestDistance.value > (self.Near.value*10):
        self.Near.value *= 1.5
    
    self.Near.value = min(self.Near.value, 10)
    self.Near.value = max(self.Near.value, 0.0001)
    self.Far.value = self.Near.value * 1000.0


def start(scenename):
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
    graph.Root.value.Children.value.extend([scene, navigation])

    size = avango.gua.Vec2ui(2560, 1440)

    window = avango.gua.nodes.GlfwWindow(
        Size=size,
        LeftResolution=size
        )

    avango.gua.register_window("window", window)

    cam = avango.gua.nodes.CameraNode(
        LeftScreenPath="/navigation/screen",
        SceneGraph="scenegraph",
        Resolution=size,
        OutputWindowName="window",
        Transform=avango.gua.make_trans_mat(0.0, 0.0, 0.0)
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
        Transform=avango.gua.make_trans_mat(0.0, 0.0, -1.0),
        Width=0.8,
        Height=0.45,
        )
    navigation.Children.value.append(screen)

    distance_cube_map = avango.gua.nodes.DepthMapNode(
      Name="navigation_depth_cube_map",
      NearClip=0.0001,
      FarClip=1000.0,
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

    #Clipping update
    clip_updater = ClippingUpdater(
      Near=0.1,
      Far=10.0,
    )
    clip_updater.ClosestDistance.connect_from(navi.ClosestDistance)

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
    logger.Input_MinDistance.connect_from(navi.ClosestDistance)
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
  else:
    print("please pass a scene name as first argument, now loading monkey scene")
    start("monkey")

