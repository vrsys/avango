import sys
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

from src.AtlasTile import AtlasTile
from src.StudyScript import StudyScript
from src.DynamicQuad import DynamicQuad
# from src.MultiViewExplorer import MultiViewExplorer
from src.MultiWindowVisualizer import MultiWindowVisualizer
from src.PhotoProjection import PhotoProjection
from src.TrackedLenseProjection import TrackedLenseProjection
from src.PerspectivePicker import PerspectivePicker
from src.picker import Picker
from src.MultiUserViewingSetup import MultiUserViewingSetup
from src.SpheronInput import DualSpheronInput
from src.KeyboardInput import KeyboardInput, KeyboardDevice
from src.GriffinInput import GriffinInput
from src.SpacemouseInput import SpacemouseInput
from src.GriffinRotationNavigation import GriffinRotationNavigation
from src.SpheronNavigation import SpheronNavigation
from src.SteeringNavigation import SteeringNavigation


nettrans = avango.gua.nodes.NetTransform(Name="net",
                                         # specify role, ip, and port
                                         Groupname="AVSERVER|127.0.0.1|7432")
                                         # Groupname="AVSERVER|141.54.147.59|7432")
                                         # Groupname="AVSERVER|141.54.147.60|7432")

def make_node_distributable(node):
    for child in node.Children.value:
        print(child.Name.value)
        make_node_distributable(child)
    nettrans.distribute_object(node)


def make_material_distributable(mat):
    nettrans.distribute_object(mat)

