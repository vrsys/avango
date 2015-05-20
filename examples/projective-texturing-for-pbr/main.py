import avango
import avango.script
import avango.gua
from examples_common.GuaVE import GuaVE
import examples_common.device
import examples_common.navigator

from projector import XML_Projector

import sys


class Toggler(avango.script.Script):
    EnableOut = avango.SFBool()
    KeyIn = avango.SFBool()

    def __init__(self):
        self.super(Toggler).__init__()
        self.key_in = False

    def evaluate(self):
        if self.KeyIn.value is True and self.key_in is False:
            print("toggle Segmentation")
            self.EnableOut.value = not self.EnableOut.value
        self.key_in = self.KeyIn.value


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
    plodloader.UploadBudget.value = 16
    plodloader.RenderBudget.value = 1024
    plodloader.OutOfCoreBudget.value = 1024 * 6
    
    # filename = "/home/yuqo8702/Desktop/Area-7_Rosa-Camuna_knn.kdn"
    plod_node = plodloader.create_geometry_from_file(
        filename,
        avango.gua.PLODLoaderFlags.DEFAULTS,
    )

    plod_node.Material.value = projector.Material.value

    # setup viewing
    size = avango.gua.Vec2ui(2560, 1440)

    window = avango.gua.nodes.GlfwWindow(
        Size=size,
        LeftResolution=size,
    )

    avango.gua.register_window("window", window)

    screen = avango.gua.nodes.ScreenNode(
        Name="screen",
        Width=0.64,
        Height=0.36,
        Transform=avango.gua.make_trans_mat(0.0, 0.0, -2.0),
    )

    cam = avango.gua.nodes.CameraNode(
        Name="cam",
        LeftScreenPath="/cam/screen",
        SceneGraph="scenegraph",
        Resolution=size,
        OutputWindowName="window",
        Children=[screen],
        Transform=avango.gua.make_trans_mat(
            499.211641143046,
            -1095.8880106695,
            133.737704808047,
        ),
    )

    pipeline_description = avango.gua.nodes.PipelineDescription(
        Passes=[
            avango.gua.nodes.TriMeshPassDescription(),
            avango.gua.nodes.PLODPassDescription(),
            avango.gua.nodes.LightVisibilityPassDescription(),
            avango.gua.nodes.ResolvePassDescription(),
        ])

    cam.PipelineDescription.value = pipeline_description

    graph.Root.value.Children.value = [cam, projector.group_node, plod_node]

    #navigation
    navigator = examples_common.navigator.Navigator()
    navigator.StartLocation.value = cam.Transform.value.get_translate()
    navigator.OutTransform.connect_from(cam.Transform)

    navigator.RotationSpeed.value = 0.1
    navigator.MotionSpeed.value = 0.01
    cam.Transform.connect_from(navigator.OutTransform)

    #toggling
    keyboard = examples_common.device.KeyboardDevice()
    toggler = Toggler()
    toggler.KeyIn.connect_from(keyboard.KeyT)
    projector.Enable.connect_from(toggler.EnableOut)

    #setup viewer
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()


if __name__ == '__main__':
    start(sys.argv[1])
