import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE

import navigator

def start():
    # setup scenegraph
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
    loader = avango.gua.nodes.TriMeshLoader()

    scene = avango.gua.nodes.TransformNode(
      Transform=avango.gua.make_identity_mat(),
      Name="scene"
      )
    navigation = avango.gua.nodes.TransformNode(
      Transform=avango.gua.make_trans_mat(0.0, 0.0, 4.0),
      Name="navigation"
      )
    graph.Root.value.Children.value.extend([scene, navigation])

    monkey1 = loader.create_geometry_from_file(
        "monkey",
        "data/objects/monkey.obj",
        avango.gua.LoaderFlags.NORMALIZE_SCALE)
    scene.Children.value.append(monkey1)

    light = avango.gua.nodes.LightNode(
        Type=avango.gua.LightType.POINT,
        Name="light",
        Color=avango.gua.Color(1.0, 1.0, 1.0),
        Brightness=100.0,
        Transform=(avango.gua.make_trans_mat(1, 1, 5) *
                   avango.gua.make_scale_mat(30, 30, 30))
        )
    scene.Children.value.append(light)

    size = avango.gua.Vec2ui(1024, 768)

    window = avango.gua.nodes.GlfwWindow(
        Size=size,
        LeftResolution=size
        )

    avango.gua.register_window("window", window)

    cam = avango.gua.nodes.CameraNode(
        LeftScreenPath="/navigation/screen",
        SceneGraph="scenegraph",
        Resolution=size,
        OutputWindowName="window",
        Transform=avango.gua.make_trans_mat(0.0, 0.0, 0.0)
        )
    navigation.Children.value.append(cam)

    res_pass = avango.gua.nodes.ResolvePassDescription()
    res_pass.EnableSSAO.value = True
    res_pass.SSAOIntensity.value = 4.0
    res_pass.SSAOFalloff.value = 10.0
    res_pass.SSAORadius.value = 7.0

    #res_pass.EnableScreenSpaceShadow.value = True

    res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1, 0.1, 0.1)
    res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
    res_pass.Exposure.value = 1.0
    res_pass.BackgroundColor.value = avango.gua.Color(0.45, 0.5, 0.6)

    anti_aliasing = avango.gua.nodes.SSAAPassDescription()

    pipeline_description = avango.gua.nodes.PipelineDescription(
        Passes=[
            avango.gua.nodes.DepthMapPassDescription(),
            avango.gua.nodes.TriMeshPassDescription(),
            avango.gua.nodes.LightVisibilityPassDescription(),
            res_pass,
            anti_aliasing,
            ])

    cam.PipelineDescription.value = pipeline_description

    screen = avango.gua.nodes.ScreenNode(
        Name="screen",
        Transform=avango.gua.make_trans_mat(0.0, 0.0, -1.0),
        Width=1,
        Height=0.75,
        )
    navigation.Children.value.append(screen)

    distance_cube_map = avango.gua.nodes.DepthMapNode(
      Name="navigation_depth_cube_map"
      )
    navigation.Children.value.append(distance_cube_map)
    # distance_cube_map2 = avango.gua.nodes.DepthMapNode(
    #   Transform=avango.gua.make_trans_mat(0.0, 0.0, 1.0))
    # navigation.Children.value.append(distance_cube_map2)
    

    navi = navigator.MulitScaleNavigator()
    navi.StartLocation.value = navigation.Transform.value.get_translate()
    navi.OutTransform.connect_from(navigation.Transform)

    navi.RotationSpeed.value = 0.2
    navi.MaxMotionSpeed.value = 0.04
    navi.DepthMapNode.value = distance_cube_map

    navigation.Transform.connect_from(navi.OutTransform)

    #setup viewer
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()


if __name__ == '__main__':
  start()