def start(user_id):
    print("USER   ", user_id)
    study_group = 'A' if user_id % 2 == 0 else "B"

    # setup scene graph
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
    graph.Root.value.Children.value.append(nettrans)
    # nettrans.Children.value.append(trans_node)

    # Create basic loaders
    mesh_loader = avango.gua.nodes.TriMeshLoader()
    dt_loader = avango.gua.nodes.DynamicTriangleLoader()
    aux_loader = avango.gua.lod.nodes.Aux()
    
    # load model
    reconstruction = mesh_loader.create_geometry_from_file(
        "reconstruction",
        # "/home/senu8384/Desktop/Kopf/kopf_highres.obj",
        "/home/senu8384/Desktop/master-thesis/data/Terrakottaarmee_Bogenschuetze_T21_G18_01/avango_lod/Bogenschuetze-01.obj",
        avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS
    )    

    # reconstruction_center = avango.gua.Vec3(0.0, -0.2, 0.0) # good value for figure
    reconstruction_center = avango.gua.Vec3(0.0, 0.5, 0.0)
    reconstruction.Transform.value = avango.gua.make_trans_mat(reconstruction_center) * \
            avango.gua.make_rot_mat(90.0,-1,0,0) * \
            avango.gua.make_rot_mat(90.0,0,0,1) * \
            avango.gua.make_scale_mat(0.0014)
            # avango.gua.make_scale_mat(0.00105) # good size for figure

    # update material properties (in whole subtree)
    stack = [(reconstruction)]
    while stack:
        _node = stack.pop()
        
        if _node.has_field("Material") == True:
            _node.Material.value.set_uniform("Metalness", 0.0)
            _node.Material.value.set_uniform("Roughness", 1.0)
            _node.Material.value.set_uniform("Emissivity", 1.0)

        stack.extend(
          [(_child_node) for _child_node in reversed(_node.Children.value)])
    nettrans.Children.value.append(reconstruction)
    # graph.Root.value.Children.value.append(reconstruction)

    # Init Atlas Images
    location_data = []
    atlas_image_locations = []
    try:
        with open('/home/senu8384/Desktop/master-thesis/data/study/positions-a.lst') as f:
            for line in f:
                location_data.append(line.replace('\n', '').replace('[', '').replace(']', ''))

            for item in location_data:
                items = [float(x) for x in item.split(' ')]
                mat = avango.gua.from_list(items)
                atlas_image_locations.append(mat)    
    except FileNotFoundError:
        print('File does not exist.')
    
    view_num = len(location_data)
    atlas_path = '/home/senu8384/Desktop/master-thesis/data/study/trial_a_tex.atlas'
    vt_mat = avango.gua.nodes.Material()
    vt_mat.set_uniform("vt_images", atlas_path)
    vt_mat.EnableVirtualTexturing.value = True

    atlas_tiles = []
    atlas_trans_node = avango.gua.nodes.TransformNode(Name="atlas_trans_node",
                                                      # Transform=avango.gua.make_trans_mat(0.0,-0.5,0.0)
                                                      )
    atlas_tiles_node = dt_loader.create_empty_geometry(
            "AtlasTiles", 
            "atlas_tiles.lob", 
            avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.MAKE_PICKABLE)

    atlas_tiles_node.Material.value = vt_mat
    atlas_tiles_node.Material.value.set_uniform("Metalness", 0.0)
    atlas_tiles_node.Material.value.set_uniform("Emissivity", 1.0)
    atlas_tiles_node.Material.value.set_uniform("Roughness", 1.0)
    atlas_tiles_node.Material.value.set_uniform("vt_images", atlas_path)
    atlas_tiles_node.Material.value.EnableVirtualTexturing.value = True
    
    for quad_id in range(view_num):
    # for quad_id in range(3):
        at = AtlasTile(graph, atlas_tiles_node, quad_id, atlas_image_locations[quad_id], 14, 15)
        atlas_tiles.append(at)
        
    graph.Root.value.Children.value.append(atlas_trans_node)
    atlas_trans_node.Children.value.append(atlas_tiles_node)
    

    multi_view_trans_node = avango.gua.nodes.TransformNode(Name="multi_view_trans_node")
    multi_view_trans_node.Transform.value = avango.gua.make_trans_mat(-4.0,1.0,1.0) *\
                                            avango.gua.make_rot_mat(90.0, 0.0, 1.0, 0.0)
    nettrans.Children.value.append(multi_view_trans_node)                                            

    multi_window_viz = MultiWindowVisualizer()
    multi_window_viz.my_constructor(multi_view_trans_node,
                                   atlas_path, atlas_tiles,
                                   4.07, 2.3)

    perspective_picker = PerspectivePicker()
    perspective_picker.my_constructor()
    perspective_picker.set_localized_image_list(atlas_tiles)
    perspective_picker.set_visualizer(multi_window_viz, 'texture')

    study_script = StudyScript()
    study_script.my_constructor(graph, user_id, "/home/senu8384/Desktop/master-thesis/avango/examples/verification_study/src/study_files/part_1_" + study_group + ".json")

    hostname = subprocess.Popen(["hostname"], stdout=subprocess.PIPE, universal_newlines=True).communicate()[0]
    hostname = hostname.strip("\n")

    vt_backend = avango.gua.VTBackend()
        
    if hostname == "hydra" or hostname == "argos":
        print('Init powerwall setup!')
        viewingSetup = None

        tracked_lense_projection = TrackedLenseProjection()
        tracked_lense_projection.my_constructor()
        tracked_lense_projection.set_localized_image_list(atlas_tiles)
        tracked_lense_projection.Graph.value = graph
        tracked_lense_projection.Texture.value = atlas_path
        graph.Root.value.Children.value.append(tracked_lense_projection.group_node)

        if hostname == "hydra":
            ## DLP wall 4-user setup
            viewingSetup = MultiUserViewingSetup(
                SCENEGRAPH = graph,
                WINDOW_RESOLUTION = avango.gua.Vec2ui(4096*2, 2160),
                SCREEN_DIMENSIONS = avango.gua.Vec2(4.97, 2.77),
                TRACKING_TRANSMITTER_OFFSET = avango.gua.make_trans_mat(0.0,-1.435,2.0),
                DISPLAY_STRING_LIST = [":0.4", ":0.1", ":0.0", ":0.3", ":0.2", ":0.5"], # number of available GPUs (users)                
                LEFT_POSITION = avango.gua.Vec2ui(90, 0),
                LEFT_RESOLUTION = avango.gua.Vec2ui(4096 -90 -95, 2160),
                RIGHT_POSITION = avango.gua.Vec2ui(4096 + 90, 0),
                RIGHT_RESOLUTION = avango.gua.Vec2ui(4096 -90 -95, 2160),
                )
            viewingSetup.init_user(HEADTRACKING_SENSOR_STATION = "tracking-dbl-glasses-G")
            viewingSetup.init_user(HEADTRACKING_SENSOR_STATION = "tracking-dbl-glasses-H")
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
        ''' 
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
        '''

        spacemouse_input = SpacemouseInput(            
            DEVICE_STATION = "device-spacemouse0",
            TRANSLATION_FACTOR = 0.1,
            ROTATION_FACTOR = 0.25,
            #SCALE_FACTOR = 1.0,
            )

        navigation = SteeringNavigation(            
            #REFERENCE_TRACKING_STATION = "tracking-new-spheron",
            #TRANSMITTER_OFFSET = avango.gua.make_trans_mat(0.0,0.045,0.0), # transformation into tracking coordinate system        
            )

        keyboard = KeyboardDevice()

        griffin_input = GriffinInput(DEVICE_STATION = "device-griffin")

        griffin_navigation = GriffinRotationNavigation()

        griffin_navigation.assign_input(griffin_input)

        #navigation.assign_input(spheron_input)
        navigation.assign_input(spacemouse_input)
        if viewingSetup:
            viewingSetup.navigation_node.Transform.connect_from(navigation.get_platform_matrix_field())
            # viewingSetup.navigation_node.Transform.connect_from(griffin_navigation.get_platform_matrix_field())

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
        # dynamic_transform.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 1.0)
        dynamic_quad = DynamicQuad(dynamic_transform, width=0.2, height=0.2)
        dynamic_lense = dynamic_quad.get_node()

        pointer_tracking_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
        #pointer_tracking_sensor.Station.value = "tracking-dbl-pointer-1"
        # pointer_tracking_sensor.Station.value = "tracking-b11-large-wall-pointer-3"
        # pointer_tracking_sensor.TransmitterOffset.value = avango.gua.make_trans_mat(0.0,-1.62, 1.6)
        pointer_tracking_sensor.Station.value = "tracking-dbl-pointer-3"
        pointer_tracking_sensor.TransmitterOffset.value = avango.gua.make_trans_mat(0.0,-1.445,2.0)
        pointer_tracking_sensor.ReceiverOffset.value = avango.gua.make_identity_mat()
        # pointer_tracking_sensor.ReceiverOffset.value = avango.gua.make_rot_mat(-90.0,1,0,0) * avango.gua.make_trans_mat(0.0,0.25,0.0)

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
        tracked_lense_projection.set_projection_lense(dynamic_lense, pointer_node)
        perspective_picker.set_projection_lense(dynamic_lense, dynamic_transform)
        tracked_lense_projection.Button0.connect_from(pointer_button_sensor.Button0)
        tracked_lense_projection.Button1.connect_from(pointer_button_sensor.Button1)

        study_script.set_screen(viewingSetup.screen_node)
        study_script.StudyStateButton.connect_from(keyboard.KeySpace )
        study_script.StudyStateButton.connect_from(griffin_input.get_button0_field() )
        # study_script.IndicatorButton.connect_from(griffin_input.get_button0_field() )
        study_script.NextButton.connect_from(keyboard.KeyN)
        study_script.PrevButton.connect_from(keyboard.KeyM)

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

        photo_projection = PhotoProjection()
        photo_projection.my_constructor()
        photo_projection.set_localized_image_list(atlas_tiles)
        photo_projection.Graph.value = graph
        photo_projection.Texture.value = atlas_path
        graph.Root.value.Children.value.append(photo_projection.group_node)

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
        dynamic_transform.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 2.1)
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

        dynamic_tri_pass = avango.gua.nodes.DynamicTrianglePassDescription()
        lvis_pass = avango.gua.nodes.LightVisibilityPassDescription()
        pipeline_description = avango.gua.nodes.PipelineDescription(
        Passes=[
                avango.gua.nodes.TriMeshPassDescription(),
                # plod_pass,
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

        keyboard = KeyboardDevice()
        study_script.set_screen(screen)

        # setup navigator
        navigator = examples_common.navigator.Navigator()
        navigator.StartLocation.value = camera.Transform.value.get_translate()
        navigator.OutTransform.connect_from(camera.Transform)
        navigator.RotationSpeed.value = 0.2
        navigator.MotionSpeed.value = 0.04

        griffin_input = GriffinInput(DEVICE_STATION = "device-griffin")

        griffin_navigation = GriffinRotationNavigation()

        griffin_navigation.assign_input(griffin_input)

        camera.Transform.connect_from(navigator.OutTransform)
        # camera.Transform.connect_from(griffin_navigation.get_platform_matrix_field())
        photo_projection.Button0.connect_from(navigator.Mouse.ButtonLeft)
        photo_projection.Button1.connect_from(navigator.Mouse.ButtonRight)
        perspective_picker.Button0.connect_from(navigator.Mouse.ButtonLeft)
        study_script.IndicatorButton.connect_from(navigator.Keyboard.KeyI)
        study_script.StudyStateKeyboardButton.connect_from(keyboard.KeySpace)
        study_script.NextButton.connect_from(navigator.Keyboard.KeyN)
        study_script.PrevButton.connect_from(navigator.Keyboard.KeyM)
        # capture_script.K_Key.connect_from(navigator.Keyboard.KeyK)
        # capture_script.C_Key.connect_from(navigator.Keyboard.KeyC)
        # capture_script.V_Key.connect_from(navigator.Keyboard.KeyV)
        # capture_script.M_Key.connect_from(navigator.Keyboard.KeyM)
        # capture_script.N_Key.connect_from(navigator.Keyboard.KeyN)
            
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
    user_id = 0
    if len(sys.argv) > 1:
        user_id = int(sys.argv[1])
    start(user_id)


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

