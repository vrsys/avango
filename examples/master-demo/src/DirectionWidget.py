import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import avango.gua.lod
from examples_common.GuaVE import GuaVE
import examples_common.navigator
from examples_common.GuaVE import GuaVE

import random
import os
import time
import math
import subprocess
import json


class CaptureScript(avango.script.Script):

    Button0 = avango.SFBool()
    Button1 = avango.SFBool()
    R_Key = avango.SFBool()
    T_Key = avango.SFBool()
    I_Key = avango.SFBool()
    K_Key = avango.SFBool()
    C_Key = avango.SFBool()
    V_Key = avango.SFBool()
    M_Key = avango.SFBool()
    N_Key = avango.SFBool()
    U_Key = avango.SFBool()

    def __init__(self):
        self.super(CaptureScript).__init__()

    def my_constructor(self, scenegraph, geo_name, navigator, sphere, screen_grab_pass):
        self.graph = scenegraph
        self.navigator = navigator
        self.sphere = sphere
        self.screen_grab_pass = screen_grab_pass

        self.mesh_loader = avango.gua.nodes.TriMeshLoader()

        self.always_evaluate(False)
        random.seed(420)
        self.frame_count = 0
        self.image_count = 0
        self.angle = 0
        self.last_level = 0
        self.current_level = 0
        self.path = os.path.dirname(os.path.realpath(__file__))

        # self.geo = 'terra'
        # self.geo = 'wappen'
        self.geo = geo_name

        self.image_cam_positions = []
        self.image_cam_positions_rot = []
        self.marker_matrices_list = []
        # self.position_list_file = self.path + '/part1-50.lst'
        self.free_mode = False
        self.indicate = False
        self.indicate_perspectives = False
        self.mouse_click_flag = False
        self.set_marker_flag = False
        self.capture_mode = 'position' # take images or capture position
        self.read_mode = False # if read mode is true camera will be set to positions of cam list
        self.cam_location_list = []
        self.max_artificial_images = 100

        self.button0_pressed = False
        self.button1_pressed = False
        self.r_pressed = False
        self.t_pressed = False
        self.i_pressed = False
        self.k_pressed = False
        self.c_pressed = False
        self.v_pressed = False
        self.m_pressed = False
        self.n_pressed = False
        self.u_pressed = False

        self.camera = None
        self.cam_trans = None
        self.center = avango.gua.Vec3(0.0, 0.0, 0.0)
        self.last_cam_mat = avango.gua.make_trans_mat(self.center)

        # init indicators and markers
        self.indicator_mat = avango.gua.nodes.Material()
        sphere_color = avango.gua.Vec4(1.0, 1.0, 1.0, 1.0)
        # sphere_color.normalize()
        self.indicator_mat.set_uniform("Color", sphere_color)
        self.indicator_mat.set_uniform("Roughness", 1.0)
        self.indicator_mat.set_uniform("Emissivity", 1.0)

        self.dir_indicator_mat = avango.gua.nodes.Material()
        sphere_color = avango.gua.Vec4(0.5, 0.9, 0.5, 0.5)
        sphere_color.normalize()
        self.dir_indicator_mat.set_uniform("Color", sphere_color)
        self.dir_indicator_mat.set_uniform("Roughness", 1.0)
        self.dir_indicator_mat.set_uniform("Emissivity", 1.0)

        self.cam_indicator_mat = avango.gua.nodes.Material()
        sphere_color = avango.gua.Vec4(0.5, 0.9, 1.0, 0.5)
        # sphere_color.normalize()
        self.cam_indicator_mat.set_uniform("Color", sphere_color)
        self.cam_indicator_mat.set_uniform("Roughness", 1.0)
        self.cam_indicator_mat.set_uniform("Emissivity", 1.0)

        self.marker_mat = avango.gua.nodes.Material()
        marker_color = avango.gua.Vec4(1.0, 0.1, 0.1, 0.7)
        # sphere_color.normalize()
        self.marker_mat.set_uniform("Color", marker_color)
        self.marker_mat.set_uniform("Roughness", 1.0)
        self.marker_mat.set_uniform("Emissivity", 1.0)

        self.indicators = []
        self.direction_indicators = []
        self.markers = []
        self.camera_indicator = self.mesh_loader.create_geometry_from_file(
                "cam_indicator", "data/objects/cube.obj",
                self.cam_indicator_mat, avango.gua.LoaderFlags.DEFAULTS)

        self.marker_transform = avango.gua.nodes.TransformNode(Name='marker_transform')

        self.marker_indicator = self.mesh_loader.create_geometry_from_file(
            "marker_indicator", "data/objects/torus.obj",
            self.marker_mat, avango.gua.LoaderFlags.DEFAULTS)
        self.marker_indicator.Transform.value = avango.gua.make_scale_mat(0.05,0.05, 0.75) * avango.gua.make_rot_mat(90.0, 1.0, 0.0, 0.0)   
        self.marker_transform.Children.value.append(self.marker_indicator)
  
       
    def set_camera(self, camera, cam_trans, cam_dis, cam_x_rot, cam_y_rot, center):
        # self.always_evaluate(True)

        self.camera = camera
        self.cam_dis = cam_dis
        self.cam_x_rot = cam_x_rot
        self.cam_y_rot = cam_y_rot
        self.cam_trans = cam_trans
        self.center = center

    def get_heatmap_material(self, alpha):
        mat = avango.gua.nodes.Material()
        col = map_from_to(alpha, 0, 180, 0.5, 0.0)
        rand_color = avango.gua.Vec4(0.2, col+0.5, 0.2, 1.0)
        mat.set_uniform("Color", rand_color)
        mat.set_uniform("Roughness", 1.0)
        mat.set_uniform("Emissivity", 1.0)

        return mat

    def add_camera_matrix(self):
        mat = self.camera.WorldTransform.value
        self.cam_location_list.append(mat)
        if self.indicate:
            dir_indicator = self.mesh_loader.create_geometry_from_file(
                    "dir_indicator_" + str(len(self.indicators)), "data/objects/cube.obj",
                    self.dir_indicator_mat, avango.gua.LoaderFlags.DEFAULTS)
            self.direction_indicators.append(dir_indicator)

            self.indicators.append(indicator)
            self.graph.Root.value.Children.value.append(indicator)

    def set_mode(self, mode):
        self.capture_mode = mode

    def toggle_indicators(self, flag):

        if flag:
            print('Show indicators', len(self.cam_location_list))
            self.indicate = True
            self.indicate_perspectives = True
            self.camera.Children.value.append(self.sphere)
            self.sphere.Children.value.append(self.camera_indicator)
            self.camera_indicator.Transform.value = avango.gua.make_trans_mat(0.0,0.0,1.0) * avango.gua.make_scale_mat(0.05)
            for i in self.cam_location_list:
                
                indicator = self.mesh_loader.create_geometry_from_file(
                    "indicator_" + str(len(self.indicators)), "data/objects/cube.obj",
                    avango.gua.LoaderFlags.DEFAULTS)
                indicator.Transform.value = i * avango.gua.make_scale_mat(0.03, 0.03, 0.2)
                print(indicator.WorldTransform.value.get_scale())
                indicator.Material.value.set_uniform('Emissivity', 1.0)

                dir_indicator = self.mesh_loader.create_geometry_from_file(
                    "dir_indicator_" + str(len(self.indicators)), "data/objects/cube.obj",
                    self.dir_indicator_mat, avango.gua.LoaderFlags.DEFAULTS)
                self.direction_indicators.append(dir_indicator)
                self.indicators.append(indicator)
                self.graph.Root.value.Children.value.append(indicator)
        else:
            print('Dont show indicators')
            self.indicate = False
            self.indicate_perspectives = False
            self.sphere.Children.value.remove(self.camera_indicator)
            self.camera.Children.value.remove(self.sphere)
            for i in self.indicators:
                self.graph.Root.value.Children.value.remove(i)
            for i in self.direction_indicators:
                self.graph.Root.value.Children.value.remove(i)
            self.indicators = []
            self.direction_indicators = []
            

    def evaluate(self):
        if self.indicate_perspectives:
            # https://math.stackexchange.com/questions/1939423/calculate-if-vector-intersects-sphere
            C = self.sphere.WorldTransform.value.get_translate()
            r = 0.1
            for idx, ind in enumerate(self.direction_indicators):
                self.graph.Root.value.Children.value.remove(ind)
                P = self.indicators[idx].WorldTransform.value.get_translate()
                U = C - P # direction vector
                U.normalize()
                _rot_mat = avango.gua.make_rot_mat(self.camera.WorldTransform.value.get_rotate_scale_corrected())
                B = _rot_mat * avango.gua.Vec3(0.0,0.0,-1.0)
                B = avango.gua.Vec3(B.x,B.y,B.z) # 
                _dot = U.x*B.x + U.y*B.y + U.z*B.z
                _dot = 1 if _dot >= 1 else _dot
                _dot = -1 if _dot <= -1 else _dot
                alpha = math.degrees(math.acos(_dot))
                if alpha <= 110:
                    Q = P - C
                    a = 1
                    dotUQ = U.x*Q.x + U.y*Q.y + U.z*Q.z
                    b = 2*dotUQ
                    dotQQ = Q.x*Q.x + Q.y*Q.y + Q.z*Q.z
                    c = dotQQ - r*r;
                    d = b*b - 4*a*c;  # discriminant of quadratic
                    
                    if d >= 0:
                        # then solutions are real, so there are intersections
                        sol1 = (-b- math.sqrt(d))/(2*a)    
                        if sol1 >= 0:
                            P1 = avango.gua.Vec3(P.x + sol1*U.x, P.y + sol1*U.y, P.z + sol1*U.z); # first intersection
                            
                            self.graph.Root.value.Children.value.append(ind)
                            ind.Material.value = self.get_heatmap_material(alpha)
                            ind.Transform.value = avango.gua.make_trans_mat(P1) * avango.gua.make_scale_mat(0.005, 0.005, 0.005)


    @field_has_changed(Button0)
    def button0_changed(self):
        if self.Button0.value:
            if self.button0_pressed == False:
                if self.mouse_click_flag:
                    self.capture('obj_image')
                if self.set_marker_flag:
                    self.set_marker()
            self.button0_pressed = True
        else:
            self.button0_pressed = False

    @field_has_changed(Button1)
    def button1_changed(self):
        if self.Button1.value:
            if self.button1_pressed == False:
                if self.free_mode:
                    self.stop_taking_images()
                if self.indicate:
                    if self.indicate_perspectives:
                        self.indicate_perspectives = False
                        for idx, ind in enumerate(self.direction_indicators):
                            self.graph.Root.value.Children.value.remove(ind)
                        self.sphere.Children.value.remove(self.camera_indicator)
                        self.camera.Children.value.remove(self.sphere)
                        
                    else:
                        self.indicate_perspectives = True
                        self.camera.Children.value.append(self.sphere)
                        self.sphere.Children.value.append(self.camera_indicator)
                        self.camera_indicator.Transform.value = avango.gua.make_trans_mat(0.0,0.0,1.0) * avango.gua.make_scale_mat(0.05)

            self.button1_pressed = True
        else:
            self.button1_pressed = False

    @field_has_changed(R_Key)
    def r_key_changed(self):
        if self.R_Key.value:
            if self.r_pressed == False:
                if self.read_mode:
                    print('Read mode off')
                    self.read_mode = False
                else:
                    print('Read mode on')
                    print('Locations:', len(self.cam_location_list))
                    self.read_mode = True

            self.r_pressed = True
        else:
            self.r_pressed = False

    @field_has_changed(T_Key)
    def t_key_changed(self):
        if self.T_Key.value:
            if self.t_pressed == False:
                if self.free_mode:
                    print('Take images')
                    self.start_taking_images()
                else:
                    print('Stop taking images')
                    self.stop_taking_images()

            self.t_pressed = True
        else:
            self.t_pressed = False 

    @field_has_changed(I_Key)
    def i_key_changed(self):
        if self.I_Key.value:
            if self.i_pressed == False:
                if self.indicate:
                   self.toggle_indicators(False)
                else:
                    self.toggle_indicators(True)
            self.i_pressed = True
        else:
            self.i_pressed = False

    @field_has_changed(K_Key)
    def K_key_changed(self):
        if self.K_Key.value:
            if self.k_pressed == False:
                self.cam_location_list = []
                self.read_cam_list()
                print('Read file list')
           
            self.k_pressed = True
        else:
            self.k_pressed = False
    
    @field_has_changed(C_Key)
    def c_key_changed(self):
        # CLEAR cam list
        if self.C_Key.value:
            if self.c_pressed == False:
                print('Clear camera locations')
                self.cam_location_list = []           
            self.c_pressed = True
        else:
            self.c_pressed = False

    @field_has_changed(V_Key)
    def v_key_changed(self):
        if self.V_Key.value:
            if self.v_pressed == False:
                # self.write_cam_list()
                print('Saved camera locations to file', self.position_list_file+'.lst')
           
            self.v_pressed = True
        else:
            self.v_pressed = False


    @field_has_changed(M_Key)
    def m_key_changed(self):
        if self.M_Key.value:
            if self.m_pressed == False:
                if self.capture_mode == 'image':
                    # print('Set mode to "position"')
                    # self.set_mode('position')
                elif self.capture_mode == 'position':
                    # print('Set mode to "image"')
                    # self.set_mode('image')
           
            self.m_pressed = True
        else:
            self.m_pressed = False

    @field_has_changed(N_Key)
    def n_key_changed(self):
        if self.N_Key.value:
            if self.n_pressed == False:
                if self.mouse_click_flag:
                    self.mouse_click_flag = False
                else:
                    self.mouse_click_flag = True
            self.n_pressed = True
        else:
            self.n_pressed = False

    @field_has_changed(U_Key)
    def u_key_changed(self):
        if self.U_Key.value:
            if self.u_pressed == False:
                if self.set_marker_flag:
                    # self.set_marker_flag = False
                    # self.camera.Children.value.remove(self.marker_transform)
                else:
                    # self.set_marker_flag = True
                    # self.camera.Children.value.append(self.marker_transform)
                    # self.marker_transform.Transform.value = avango.gua.make_trans_mat(0.0,0.0,-0.5)
            self.u_pressed = True
        else:
            self.u_pressed = False


def map_from_to(x,a,b,c,d):
    y=(x-a)/(b-a)*(d-c)+c
    return y