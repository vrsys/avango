import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import avango.gua.lod
import examples_common.navigator
from examples_common.GuaVE import GuaVE

from src.localized_image import LocalizedImageController
from src.vtprojector import VTProjector, AutoVTProjector
from src.projector import Projector
from src.picker import Picker


def append_indicator(loader, fallback_mat, pos, parent):
    indicator_trans = avango.gua.nodes.TransformNode(Name="indicator_trans")
    indicator_trans.Transform.value = avango.gua.make_trans_mat(0, -0.00, -0.80)

    # pos_indicator_geo = loader.create_geometry_from_file("indicator", "data/objects/cube.obj",
    #                                                      fallback_mat, avango.gua.LoaderFlags.DEFAULTS)

    # pos_indicator_geo.Transform.value = pos * \
    #                                     avango.gua.make_scale_mat(0.015, 0.015, 0.015)

    dir_indicator_geo = loader.create_geometry_from_file("indicator", "data/objects/cube.obj",
                                                         fallback_mat, avango.gua.LoaderFlags.DEFAULTS)

    dir_indicator_geo.Transform.value = pos * \
                                        avango.gua.make_scale_mat(0.001, 0.001, 1.5)
    # indicator_geo.Transform.value = avango.gua.make_trans_mat(pos[0], pos[1], pos[2]) * \
    #                                 avango.gua.make_scale_mat(0.015, 0.015, 0.015)

    parent.Children.value.append(indicator_trans)
    # indicator_trans.Children.value.append(pos_indicator_geo)
    indicator_trans.Children.value.append(dir_indicator_geo)


def start():
    # setup scene graph
    graph = avango.gua.nodes.SceneGraph(Name="scene")

    # Create basic loaders
    mesh_loader = avango.gua.nodes.TriMeshLoader()
    lod_loader = avango.gua.lod.nodes.LodLoader()

    # setup scene
    transform_node = setup_scene(graph, mesh_loader, lod_loader)

    #### Create app logic below here ####

    # Create localized image controller
    localized_image_controller = LocalizedImageController(graph,
        transform_node, 
        "/home/ephtron/Documents/master-render-files/salem/salem_atlas.aux",
        "/home/ephtron/Documents/master-render-files/salem/salem.atlas")

    projector = localized_image_controller.get_projector()

    #### Create app logic above here ####

    # config window size
    width = 1920;
    height = int(width * 9.0 / 16.0)
    size = avango.gua.Vec2ui(width, height)

    # setup view
    cam, screen = setup_camera(graph, size)

    setup_picker(mesh_loader, cam, graph)

    # add prototyp lense
    dynamic_quad = mesh_loader.create_geometry_from_file("dynamic_quad", "data/objects/plane.obj", avango.gua.LoaderFlags.NORMALIZE_SCALE)
    dynamic_quad.Material.value.set_uniform("Metalness", 0.0)
    dynamic_quad.Material.value.set_uniform("Emissivity", 1.0)
    dynamic_quad.Material.value.set_uniform("Roughness", 1.0)
    dynamic_quad.Material.connect_from(projector.Material)

    dynamic_quad.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 2.1) *\
                                   avango.gua.make_rot_mat(90.0, 1.0, 0.0, 0.0) * \
                                   avango.gua.make_scale_mat(0.1)
    screen.Children.value.append(dynamic_quad)

    # dynamic_quad = dynamic_tri_loader.create_empty_geometry("dynamic_quad", "data/objects/plane.obj")
    # dynamic_quad.push_vertex(-2.0,  2.0, -2.0, 0.0, 0.0, 0.0, 0.1, 0.1, 0.1);
    # dynamic_quad.push_vertex( 2.0, -2.0, -2.0, 0.0, 0.0, 0.0, 0.1, 0.1, 0.2);
    # dynamic_quad.push_vertex( 2.0,  2.0, -2.0, 0.0, 0.0, 0.0, 0.1, 0.2, 0.2);

    # dynamic_quad.Material.value.set_uniform("vt_test", "/home/ephtron/Documents/master-render-files/salem/salem.atlas")
    # dynamic_quad.Material.value.EnableVirtualTexturing.value = True
    # tn.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 1.0)
    
    # graph.Root.value.Children.value.append(tn)
    
    # graph.Root.value.Children.value.append(dynamic_quad)
    
    # setup render passes
    setup_render_passes(cam)

    # setup Window
    win = setup_window(size)

    # setup navigator
    navi = setup_navigator(cam)
    projector.Transform2.connect_from(navi.OutTransform)
    # tn.Transform.connect_from(navi.)
    # vtprojector.Transform.connect_from(navi.OutTransform)

    # setup viewer with scenegraph
    setup_viewer(graph, win)


