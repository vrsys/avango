import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE
from examples_common.navigator import Navigator

from projector import XML_Projector

def start():

    # setup scenegraph
    graph  = avango.gua.nodes.SceneGraph(Name = "scenegraph")
    loader = avango.gua.nodes.TriMeshLoader()

    projector = XML_Projector()
    projector.load_XML("data/orthoimage.d.xml", graph.Root.value)
    projector.Texture.value = "data/textures/segMask.tga"

    # monkey = loader.create_geometry_from_file("monkey", "data/objects/monkey.obj", avango.gua.LoaderFlags.NORMALIZE_SCALE)
    # monkey.Material.connect_from(projector.Material)
    # monkey.Transform.value = \
    #     avango.gua.make_trans_mat(
    #         499.211641143046,
    #         -1095.8880106695,
    #         130.737704808047,
    #     )*\
    #     avango.gua.make_scale_mat(0.1)

    plodloader = avango.gua.nodes.PLODLoader()
    plodloader.UploadBudget.value = 32
    plodloader.RenderBudget.value = 2048
    plodloader.OutOfCoreBudget.value = 4096
    plod_node = plodloader.create_geometry_from_file(
      "/home/yuqo8702/Desktop/Area-7_Rosa-Camuna_knn.kdn",
      avango.gua.PLODLoaderFlags.DEFAULTS
    )
    plod_node.Material.connect_from(projector.Material)

    light = avango.gua.nodes.LightNode(
        Type=avango.gua.LightType.POINT,
        Name = "light",
        Color = avango.gua.Color(1.0, 1.0, 1.0),
        Brightness = 50.0,
        Transform = avango.gua.make_trans_mat(1, 1, 5) * avango.gua.make_scale_mat(15, 15, 15)
    )

    size = avango.gua.Vec2ui(1024, 768)

    window = avango.gua.nodes.GlfwWindow(
        Size = size,
        LeftResolution = size
    )

    avango.gua.register_window("window", window)

    cam = avango.gua.nodes.CameraNode(
        LeftScreenPath = "/screen",
        SceneGraph = "scenegraph",
        Resolution = size,
        OutputWindowName = "window",
        Transform = avango.gua.make_trans_mat(0.0, 0.0, 2.0),
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
        Name = "screen",
        Width = 0.4,
        Height = 0.3,
        Children = [cam],
        # Transform=avango.gua.make_identity_mat(),
        Transform=avango.gua.make_trans_mat(
            499.211641143046,
            -1095.8880106695,
            130.737704808047,
        ),
    )

    graph.Root.value.Children.value = [light, screen, projector.group_node, plod_node]

    #setup viewer
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    # # projector movement
    # navigator = Navigator()
    # navigator.OutTransform.value = avango.gua.make_identity_mat()
    # navigator.StartLocation.value = avango.gua.Vec3()

    # navigator.RotationSpeed.value = 0.2
    # navigator.MotionSpeed.value = 0.04

    # monkey.Transform.connect_from(navigator.OutTransform)


    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()



if __name__ == '__main__':
    start()

