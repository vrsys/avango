import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE
import sys
import examples_common.navigator

#STEREO_MODE = avango.gua.StereoMode.ANAGLYPH_RED_CYAN
# STEREO_MODE = avango.gua.StereoMode.SEPARATE_WINDOWS
# STEREO_MODE = avango.gua.StereoMode.ANAGLYPH_RED_GREEN
STEREO_MODE = avango.gua.StereoMode.SIDE_BY_SIDE
# STEREO_MODE = avango.gua.StereoMode.CHECKERBOARD
# STEREO_MODE = avango.gua.StereoMode.NVIDIA_3D_VISION
# STEREO_MODE = avango.gua.StereoMode.QUAD_BUFFERED

class TimedRotate(avango.script.Script):
  TimeIn = avango.SFFloat()
  MatrixOut = avango.gua.SFMatrix4()
  LeftWindow = 0
  #RightWindow = 0
  
  def set_left_window(self, left_window):
    self.LeftWindow = left_window
  #def set_right_window(self, right_window):
  #  self.RightWindow = right_window

  @field_has_changed(TimeIn)
  def update(self):
    self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value*20.0, 0.0, 1.0, 0.0)
    
    if self.LeftWindow != 0:# and self.RightWindow != 0: 
      print("RenderingFPSLeftRight: " + str(self.LeftWindow.RenderingFPS.value))

def start(filename):

    # setup scenegraph
    graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")

    loader = avango.gua.nodes.TriMeshLoader()
    monkey = loader.create_geometry_from_file("monkey", "/mnt/data_internal/geometry_data/confidential/Kaisersaal/Ktris_15000/Bam_Kai_o_L_12_ct_1-5.obj", avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS)
    monkey.Transform.value = avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0)
    spointsloader = avango.gua.nodes.SPointsLoader()
    spoints_geode = spointsloader.load("kinect", filename)
    spoints_geode.Transform.value = avango.gua.make_rot_mat(180.0, 0.0, 1.0, 0.0) 
    light = avango.gua.nodes.LightNode(
      Type=avango.gua.LightType.POINT,
      Name = "light",
      Color = avango.gua.Color(1.0, 1.0, 1.0),
      Brightness = 300
    )
    light.Transform.value = avango.gua.make_trans_mat(1, 2, 2) * avango.gua.make_scale_mat(15, 15, 15)

    # setup viewing
    width = 1080 #1024
    height = 1200 #768
    eye_size = avango.gua.Vec2ui(width, height)
    window_size = avango.gua.Vec2ui(width, height)
    left_pos = avango.gua.Vec2ui(0, 0)
    right_pos = avango.gua.Vec2ui(0, 0)

    if STEREO_MODE == avango.gua.StereoMode.SIDE_BY_SIDE:
        right_pos.x = width + 1
        window_size.x *= 2

    window_left = avango.gua.nodes.Window(
        Size = window_size,
        Display = ":0.0",
        #SwapGroup = 1,
        #SwapBarrier = 1,
        LeftPosition = left_pos,
        LeftResolution = eye_size,
        RightPosition = right_pos,
        RightResolution = eye_size,
        StereoMode = STEREO_MODE)

    window_left.EnableVsync.value = False
    avango.gua.register_window("window_left", window_left)

    #window_right = avango.gua.nodes.Window(
    #    Size = window_size,
    #    Display = ":0.0",  # ":0.1",
        #SwapGroup = 1,
        #SwapBarrier = 1,
    #    LeftPosition = left_pos,
    #    LeftResolution = eye_size,
    #    RightPosition = right_pos,
    #    RightResolution = eye_size,
    #    StereoMode = STEREO_MODE)


    #window_right.EnableVsync.value = False
    #avango.gua.register_window("window_right", window_right)

    cam = avango.gua.nodes.CameraNode(
        Name = "cam",
        LeftScreenPath = "/screen",
        RightScreenPath = "/screen",
        SceneGraph = "scenegraph",
        Resolution = eye_size,
        EyeDistance = 0.06,
        EnableStereo = True,
        LeftOutputWindow = "window_left",
        Transform = avango.gua.make_trans_mat(0.0, 0.0, 0.5)
        # NearClip =
    )

    res_pass = avango.gua.nodes.ResolvePassDescription()
    res_pass.EnableSSAO.value = True
    res_pass.SSAOIntensity.value = 4.0
    res_pass.SSAOFalloff.value = 10.0
    res_pass.SSAORadius.value = 7.0

    #res_pass.EnableScreenSpaceShadow.value = True

    res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1, 0.1, 0.1)
    res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
    res_pass.Exposure.value = 1.0
    res_pass.BackgroundColor.value = avango.gua.Color(0.45, 0.5, 0.6)

    anti_aliasing = avango.gua.nodes.SSAAPassDescription()


    pipeline_description = avango.gua.nodes.PipelineDescription(
        Passes=[
            avango.gua.nodes.TriMeshPassDescription(),
            avango.gua.nodes.LightVisibilityPassDescription(),
            avango.gua.nodes.SPointsPassDescription(),
            res_pass,
            anti_aliasing,
        ])
    cam.PipelineDescription.value = pipeline_description




    #cam.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 0.0)
    screen = avango.gua.nodes.ScreenNode(Name = "screen", Width = 0.255, Height = 0.285)
    screen.Transform.value = avango.gua.make_trans_mat(0.0, 1.0, 2.5)
    screen.Children.value = [cam]

    navigator = examples_common.navigator.Navigator()
    navigator.StartLocation.value = screen.Transform.value.get_translate()
    navigator.OutTransform.connect_from(screen.Transform)

    navigator.RotationSpeed.value = 0.12/3.0
    navigator.MotionSpeed.value = 0.07

    screen.Transform.connect_from(navigator.OutTransform)


    graph.Root.value.Children.value = [monkey, spoints_geode, light, screen]
    #graph.Root.value.Children.value = [ spoints_geode, light, screen]
    #setup viewer
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window_left]#, window_right]
    viewer.DesiredFPS.value = 500.0

    monkey_updater = TimedRotate()

    timer = avango.nodes.TimeSensor()
    monkey_updater.TimeIn.connect_from(timer.Time)

    spoints_geode.Transform.connect_from(monkey_updater.MatrixOut)

    monkey_updater.set_left_window(window_left)
    #monkey_updater.set_right_window(window_right)
    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()


if __name__ == '__main__':
  start(sys.argv[1])

