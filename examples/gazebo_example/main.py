import avango
import avango.gua
import avango.gua.nrp
import avango.script
from avango.gua import *
from examples_common.GuaVE import GuaVE


def start():
    # setup scenegraph
    graph = nodes.SceneGraph(Name="scenegraph")

    nrp_root = avango.gua.nrp.NRPNode()

    size = Vec2ui(1024, 768)

    window = nodes.GlfwWindow(Size=size, LeftResolution=size)

    register_window("window", window)

    cam = nodes.CameraNode(LeftScreenPath="/screen", SceneGraph="scenegraph", Resolution=size,
                           OutputWindowName="window", Transform=make_trans_mat(0.0, 0.0, 3.5))

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

    screen = nodes.ScreenNode(Name="screen", Width=2, Height=1.5, Children=[cam])

    graph.Root.value.Children.value = [nrp_root, screen]

    viewer = nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    cam.PipelineDescription.value.touch()
    viewer.run()


if __name__ == '__main__':
    start()
