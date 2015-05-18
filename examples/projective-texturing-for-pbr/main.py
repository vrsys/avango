import avango
import avango.script
import avango.gua
from examples_common.GuaVE import GuaVE
import examples_common.navigator

from projector import XML_Projector

import sys


def start(filename):

    # setup scenegraph
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
    loader = avango.gua.nodes.TriMeshLoader()

    # init Projector
    projector = XML_Projector()
    projector.load_XML("data/orthoimage.d.xml", graph.Root.value)
    projector.Texture.value = "data/textures/segMask.tga"

    # init point cloud
    plodloader = avango.gua.nodes.PLODLoader()
    plodloader.UploadBudget.value = 32
    plodloader.RenderBudget.value = 2048
    plodloader.OutOfCoreBudget.value = 4096
    
    # filename = "/home/yuqo8702/Desktop/Area-7_Rosa-Camuna_knn.kdn"
    plod_node = plodloader.create_geometry_from_file(
        filename,
        avango.gua.PLODLoaderFlags.DEFAULTS,
    )

    plod_node.Material.value = projector.Material.value

    # setup viewing
    size = avango.gua.Vec2ui(1920, 1080)

    window = avango.gua.nodes.GlfwWindow(
        Size=size,
        LeftResolution=size
    )

    avango.gua.register_window("window", window)

    cam = avango.gua.nodes.CameraNode(
        LeftScreenPath="/screen",
        SceneGraph="scenegraph",
        Resolution=size,
        OutputWindowName="window",
        Transform=avango.gua.make_trans_mat(0.0, 0.0, 2.0),
    )

    pipeline_description = avango.gua.nodes.PipelineDescription(
        Passes=[
            avango.gua.nodes.TriMeshPassDescription(),
            avango.gua.nodes.PLODPassDescription(),
            avango.gua.nodes.LightVisibilityPassDescription(),
            avango.gua.nodes.ResolvePassDescription(),
        ])

    cam.PipelineDescription.value = pipeline_description

    screen = avango.gua.nodes.ScreenNode(
        Name="screen",
        Width=0.64,
        Height=0.36,
        Children=[cam],
        Transform=avango.gua.make_trans_mat(
            499.211641143046,
            -1095.8880106695,
            130.737704808047,
        ),
    )

    graph.Root.value.Children.value = [screen, projector.group_node, plod_node]

    #navigation
    navigator = examples_common.navigator.Navigator()
    navigator.StartLocation.value = screen.Transform.value.get_translate()
    navigator.OutTransform.connect_from(screen.Transform)

    navigator.RotationSpeed.value = 0.2
    navigator.MotionSpeed.value = 0.04
    screen.Transform.connect_from(navigator.OutTransform)

    #setup viewer
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()


if __name__ == '__main__':
    start(sys.argv[1])
