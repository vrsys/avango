import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import avango.oculus
from examples_common.GuaVE import GuaVE


class TimedRotate(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.gua.SFMatrix4()

    def evaluate(self):
        self.MatrixOut.value = avango.gua.make_rot_mat(
            self.TimeIn.value * 2.0, 0.0, 1.0, 0.0)


def start():
    # setup scenegraph
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
    loader = avango.gua.nodes.TriMeshLoader()

    room = loader.create_geometry_from_file(
        "room",
        "data/objects/re_room/RE2 Briefing room.obj",
        avango.gua.LoaderFlags.LOAD_MATERIALS)

    transform1 = avango.gua.nodes.TransformNode(
        Children=[room]
        )

    light = avango.gua.nodes.LightNode(
        Type=avango.gua.LightType.POINT,
        Name="light",
        Color=avango.gua.Color(1.0, 1.0, 1.0),
        Brightness=100.0,
        Transform=(avango.gua.make_trans_mat(1, 1, 5) *
                   avango.gua.make_scale_mat(30, 30, 30))
        )
    
    window = avango.oculus.nodes.OculusWindow()

    #notice, that the oculus screen transforms and translations are automatically
    #computed by the oculus. do not try to enter them yourself, or you will
    #most likely get a wrong result due to influence of the lenses

    #accessible fields:
    #    SensorOrientation ##head pose (rotation and translation)
    #    Resolution ##window resolution
    #    EyeResolution ##recommended eye resolution (behaves strange so far)
    #    LeftScreenSize  ## size of left screen in meters
    #    RightScreenSize ## size of right screen in meters
    #    LeftScreenTranslation  ## translation of left screen in meters
    #    RightScreenTranslation ## translation of right screen in meters 
    #    EyeDistance ## distance between both eyes in meters. for SDKs < v0.6, this is fixed to 0.064

    # start the window in fullscreen and with the oculus window as primary display in order to fit the
    # window nicely on the HMD
    
    window.Size.value = window.Resolution.value
    window.EnableVsync.value = False
    window.EnableFullscreen.value = False

    avango.gua.register_window("window", window)

    left_screen = avango.gua.nodes.ScreenNode(
        Name="left_screen",
        Width=window.LeftScreenSize.value.x,
        Height=window.LeftScreenSize.value.y,
        Transform=avango.gua.make_trans_mat(
            window.LeftScreenTranslation.value
            )
        )

    right_screen = avango.gua.nodes.ScreenNode(
        Name="right_screen",
        Width=window.RightScreenSize.value.x,
        Height=window.RightScreenSize.value.y,
        Transform=avango.gua.make_trans_mat(
            window.RightScreenTranslation.value
            )
        )

    cam = avango.gua.nodes.CameraNode(
        LeftScreenPath="/nav/cam/left_screen",
        RightScreenPath="/nav/cam/right_screen",
        SceneGraph="scenegraph",
        Resolution=window.Resolution.value,
        OutputWindowName="window",
        EyeDistance=window.EyeDistance.value,
        EnableStereo=True,
        Name="cam",
        Children=[left_screen, right_screen]
        )

    nav = avango.gua.nodes.TransformNode(
        Name="nav",
        Transform=avango.gua.make_trans_mat(0.0, 1.8, 0.0),
        Children=[cam]
        )

    graph.Root.value.Children.value = [transform1, light, nav]

    res_pass = avango.gua.nodes.ResolvePassDescription()
    #res_pass.EnableSSAO.value = True
    #res_pass.SSAOIntensity.value = 4.0
    #res_pass.SSAOFalloff.value = 10.0
    #res_pass.SSAORadius.value = 7.0

    #res_pass.EnableScreenSpaceShadow.value = True

    res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1, 0.1, 0.1)
    res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
    res_pass.Exposure.value = 1.0
    res_pass.BackgroundColor.value = avango.gua.Color(0.45, 0.5, 0.6)
    res_pass.BackgroundMode.value = avango.gua.BackgroundMode.CUBEMAP_TEXTURE

    anti_aliasing = avango.gua.nodes.SSAAPassDescription()

    pipeline_description = avango.gua.nodes.PipelineDescription(
        Passes=[
            avango.gua.nodes.TriMeshPassDescription(),
            avango.gua.nodes.LightVisibilityPassDescription(),
            res_pass,
            anti_aliasing,
            ])

    cam.PipelineDescription.value = pipeline_description

    #setup viewer
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    #monkey_updater = TimedRotate()

    #timer = avango.nodes.TimeSensor()
    #monkey_updater.TimeIn.connect_from(timer.Time)

    cam.Transform.connect_from(window.SensorOrientation)
    #transform1.Transform.connect_from(monkey_updater.MatrixOut)

    #guaVE = GuaVE()
    #guaVE.start(locals(), globals())

    #while True :
    #    viewer.frame()
        
        
    viewer.run()

if __name__ == '__main__':
  start()

