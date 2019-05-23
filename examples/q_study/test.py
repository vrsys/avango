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
from src.PhotoProjector import PhotoProjector
from src.PhotoProjection import PhotoProjection
from src.MyProjector import MyProjector
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
    # graph.Root.value.Children.value.append(floor)

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

    # QUAD 1
    quad_trans_1_node = avango.gua.nodes.TransformNode(Name='quad_trans_0')
    quad_trans_1_node.Transform.value = avango.gua.make_trans_mat(1.0, 0.0, 0.0)
    graph.Root.value.Children.value.append(quad_trans_1_node)

    dq1 = DynamicQuad(quad_trans_1_node, 0.5, 0.5)
    quad_1_node = dq1.get_node()
    
    projector1 = PhotoProjector()
    projector1.my_constructor(graph.Root.value, quad_1_node, quad_trans_1_node, localized_images)
    projector1.Graph.value = graph
    projector1.Texture.value = atlas_path
    projector1.update_images(1)
    projector1.Transform.value = avango.gua.make_trans_mat(1.0, 0.0, 1.0) 
    
    # QUAD 2
    quad_trans_2_node = avango.gua.nodes.TransformNode(Name='quad_trans_1')
    quad_trans_2_node.Transform.value = avango.gua.make_trans_mat(-1.0, 0.0, 0.0)
    graph.Root.value.Children.value.append(quad_trans_2_node)
    dq2 = DynamicQuad(quad_trans_2_node, 0.5, 0.5)
    quad_2_node = dq2.get_node()
    projector2 = MyProjector()
    projector2.my_constructor('2')
    projector2.set_localized_image_list(localized_images)
    projector2.Graph.value = graph
    projector2.Texture.value = atlas_path
    projector2.Transform.value = avango.gua.make_trans_mat(-1.0, 0.0, 1.0)

    graph.Root.value.Children.value.append(projector2.group_node)
    print('projector pos',projector2.Transform.value.get_translate())
    print('pos',projector2.group_node.WorldTransform.value.get_translate() )
    projector2.set_projection_lense(quad_2_node, quad_trans_2_node)

    quad_2_node.Material.connect_from(projector2.Material)

    # QUAD 3
    quad_trans_3_node = avango.gua.nodes.TransformNode(Name='quad_trans_2')
    quad_trans_3_node.Transform.value = avango.gua.make_trans_mat(-2.0, 0.0, 0.0)
    graph.Root.value.Children.value.append(quad_trans_3_node)
    dq3 = DynamicQuad(quad_trans_3_node, 0.5, 0.5)
    quad_3_node = dq3.get_node()
    projector3 = MyProjector()
    projector3.my_constructor('3')
    projector3.set_localized_image_list(localized_images)
    projector3.Graph.value = graph
    projector3.Texture.value = atlas_path
    projector3.Transform.value = avango.gua.make_trans_mat(-2.0, 0.0,2.0)

    graph.Root.value.Children.value.append(projector3.group_node)
    print('projector pos',projector3.Transform.value.get_translate())
    print('pos',projector3.group_node.WorldTransform.value.get_translate() )
    projector3.set_projection_lense(quad_3_node, quad_trans_3_node)

    quad_3_node.Material.connect_from(projector3.Material)







    # for p in multi_view_explorer.projectors:
    #     print('prj num',p.Number.value)
    #     print('prj real own',p.material)
    #     print('prj real mat',p.material.value)
    #     print('prj mat',p.Material.value)
    #     print('prj sf mat',p.Material)
        
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
    quad_1_node.Material.connect_from(projector1.Material)
    # photo_projection.Button0.connect_from(navigator.Mouse.ButtonLeft)
    # photo_projection.Button1.connect_from(navigator.Mouse.ButtonRight)
    
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
