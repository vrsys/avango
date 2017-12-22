import avango
import avango.script
import avango.gua
from examples_common.GuaVE import GuaVE


class TimedVertexPush(avango.script.Script)
    TimeIn = avango.SFFloat()
    
class TimedRotate(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.gua.SFMatrix4()

    def evaluate(self):
        self.MatrixOut.value = avango.gua.make_trans_mat(0.0, -0.3, 1.0) *  avango.gua.make_rot_mat(self.TimeIn.value * 40.0,
                                                       0.0, 1.0, 0.0)


def start():
    # setup scenegraph
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")


    loader = avango.gua.nodes.LineStripLoader()


#    line_strip_geode = loader.create_geometry_from_file(
#        "line_strip_model", "data/objects/spiraled_avatar.lob",
#        avango.gua.LoaderFlags.NORMALIZE_SCALE)

    line_strip_geode = loader.create_empty_geometry(
        "line_strip_model", "data/objects/spiraled_avatar.lob")


    transform1_additional_scale = avango.gua.nodes.TransformNode(Transform=avango.gua.make_scale_mat(0.66),
                                                Children=[line_strip_geode])

    transform1 = avango.gua.nodes.TransformNode(Children=[transform1_additional_scale])


    light = avango.gua.nodes.LightNode(
        Type=avango.gua.LightType.POINT,
        Name="light",
        Color=avango.gua.Color(1.0, 1.0, 1.0),
        Brightness=100.0,
        Transform=(avango.gua.make_trans_mat(1, 1, 5) *
                   avango.gua.make_scale_mat(30, 30, 30)))

    size = avango.gua.Vec2ui(1024, 768)

    window = avango.gua.nodes.GlfwWindow(Size=size, LeftResolution=size)

    avango.gua.register_window("window", window)

    cam = avango.gua.nodes.CameraNode(
        LeftScreenPath="/screen",
        SceneGraph="scenegraph",
        Resolution=size,
        OutputWindowName="window",
        Transform=avango.gua.make_trans_mat(0.0, 0.0, 3.5))

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

    pipeline_description = avango.gua.nodes.PipelineDescription(Passes=[
        avango.gua.nodes.LineStripPassDescription(),
        avango.gua.nodes.LightVisibilityPassDescription(),
        res_pass,
        anti_aliasing,
    ])

    cam.PipelineDescription.value = pipeline_description

    screen = avango.gua.nodes.ScreenNode(Name="screen",
                                         Width=2,
                                         Height=1.5,
                                         Children=[cam])

    graph.Root.value.Children.value = [transform1, light, screen]

    #setup viewer
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    avatar_rotator = TimedRotate()

    timer = avango.nodes.TimeSensor()
    avatar_rotator.TimeIn.connect_from(timer.Time)

    transform1.Transform.connect_from(avatar_rotator.MatrixOut)
    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()


if __name__ == '__main__':
    start()
