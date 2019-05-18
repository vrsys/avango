import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import avango.gua.lod
from examples_common.GuaVE import GuaVE
import examples_common.navigator


from src.CaptureTool import CaptureScript
from src.picker import Picker
import random
import os
import time
import math

from src.LocalizedImage import LocalizedImageQuad
from src.DynamicQuad import DynamicQuad

def get_atlas_scale_factor():
    
    # auto atlas = new vt::pre::AtlasFile(settings_.atlas_file_.c_str());
    image_width    = 2**17
    image_height   = 2**17

    # // tile's width and height without padding
    tile_inner_width  = 8192
    tile_inner_height = 4608

    # // Quadtree depth counter, ranges from 0 to depth-1
    # uint64_t depth = atlas->getDepth();

    # factor_u  =  image_width  / (tile_inner_width  * 15)
    # factor_v  =  image_height / (tile_inner_height * 14)
    atlas_width = 122880
    atlas_height = 64512

    factor_u  = (tile_inner_width  * 15) / image_width
    factor_v  = (tile_inner_height * 14) / image_height
    # print(' factor u v', factor_u, factor_v)
    return max(factor_u, factor_v)


def create_vt_quad(dt_node, quad_id, trans_mat):

    atlas_width  = 2**17
    atlas_height = 2**16
    # factor_u  = (tile_inner_width  * 15) / image_width # 0.9375
    factor_u  = 0.9375
    factor_u  = 122880 / atlas_width
    # factor_v  = (tile_inner_height * 14) / image_height # 0.4921875
    # factor_v  = 0.4921875
    factor_v  = 64512 / atlas_height
    # print('fakldjfajfj',64512 / (2**17))
    rows = 14
    cols = 15
    print('quad',  quad_id, 'col:', quad_id % cols ,'row:', quad_id // cols)
    # x = map_from_to(quad_id % cols, 0, cols, 0.0, 1.0)
    x = quad_id % cols
    # y = map_from_to(quad_id // cols, 0, rows, 1.0, 0.0)
    y = quad_id // cols

    print('pos x y ', x, y)
    # y = quad_id // cols
    quad_transform = trans_mat * avango.gua.make_rot_mat(-90,0,0,1.0)

    width = 0.05
    height = width * 9.0 / 16.0
    # print('width height', width, height)
    # print('atlas scale: ', get_atlas_scale_factor())
    # factor = get_atlas_scale_factor()

   
    tile_w = 8192 / 122880
    tile_h = 4608 / 64512
    print('tile w h',tile_w, tile_h)
    # tile_pos_x = x / atlas_height * factor
    tile_pos_x =(x * tile_w ) * factor_u
    # tile_pos_y = y / 64512 * tile_h + (1 - factor)
    # tile_pos_y = map_from_to(y * tile_h, 0.0, 1.0, 1.0, 0.0 )
    # tile_pos_y = 1 - ((y * tile_h) * 64512 / (2**17) )
    tile_pos_y = 1 - ((y * tile_h) * factor_v )

    ##
    t_atlas_width  = 122880
    t_atlas_height = 64512

    t_factor_u  = 122880 / t_atlas_width
    t_factor_v  = 64512 / t_atlas_height

    t_tile_w = 8192 / t_atlas_height * t_factor_u
    t_tile_h = 8192 / t_atlas_width * t_factor_u

    tile_pos_x = x / t_atlas_height * t_factor_u
    tile_pos_y = y / t_atlas_height * tile_h + (1 - t_factor_u)
    ##


    ##
    x_min = 0.0
    x_max = 1.0
    y_min = 1.0
    y_max = 0.5

    ##
    print('pos tx ty ', tile_pos_x, tile_pos_y)

    pos = quad_transform * avango.gua.Vec3( width, height, 0.0)
    uv  = avango.gua.Vec2(tile_pos_x + tile_w, tile_pos_y + tile_h)
    # uv  = avango.gua.Vec2(1.0, 1.0)
    # uv  = avango.gua.Vec2(1.0, 1.0)
    dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
    
    pos = quad_transform * avango.gua.Vec3(-width, -height, 0.0)
    uv  = avango.gua.Vec2(tile_pos_x, tile_pos_y)
    # uv  = avango.gua.Vec2(0.0, 0.5)
    # uv  = avango.gua.Vec2(x_min, 0.5)
    dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
    
    pos = quad_transform * avango.gua.Vec3(width, -height, 0.0)
    uv  = avango.gua.Vec2(tile_pos_x + tile_w, tile_pos_y)
    # uv  = avango.gua.Vec2(1.0, 0.5)
    # uv  = avango.gua.Vec2(1.0, 0.5)
    dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)

    pos = quad_transform * avango.gua.Vec3( width, height, 0.0)
    uv  = avango.gua.Vec2(tile_pos_x + tile_w, tile_pos_y + tile_h)
    # uv  = avango.gua.Vec2(1.0, 1.0)
    # uv  = avango.gua.Vec2(1.0, 1.0)
    dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
    
    pos = quad_transform * avango.gua.Vec3(-width, height, 0.0)
    uv  = avango.gua.Vec2(tile_pos_x, tile_pos_y + tile_h)
    # uv  = avango.gua.Vec2(0.0, 1.0)
    # uv  = avango.gua.Vec2(x_min, 1.0)
    dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)

    pos = quad_transform * avango.gua.Vec3(-width, -height, 0.0)
    uv  = avango.gua.Vec2(tile_pos_x, tile_pos_y)
    # uv  = avango.gua.Vec2(0.0, 0.5)
    # uv  = avango.gua.Vec2(x_min, y_max)
    dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)