def setup_scene(graph, mesh_loader, lod_loader):
    # add transform node for plod object
    trans_node = avango.gua.nodes.TransformNode(Name="scene")
    trans_node.Transform.value = avango.gua.make_trans_mat(0, 0.0, 0) * \
                                 avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0)
    plod_trans_node = avango.gua.nodes.TransformNode(Name="scene")
    plod_trans_node.Transform.value = avango.gua.make_trans_mat(0, 0, 0.5)
    graph.Root.value.Children.value.append(trans_node)
    trans_node.Children.value.append(plod_trans_node)

    # configure lod loader 
    lod_loader.UploadBudget.value = 64
    lod_loader.RenderBudget.value = 2048
    lod_loader.OutOfCoreBudget.value = 4096

    # load salem point cloud
    plod_node = lod_loader.load_lod_pointcloud(
        "/home/ephtron/Documents/master-render-files/salem/salem_02.bvh",
        avango.gua.lod.LoaderFlags.NORMALIZE_SCALE |
        avango.gua.lod.LoaderFlags.NORMALIZE_POSITION)

    # plod_node.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 1.0, 1.0, 1.0))
    plod_node.Material.value.set_uniform("Emissivity", 1.0)
    plod_node.ShadowMode.value = 1
    plod_trans_node.Children.value.append(plod_node)

    floor = mesh_loader.create_geometry_from_file("floor",
                                                  "data/objects/plane.obj",
                                                  avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.MAKE_PICKABLE
                                                  )

    floor.Transform.value = avango.gua.make_scale_mat(4, 1, 4) * avango.gua.make_trans_mat(0, -0.4, 0)
    # floor.ShadowMode.value = 1
    graph.Root.value.Children.value.append(floor)

    # add light nodes
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
    spot_light_1.Transform.value = avango.gua.make_trans_mat(0.0, 3.0, 0.0) * \
                                   avango.gua.make_rot_mat(-90, 1, 0, 0) * \
                                   avango.gua.make_scale_mat(4)
    graph.Root.value.Children.value.append(spot_light_1)

    point_light1 = avango.gua.nodes.LightNode(Name="point_light1",
                                              Type=avango.gua.LightType.POINT,
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
    # graph.Root.value.Children.value.append(point_light1)

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
                                           Brightness=2)
    sun_light.Transform.value = avango.gua.make_rot_mat(210, 0, 1, 0) * avango.gua.make_rot_mat(-50.0, 1.0, 0.0, 0.0)
    # graph.Root.value.Children.value.append(sun_light)
    return trans_node


def setup_camera(graph, size):
    screen = avango.gua.nodes.ScreenNode(Name="screen",
                                         Width=4.8,
                                         Height=2.7)
    screen.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -2.5)

    camera = avango.gua.nodes.CameraNode(Name="cam",
                                         LeftScreenPath="/cam/screen",
                                         RightScreenPath="/cam/screen",
                                         SceneGraph="scene",
                                         Resolution=size,
                                         OutputWindowName="window",
                                         EyeDistance=0.2,
                                         EnableStereo=False,
                                         Children=[screen],
                                         Transform=avango.gua.make_trans_mat(0.0, 0.0, 2.0))
    graph.Root.value.Children.value.append(camera)


    return camera, screen

def setup_picker(mesh_loader, camera, graph):
    # setup pick ray
    pick_ray = avango.gua.nodes.RayNode(Name = "pick_ray")
    pick_ray.Transform.value = avango.gua.make_trans_mat(0.0, -0.15, 0.0) * \
                               avango.gua.make_scale_mat(1.0, 1.0, 1.0)

    ray_geom = mesh_loader.create_geometry_from_file(
        "ray_geom",
        "data/objects/cylinder.obj",
        avango.gua.LoaderFlags.DEFAULTS)
  
    ray_geom.Transform.value = avango.gua.make_scale_mat(0.01, 0.01, 10)
    pick_ray.Children.value.append(ray_geom)

    picker = Picker()
    picker.SceneGraph.value = graph
    picker.Ray.value = pick_ray
    camera.Children.value.append(pick_ray)

def setup_render_passes(camera):
    res_pass = avango.gua.nodes.ResolvePassDescription()
    res_pass.EnableSSAO.value = False
    res_pass.SSAOIntensity.value = 3.0
    res_pass.SSAOFalloff.value = 20.0
    res_pass.SSAORadius.value = 10.0
    res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1, 0.1, 0.1)
    res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
    res_pass.Exposure.value = 1.0
    res_pass.BackgroundMode.value = avango.gua.BackgroundMode.CUBEMAP_TEXTURE
    res_pass.BackgroundTexture.value = "awesome_skymap"
    # res_pass.BackgroundColor.value = avango.gua.Color(0.45, 0.5, 0.6)
    res_pass.VignetteColor.value = avango.gua.Vec4(0, 0, 0, 1)

    plod_pass = avango.gua.lod.nodes.PLodPassDescription()
    plod_pass.SurfelRenderMode.value = avango.gua.lod.RenderFlags.HQ_TWO_PASS
    # plod_pass.SurfelRenderMode.value = avango.gua.lod.RenderFlags.LQ_ONE_PASS

    pipeline_description = avango.gua.nodes.PipelineDescription(
        Passes=[avango.gua.nodes.TriMeshPassDescription(),
                plod_pass,
                avango.gua.nodes.DynamicTrianglePassDescription(),
                avango.gua.nodes.DeferredVirtualTexturingPassDescription(),
                avango.gua.nodes.SkyMapPassDescription(OutputTextureName="awesome_skymap"),
                avango.gua.nodes.LightVisibilityPassDescription(),
                res_pass,
                # avango.gua.nodes.DebugViewPassDescription()
                ],
        EnableABuffer=False)

    camera.PipelineDescription.value = pipeline_description

def setup_window(size):
    window = avango.gua.nodes.Window(Size=size,
                                     Title="shadows",
                                     LeftResolution=size,
                                     RightResolution=size,
                                     EnableVsync=False,
                                     StereoMode=avango.gua.StereoMode.MONO
                                     )
    avango.gua.register_window("window", window)

    return window

def setup_navigator(camera):
    navigator = examples_common.navigator.Navigator()
    navigator.StartLocation.value = camera.Transform.value.get_translate()
    navigator.OutTransform.connect_from(camera.Transform)

    navigator.RotationSpeed.value = 0.2
    navigator.MotionSpeed.value = 0.04

    camera.Transform.connect_from(navigator.OutTransform)
    return navigator

def setup_viewer(graph, window):
    viewer = avango.gua.nodes.Viewer()
    # viewer.DesiredFPS.value = 200
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    timer = avango.nodes.TimeSensor()

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()


if __name__ == '__main__':
    start()
