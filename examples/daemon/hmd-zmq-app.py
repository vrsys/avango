import avango
import avango.script
import avango.daemon
import avango.gua
from examples_common.GuaVE import GuaVE



def start():
    # device sensor listening to the daemon values
    device1 = avango.daemon.nodes.DeviceSensor(
        DeviceService=avango.daemon.DeviceService())
    device2 = avango.daemon.nodes.DeviceSensor(
        DeviceService=avango.daemon.DeviceService())
    device3 = avango.daemon.nodes.DeviceSensor(
        DeviceService=avango.daemon.DeviceService())
    device4 = avango.daemon.nodes.DeviceSensor(
        DeviceService=avango.daemon.DeviceService())
    # station name determines which device is used
    device1.Station.value = "vr-device-0" 
    device2.Station.value = "vr-device-1" 
    device3.Station.value = "vr-device-2" 
    device4.Station.value = "vr-device-5" 
    print("Device1 ID = " , device1.Value0)
    print("Device2 ID = " , device2.Value0)
    print("Device3 ID = " , device3.Value0)
    print("Device4 ID = " , device4.Value0)


    # setup scenegraph
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
    loader = avango.gua.nodes.TriMeshLoader()

    sphere1 = loader.create_geometry_from_file(
        "sphere1", "data/objects/sphere.obj",
        avango.gua.LoaderFlags.NORMALIZE_SCALE)

    sphere1.Material.value.set_uniform("Color",
                                      avango.gua.Vec4(1.0, 0.766, 0.336, 1.0))
    sphere1.Material.value.set_uniform("Roughness", 0.3)
    sphere1.Material.value.set_uniform("Metalness", 1.0)

    sphere1.Transform.value = avango.gua.make_scale_mat(0.1)

    cylinder1 = loader.create_geometry_from_file(
        "cylinder1", "data/objects/cylinder.obj",
        avango.gua.LoaderFlags.NORMALIZE_SCALE)

    cylinder1.Material.value.set_uniform("Color",
                                      avango.gua.Vec4(1.0, 0.766, 0.336, 1.0))
    cylinder1.Material.value.set_uniform("Roughness", 0.3)
    cylinder1.Material.value.set_uniform("Metalness", 1.0)

    cylinder1.Transform.value = avango.gua.make_scale_mat(0.1)

    cube2 = loader.create_geometry_from_file(
        "cube2", "data/objects/cylinder.obj",
        avango.gua.LoaderFlags.NORMALIZE_SCALE)

    cube2.Material.value.set_uniform("Color",
                                      avango.gua.Vec4(1.0, 0.766, 0.336, 1.0))
    cube2.Material.value.set_uniform("Roughness", 0.3)
    cube2.Material.value.set_uniform("Metalness", 1.0)

    cube2.Transform.value = avango.gua.make_scale_mat(0.1)

    cube1 = loader.create_geometry_from_file(
        "cube1", "data/objects/cube.obj",
        avango.gua.LoaderFlags.NORMALIZE_SCALE)

    cube1.Material.value.set_uniform("Color",
                                      avango.gua.Vec4(1.0, 0.766, 0.336, 1.0))
    cube1.Material.value.set_uniform("Roughness", 0.3)
    cube1.Material.value.set_uniform("Metalness", 1.0)

    cube1.Transform.value = avango.gua.make_scale_mat(0.1)

    transform_sphere = avango.gua.nodes.TransformNode(Children=[sphere1])
    transform_sphere.Transform.connect_from(device1.Matrix)

    transform_cylinder1 = avango.gua.nodes.TransformNode(Children=[cylinder1])
    transform_cylinder1.Transform.connect_from(device2.Matrix)

    transform_cube2 = avango.gua.nodes.TransformNode(Children=[cube2])
    transform_cube2.Transform.connect_from(device3.Matrix)

    transform_cube = avango.gua.nodes.TransformNode(Children=[cube1])
    transform_cube.Transform.connect_from(device4.Matrix)

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

    graph.Root.value.Children.value = [transform_sphere, transform_cylinder1, transform_cube2, transform_cube, light, screen]

    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()


if __name__ == '__main__':
    start()