def start():
    """
    Capture script starts in free mode:
    Free Mode: User can move freely, by clicking the user adds 
    """
  
    vt_backend = avango.gua.VTBackend()
    graph  = avango.gua.nodes.SceneGraph(Name = "scene")
    mesh_loader = avango.gua.nodes.TriMeshLoader()
    dt_loader = avango.gua.nodes.DynamicTriangleLoader() 
    sphere_mat = avango.gua.nodes.Material()

    sphere_color = avango.gua.Vec4(0.6, 0.6, 0.9, 1.0)
    sphere_color.normalize()

    sphere_mat.set_uniform("Color", sphere_color)
    sphere_mat.set_uniform("Roughness", random.random())
    sphere_mat.set_uniform("Metalness", random.random())
    sphere = mesh_loader.create_geometry_from_file(
        "sphere",
        "data/objects/sphere3.obj",
        sphere_mat,
        avango.gua.LoaderFlags.DEFAULTS
    )

    sphere.Transform.value = avango.gua.make_trans_mat(0, 0.0 , -1.0) * avango.gua.make_scale_mat(0.1, 0.1, 0.1)

    # load model to take images of
    photo_model = mesh_loader.create_geometry_from_file(
        "photo_model",
        # "data/objects/cube.obj",
        # "/home/senu8384/Desktop/master-thesis/data/01_Igeler-Saeule_Kopie/Igeler-Saeule_RLM.OBJ/Igeler-Saeule_RLM.obj",
        "/home/senu8384/Desktop/master-thesis/data/Terrakottaarmee_Bogenschuetze_T21_G18_01/avango_lod/Bogenschuetze-01.obj",
        # avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.NORMALIZE_POSITION | avango.gua.LoaderFlags.NORMALIZE_SCALE | avango.gua.LoaderFlags.LOAD_MATERIALS
        avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS
    )    

    photo_model_center = avango.gua.Vec3(0.0, 0.5, 0.0)

    photo_model.Transform.value = avango.gua.make_trans_mat(0.0,0.5,0.0) * \
            avango.gua.make_rot_mat(90.0,-1,0,0) * \
            avango.gua.make_rot_mat(90.0,0,0,1) * \
            avango.gua.make_scale_mat(0.0014)

    # update material properties (in whole subtree)
    stack = [(photo_model)]
    while stack:
        _node = stack.pop()
        
        if _node.has_field("Material") == True:
            _node.Material.value.set_uniform("Metalness", 0.0)
            _node.Material.value.set_uniform("Roughness", 1.0)
            _node.Material.value.set_uniform("Emissivity", 1.0)

        stack.extend(
          [(_child_node) for _child_node in reversed(_node.Children.value)])
    graph.Root.value.Children.value.append(photo_model)

    # Init localized Images
    data = []
    cam_location_list = []
    try:
        with open('/home/senu8384/Desktop/master-thesis/data/positions-set-z.lst') as f:
            for line in f:
                data.append(line.replace('\n', '').replace('[', '').replace(']', ''))

            for item in data:
                items = [float(x) for x in item.split(' ')]
                mat = avango.gua.from_list(items)
                cam_location_list.append(mat)    
    except FileNotFoundError:
        print('File does not exist.')
    
    view_num = len(data)
    print('###############', view_num)
    atlas_path = '/home/senu8384/Desktop/master-thesis/data/a_textures.atlas'
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
    
    for quad_id in range(view_num):
        create_vt_quad(localized_images_node, quad_id, cam_location_list[quad_id])
        
    graph.Root.value.Children.value.append(localized_images_node)

    # width = 6144
    # width = 8192
    width = 2560;
    height = int(width * 9.0 / 16.0)
    size = avango.gua.Vec2ui(width, height)

    camera_transform = avango.gua.nodes.TransformNode(Name='cam_trans')

    screen = avango.gua.nodes.ScreenNode(Name = "screen",
                                       # Width = 4.8,
                                       # Height = 2.7)
                                       # Width = 0.48, Height = 0.27)
                                       #Width = 0.256, Height = 0.144)
                                       Width = 0.6, Height = 0.338)
                                       # Width = 0.05 * 2,
                                       # Height = 0.07492871690427698 * 2) 
                                       # Width = 0.07492871690427698 * 2,
                                       # Height = 0.05 * 2) 

    screen.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -0.8)

    camera = avango.gua.nodes.CameraNode(
        Name = "cam",
        LeftScreenPath = "/cam_trans/cam/screen",
        RightScreenPath = "/cam_trans/cam/screen",
        SceneGraph = "scene",
        Resolution = size,
        OutputWindowName = "window",
        EyeDistance = 0.2,
        EnableStereo = False,
        Children = [screen],
        Transform = avango.gua.make_trans_mat(0.0, 0.0, 0.0)
    )

    screen_grab_pass = avango.gua.nodes.ScreenGrabPassDescription()
    screen_grab_pass.setOutputPrefix("/home/senu8384/Desktop/pics/image_")

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
            dynamic_tri_pass,
            lvis_pass,
            res_pass
            # avango.gua.nodes.DebugViewPassDescription()
            ],  
    EnableABuffer=False)  # set transparency


    camera.PipelineDescription.value = pipeline_description
    vt_backend.add_camera(camera)

    vt_backend.start_backend()


    graph.Root.value.Children.value.append(camera_transform)
    camera_transform.Children.value.append(camera)

    # window = avango.gua.nodes.Window(
    window = avango.gua.nodes.GlfwWindow(
        Size = size,
        Title = "shadows",
        LeftResolution = size,
        RightResolution = size,
        EnableVsync = False,
        StereoMode = avango.gua.StereoMode.MONO,
        EnableFullscreen = True
    )

    avango.gua.register_window("window", window)

    navigator = examples_common.navigator.Navigator()
    navigator.StartLocation.value = camera.Transform.value.get_translate()
    navigator.OutTransform.connect_from(camera.Transform)
    navigator.RotationSpeed.value = 0.2
    navigator.MotionSpeed.value = 0.04
    camera_transform.Transform.connect_from(navigator.OutTransform)

    # capture_script = CaptureScript()
    # capture_script.my_constructor(graph, navigator, sphere, screen_grab_pass)
    # capture_script.set_camera(camera, camera_transform, camera_distance,
    #                         camera_x_rot, camera_y_rot, photo_model_center)
    # capture_script.Button0.connect_from(navigator.Mouse.ButtonLeft)
    # capture_script.Button1.connect_from(navigator.Mouse.ButtonRight)
    # capture_script.R_Key.connect_from(navigator.Keyboard.KeyR)
    # capture_script.T_Key.connect_from(navigator.Keyboard.KeyT)
    # capture_script.I_Key.connect_from(navigator.Keyboard.KeyI)
    # capture_script.K_Key.connect_from(navigator.Keyboard.KeyK)
    # capture_script.C_Key.connect_from(navigator.Keyboard.KeyC)
    # capture_script.V_Key.connect_from(navigator.Keyboard.KeyV)
    # capture_script.M_Key.connect_from(navigator.Keyboard.KeyM)
    # capture_script.N_Key.connect_from(navigator.Keyboard.KeyN)

    viewer = avango.gua.nodes.Viewer()
    # viewer.DesiredFPS.value = 200
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    timer = avango.nodes.TimeSensor()

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()

