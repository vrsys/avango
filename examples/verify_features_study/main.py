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
import numpy

from src.AtlasTile import AtlasTile
from src.StudyScript import StudyScript
from src.DynamicQuad import DynamicQuad
# from src.MultiViewExplorer import MultiViewExplorer
from src.WallVisualizer import WallVisualizer
from src.MultiWindowVisualizer import MultiWindowVisualizer
from src.PhotoProjection import PhotoProjection
from src.TrackedLenseProjection import TrackedLenseProjection
from src.OrigPerspectivePicker import PerspectivePicker
from src.WallPerspectivePicker import WallPerspectivePicker
# from src.PerspectivePicker import PerspectivePicker
from src.picker import Picker
from src.MultiUserViewingSetup import MultiUserViewingSetup
from src.SpheronInput import DualSpheronInput
from src.KeyboardInput import KeyboardInput, KeyboardDevice
from src.GriffinInput import GriffinInput
from src.SpacemouseInput import SpacemouseInput
from src.GriffinRotationNavigation import GriffinRotationNavigation
from src.SpheronNavigation import SpheronNavigation
from src.SteeringNavigation import SteeringNavigation

class Picker(avango.script.Script):
    SceneGraph = avango.gua.SFSceneGraph()
    Ray        = avango.gua.SFRayNode()
    Options    = avango.SFInt()
    BlackList  = []
    WhiteList  = []
    Results    = avango.gua.MFPickResult()

    def __init__(self):
        self.super(Picker).__init__()
        self.always_evaluate(True)

        self.SceneGraph.value = avango.gua.nodes.SceneGraph()
        self.Ray.value  = avango.gua.nodes.RayNode()
        self.Options.value = avango.gua.PickingOptions.GET_TEXTURE_COORDS \
                             | avango.gua.PickingOptions.GET_WORLD_NORMALS \
                             | avango.gua.PickingOptions.INTERPOLATE_NORMALS \
                             | avango.gua.PickingOptions.GET_WORLD_POSITIONS #\
                             # | avango.gua.PickingOptions.PICK_ONLY_FIRST_FACE

    def evaluate(self):
        results = self.SceneGraph.value.ray_test(self.Ray.value,
                                             self.Options.value,
                                             self.BlackList,
                                             self.WhiteList)

        print(len(results.value) )
        if len(results.value) > 0:
            print(results.value[0].Name.value)
            print(results.value[0].Name)
            first_hit = results.value[0]
            print(first_hit.WorldPosition.value)
        self.Results.value = results.value

nettrans = avango.gua.nodes.NetTransform(Name="net",
                                         # specify role, ip, and port
                                         # Groupname="AVSERVER|127.0.0.1|7432")
                                         # Groupname="AVSERVER|141.54.147.59|7432")
                                         Groupname="AVSERVER|141.54.147.60|7432")

def make_node_distributable(node):
    for child in node.Children.value:
        print(child.Name.value)
        make_node_distributable(child)
    nettrans.distribute_object(node)


def make_material_distributable(mat):
    nettrans.distribute_object(mat)



