import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
# import avango.gua.lod
import avango.daemon
import examples_common.navigator
from examples_common.GuaVE import GuaVE

import subprocess
import json
# import lib.transformations as t3d_util
import numpy

from src.LocalizedImage import LocalizedImageQuad
from src.DynamicQuad import DynamicQuad
from src.MultiViewExplorer import MultiViewExplorer
from src.MultiWindowVisualizer import MultiWindowVisualizer
from src.ImageExplorer import ImageExplorer
from src.PhotoProjection import PhotoProjection
from src.PerspectivePicker import PerspectivePicker
from src.picker import Picker
from src.MultiUserViewingSetup import MultiUserViewingSetup
from src.SpheronInput import DualSpheronInput
from src.SpheronNavigation import SpheronNavigation

nettrans = avango.gua.nodes.NetTransform(Name="net",
                                         # specify role, ip, and port
                                         Groupname="AVSERVER|127.0.0.1|7432")

def make_node_distributable(node):
    for child in node.Children.value:
        print(child.Name.value)
        make_node_distributable(child)
    nettrans.distribute_object(node)


def make_material_distributable(mat):
    nettrans.distribute_object(mat)

def start():

    interaction_mode = 'both' # 'perspective' #  'multi'
    environment_mode = 'desktop' #  'powerwall'
    
    # setup scene graph
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")

    # Create basic loaders
    mesh_loader = avango.gua.nodes.TriMeshLoader()
    lod_loader = avango.gua.lod.nodes.LodLoader()
    dt_loader = avango.gua.nodes.DynamicTriangleLoader()
    aux_loader = avango.gua.lod.nodes.Aux()

    aux_path = "/home/ephtron/Documents/master-render-files/salem/salem_atlas.aux"
    atlas_path = "/home/ephtron/Documents/master-render-files/salem/salem.atlas"
    # aux_path = "/opt/3d_models/lamure/provenance/salem/salem_atlas.aux"
    # atlas_path = "/opt/3d_models/lamure/provenance/salem/salem.atlas"

    # setup scene
    # add transform node for plod object
    trans_node = avango.gua.nodes.TransformNode(Name="scene_trans")
    trans_node.Transform.value = avango.gua.make_trans_mat(0, 0.0, 0) * \
                                 avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0)
    plod_trans_node = avango.gua.nodes.TransformNode(Name="scene")
    # plod_trans_node.Transform.value = avango.gua.make_trans_mat(0, 0, 0.0)

    graph.Root.value.Children.value.append(trans_node)
    graph.Root.value.Children.value.append(nettrans)
    trans_node.Children.value.append(plod_trans_node)

    # configure lod loader 
    lod_loader.UploadBudget.value = 64
    lod_loader.RenderBudget.value = 1024
    lod_loader.OutOfCoreBudget.value = 2048

    # load salem point cloud
    plod_node = lod_loader.load_lod_pointcloud(
        "/home/ephtron/Documents/master-render-files/salem/salem_02.bvh", avango.gua.LoaderFlags.DEFAULTS)
        # "/opt/3d_models/lamure/provenance/salem/salem_02.bvh", avango.gua.LoaderFlags.DEFAULTS)
    plod_node.Material.value.set_uniform("Emissivity", 1.0)
    # plod_node.ShadowMode.value = 1
    plod_trans_node.Children.value.append(plod_node)

    floor = mesh_loader.create_geometry_from_file("floor",
                                                  "data/objects/plane.obj",
                                                  avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.MAKE_PICKABLE)

    floor.Transform.value = avango.gua.make_trans_mat(0, -0.4, 0) * avango.gua.make_scale_mat(4, 1, 4) 
    graph.Root.value.Children.value.append(floor)

    # Create localized image controller
    view_num = 0
    atlas_tiles_num = 0
    atlas = None
    vt_mat = avango.gua.nodes.Material()
    vt_mat.set_uniform("vt_images", atlas_path)
    vt_mat.EnableVirtualTexturing.value = True

    localized_images = []

    localized_images_node = dt_loader.create_empty_geometry(
            "LozalizedImages", 
            "localized_images.lob", 
            avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.MAKE_PICKABLE)

    localized_images_node.Material.value = vt_mat
    localized_images_node.Material.value.set_uniform("Metalness", 0.0)
    localized_images_node.Material.value.set_uniform("Emissivity", 1.0)
    localized_images_node.Material.value.set_uniform("Roughness", 1.0)
    localized_images_node.Material.value.set_uniform("vt_images", atlas_path)
    localized_images_node.Material.value.EnableVirtualTexturing.value = True
    print('VT MAT', vt_mat)
    # localized_images_node.Material.value.EnableBackfaceCulling.value = False

    aux_loader.load_aux_file(aux_path);
    view_num = aux_loader.get_num_views()
    atlas_tiles_num = aux_loader.get_num_atlas_tiles()
    atlas = aux_loader.get_atlas()
    
    # cam_location_list = []

    # for quad_id in range(5):
    for quad_id in range(view_num):
        # create_localized_quad(quad_id)
        view = aux_loader.get_view(quad_id)
        atlas_tile = aux_loader.get_atlas_tile(quad_id)
        atlas = aux_loader.get_atlas()
        quad = LocalizedImageQuad(graph, localized_images_node, quad_id, view, atlas_tile, atlas)
        localized_images.append(quad)

    trans_node.Children.value.append(localized_images_node)

    # create multi view explorers transform node
    multi_view_trans_node = avango.gua.nodes.TransformNode(Name="multi_view_trans_node")
    multi_view_trans_node.Transform.value = avango.gua.make_trans_mat(-4.0,1.0,1.0) *\
                                            avango.gua.make_rot_mat(90.0, 0.0, 1.0, 0.0)
    nettrans.Children.value.append(multi_view_trans_node)                                            
    # image_explorer = ImageExplorer()
    # image_explorer.my_constructor(graph, multi_view_trans_node,
    #                                atlas_path, localized_images,
    #                                3.0, 1.8)

    multi_window_visualizer = MultiWindowVisualizer()
    multi_window_visualizer.my_constructor(graph, multi_view_trans_node,
                                   atlas_path, localized_images,
                                   3.0, 1.8)
    # multi_view_explorer.my_constructor()

    photo_projection = PhotoProjection()
    photo_projection.my_constructor()
    photo_projection.set_localized_image_list(localized_images)
    photo_projection.Graph.value = graph
    photo_projection.Texture.value = atlas_path

    graph.Root.value.Children.value.append(photo_projection.group_node)

    perspective_picker = PerspectivePicker()
    perspective_picker.my_constructor()
    perspective_picker.set_localized_image_list(localized_images)
    # perspective_picker.set_visualizer(image_explorer)

    # config window size
    width = 1920;
    height = int(width * 9.0 / 16.0)
    size = avango.gua.Vec2ui(width, height)

    # setup Window
    window = avango.gua.nodes.Window(Size=size,
                                 Title="Efficient Exploration",
                                 LeftResolution=size,
                                 RightResolution=size)
    avango.gua.register_window("window", window)

    # setup viewer
    screen = avango.gua.nodes.ScreenNode(Name="screen",
                                     Width=4.8,
                                     Height=2.7)
    screen.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -2.5)

    client_screen = avango.gua.nodes.ScreenNode(Name="clientscreen", Width=4, Height=3)

    camera = avango.gua.nodes.CameraNode(Name="cam",
                                         ViewID=1,
                                         LeftScreenPath="/cam/screen",
                                         RightScreenPath="/cam/screen",
                                         SceneGraph="scenegraph",
                                         Resolution=size,
                                         OutputWindowName="window",
                                         EyeDistance=0.0,
                                         EnableStereo=False,
                                         Children=[screen],
                                         Transform=avango.gua.make_trans_mat(0.0, 0.0, 2.0))
    graph.Root.value.Children.value.append(camera)
    
    dynamic_transform = avango.gua.nodes.TransformNode(Name='dynamic_quad_trans')
    dynamic_transform.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 2.0)
    dynamic_quad = DynamicQuad(dynamic_transform, width=0.2, height=0.2)
    dynamic_lense = dynamic_quad.get_node()

    screen.Children.value.append(dynamic_transform)
    photo_projection.set_projection_lense(dynamic_lense, dynamic_transform)
    perspective_picker.set_projection_lense(dynamic_lense, dynamic_transform)

    res_pass = avango.gua.nodes.ResolvePassDescription()
    res_pass.EnableSSAO.value = False
    res_pass.SSAOIntensity.value = 3.0
    res_pass.SSAOFalloff.value = 20.0
    res_pass.SSAORadius.value = 10.0
    res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1, 0.7, 0.6)
    res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
    res_pass.Exposure.value = 1.0
    # res_pass.BackgroundMode.value = avango.gua.BackgroundMode.CUBEMAP_TEXTURE
    res_pass.BackgroundTexture.value = "awesome_skymap"
    res_pass.BackgroundColor.value = avango.gua.Color(0.40, 0.5, 0.6)
    # res_pass.VignetteColor.value = avango.gua.Vec4(0, 0, 0, 1)

    plod_pass = avango.gua.lod.nodes.PLodPassDescription()
    plod_pass.SurfelRenderMode.value = avango.gua.lod.RenderFlags.HQ_TWO_PASS
    # plod_pass.SurfelRenderMode.value = avango.gua.lod.RenderFlags.LQ_ONE_PASS
    dynamic_tri_pass = avango.gua.nodes.DynamicTrianglePassDescription()
    lvis_pass = avango.gua.nodes.LightVisibilityPassDescription()
    pipeline_description = avango.gua.nodes.PipelineDescription(
    Passes=[
            # avango.gua.nodes.TriMeshPassDescription(),
            plod_pass,
            dynamic_tri_pass,
            lvis_pass,
            res_pass
            # avango.gua.nodes.DebugViewPassDescription()
            ],  
    EnableABuffer=False)  # set transparency

    client_cam = avango.gua.nodes.CameraNode(
        ViewID=2,
        LeftScreenPath="/net/clientscreen",
        # RightScreenPath="/net/client-screen",
        SceneGraph="scenegraph",
        Resolution=avango.gua.Vec2ui(800, 600),
        OutputWindowName="client_window",
        Transform=avango.gua.make_trans_mat(0.0, 0.0, 4.5),
        PipelineDescription=pipeline_description)

    nettrans.Children.value.append(client_screen)
    # nettrans.Children.value.append(multi_view_trans_node)
    client_screen.Children.value.append(client_cam)
    make_node_distributable(client_screen)
    # make_node_distributable(multi_view_trans_node)
    
    nettrans.distribute_object(plod_pass)
    nettrans.distribute_object(dynamic_tri_pass)
    nettrans.distribute_object(lvis_pass)
    nettrans.distribute_object(res_pass)

    for p in pipeline_description.Passes.value:
        nettrans.distribute_object(p)
    nettrans.distribute_object(pipeline_description)

    camera.PipelineDescription.value = pipeline_description

    # create backend for virtual texture and vt update
    vt_backend = avango.gua.VTBackend()
    vt_backend.add_camera(camera)
    vt_backend.add_camera(client_cam)
    vt_backend.start_backend()
    
    # setup navigator
    navigator = examples_common.navigator.Navigator()
    navigator.StartLocation.value = camera.Transform.value.get_translate()
    navigator.OutTransform.connect_from(camera.Transform)
    navigator.RotationSpeed.value = 0.2
    navigator.MotionSpeed.value = 0.04

    camera.Transform.connect_from(navigator.OutTransform)
    photo_projection.Button0.connect_from(navigator.Mouse.ButtonLeft)
    perspective_picker.Button0.connect_from(navigator.Mouse.ButtonLeft)
    photo_projection.Button1.connect_from(navigator.Mouse.ButtonRight)
    
    # setup viewer with scenegraph
    viewer = avango.gua.nodes.Viewer()
    # viewer.DesiredFPS.value = 200
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]
    print_graph(graph.Root.value)

    timer = avango.nodes.TimeSensor()

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()


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


