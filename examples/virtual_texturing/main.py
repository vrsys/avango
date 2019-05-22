import avango
import avango.script
import avango.gua
from examples_common.GuaVE import GuaVE


class TimedRotate(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.gua.SFMatrix4()

    Window = 0

    def set_window(self, window):
        self.Window = window

    def evaluate(self):
        # move the abstract geometry in a pseudo-random manner
        self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value * 10.0, 1.0, 1.0, 1.0)
        if 0 != self.Window:
            print("RenderingFPS :" + str(self.Window.RenderingFPS.value))


def start():
    # setup scenegraph
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
    loader = avango.gua.nodes.TriMeshLoader()

    earth_geode_1 = loader.create_geometry_from_file(
        "earth_1", "/mnt/terabytes_of_textures/FINAL_DEMO_DATA/earth.obj",
        avango.gua.LoaderFlags.NORMALIZE_SCALE)

    earth_geode_2 = loader.create_geometry_from_file(
        "earth_2", "/mnt/terabytes_of_textures/FINAL_DEMO_DATA/earth.obj",
        avango.gua.LoaderFlags.NORMALIZE_SCALE)

    # apply vt color 1:1
    _stack = [earth_geode_1]
    while _stack:
        _node = _stack.pop()

        if _node.has_field("Material"):
            _node.Material.value.set_uniform("Roughness", 1.0)
            _node.Material.value.set_uniform("vt_test_color", "/mnt/terabytes_of_textures/FINAL_DEMO_DATA/earth_colour_86400x43200_256x256_1_rgb.atlas")
            _node.Material.value.EnableVirtualTexturing.value = True

        _stack.extend([_child_node for _child_node in _node.Children.value])

    earth_geode_2.Material.value.set_uniform("Roughness", 0.5)
    earth_geode_2.Material.value.set_uniform("vt_test_elevation", "/mnt/terabytes_of_textures/FINAL_DEMO_DATA/earth_elevation_43200x21600_256x256_1_rgb.atlas")
    earth_geode_2.Material.value.EnableVirtualTexturing.value = True

    transform1 = avango.gua.nodes.TransformNode(
        Transform=avango.gua.make_trans_mat(-0.5, 0.0, 0.0) * avango.gua.make_scale_mat(0.66),
        Children=[earth_geode_1])

    transform2 = avango.gua.nodes.TransformNode(
        Transform=avango.gua.make_trans_mat(0.5, 0.0, 0.0) * avango.gua.make_scale_mat(0.66),
        Children=[earth_geode_2])

    light = avango.gua.nodes.LightNode(
        Type=avango.gua.LightType.POINT,
        Name="light",
        Color=avango.gua.Color(1.0, 1.0, 1.0),
        Brightness=100.0,
        Transform=(avango.gua.make_trans_mat(1, 1, 5) * avango.gua.make_scale_mat(30, 30, 30)))

    size = avango.gua.Vec2ui(3840, 2160)

    window1 = avango.gua.nodes.GlfwWindow(Size=size, LeftResolution=size)
    window1.EnableVsync.value = False

    avango.gua.register_window("window1", window1)

    cam1 = avango.gua.nodes.CameraNode(
        LeftScreenPath="/screen",
        SceneGraph="scenegraph",
        Resolution=size,
        OutputWindowName="window1",
        Transform=avango.gua.make_trans_mat(0.0, 0.0, 1.0))

    # window2 = avango.gua.nodes.GlfwWindow(Size=size, LeftResolution=size)
    # window2.EnableVsync.value = False
    #
    # avango.gua.register_window("window2", window2)
    #
    # cam2 = avango.gua.nodes.CameraNode(
    #     LeftScreenPath="/screen",
    #     SceneGraph="scenegraph",
    #     Resolution=size,
    #     OutputWindowName="window2",
    #     Transform=avango.gua.make_trans_mat(0.0, 0.0, 3.5))
    #
    # window3 = avango.gua.nodes.GlfwWindow(Size=size, LeftResolution=size)
    # window3.EnableVsync.value = False
    #
    # avango.gua.register_window("window3", window3)
    #
    # cam3 = avango.gua.nodes.CameraNode(
    #     LeftScreenPath="/screen",
    #     SceneGraph="scenegraph",
    #     Resolution=size,
    #     OutputWindowName="window3",
    #     Transform=avango.gua.make_trans_mat(0.0, 0.0, 3.5))
    #
    # window4 = avango.gua.nodes.GlfwWindow(Size=size, LeftResolution=size)
    # window4.EnableVsync.value = False
    #
    # avango.gua.register_window("window4", window4)
    #
    # cam4 = avango.gua.nodes.CameraNode(
    #     LeftScreenPath="/screen",
    #     SceneGraph="scenegraph",
    #     Resolution=size,
    #     OutputWindowName="window4",
    #     Transform=avango.gua.make_trans_mat(0.0, 0.0, 3.5))

    res_pass = avango.gua.nodes.ResolvePassDescription()
    res_pass.EnableSSAO.value = True
    res_pass.SSAOIntensity.value = 4.0
    res_pass.SSAOFalloff.value = 10.0
    res_pass.SSAORadius.value = 7.0

    res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1, 0.1, 0.1)
    res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
    res_pass.Exposure.value = 1.0
    res_pass.BackgroundColor.value = avango.gua.Color(0.45, 0.5, 0.6)

    anti_aliasing = avango.gua.nodes.SSAAPassDescription()

    pipeline_description = avango.gua.nodes.PipelineDescription(Passes=[
        avango.gua.nodes.TriMeshPassDescription(),
        # avango.gua.nodes.LightVisibilityPassDescription(),
        # res_pass,
        # anti_aliasing,
    ])

    cam1.PipelineDescription.value = pipeline_description
    # cam2.PipelineDescription.value = pipeline_description
    # cam3.PipelineDescription.value = pipeline_description
    # cam4.PipelineDescription.value = pipeline_description

    screen = avango.gua.nodes.ScreenNode(Name="screen",
                                         Width=3.840,
                                         Height=2.160,
                                         Children=[cam1])  # , cam2 , cam3, cam4])

    graph.Root.value.Children.value = [transform1, transform2, light, screen]

    vt_backend = avango.gua.VTBackend()
    vt_backend.add_camera(cam1)
    # vt_backend.add_camera(cam2)
    # vt_backend.add_camera(cam3)
    # vt_backend.add_camera(cam4)
    vt_backend.start_backend()

    # setup viewer
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window1]  # , window2, window3, window4]
    viewer.DesiredFPS.value = 1000.0

    monkey_updater = TimedRotate()
    monkey_updater.set_window(window1)

    timer = avango.nodes.TimeSensor()
    monkey_updater.TimeIn.connect_from(timer.Time)

    earth_geode_1.Transform.connect_from(monkey_updater.MatrixOut)
    earth_geode_2.Transform.connect_from(monkey_updater.MatrixOut)

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    # pipeline_description.touch()
    viewer.run()


if __name__ == '__main__':
    start()