def start(task, part, user_id):
    study_task = task
    study_part = part
    study_user = user_id
    study_group = 'A' if user_id % 2 == 0 else 'B'
    study_start_block = user_id % 3

    study_condition = None
    study_geo = None
    study_geo_version = None

    geometry_object_path = None

    tile_position_path = None
    wall_perspectives_path = None
    
    if study_task == 'points':
        if study_part == 1 or study_part == 4:
            if study_start_block == 0:
                study_geo = 'terra'
                study_geo_version = 1 if study_part == 1 else 2
            elif study_start_block == 1:
                study_geo = 'head'
                study_geo_version = 1 if study_part == 1 else 2
            elif study_start_block == 2:
                study_geo = 'wappen'
                study_geo_version = 1 if study_part == 1 else 2
        elif study_part == 2 or study_part == 5:
            if study_start_block == 0:
                study_geo = 'head'
                study_geo_version = 1 if study_part == 2 else 2
            elif study_start_block == 1:
                study_geo = 'wappen'
                study_geo_version = 1 if study_part == 2 else 2
            elif study_start_block == 2:
                study_geo = 'terra'
                study_geo_version = 1 if study_part == 2 else 2
        elif study_part == 3 or study_part == 6:
            if study_start_block == 0:
                study_geo = 'wappen'
                study_geo_version = 1 if study_part == 3 else 2
            elif study_start_block == 1:
                study_geo = 'terra'
                study_geo_version = 1 if study_part == 3 else 2
            elif study_start_block == 2:
                study_geo = 'head'
                study_geo_version = 1 if study_part == 3 else 2

        if study_geo == 'terra':
            geometry_object_path = "/home/senu8384/Desktop/master-thesis/data/study/geometry/terra-points/Bogenschuetze-01.obj"
            tile_position_path = "/home/senu8384/Desktop/master-thesis/data/study/task-points/tp-terra-version1/image_positions_rot.lst"
            marker_path = "/home/senu8384/Desktop/master-thesis/data/study/points_terra_marker_list_corrected.json"
            wall_perspectives_path = "/home/senu8384/Desktop/master-thesis/avango/examples/verify_features_study/wall_perspectives_terra_task_points_perfect.json"

            if study_geo_version == 1:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-points/tp-terra-version1/tp_terra_version1_tex.atlas"
                
            elif study_geo_version == 2:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-points/tp-terra-version2/tp_terra_version2_tex.atlas"

        elif study_geo == 'head':
            geometry_object_path = "/home/senu8384/Desktop/master-thesis/data/study/geometry/head-points/kopf_midres.obj"
            tile_position_path = "/home/senu8384/Desktop/master-thesis/data/study/task-points/tp-head-version1/image_positions_rot.lst"
            marker_path = "/home/senu8384/Desktop/master-thesis/data/study/points_head_marker_list.json"
            wall_perspectives_path = "/home/senu8384/Desktop/master-thesis/avango/examples/verify_features_study/wall_perspectives_head_task_points.json"

            if study_geo_version == 1:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-points/tp-head-version1/tp_head_version1_tex.atlas"
            elif study_geo_version == 2:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-points/tp-head-version2/tp_head_version2_tex.atlas"

        elif study_geo == 'wappen':
            geometry_object_path = "/home/senu8384/Desktop/master-thesis/data/study/geometry/wappen-points/wappen_midres.obj"
            tile_position_path = "/home/senu8384/Desktop/master-thesis/data/study/task-points/tp-wappen-version1/image_positions_rot.lst"
            marker_path = "/home/senu8384/Desktop/master-thesis/data/study/points_wappen_marker_list.json"
            wall_perspectives_path = "/home/senu8384/Desktop/master-thesis/avango/examples/verify_features_study/wall_perspectives_wappen_task_points.json"
            
            if study_geo_version == 1:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-points/tp-wappen-version1/tp_wappen_version1_tex.atlas"
            elif study_geo_version == 2:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-points/tp-wappen-version2/tp_wappen_version2_tex.atlas"

        if study_group == 'A':
            study_condition = 'lense' if study_part <= 3 else 'wall'
        elif study_group == 'B':
            study_condition = 'wall' if study_part <= 3 else 'lense'

        feature_values_path = "/home/senu8384/Desktop/master-thesis/data/study/" + study_task + "_" + study_geo + "_v" + str(study_geo_version) + "_feature_values.json"

    elif study_task == 'real':
        if study_part == 1 or study_part == 3:
            if study_start_block == 0:
                study_geo = 'head'
                study_geo_version = 1 if study_part == 1 else 2
            elif study_start_block == 1:
                study_geo = 'wappen'
                study_geo_version = 1 if study_part == 1 else 2
        elif study_part == 2 or study_part == 4:
            if study_start_block == 0:
                study_geo = 'wappen'
                study_geo_version = 1 if study_part == 2 else 2
            elif study_start_block == 1:
                study_geo = 'head'
                study_geo_version = 1 if study_part == 2 else 2

        if study_geo == 'head':
            geometry_object_path = "/home/senu8384/Desktop/master-thesis/data/study/geometry/head-real/kopf_midres.obj"
            tile_position_path = "/home/senu8384/Desktop/master-thesis/data/study/task-real/tr-head-version2/image_positions_rot.lst"
            marker_path = "/home/senu8384/Desktop/master-thesis/data/study/real_head_marker_list.json"
            wall_perspectives_path = "/home/senu8384/Desktop/master-thesis/avango/examples/verify_features_study/wall_perspectives_head_task_real.json"

            if study_geo_version == 1:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-real/tr-head-version1/tr_head_version1_tex.atlas"
            elif study_geo_version == 2:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-real/tr-head-version2/tr_head_version2_tex.atlas"

        elif study_geo == 'wappen':
            geometry_object_path = "/home/senu8384/Desktop/master-thesis/data/study/geometry/wappen-real/wappen_midres.obj"
            tile_position_path = "/home/senu8384/Desktop/master-thesis/data/study/task-real/tr-wappen-version1/image_positions_rot.lst"
            marker_path = "/home/senu8384/Desktop/master-thesis/data/study/real_wappen_marker_list.json"
            wall_perspectives_path = "/home/senu8384/Desktop/master-thesis/avango/examples/verify_features_study/wall_perspectives_head_task_real.json"

            if study_geo_version == 1:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-real/tr-wappen-version1/tr_wappen_version1_tex.atlas"
            elif study_geo_version == 2:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-real/tr-wappen-version2/tr_wappen_version2_tex.atlas"

        if study_group == 'A':
            study_condition = 'lense' if study_part <= 2 else 'wall'
        elif study_group == 'B':
            study_condition = 'wall' if study_part <= 2 else 'lense'

        feature_values_path = "/home/senu8384/Desktop/master-thesis/data/study/" + study_task + "_" + study_geo + "_v" + str(study_geo_version) + "_feature_values.json"

    print("Starting study part:", study_part, "Task: ", study_task, "with user:", user_id, "Condition:", study_condition,
          "with geometry", study_geo , study_geo_version, study_group)
    print("atlas path", atlas_path)

    # setup scene graph
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
    graph.Root.value.Children.value.append(nettrans)
    # setup study script
    study_script = StudyScript()
    study_script.my_constructor(graph, user_id, study_task, study_part, study_condition, study_geo, study_geo_version, marker_path, wall_perspectives_path, feature_values_path)

    mesh_loader = avango.gua.nodes.TriMeshLoader()
    dt_loader = avango.gua.nodes.DynamicTriangleLoader()

    # Create basic loaders
    reconstruction = None
    if study_geo == 'terra':
        print('Loading Terra Statue')
        # load study model
        reconstruction = mesh_loader.create_geometry_from_file(
            "reconstruction",
            geometry_object_path,
            avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS | avango.gua.LoaderFlags.MAKE_PICKABLE) 
        reconstruction.Tags.value = ["pick"]   

        reconstruction_center = avango.gua.Vec3(0.0, -0.2, 0.0)
        reconstruction.Transform.value = avango.gua.make_trans_mat(reconstruction_center) * \
                avango.gua.make_rot_mat(90.0,-1,0,0) * \
                avango.gua.make_rot_mat(90.0,0,0,1) * \
                avango.gua.make_scale_mat(0.00105)

    elif study_geo == 'wappen':
        print('Loading Wappen')
        reconstruction = mesh_loader.create_geometry_from_file(
            "reconstruction2",
            geometry_object_path,
            avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS | avango.gua.LoaderFlags.MAKE_PICKABLE
        )    
        reconstruction_center = avango.gua.Vec3(-0.58, -0.25, 3.15)
        reconstruction.Transform.value = avango.gua.make_trans_mat(reconstruction_center) * \
                avango.gua.make_rot_mat(180.0,1,0,0) * \
                avango.gua.make_rot_mat(  8.5,0,1,0) * \
                avango.gua.make_rot_mat(  4.2,0,0,1) * \
                avango.gua.make_scale_mat(0.85)
    elif study_geo == 'head':
        print('Loading Head')
        reconstruction = mesh_loader.create_geometry_from_file(
            "reconstruction3",
            geometry_object_path,
            avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS | avango.gua.LoaderFlags.MAKE_PICKABLE
        )    
        
        reconstruction_center = avango.gua.Vec3(0.651, -1.57, 6.22)
        reconstruction.Transform.value = avango.gua.make_trans_mat(reconstruction_center) * \
                avango.gua.make_rot_mat(196.0,1,0,0) * \
                avango.gua.make_rot_mat( -10.0,0,1,0) * \
                avango.gua.make_rot_mat(  4.8,0,0,1) * \
                avango.gua.make_scale_mat(1.5)

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
    graph.Root.value.Children.value.append(reconstruction)

    # Init Atlas Images
    location_data = []
    atlas_image_locations = []
    try:
        with open(tile_position_path) as f:
            for line in f:
                location_data.append(line.replace('\n', '').replace('[', '').replace(']', ''))

            for item in location_data:
                items = [float(x) for x in item.split(' ')]
                mat = avango.gua.from_list(items)
                atlas_image_locations.append(mat)    
    except FileNotFoundError:
        print('File does not exist.')
    
    view_num = len(location_data)
    vt_mat = avango.gua.nodes.Material()
    print(atlas_path)
    vt_mat.set_uniform("vt_images", atlas_path)
    vt_mat.EnableVirtualTexturing.value = True

    atlas_tiles = []
    atlas_trans_node = avango.gua.nodes.TransformNode(Name="atlas_trans_node")
    atlas_trans_node.Tags.value = ["client"]
    atlas_tiles_node = dt_loader.create_empty_geometry(
            "AtlasTiles", 
            "atlas_tiles.lob", 
            avango.gua.LoaderFlags.DEFAULTS)

    atlas_tiles_node.Material.value = vt_mat
    atlas_tiles_node.Material.value.set_uniform("Metalness", 0.0)
    atlas_tiles_node.Material.value.set_uniform("Emissivity", 1.0)
    atlas_tiles_node.Material.value.set_uniform("Roughness", 1.0)
    atlas_tiles_node.Material.value.set_uniform("vt_images", atlas_path)
    atlas_tiles_node.Material.value.EnableVirtualTexturing.value = True
    
    # create Atlas Tiles
    for quad_id in range(view_num):
        at = AtlasTile(graph, atlas_tiles_node, quad_id, atlas_image_locations[quad_id], 14, 15)
        atlas_tiles.append(at)
        
    graph.Root.value.Children.value.append(atlas_trans_node)
    atlas_trans_node.Children.value.append(atlas_tiles_node)

    multi_view_trans_node = avango.gua.nodes.TransformNode(Name="multi_view_trans_node")
    multi_view_trans_node.Transform.value = avango.gua.make_trans_mat(-4.0,1.0,1.0) *\
                                            avango.gua.make_rot_mat(90.0, 0.0, 1.0, 0.0)
    multi_view_trans_node.Tags.value = ['client']                                            
    nettrans.Children.value.append(multi_view_trans_node)

    multi_window_viz = MultiWindowVisualizer()
    multi_window_viz.my_constructor(multi_view_trans_node,
                                   atlas_path, atlas_tiles,
                                   4.07, 2.3, rows=2, columns=2)

    # perspective_picker = WallPerspectivePicker()
    # perspective_picker.my_constructor()
    # perspective_picker.set_localized_image_list(atlas_tiles)
    # perspective_picker.set_visualizer(multi_window_viz, 'texture')

    hostname = subprocess.Popen(["hostname"], stdout=subprocess.PIPE, universal_newlines=True).communicate()[0]
    hostname = hostname.strip("\n")

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
                DISPLAY_STRING_LIST = [":0.4", ":0.1", ":0.0", ":0.3", ":0.2", ":0.5"], # number of available GPUs (users)                
                LEFT_POSITION = avango.gua.Vec2ui(90, 0),
                LEFT_RESOLUTION = avango.gua.Vec2ui(4096 -90 -95, 2160),
                RIGHT_POSITION = avango.gua.Vec2ui(4096 + 90, 0),
                RIGHT_RESOLUTION = avango.gua.Vec2ui(4096 -90 -95, 2160),
                )
            viewingSetup.init_user(HEADTRACKING_SENSOR_STATION = "tracking-dbl-glasses-G")
            viewingSetup.init_user(HEADTRACKING_SENSOR_STATION = "tracking-dbl-glasses-H")
            # viewingSetup.init_user(HEADTRACKING_SENSOR_STATION = "tracking-dbl-glasses-I")

        # setup navigation and input devices
        keyboard = KeyboardDevice()

        griffin_input = GriffinInput(DEVICE_STATION = "device-griffin")
        griffin_navigation = GriffinRotationNavigation()
        griffin_navigation.assign_input(griffin_input)
        if viewingSetup:
            viewingSetup.navigation_node.Transform.connect_from(griffin_navigation.get_platform_matrix_field())

        pointer_tracking_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())

        pointer_tracking_sensor.Station.value = "tracking-dbl-pointer-3"
        pointer_tracking_sensor.TransmitterOffset.value = avango.gua.make_trans_mat(0.0,-1.445,2.0)
        pointer_tracking_sensor.ReceiverOffset.value = avango.gua.make_identity_mat()

        pointer_button_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
        #pointer_tracking_sensor.Station.value = "tracking-dbl-pointer-1"
        pointer_button_sensor.Station.value = "device-pointer3"

        pointer_node = avango.gua.nodes.TransformNode(Name = "pointer_node")
        pointer_node.Transform.connect_from(pointer_tracking_sensor.Matrix)
        if viewingSetup:
            viewingSetup.navigation_node.Children.value.append(pointer_node)

        perspective_picker = None
        tracked_lense_projection = None
        if study_condition == 'lense':
            tracked_lense_projection = TrackedLenseProjection()
            tracked_lense_projection.my_constructor(graph)
            tracked_lense_projection.set_localized_image_list(atlas_tiles)
            tracked_lense_projection.Graph.value = graph
            tracked_lense_projection.Texture.value = atlas_path
            graph.Root.value.Children.value.append(tracked_lense_projection.group_node)
            # perspective_picker = PerspectivePicker()
            # perspective_picker.my_constructor()
            # perspective_picker.set_localized_image_list(atlas_tiles)
            # perspective_picker.set_visualizer(multi_window_viz, 'texture')

            dynamic_transform = avango.gua.nodes.TransformNode(Name='dynamic_quad_trans')
            dynamic_transform.Transform.value = avango.gua.make_rot_mat(90.0,0,1,0) * avango.gua.make_rot_mat(-90.0,1,0,0) *\
                                                avango.gua.make_trans_mat(-0.15, 0.23, 0.0)
            dynamic_quad = DynamicQuad(dynamic_transform, width=0.2, height=0.2)
            dynamic_lense = dynamic_quad.get_node()

            # pointer_node.Children.value.append(dynamic_lense)
            pointer_node.Children.value.append(dynamic_transform)
            pointer_node.Children.value.append(tracked_lense_projection.marker_transform)
            # perspective_picker.set_projection_lense(dynamic_lense, dynamic_transform, dynamic_transform)
            # perspective_picker.Button0.connect_from(pointer_button_sensor.Button0)

        elif study_condition == 'wall':
            print('condition wall!!!!!!!!!')
            # picker = setup_picker(mesh_loader, pointer_node, graph)
            perspective_picker = WallPerspectivePicker()
            perspective_picker.my_constructor(pointer_node)
            # perspective_picker.PickedNodes.connect_from(picker.Results)
            perspective_picker.set_localized_image_list(atlas_tiles)
            perspective_picker.set_visualizer(multi_window_viz, 'texture')
            perspective_picker.SceneGraph.value = graph
            perspective_picker.Button0.connect_from(pointer_button_sensor.Button0)

        # client viewing setup for tiled wall
        client_navigation = avango.gua.nodes.TransformNode(Name = "client_navigation")
        client_navigation.Transform.value = avango.gua.make_trans_mat(0.0,0.0,0.0)
        client_navigation.Transform.value = avango.gua.make_trans_mat(-4.0,1.0,1.0) *\
                                            avango.gua.make_rot_mat(90.0, 0.0, 1.0, 0.0)
        nettrans.Children.value.append(client_navigation)
        client_screen = avango.gua.nodes.ScreenNode(Name="client_screen", Width=4.07, Height=2.3)
        client_navigation.Children.value.append(client_screen)


        # client_resolve_pass.BackgroundColor.value = avango.gua.Color(0.0, 0.0, 0.0)

        client_pipeline_description = viewingSetup.user_list[0].pipeline_description # hacky !!!


        client_cam = avango.gua.nodes.CameraNode(
            Name = "client_cam",
            ViewID=2,
            LeftScreenPath="/net/client_navigation/client_screen",
            SceneGraph="scenegraph",
            Resolution=avango.gua.Vec2ui(7680, 4320),
            OutputWindowName="client_window",
            Transform=avango.gua.make_trans_mat(0.0, 0.0, 3.0),
            PipelineDescription = client_pipeline_description)
       
        client_navigation.Children.value.append(client_cam)

        make_node_distributable(multi_view_trans_node)
        make_node_distributable(client_navigation) # distribute clien camera setup subtree
        
        for p in client_pipeline_description.Passes.value:
            nettrans.distribute_object(p)
        nettrans.distribute_object(client_pipeline_description)
        
        # pointer_node = avango.gua.nodes.TransformNode(Name = "pointer_node")
        # pointer_node.Transform.connect_from(pointer_tracking_sensor.Matrix)

        # # graph.Root.value.Children.value.append(dynamic_lense)
        if tracked_lense_projection:
            tracked_lense_projection.set_projection_lense(dynamic_lense, pointer_node, dynamic_transform)
            tracked_lense_projection.Button0.connect_from(pointer_button_sensor.Button1)
            tracked_lense_projection.Button1.connect_from(pointer_button_sensor.Button0)


        study_script.set_screen(viewingSetup.screen_node)
        study_script.StudyStateKeyboardButton.connect_from(keyboard.KeySpace)

        study_script.StudyStateCorrectButton.connect_from(keyboard.KeyRight)
        study_script.StudyStateFalseButton.connect_from(keyboard.KeyLeft)

        study_script.StudyStateButton.connect_from(griffin_input.get_button0_field() )
        # study_script.IndicatorButton.connect_from(griffin_input.get_button0_field() )
        study_script.NextButton.connect_from(keyboard.KeyN)
        study_script.PrevButton.connect_from(keyboard.KeyM)

        if study_condition == 'lense':

            study_script.set_picker_and_navigation(tracked_lense_projection, griffin_navigation)
        elif study_condition == 'wall':

            study_script.set_picker_and_navigation(perspective_picker, griffin_navigation)
        
        study_script.set_image_updater(perspective_picker)
        study_script.set_wall(multi_window_viz)

        for _user in viewingSetup.user_list:
            vt_backend.add_camera(_user.camera_node)

        # vt_backend.add_camera(client_cam)
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
                                             Transform=avango.gua.make_trans_mat(0.0, 0.0, 2.0),
                                             BlackList = ["client"])
        graph.Root.value.Children.value.append(camera)

        perspective_picker = WallPerspectivePicker()
        perspective_picker.my_constructor(screen)
        perspective_picker.set_localized_image_list(atlas_tiles)
        perspective_picker.set_visualizer(multi_window_viz, 'texture')


        pick_ray = avango.gua.nodes.RayNode(Name = "pick_ray")
        pick_ray.Transform.value = avango.gua.make_trans_mat(0.0, -1.0, 0.0) * \
                                 avango.gua.make_scale_mat(1.0, 1.0, 50.0)

        ray_geom = mesh_loader.create_geometry_from_file(
            "ray_geom",
            "data/objects/cylinder.obj",
            avango.gua.LoaderFlags.DEFAULTS)

        ray_geom.Transform.value = avango.gua.make_scale_mat(0.1, 0.1, 100)
        pick_ray.Children.value.append(ray_geom)

        picker = Picker()
        picker.SceneGraph.value = graph
        picker.Ray.value = pick_ray

        dynamic_transform = avango.gua.nodes.TransformNode(Name='dynamic_quad_trans')
        dynamic_transform.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 2.1)
        dynamic_quad = DynamicQuad(dynamic_transform, width=0.2, height=0.2)
        dynamic_lense = dynamic_quad.get_node()

        screen.Children.value.append(dynamic_transform)
        screen.Children.value.append(pick_ray)
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

        dynamic_tri_pass = avango.gua.nodes.DynamicTrianglePassDescription()
        lvis_pass = avango.gua.nodes.LightVisibilityPassDescription()
        pipeline_description = avango.gua.nodes.PipelineDescription(
        Passes=[avango.gua.nodes.TriMeshPassDescription(),
                dynamic_tri_pass,
                lvis_pass,
                res_pass],  
        EnableABuffer=True)  # set transparency

         # client viewing setup for tiled wall
        client_navigation = avango.gua.nodes.TransformNode(Name = "client_navigation")
        client_navigation.Transform.value = avango.gua.make_trans_mat(0.0,0.0,0.0)
        client_navigation.Transform.value = avango.gua.make_trans_mat(-4.0,1.0,1.0) *\
                                            avango.gua.make_rot_mat(90.0, 0.0, 1.0, 0.0)
        nettrans.Children.value.append(client_navigation)

        client_screen = avango.gua.nodes.ScreenNode(Name="client_screen", Width=4.07, Height=2.3)
        client_navigation.Children.value.append(client_screen)

        client_cam = avango.gua.nodes.CameraNode(
            Name = "client_cam",
            ViewID=2,
            LeftScreenPath="/net/client_navigation/client_screen",
            SceneGraph="scenegraph",
            Resolution=avango.gua.Vec2ui(7680, 4320),
            # Resolution=avango.gua.Vec2ui(800, 600),
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
        # print_graph(graph.Root.value)
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

def setup_picker(mesh_loader, node, graph):
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
    node.Children.value.append(pick_ray)
    return Picker

if __name__ == '__main__':
    user_id = 0
    if len(sys.argv) > 2:
        task = str(sys.argv[1])
        part = int(sys.argv[2])
        user_id = int(sys.argv[3])
    else:
        print("Restart program with user id - ./start.sh #user ")
    start(task, part, user_id)
    