### helper functions ###

## print the subgraph under a given node to the console
def print_graph(root_node):
    stack = [(root_node, 0)]
    while stack:
        node, level = stack.pop()
        print("│   " * level + "├── {0} <{1}>".format(
          node.Name.value, node.__class__.__name__))
        stack.extend(
          [(child, level + 1) for child in reversed(node.Children.value)])

## print all fields of a fieldcontainer to the console
def print_fields(node, print_values = False):
    for i in range(node.get_num_fields()):
        field = node.get_field(i)
        print("→ {0} <{1}>".format(field._get_name(), field.__class__.__name__))
    if print_values:
        print("  with value '{0}'".format(field.value))



if __name__ == '__main__':

    start()


# # add light nodes
# spot_light_1 = avango.gua.nodes.LightNode(Name="spot_light_1",
#                                           Type=avango.gua.LightType.SPOT,
#                                           Color=avango.gua.Color(1.0, 1.0, 1.0),
#                                           EnableShadows=True,
#                                           ShadowMapSize=1024,
#                                           ShadowOffset=0.002,
#                                           ShadowMaxDistance=10,
#                                           Falloff=1.5,
#                                           ShadowNearClippingInSunDirection=0.1,
#                                           ShadowFarClippingInSunDirection=10.0,
#                                           Softness=2,
#                                           Brightness=20)
# spot_light_1.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 0.0) * \
#                                avango.gua.make_rot_mat(-90, 1, 0, 0) * \
#                                avango.gua.make_scale_mat(4)

# # screen.Children.value.append(spot_light_1)
# spot_light_1.Transform.value = camera.Transform.value * avango.gua.make_trans_mat(0.0,0.0,0.7)