def map_from_to(x,a,b,c,d):
    y=(x-a)/(b-a)*(d-c)+c
    return y

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


if __name__ == '__main__':
    start()

"""

    atlas_width  = 2**17
    atlas_height = 2**16
    # factor_u  = (tile_inner_width  * 15) / image_width # 0.9375
    factor_u  = 0.9375
    factor_u  = 122880 / atlas_width
    # factor_v  = (tile_inner_height * 14) / image_height # 0.4921875
    # factor_v  = 0.4921875
    factor_v  = 64512 / atlas_height
    # print('fakldjfajfj',64512 / (2**17))
    rows = 14
    cols = 15
    print('quad',  quad_id, 'col:', quad_id % cols ,'row:', quad_id // cols)
    # x = map_from_to(quad_id % cols, 0, cols, 0.0, 1.0)
    x = quad_id % cols
    # y = map_from_to(quad_id // cols, 0, rows, 1.0, 0.0)
    y = quad_id // cols

    # print('pos x y ', x, y)
    # y = quad_id // cols
    quad_transform = trans_mat * avango.gua.make_rot_mat(-90,0,0,1.0)

    width = 0.05
    height = width * 9.0 / 16.0
    # print('width height', width, height)
    # print('atlas scale: ', get_atlas_scale_factor())
    # factor = get_atlas_scale_factor()

   
    tile_w = 8192 / 122880
    tile_h = 4608 / 64512
    # print('tile w h',tile_w, tile_h)
    # tile_pos_x = x / atlas_height * factor
    tile_pos_x =(x * tile_w ) * factor_u
    # tile_pos_y = y / 64512 * tile_h + (1 - factor)
    # tile_pos_y = map_from_to(y * tile_h, 0.0, 1.0, 1.0, 0.0 )
    # tile_pos_y = 1 - ((y * tile_h) * 64512 / (2**17) )
    tile_pos_y = 1 - ((y * tile_h) * factor_v )

    ##
    t_atlas_width  = 122880
    t_atlas_height = 64512

    t_factor_u  = 122880 / 2**17
    t_factor_v  = 64512 / 2**17

    t_tile_w = 8192 / t_atlas_height * t_factor_u
    t_tile_h = 8192 / t_atlas_width * t_factor_u

    tile_pos_x = x / t_atlas_height * t_factor_u
    tile_pos_y = y / t_atlas_height * tile_h + (1 - t_factor_u)

    ##
    t_factor_u  = 122880 / 2**17
    t_factor_v  = 64512 / 2**17

    t_w  = (122880 + 980) / 2**17 / cols
    # t_w  = 122880 / 2**17 / cols
    t_h  = (64512+518) / 2**17 / rows
    # print('tile w',t_w)
    #0.950787
    ##
    x = quad_id % cols
    x_min = x * (t_w)
    # x_max = t_factor_u /15
    x_max = x_min + (t_w)
    print('x min',x_min,'x max', x_max)


    y_min = 1 - (y * t_h)
    # y_max = 1 - t_factor_v /14
    y_max = 1 - (y * t_h + t_h)
    print('y min',y_min,'y max', y_max)

    ##
    # print('pos tx ty ', tile_pos_x, tile_pos_y)

    pos = quad_transform * avango.gua.Vec3( width, height, 0.0)
    uv  = avango.gua.Vec2(tile_pos_x + tile_w, tile_pos_y + tile_h)
    # uv  = avango.gua.Vec2(1.0, 1.0)
    uv  = avango.gua.Vec2(x_max, y_min)
    dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
    
    pos = quad_transform * avango.gua.Vec3(-width, -height, 0.0)
    uv  = avango.gua.Vec2(tile_pos_x, tile_pos_y)
    # uv  = avango.gua.Vec2(0.0, 0.5)
    uv  = avango.gua.Vec2(x_min, y_max)
    dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
    
    pos = quad_transform * avango.gua.Vec3(width, -height, 0.0)
    uv  = avango.gua.Vec2(tile_pos_x + tile_w, tile_pos_y)
    # uv  = avango.gua.Vec2(1.0, 0.5)
    uv  = avango.gua.Vec2(x_max, y_max)
    dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)

    pos = quad_transform * avango.gua.Vec3( width, height, 0.0)
    uv  = avango.gua.Vec2(tile_pos_x + tile_w, tile_pos_y + tile_h)
    # uv  = avango.gua.Vec2(1.0, 1.0)
    uv  = avango.gua.Vec2(x_max, y_min)
    dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
    
    pos = quad_transform * avango.gua.Vec3(-width, height, 0.0)
    uv  = avango.gua.Vec2(tile_pos_x, tile_pos_y + tile_h)
    # uv  = avango.gua.Vec2(0.0, 1.0)
    uv  = avango.gua.Vec2(x_min, y_min)
    dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)

    pos = quad_transform * avango.gua.Vec3(-width, -height, 0.0)
    uv  = avango.gua.Vec2(tile_pos_x, tile_pos_y)
    # uv  = avango.gua.Vec2(0.0, 0.5)
    uv  = avango.gua.Vec2(x_min, y_max)
    dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)

    ## RICHTIG
    # t_w  = 122880 / 2**17 / cols
    # t_h  = 64512 / 2**17 / rows

    ##
    # x_min = 0.0
    # x_max = t_factor_u /15
    # x_max = t_w


    # y_min = 1.0
    # y_max = 1 - t_factor_v /14
    # y_max = 1 - t_h

    ##
"""