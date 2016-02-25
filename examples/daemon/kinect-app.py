import avango
import avango.script
import avango.daemon
import avango.gua
from examples_common.GuaVE import GuaVE


def start():
    # device sensor listening to the daemon values
    device = avango.daemon.nodes.DeviceSensor(
        DeviceService=avango.daemon.DeviceService())
    # station name determines which device is used
    device.Station.value = "kinect-head-0"  # 0 for first xbox controller

    # setup scenegraph
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
    loader = avango.gua.nodes.TriMeshLoader()

    monkey = loader.create_geometry_from_file(
        "monkey", "data/objects/monkey.obj",
        avango.gua.LoaderFlags.NORMALIZE_SCALE)

    monkey.Material.value.set_uniform("Color",
                                      avango.gua.Vec4(1.0, 0.766, 0.336, 1.0))
    monkey.Material.value.set_uniform("Roughness", 0.3)
    monkey.Material.value.set_uniform("Metalness", 1.0)

    transform = avango.gua.nodes.TransformNode(Children=[monkey])
    transform.Transform.connect_from(device.Matrix)

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
    res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1, 0.1, 0.1)
    res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
    res_pass.Exposure.value = 1.0
    res_pass.BackgroundColor.value = avango.gua.Color(0.45, 0.5, 0.6)

    anti_aliasing = avango.gua.nodes.SSAAPassDescription()

    pipeline_description = avango.gua.nodes.PipelineDescription(Passes=[
        avango.gua.nodes.TriMeshPassDescription(),
        avango.gua.nodes.LightVisibilityPassDescription(),
        res_pass,
        anti_aliasing,
    ])

    cam.PipelineDescription.value = pipeline_description

    screen = avango.gua.nodes.ScreenNode(Name="screen",
                                         Width=2,
                                         Height=1.5,
                                         Children=[cam])

    graph.Root.value.Children.value = [transform, light, screen]

    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()


if __name__ == '__main__':
    start()
