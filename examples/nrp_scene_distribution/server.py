import sys
import avango
import avango.gua
import avango.gua.nrp
import avango.gua.lod
import avango.script
from avango.script import field_has_changed
from avango.gua import *
from avango.gua.nrp import *

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


def make_node_distributable(nettrans, node):
    for child in node.Children.value:
        make_node_distributable(nettrans, child)
    nettrans.distribute_object(node)


def start(filename):
    # setup scenegraph
    graph = nodes.SceneGraph(Name="scenegraph")

    nrp_root = nrp_nodes.NRPNode()
    nrp_root.Transform.value = make_trans_mat(-3.6, -1.05, -6.0)
    nrp_root.Name.value = "nrp_root"

    nrp_interactive = nrp_nodes.NRPInteractiveNode()
    nrp_interactive.Name.value = "interactive_transform"

    loader = nodes.TriMeshLoader()

    mat_desc = nodes.MaterialShaderDescription()
    mat_desc.load_from_file("data/materials/SimpleMaterial.gmd")
    avango.gua.register_material_shader(mat_desc, "mat")

    mat = nodes.Material(ShaderName="mat")

    wrapper = nodes.TransformNode(Name="wrapper")
    baseball = loader.create_geometry_from_file(
        "interactive_baseball_1",
        "data/objects/sphere.obj",
        avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.MAKE_PICKABLE
    )
    baseball.Material.value.set_uniform("Color", avango.gua.Vec4(0., 1., 0., 1.0))
    baseball.Material.value.set_uniform("Roughness", 0.6)
    baseball.Material.value.set_uniform("Metalness", 0.1)
    baseball.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -2.0) * avango.gua.make_scale_mat(0.1, 0.1, 0.1)
    wrapper.Children.value.append(baseball)
    nrp_interactive.Children.value.append(wrapper)

    monkey = loader.create_geometry_from_file("monkey", "data/objects/monkey.obj")
    monkey.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 0.266, 0.136, 1.0))
    monkey.Material.value.set_uniform("Roughness", 0.6)
    monkey.Material.value.set_uniform("Metalness", 0.1)
    monkey.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -2.0)

    videoloader = nodes.Video3DLoader()
    video_geode = videoloader.load("kinect", filename)
    video_transform = nodes.TransformNode(Name="video_transform", Children=[video_geode])
    video_transform.Transform.value = avango.gua.make_trans_mat(0.0, -2.0, 0.0)

    lod_loader = avango.gua.lod.nodes.LodLoader()
    lod_loader.UploadBudget.value = 64
    lod_loader.RenderBudget.value = 2048
    lod_loader.OutOfCoreBudget.value = 4096

    lod_node = lod_loader.load_lod_pointcloud(
        "/opt/3d_models/point_based/plod/pig_pr.bvh",
        avango.gua.lod.LoaderFlags.NORMALIZE_SCALE |
        avango.gua.lod.LoaderFlags.NORMALIZE_POSITION)
    lod_node.Material.value.set_uniform("Emissivity", 1.0)
    lod_node.ShadowMode.value = 1
    lod_transform = nodes.TransformNode(Name="lod_transform", Children=[lod_node])
    lod_transform.Transform.value = avango.gua.make_trans_mat(-2.0, 0.0, 0.0)

    size = Vec2ui(2560, 1440)

    window = nodes.GlfwWindow(Title="HBP NRP @ BUW: Distribution Server", Size=size, LeftResolution=size)

    register_window("server_window", window)

    logger = avango.gua.nodes.Logger(EnableWarning=False)

    screen = nodes.ScreenNode(Name="screen", Width=4.8, Height=2.7)
    screen.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -2.5)

    pass_nrp = nrp_nodes.NRPPassDescription()
    pass_tri = nodes.TriMeshPassDescription()
    # pass_txt = nodes.TexturedQuadPassDescription()
    pass_plod = avango.gua.lod.nodes.PLodPassDescription()
    pass_plod.SurfelRenderMode.value = avango.gua.lod.RenderFlags.HQ_TWO_PASS
    pass_v3d = nodes.Video3DPassDescription()
    pass_lit = nodes.LightVisibilityPassDescription()
    pass_res = nodes.ResolvePassDescription()
    # pass_txs = nodes.TexturedScreenSpaceQuadPassDescription()
    pass_ssa = nodes.SSAAPassDescription()

    pass_res.Exposure.value = 1.0

    pipeline_description = nodes.PipelineDescription(Passes=[pass_nrp,
                                                             pass_tri,
                                                             # pass_txt,
                                                             pass_plod,
                                                             pass_v3d,
                                                             pass_lit,
                                                             pass_res,
                                                             # pass_txs,
                                                             pass_ssa])

    pipeline_description.EnableABuffer.value = True
    pipeline_description.touch()

    server_cam = nrp_nodes.NRPCameraNode()
    server_cam.Name.value = "server_cam"
    server_cam.ViewID.value = 1
    server_cam.LeftScreenPath.value = "/net/navigation/screen"
    server_cam.SceneGraph.value = "scenegraph"
    server_cam.Resolution.value = size
    server_cam.OutputWindowName.value = "server_window"
    server_cam.PipelineDescription.value = pipeline_description

    client_cam = nrp_nodes.NRPCameraNode()
    client_cam.Name.value = "client_cam"
    client_cam.ViewID.value = 2
    client_cam.LeftScreenPath.value = "/net/navigation/screen"
    client_cam.SceneGraph.value = "scenegraph"
    client_cam.Resolution.value = size
    client_cam.OutputWindowName.value = "client_window"
    client_cam.PipelineDescription.value = pipeline_description

    navigation = nodes.TransformNode(Name="navigation")
    navigation.Children.value = [screen, server_cam, client_cam]
    navigation.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 3.5)

    ### NET TRANSFORMATION BEGIN

    nettrans = avango.gua.nodes.NetTransform(Name="net", Groupname="AVSERVER|127.0.0.1|7432")

    for p in pipeline_description.Passes.value:
        nettrans.distribute_object(p)
    nettrans.distribute_object(pipeline_description)

    make_node_distributable(nettrans, navigation)
    make_node_distributable(nettrans, video_transform)

    nettrans.distribute_object(mat)
    nettrans.distribute_object(monkey)

    nettrans.Children.value = [navigation, monkey, video_transform]

    ### NET TRANSFORMATION END

    # dummy1 = nodes.TransformNode()
    # dummy2 = nodes.TransformNode()

    # dummy2.Children.value = [nrp_root]
    # dummy1.Children.value = [dummy2]

    graph.Root.value.Children.value = [nettrans, nrp_root, nrp_interactive, lod_transform]

    navigator = examples_common.navigator.Navigator()
    navigator.StartLocation.value = navigation.Transform.value.get_translate()
    navigator.OutTransform.connect_from(navigation.Transform)

    navigator.RotationSpeed.value = 0.2
    navigator.MotionSpeed.value = 0.04

    wrapper.Transform.connect_from(navigator.OutTransform)
    navigation.Transform.connect_from(navigator.OutTransform)

    viewer = nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()


if __name__ == '__main__':
    start(sys.argv[1])
