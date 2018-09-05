import avango
import avango.gua
import avango.gua.nrp
import avango.script
from avango.gua import *

import examples_common.navigator
from examples_common.GuaVE import GuaVE


def start():
    # setup scenegraph
    graph = nodes.SceneGraph(Name="scenegraph")

    nrp_root = avango.gua.nrp.NRPNode()
    nrp_root.Transform.value = make_trans_mat(-3.6, -1.05, -6.0)

    nrp_interactive = avango.gua.nrp.NRPInteractiveNode()

    loader = nodes.TriMeshLoader()

    teapot_1 = loader.create_geometry_from_file("teapot_1", "data/objects/teapot.obj", LoaderFlags.NORMALIZE_SCALE)
    teapot_1.Transform.value = make_trans_mat(1., 0., 0.)

    teapot_2 = loader.create_geometry_from_file("teapot_2", "data/objects/teapot.obj", LoaderFlags.NORMALIZE_SCALE)
    teapot_2.Transform.value = make_trans_mat(0., 1., 0.)

    teapot_1.Children.value = [teapot_2]
    nrp_interactive.Children.value = [teapot_1]

    size = Vec2ui(2560, 1440)

    window = nodes.GlfwWindow(Size=size, LeftResolution=size)

    register_window("window", window)

    cam = avango.gua.nrp.NRPCameraNode()

    cam.LeftScreenPath.value ="/screen"
    cam.SceneGraph.value="scenegraph"
    cam.Resolution.value=size
    cam.OutputWindowName.value="window"
    cam.Transform.value=make_trans_mat(0., 0., 2.0)

    res_pass = nodes.ResolvePassDescription()

    res_pass.EnvironmentLightingColor.value = Color(0.1, 0.1, 0.1)
    res_pass.ToneMappingMode.value = ToneMappingMode.UNCHARTED
    res_pass.Exposure.value = 1.0
    res_pass.BackgroundColor.value = Color(0.45, 0.5, 0.6)

    cam.PipelineDescription.value = nodes.PipelineDescription(Passes=[avango.gua.nrp.NRPPassDescription(),
                                                                      nodes.TriMeshPassDescription(),
                                                                      nodes.LightVisibilityPassDescription(),
                                                                      res_pass,
                                                                      nodes.SSAAPassDescription()])

    cam.PipelineDescription.value.EnableABuffer.value = True

    screen = nodes.ScreenNode(Name="screen", Width=2, Height=1.5, Children=[cam, nrp_interactive])

    graph.Root.value.Children.value = [nrp_root, screen]

    navigator = examples_common.navigator.Navigator()
    navigator.StartLocation.value = cam.Transform.value.get_translate()
    navigator.OutTransform.connect_from(cam.Transform)

    navigator.RotationSpeed.value = 0.2
    navigator.MotionSpeed.value = 0.04

    cam.Transform.connect_from(navigator.OutTransform)

    viewer = nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    cam.PipelineDescription.value.touch()
    viewer.run()


if __name__ == '__main__':
    start()
