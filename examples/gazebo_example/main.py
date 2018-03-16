import avango
import avango.script
import avango.gua
from avango.gua import *
import avango.gua.nrp
from examples_common.GuaVE import GuaVE

import math


def start():
    # setup scenegraph
    graph = nodes.SceneGraph(Name="scenegraph")
    loader = nodes.TriMeshLoader()

    cube = loader.create_geometry_from_file("cube", "data/objects/cube.obj", LoaderFlags.NORMALIZE_SCALE)

    transform_cube = avango.gua.nrp.NRPNode(Transform=make_scale_mat(0.5), Children=[cube])

    light = nodes.LightNode(Type=LightType.POINT, Name="light", Color=Color(1.0, 1.0, 1.0), Brightness=100.0,
                            Transform=(make_trans_mat(1, 1, 5) * make_scale_mat(30, 30, 30)))

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

    cam.PipelineDescription.value = nodes.PipelineDescription(Passes=[nodes.TriMeshPassDescription(),
                                                                      nodes.LightVisibilityPassDescription(),
                                                                      res_pass])

    cam.PipelineDescription.value.EnableABuffer.value = True

    screen = nodes.ScreenNode(Name="screen", Width=2, Height=1.5, Children=[cam])

    graph.Root.value.Children.value = [transform_cube, light, screen]

    viewer = nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    cam.PipelineDescription.value.touch()
    viewer.run()


if __name__ == '__main__':
    start()
