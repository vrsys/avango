import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import avango.gua.lod
import examples_common.navigator
from examples_common.GuaVE import GuaVE

## print the subgraph under a given node to the console
def print_graph(root_node):
    stack = [(root_node, 0)]
    while stack:
        node, level = stack.pop()
        print("│   " * level + "├── {0} <{1}>".format(
            node.Name.value, node.__class__.__name__))
        stack.extend(
            [(child, level + 1) for child in reversed(node.Children.value)])


## print all fields of a field container to the console
def print_fields(node, print_values=False):
    for i in range(node.get_num_fields()):
        field = node.get_field(i)
        print("→ {0} <{1}>".format(field._get_name(), field.__class__.__name__))
        if print_values:
            print("  with value '{0}'".format(field.value))

#screen_grab_pass = avango.gua.nodes.ScreenGrabPassDescription()
#screen_grab_pass.setOutputPrefix("/home/tihi6213/Desktop/pic_")

class TimedRotate(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.gua.SFMatrix4()
    frame = 0

    Window = 0

    def set_window(self, window):
        self.Window = window

    def evaluate(self):
        # move the abstract geometry in a pseudo-random manner
        self.MatrixOut.value = avango.gua.make_trans_mat(0, -0.75, -2.0) * \
                               avango.gua.make_rot_mat(self.TimeIn.value * 20.0, 0.0, 1.0, 0.0)
        self.frame = self.frame + 1
        if 0 != self.Window and self.frame % 200 == 0:
            print("RenderingFPS :" + str(self.Window.RenderingFPS.value))
            #screen_grab_pass.grabNext()


def start():
    graph = avango.gua.nodes.SceneGraph(Name="scene")
    mesh_loader = avango.gua.nodes.TriMeshLoader()

    transform = avango.gua.nodes.TransformNode(Name="transform")
    graph.Root.value.Children.value.append(transform)

    # configure lod backend
    lod_loader = avango.gua.lod.nodes.LodLoader()
    lod_loader.OutOfCoreBudget.value = 16384
    lod_loader.RenderBudget.value = 4096
    lod_loader.UploadBudget.value = 16

    vt_mat = avango.gua.create_material(avango.gua.MaterialCapabilities.COLOR_VALUE | avango.gua.MaterialCapabilities.ROUGHNESS_VALUE |
                                              avango.gua.MaterialCapabilities.METALNESS_VALUE | avango.gua.MaterialCapabilities.EMISSIVITY_VALUE)

    vt_mat.set_uniform("Color", avango.gua.Vec4(1.0, 0.0, 0.0, 1.0))
    vt_mat.set_uniform("Emissivity", 0.0)
    vt_mat.set_uniform("Roughness", 0.0)
    vt_mat.set_uniform("Metalness", 1.0)
    vt_mat.set_uniform("vt_test", "/mnt/terabytes_of_textures/output_sensitive_rendering/SchieferTurm/schiefer_coarse/combined_texture.atlas")
    vt_mat.ShowBackFaces = False
    vt_mat.EnableVirtualTexturing.value = True

    mlod_node = lod_loader.load_lod_trimesh("mlod_node_slot", "/mnt/terabytes_of_textures/output_sensitive_rendering/SchieferTurm/schiefer_coarse/Schiefer_Turm_charts_uv.bvh",
                                            vt_mat, avango.gua.lod.LoaderFlags.NORMALIZE_SCALE | avango.gua.lod.LoaderFlags.NORMALIZE_POSITION)

    # if not isinstance(mlod_node, avango.gua.lod.MLodNode):
    #    print(str(type(mlod_node)) + " is not an instance of a M-LOD Node")
    #    return

    mlod_node.ErrorThreshold.Value = 1
    mlod_node.Material.Value = vt_mat

    monkey = mesh_loader.create_geometry_from_file(
        "monkey",
        "data/objects/monkey.obj",
        vt_mat,
        avango.gua.lod.LoaderFlags.NORMALIZE_SCALE | avango.gua.lod.LoaderFlags.NORMALIZE_POSITION
    )

    monkey.Transform.value = avango.gua.make_trans_mat(-1, 0.3, 0) * avango.gua.make_scale_mat(0.3, 0.3, 0.3)

    transform.Children.value.append(mlod_node)
    mlod_node.ShadowMode.value = 1
    transform.Children.value.append(monkey)
    monkey.ShadowMode.value = 1

    spot_light_1 = avango.gua.nodes.LightNode(Name="spot_light_1",
                                              Type=avango.gua.LightType.SPOT,
                                              Color=avango.gua.Color(1.0, 1.0, 1.0),
                                              EnableShadows=True,
                                              ShadowMapSize=1024,
                                              ShadowOffset=0.002,
                                              ShadowMaxDistance=10,
                                              Falloff=1.5,
                                              ShadowNearClippingInSunDirection=0.1,
                                              ShadowFarClippingInSunDirection=10.0,
                                              Softness=2,
                                              Brightness=20)
    spot_light_1.Transform.value = avango.gua.make_trans_mat(0.0, 3.0, 0.0) * avango.gua.make_rot_mat(-90, 1, 0, 0) * avango.gua.make_scale_mat(4)
    graph.Root.value.Children.value.append(spot_light_1)

    point_light1 = avango.gua.nodes.LightNode(
        Type=avango.gua.LightType.POINT,
        Name="point_light1",
        Color=avango.gua.Color(0.2, 1.0, 1.7),
        EnableShadows=True,
        ShadowMapSize=1024,
        ShadowMaxDistance=10,
        ShadowOffset=0.03,
        Falloff=0.5,
        ShadowNearClippingInSunDirection=0.1,
        ShadowFarClippingInSunDirection=10.0,
        Brightness=20)

    point_light1.Transform.value = avango.gua.make_trans_mat(1.5, 1.0, 1.5) * avango.gua.make_scale_mat(4)
    graph.Root.value.Children.value.append(point_light1)

    sun_light = avango.gua.nodes.LightNode(Name="sun_light",
                                           Type=avango.gua.LightType.SUN,
                                           Color=avango.gua.Color(1.0, 1.0, 0.7),
                                           EnableShadows=True,
                                           ShadowMapSize=1024,
                                           ShadowOffset=0.0005,
                                           ShadowCascadedSplits=[0.1, 4, 7, 20],
                                           ShadowMaxDistance=30,
                                           ShadowNearClippingInSunDirection=100,
                                           ShadowFarClippingInSunDirection=100,
                                           Brightness=2
                                           )
    sun_light.Transform.value = avango.gua.make_rot_mat(210, 0, 1, 0) * avango.gua.make_rot_mat(-50.0, 1.0, 0.0, 0.0)
    graph.Root.value.Children.value.append(sun_light)

    width = 3840
    height = int(width * 9.0 / 16.0)
    size = avango.gua.Vec2ui(width, height)

    screen = avango.gua.nodes.ScreenNode(Name="screen",
                                         Width=4.8,
                                         Height=2.7)

    screen.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -2.5)

    camera = avango.gua.nodes.CameraNode(
        Name="cam",
        LeftScreenPath="/cam/screen",
        RightScreenPath="/cam/screen",
        SceneGraph="scene",
        Resolution=size,
        OutputWindowName="window",
        EyeDistance=0.2,
        EnableStereo=False,
        Children=[screen],
        Transform=avango.gua.make_trans_mat(0.0, 0.0, 2.0)
    )

    res_pass = avango.gua.nodes.ResolvePassDescription()
    res_pass.EnableSSAO.value = False
    res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.4, 0.5, 0.4)
    res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
    res_pass.Exposure.value = 1.0
    res_pass.BackgroundMode.value = avango.gua.BackgroundMode.SKYMAP_TEXTURE
    res_pass.BackgroundTexture.value = "data/textures/spheremap.jpg"
    res_pass.VignetteColor.value = avango.gua.Vec4(0, 0, 0, 1)

    mlod_pass = avango.gua.lod.nodes.MLodPassDescription()

    pipeline_description = avango.gua.nodes.PipelineDescription(
        Passes=[
            avango.gua.nodes.TriMeshPassDescription(),
            mlod_pass,
            avango.gua.nodes.LightVisibilityPassDescription(),
            res_pass,
            avango.gua.nodes.SSAAPassDescription(),
            #screen_grab_pass,
            #avango.gua.nodes.DebugViewPassDescription()
        ],
        EnableABuffer=False
    )

    camera.PipelineDescription.value = pipeline_description

    graph.Root.value.Children.value.append(camera)

    window = avango.gua.nodes.Window(
        Size=size,
        Title="shadows",
        LeftResolution=size,
        RightResolution=size,
        EnableVsync=False,
        StereoMode=avango.gua.StereoMode.MONO
    )

    avango.gua.register_window("window", window)

    navigator = examples_common.navigator.Navigator()
    navigator.StartLocation.value = camera.Transform.value.get_translate()
    navigator.OutTransform.connect_from(camera.Transform)

    navigator.RotationSpeed.value = 0.2
    navigator.MotionSpeed.value = 0.04

    camera.Transform.connect_from(navigator.OutTransform)

    vt_backend = avango.gua.VTBackend()
    vt_backend.add_camera(camera)
    vt_backend.start_backend()

    viewer = avango.gua.nodes.Viewer()
    viewer.DesiredFPS.value = 1000
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    timer = avango.nodes.TimeSensor()

    piggy_updater = TimedRotate()
    piggy_updater.set_window(window)
    piggy_updater.TimeIn.connect_from(timer.Time)

    transform.Transform.connect_from(piggy_updater.MatrixOut)

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    print_graph(graph.Root.value)

    viewer.run()


if __name__ == '__main__':
    start()
