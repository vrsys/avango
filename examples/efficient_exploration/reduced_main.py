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
from src.PhotoProjection import PhotoProjection
from src.PerspectivePicker import PerspectivePicker
from src.picker import Picker
from src.MultiUserViewingSetup import MultiUserViewingSetup
from src.SpheronInput import DualSpheronInput
from src.SpheronNavigation import SpheronNavigation

nettrans = avango.gua.nodes.NetTransform(Name="net",
                                         # specify role, ip, and port
                                         Groupname="AVSERVER|127.0.0.1|7432")
                                         # Groupname="AVSERVER|141.54.147.59|7432")

def make_node_distributable(node):
    for child in node.Children.value:
        print(child.Name.value)
        make_node_distributable(child)
    nettrans.distribute_object(node)


def make_material_distributable(mat):
    nettrans.distribute_object(mat)

def start():

    aux_path = "/home/ephtron/Documents/master-render-files/salem/salem_atlas.aux"
    atlas_path = "/home/ephtron/Documents/master-render-files/salem/salem.atlas"
    # aux_path = "/opt/3d_models/lamure/provenance/salem/salem_atlas.aux"
    # atlas_path = "/opt/3d_models/lamure/provenance/salem/salem.atlas"

    # setup scene graph
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
    graph.Root.value.Children.value.append(nettrans)

    trans_node = avango.gua.nodes.TransformNode(Name="scene_trans")
    trans_node.Transform.value = avango.gua.make_trans_mat(0, 0.0, 0) * \
                                 avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0)
    graph.Root.value.Children.value.append(trans_node)
    # nettrans.Children.value.append(trans_node)

    # Create basic loaders
    mesh_loader = avango.gua.nodes.TriMeshLoader()
    lod_loader = avango.gua.lod.nodes.LodLoader()
    lod_loader.UploadBudget.value = 64
    lod_loader.RenderBudget.value = 1024
    lod_loader.OutOfCoreBudget.value = 2048

    dt_loader = avango.gua.nodes.DynamicTriangleLoader()
    aux_loader = avango.gua.lod.nodes.Aux()

    plod_trans_node = avango.gua.nodes.TransformNode(Name="scene")
    trans_node.Children.value.append(plod_trans_node)
    
    # load salem point cloud
    plod_node = lod_loader.load_lod_pointcloud(
        "/home/ephtron/Documents/master-render-files/salem/salem_02.bvh", avango.gua.LoaderFlags.DEFAULTS)
        # "/opt/3d_models/lamure/provenance/salem/salem_02.bvh", avango.gua.LoaderFlags.DEFAULTS)

    plod_node.Material.value.set_uniform("Emissivity", 1.0)
    plod_trans_node.Children.value.append(plod_node)

    floor = mesh_loader.create_geometry_from_file("floor",
                                                  "data/objects/plane.obj",
                                                  avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.MAKE_PICKABLE)

    floor.Transform.value = avango.gua.make_trans_mat(0, -0.4, 0) * avango.gua.make_scale_mat(4, 1, 4) 
    graph.Root.value.Children.value.append(floor)

    # Init localized Images
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
    
    aux_loader.load_aux_file(aux_path);
    view_num = aux_loader.get_num_views()
    atlas_tiles_num = aux_loader.get_num_atlas_tiles()
    atlas = aux_loader.get_atlas()
    
    for quad_id in range(view_num):
    # for quad_id in range(20):
        # create_localized_quad(quad_id)
        view = aux_loader.get_view(quad_id)
        atlas_tile = aux_loader.get_atlas_tile(quad_id)
        atlas = aux_loader.get_atlas()
        quad = LocalizedImageQuad(graph, localized_images_node, quad_id, view, atlas_tile, atlas)
        localized_images.append(quad)
    
    trans_node.Children.value.append(localized_images_node)

    multi_view_trans_node = avango.gua.nodes.TransformNode(Name="multi_view_trans_node")
    multi_view_trans_node.Transform.value = avango.gua.make_trans_mat(-4.0,1.0,1.0) *\
                                            avango.gua.make_rot_mat(90.0, 0.0, 1.0, 0.0)
    nettrans.Children.value.append(multi_view_trans_node)                                            

    multi_window_viz = MultiWindowVisualizer()
    multi_window_viz.my_constructor(multi_view_trans_node,
                                   atlas_path, localized_images,
                                   4.07, 2.3)

    photo_projection = PhotoProjection()
    photo_projection.my_constructor()
    photo_projection.set_localized_image_list(localized_images)
    photo_projection.Graph.value = graph
    photo_projection.Texture.value = atlas_path

    graph.Root.value.Children.value.append(photo_projection.group_node)

    perspective_picker = PerspectivePicker()
    perspective_picker.my_constructor()
    perspective_picker.set_localized_image_list(localized_images)
    perspective_picker.set_visualizer(multi_window_viz, 'texture')

    hostname = subprocess.Popen(["hostname"], stdout=subprocess.PIPE, universal_newlines=True).communicate()[0]
    hostname = hostname.strip("\n")

    print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@", hostname)
    vt_backend = avango.gua.VTBackend()
        
    if hostname == "hydra" or hostname == "argos":
        print('Init powerwall setup!')
        viewingSetup = None
        if hostname == "hydra":
            ## DLP wall 4-user setup
            viewingSetup = MultiUserViewingSetup(
                SCENEGRAPH = graph,
                WINDOW_RESOLUTION = avango.gua.Vec2ui(4096*2, 2160),
                SCREEN_DIMENSIONS = avango.gua.Vec2(4.97, 2.77),
                TRACKING_TRANSMITTER_OFFSET = avango.gua.make_trans_mat(0.0,-1.435,2.0),
                DISPLAY_STRING_LIST = [":0.2", ":0.5", ":0.4", ":0.1", ":0.3", ":0.0"], # number of available GPUs (users)
                LEFT_POSITION = avango.gua.Vec2ui(90, 0),
                LEFT_RESOLUTION = avango.gua.Vec2ui(4096 -90 -95, 2160),
                RIGHT_POSITION = avango.gua.Vec2ui(4096 + 90, 0),
                RIGHT_RESOLUTION = avango.gua.Vec2ui(4096 -90 -95, 2160),
                )
            viewingSetup.init_user(HEADTRACKING_SENSOR_STATION = "tracking-dbl-glasses-G")
            #viewingSetup.init_user(HEADTRACKING_SENSOR_STATION = "tracking-dbl-glasses-H")
            #viewingSetup.init_user(HEADTRACKING_SENSOR_STATION = "tracking-dbl-glasses-I")                

        elif hostname == "argos":
            ## large powerwall 6-user setup
            viewingSetup = MultiUserViewingSetup(
                SCENEGRAPH = graph,
                WINDOW_RESOLUTION = avango.gua.Vec2ui(4096*2, 2160),
                SCREEN_DIMENSIONS = avango.gua.Vec2(4.32, 2.7),
                TRACKING_TRANSMITTER_OFFSET = avango.gua.make_trans_mat(0.0,-1.62, 1.6),
                DISPLAY_STRING_LIST = [":0.1", ":0.5", ":0.4", ":0.3", ":0.2", ":0.0"], # number of available GPUs (users)
                LEFT_POSITION = avango.gua.Vec2ui(305, 0),
                LEFT_RESOLUTION = avango.gua.Vec2ui(4096 - 305 - 325, 2160),
                RIGHT_POSITION = avango.gua.Vec2ui(4096 + 305, 0),
                RIGHT_RESOLUTION = avango.gua.Vec2ui(4096 - 305 - 325, 2160),
                )

            viewingSetup.init_user(HEADTRACKING_SENSOR_STATION = "tracking-b11-large-wall-glasses-A")
            #viewingSetup.init_user(HEADTRACKING_SENSOR_STATION = "tracking-b11-large-wall-glasses-B")
            #viewingSetup.init_user(HEADTRACKING_SENSOR_STATION = "tracking-b11-large-wall-glasses-C")
            #viewingSetup.init_user(HEADTRACKING_SENSOR_STATION = "tracking-b11-large-wall-glasses-D")
            #viewingSetup.init_user(HEADTRACKING_SENSOR_STATION = "tracking-b11-large-wall-glasses-H")
            #viewingSetup.init_user(HEADTRACKING_SENSOR_STATION = "tracking-b11-large-wall-glasses-F")
            
        spheron_input = DualSpheronInput(
            DEVICE_STATION1 = "device-new-spheron-right",
            DEVICE_STATION2 = "device-new-spheron-left",
            TRANSLATION_FACTOR = 0.2,
            ROTATION_FACTOR = 10.0,
            #SCALE_FACTOR = 1.0,
            )

        navigation = SpheronNavigation(
            #INITIAL_MODE = 1, # groundfollowing navigation mode
            REFERENCE_TRACKING_STATION = "tracking-new-spheron",
            TRANSMITTER_OFFSET = avango.gua.make_trans_mat(0.0,0.045,0.0), # transformation into tracking coordinate system        
            )
        navigation.assign_input(spheron_input)
        if viewingSetup:
            viewingSetup.navigation_node.Transform.connect_from(navigation.get_platform_matrix_field())


        nettrans.distribute_object(multi_window_viz.get_material())
        # nettrans.distribute_object(vt_mat)

        # client viewing setup for tiled wall
        client_navigation = avango.gua.nodes.TransformNode(Name = "client_navigation")
        client_navigation.Transform.value = avango.gua.make_trans_mat(0.0,0.0,0.0)
        nettrans.Children.value.append(client_navigation)

        client_screen = avango.gua.nodes.ScreenNode(Name="client_screen", Width=4.07, Height=2.3)
        client_navigation.Children.value.append(client_screen)

        
        
        client_pipeline_description = viewingSetup.user_list[0].pipeline_description # hacky !!!
        client_cam = avango.gua.nodes.CameraNode(
            ViewID=2,
            LeftScreenPath="/net/client_navigation/client_screen",
            SceneGraph="scenegraph",
            Resolution=avango.gua.Vec2ui(7680, 4320),
            OutputWindowName="client_window",
            Transform=avango.gua.make_trans_mat(0.0, 0.0, 3.0), # head position
            PipelineDescription=client_pipeline_description)
        client_navigation.Children.value.append(client_cam)

        make_node_distributable(multi_view_trans_node)
        make_node_distributable(client_navigation) # distribute clien camera setup subtree
        
        for p in client_pipeline_description.Passes.value:
            nettrans.distribute_object(p)
        nettrans.distribute_object(client_pipeline_description)

        dynamic_transform = avango.gua.nodes.TransformNode(Name='dynamic_quad_trans')
        dynamic_transform.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 1.0)
        dynamic_quad = DynamicQuad(dynamic_transform, width=0.2, height=0.2)
        dynamic_lense = dynamic_quad.get_node()

        pointer_tracking_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
        #pointer_tracking_sensor.Station.value = "tracking-dbl-pointer-1"
        pointer_tracking_sensor.Station.value = "tracking-b11-large-wall-pointer-3"
        # pointer_tracking_sensor.TransmitterOffset.value = avango.gua.make_trans_mat(0.0,-1.445,2.0)
        pointer_tracking_sensor.TransmitterOffset.value = avango.gua.make_trans_mat(0.0,-1.62, 1.6)
        pointer_tracking_sensor.ReceiverOffset.value = avango.gua.make_identity_mat()

        pointer_button_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
        #pointer_tracking_sensor.Station.value = "tracking-dbl-pointer-1"
        pointer_button_sensor.Station.value = "device-pointer3"
        
        pointer_node = avango.gua.nodes.TransformNode(Name = "pointer_node")
        pointer_node.Transform.connect_from(pointer_tracking_sensor.Matrix)

        if viewingSetup:
            viewingSetup.navigation_node.Children.value.append(pointer_node)
        # pointer_node.Children.value.append(dynamic_quad)
        pointer_node.Children.value.append(dynamic_lense)

        # # graph.Root.value.Children.value.append(dynamic_lense)
        photo_projection.set_projection_lense(dynamic_lense, pointer_node)
        perspective_picker.set_projection_lense(dynamic_lense, dynamic_transform)
        photo_projection.Button0.connect_from(pointer_button_sensor.Button0)
        photo_projection.Button1.connect_from(pointer_button_sensor.Button2)

        for _user in viewingSetup.user_list:
            vt_backend.add_camera(_user.camera_node)

        vt_backend.add_camera(client_cam)
        vt_backend.start_backend()
        multi_window_viz.distribute = True
        
        print_graph(graph.Root.value)

        ## start application/render loop
        viewingSetup.run(locals(), globals())
        print('Setup Powerwall')
    else:
        print("Desktop setup")
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

        # setup render passes
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

         # client viewing setup for tiled wall
        client_navigation = avango.gua.nodes.TransformNode(Name = "client_navigation")
        client_navigation.Transform.value = avango.gua.make_trans_mat(0.0,0.0,0.0)
        client_navigation.Transform.value = avango.gua.make_trans_mat(-4.0,1.0,1.0) *\
                                            avango.gua.make_rot_mat(90.0, 0.0, 1.0, 0.0)
        nettrans.Children.value.append(client_navigation)

        client_screen = avango.gua.nodes.ScreenNode(Name="client_screen", Width=4.07, Height=2.3)
        client_navigation.Children.value.append(client_screen)

        # nettrans.distribute_object(multi_window_viz.get_material())
        # nettrans.distribute_object(vt_mat)
      
        client_cam = avango.gua.nodes.CameraNode(
            Name = "client_cam",
            ViewID=2,
            LeftScreenPath="/net/client_navigation/client_screen",
            SceneGraph="scenegraph",
            Resolution=avango.gua.Vec2ui(7680, 4320),
            OutputWindowName="client_window",
            Transform=avango.gua.make_trans_mat(0.0, 0.0, 3.0), # head position
            PipelineDescription=pipeline_description)
        client_navigation.Children.value.append(client_cam)
        
        make_node_distributable(multi_view_trans_node)
        make_node_distributable(client_navigation) # distribute clien camera setup subtree

        for p in pipeline_description.Passes.value:
            nettrans.distribute_object(p)
        nettrans.distribute_object(pipeline_description)

        camera.PipelineDescription.value = pipeline_description

        # create backend for virtual texture and vt update
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
        photo_projection.Button1.connect_from(navigator.Mouse.ButtonRight)
        perspective_picker.Button0.connect_from(navigator.Mouse.ButtonLeft)
        
        # setup viewer with scenegraph
        viewer = avango.gua.nodes.Viewer()
        # viewer.DesiredFPS.value = 200
        viewer.SceneGraphs.value = [graph]
        viewer.Windows.value = [window]
        print_graph(graph.Root.value)
        multi_window_viz.distribute = True

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


    # client_cam = avango.gua.nodes.CameraNode(
    #     Name="client_cam",
    #     ViewID=2,
    #     LeftScreenPath="/net/clientscreen",
    #     # RightScreenPath="/net/client-screen",
    #     SceneGraph="scenegraph",
    #     Resolution=avango.gua.Vec2ui(800, 600),
    #     OutputWindowName="client_window",
    #     Transform=avango.gua.make_trans_mat(0.0, 0.0, 4.5),                       
    #     PipelineDescription=pipeline_description)

    # nettrans.Children.value.append(client_screen)
    #nettrans.Children.value.append(multi_view_trans_node)
    # client_screen.Children.value.append(client_cam)
    # make_node_distributable(client_screen)
    #make_node_distributable(multi_view_trans_node)

    # nettrans.distribute_object(plod_pass)
    # nettrans.distribute_object(dynamic_tri_pass)
    # nettrans.distribute_object(lvis_pass)
    # nettrans.distribute_object(res_pass)

    # for p in pipeline_description.Passes.value:
    #     nettrans.distribute_object(p)
    # nettrans.distribute_object(pipeline_description)
