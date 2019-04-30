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
import subprocess


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

    def __init__(self):
        self.super(CaptureScript).__init__()

    def my_constructor(self, scenegraph, navigator):
        self.graph = scenegraph
        self.navigator = navigator

        self.mesh_loader = avango.gua.nodes.TriMeshLoader()

        self.always_evaluate(False)
        random.seed(420)
        self.frame_count = 0
        self.image_count = 0
        self.angle = 0
        self.path = os.path.dirname(os.path.realpath(__file__))
        self.position_list_file = self.path + '/camera_positions.lst'
        self.free_mode = False
        self.indicate = False
        self.capture_mode = 'position' # take images or capture position
        self.read_mode = False # if read mode is true camera will be set to positions of cam list
        self.cam_location_list = []
        self.indicators = []

        self.button0_pressed = False
        self.button1_pressed = False
        self.r_pressed = False
        self.t_pressed = False
        self.i_pressed = False
        self.k_pressed = False
        self.c_pressed = False
        self.v_pressed = False
        self.m_pressed = False

        self.camera = None
        self.cam_trans = None
        self.center = avango.gua.Vec3(0.0, 0.0, 0.0)
       
    def set_camera(self, camera, cam_trans, cam_dis, cam_x_rot, cam_y_rot, center):
        # self.always_evaluate(True)

        self.camera = camera
        self.cam_dis = cam_dis
        self.cam_x_rot = cam_x_rot
        self.cam_y_rot = cam_y_rot
        self.cam_trans = cam_trans
        self.center = center

    def write_cam_list(self):
        with open(self.position_list_file, 'w') as outfile:  
            for mat in self.cam_location_list:
                # print(avango.gua.to_list(mat))
                # line = str(mat).replace('\n', '')
                line = str(avango.gua.to_list(mat)).replace(',', '')
                outfile.write(line)
                outfile.write('\n')
        print(self.cam_location_list[0])


    def add_camera_matrix(self):
        mat = self.camera.WorldTransform.value
        self.cam_location_list.append(mat)
        if self.indicate:
            indicator = self.mesh_loader.create_geometry_from_file(
                "indicator_" + str(len(self.indicators)), "data/objects/cube.obj",
                avango.gua.LoaderFlags.DEFAULTS)
            indicator.Transform.value = mat * avango.gua.make_scale_mat(0.05, 0.05, 0.4)
            indicator.Material.value.set_uniform('Emissivity', 1.0)
            self.indicators.append(indicator)
            self.graph.Root.value.Children.value.append(indicator)

    def read_cam_list(self):
        data = []
        with open(self.position_list_file) as f:
            for line in f:
                data.append(line.replace('\n', '').replace('[', '').replace(']', ''))

            for item in data:
                items = [float(x) for x in item.split(' ')]
                mat = avango.gua.from_list(items)
                self.cam_location_list.append(mat)
        print(self.cam_location_list[0])        

    def set_mode(self, mode):
        self.capture_mode = mode

    def capture(self, name):
        print(self.capture_mode)
        if self.capture_mode == 'position':
            self.add_camera_matrix()
        if self.capture_mode == 'image':
            file_name = self.path + '/images/' + name + str(self.image_count)
            #print(file_name)
            bash_command = "env DISPLAY=:0.0 import -window ROOT " + file_name + ".png"
            process = subprocess.Popen(bash_command.split(), stdout=subprocess.PIPE)
            output, error = process.communicate()
            #print(bash_command)
        print('Captured ', self.image_count)
        self.image_count += 1  

    def start_taking_images(self):
        self.always_evaluate(True)
        self.image_count = 0
        self.free_mode = False
        self.cam_trans.Transform.disconnect_from(self.navigator.OutTransform)

    def stop_taking_images(self):
        self.always_evaluate(False)
        self.free_mode = True
        self.cam_trans.Transform.value = avango.gua.make_identity_mat()
        self.cam_dis.Transform.value = avango.gua.make_identity_mat()
        self.cam_x_rot.Transform.value = avango.gua.make_identity_mat()
        self.cam_y_rot.Transform.value = avango.gua.make_identity_mat()
        self.camera.Transform.value = avango.gua.make_identity_mat()
        self.cam_trans.Transform.connect_from(self.navigator.OutTransform)
        print('Stopped taking images')


    def toggle_indicators(self, flag):

        if flag:
            print('Show indicators', len(self.cam_location_list))
            self.indicate = True
            for i in self.cam_location_list:
                # load model to take images of
                number_str = str(len(self.indicators))
                if len(number_str) == 1:
                    number_str = '000' + number_str
                elif len(number_str) == 2:
                    number_str = '00' + number_str
                elif len(number_str) == 3:
                    number_str = '0' + number_str

                indicator = self.mesh_loader.create_geometry_from_file(
                    "indicator_" + number_str, "data/objects/cube.obj",
                    avango.gua.LoaderFlags.DEFAULTS)
                indicator.Transform.value = i * avango.gua.make_scale_mat(0.05, 0.05, 0.4)
                indicator.Material.value.set_uniform('Emissivity', 1.0)
                self.indicators.append(indicator)
                self.graph.Root.value.Children.value.append(indicator)
        else:
            print('Dont show indicators')
            self.indicate = False
            for i in self.indicators:
                self.graph.Root.value.Children.value.remove(i)
            self.indicators = []
            

    def evaluate(self):
        if self.free_mode == False:
            self.frame_count += 1
            if self.frame_count == 29:
                print('Move in read mode:', self.read_mode, self.image_count, len(self.cam_location_list))
                if self.read_mode:
                    self.set_mode('image')
                    self.cam_trans.Transform.value = avango.gua.make_identity_mat()
                    self.cam_dis.Transform.value = avango.gua.make_identity_mat()
                    self.cam_x_rot.Transform.value = avango.gua.make_identity_mat()
                    self.cam_y_rot.Transform.value = avango.gua.make_identity_mat()
                    self.camera.Transform.value = avango.gua.make_identity_mat() *  avango.gua.make_rot_mat(90,0,0,1.0)

                    if self.image_count < len(self.cam_location_list):
                        self.cam_trans.Transform.value = self.cam_location_list[self.image_count]
                    else:
                        print('image count below length of location list')
                        self.stop_taking_images()
                else:
                    distance = 5.0
                    height = 1.0
                    levels = 5
                    height_steps = height / levels
                    angle_steps = 18
                    steps = 360//angle_steps

                    offset_x = random.uniform(-0.005, 0.005)
                    offset_y = random.uniform(-0.005, 0.005)
                    self.cam_dis.Transform.value = avango.gua.make_trans_mat(offset_x, offset_y, distance)

                    height_level = self.center.y - (height / 2) + height_steps * ((self.image_count//steps) % levels) + 0.1
                    self.cam_trans.Transform.value = avango.gua.make_trans_mat(self.center.x, height_level, self.center.z)
                    
                    self.angle = (self.angle + angle_steps) % 360
                    angle_x = random.uniform(-3.5, 3.5)
                    angle_y = random.uniform(-3.5, 3.5)
                    print(angle_x, angle_y)
                    self.cam_x_rot.Transform.value = avango.gua.make_rot_mat(angle_x, 1.0, 0.0, 0.0)
                    self.cam_y_rot.Transform.value = avango.gua.make_rot_mat(angle_y, 0.0, 1.0, 0.0)
                    
                    self.cam_trans.Transform.value *= avango.gua.make_rot_mat(self.angle, 0.0, 1.0, 0.0)

            if self.frame_count == 30:
                
                self.capture('obj_image')
                if self.read_mode == False and self.image_count >= 100:
                    self.stop_taking_images()
                self.frame_count = 0

    @field_has_changed(Button0)
    def button0_changed(self):
        if self.Button0.value:
            if self.button0_pressed == False:
                self.capture('obj_image')
            self.button0_pressed = True
        else:
            self.button0_pressed = False

    @field_has_changed(Button1)
    def button1_changed(self):
        if self.Button1.value:
            if self.button1_pressed == False:
                if self.free_mode:
                    self.stop_taking_images()

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
                self.write_cam_list()
                print('Saved camera locations to file', self.position_list_file)
           
            self.v_pressed = True
        else:
            self.v_pressed = False


    @field_has_changed(M_Key)
    def m_key_changed(self):
        if self.M_Key.value:
            if self.m_pressed == False:
                if self.capture_mode == 'image':
                    print('Set mode to "position"')
                    self.set_mode('position')
                elif self.capture_mode == 'position':
                    print('Set mode to "image"')
                    self.set_mode('image')
           
            self.m_pressed = True
        else:
            self.m_pressed = False
    

def start():
    """
    Capture script starts in free mode:
    Free Mode: User can move freely, by clicking the user adds 
    """
  

    graph  = avango.gua.nodes.SceneGraph(Name = "scene")
    mesh_loader = avango.gua.nodes.TriMeshLoader()

    floor = mesh_loader.create_geometry_from_file(
        "floor",
        "data/objects/plane.obj",
        avango.gua.LoaderFlags.DEFAULTS
    )

    floor.Transform.value = avango.gua.make_trans_mat(0, -0.1 , 0) * avango.gua.make_scale_mat(4, 1, 4)
    floor.ShadowMode.value = 1
    #graph.Root.value.Children.value.append(floor)

    # load model to take images of
    photo_model = mesh_loader.create_geometry_from_file(
        "photo_model",
        # "data/objects/cube.obj",
        "/home/senu8384/Desktop/master-thesis/data/Terrakottaarmee_Bogenschuetze_T21_G18_01/avango_lod/Bogenschuetze-01.obj",
        avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.NORMALIZE_POSITION | avango.gua.LoaderFlags.NORMALIZE_SCALE | avango.gua.LoaderFlags.LOAD_MATERIALS
    )    
    photo_model_center = avango.gua.Vec3(0.0, 0.5, 0.0)
    #photo_model.Transform.value = avango.gua.make_trans_mat(photo_model_center) * avango.gua.make_rot_mat(90,0,1,0)*\
    #                                    avango.gua.make_rot_mat(-90,1,0,0) * avango.gua.make_scale_mat(0.00015)
    photo_model.Transform.value = avango.gua.make_trans_mat(0.0,0.83,0.0) * \
            avango.gua.make_rot_mat(90.0,-1,0,0) * \
            avango.gua.make_rot_mat(90.0,0,0,1) * \
            avango.gua.make_scale_mat(0.0014)
    # photo_model.Transform.value = avango.gua.make_trans_mat(photo_model_center) * avango.gua.make_scale_mat(0.5, 1.0, 0.5)
    # photo_model.Material.value.set_uniform('Color', avango.gua.Color(0.4, 0.4, 0.2))
    # photo_model.Material.value.set_uniform('Emissivity', 1.0)
    
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

    #bb = photo_model.BoundingBox.value
    #bb.center()
    #bb.Min.value
    #bb.Max.value


    graph.Root.value.Children.value.append(photo_model)


    # t = str(quad.transform).replace('\n', '')
    # cam_location_list.append(t)

    # with open('cam_transforms.txt', 'w') as outfile:  
    #     for line in cam_location_list:
    #         outfile.write(line)
    #         outfile.write('\n')

    width = 2560;
    height = int(width * 9.0 / 16.0)
    size = avango.gua.Vec2ui(width, height)

    camera_transform = avango.gua.nodes.TransformNode(Name='cam_trans')
    camera_distance = avango.gua.nodes.TransformNode(Name='cam_dis')
    camera_x_rot = avango.gua.nodes.TransformNode(Name='cam_x_rot')
    camera_y_rot = avango.gua.nodes.TransformNode(Name='cam_y_rot')

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

    # 0.05 0.07492871690427698

    screen.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -0.8)

    camera = avango.gua.nodes.CameraNode(
        Name = "cam",
        LeftScreenPath = "/cam_trans/cam_dis/cam_x_rot/cam_y_rot/cam/screen",
        RightScreenPath = "/cam_trans/cam_dis/cam_x_rot/cam_y_rot/cam/screen",
        SceneGraph = "scene",
        Resolution = size,
        OutputWindowName = "window",
        EyeDistance = 0.2,
        EnableStereo = False,
        Children = [screen],
        Transform = avango.gua.make_trans_mat(0.0, 0.0, 0.0)  * avango.gua.make_rot_mat(90,0,0,1.0)
    )

    res_pass = avango.gua.nodes.ResolvePassDescription()
    res_pass.EnableSSAO.value = False
    res_pass.SSAOIntensity.value = 3.0
    res_pass.SSAOFalloff.value = 20.0
    res_pass.SSAORadius.value = 10.0
    res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1,0.1,0.1)
    res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
    res_pass.Exposure.value = 1.0
    #res_pass.BackgroundMode.value = avango.gua.BackgroundMode.CUBEMAP_TEXTURE
    #res_pass.BackgroundTexture.value = "awesome_skymap"
    #res_pass.BackgroundMode.value = avango.gua.BackgroundMode.CUBEMAP_TEXTURE
    res_pass.BackgroundColor.value = avango.gua.Color(0.45, 0.5, 0.6)
    # res_pass.VignetteColor.value = avango.gua.Vec4(0, 0, 0, 1)

    pipeline_description = avango.gua.nodes.PipelineDescription(
        Passes = [
            avango.gua.nodes.TriMeshPassDescription(),
            #avango.gua.nodes.SkyMapPassDescription(
            #    OutputTextureName="awesome_skymap"
            #),
            avango.gua.nodes.LightVisibilityPassDescription(),
            res_pass,
    ],
        EnableABuffer = False
    )

    camera.PipelineDescription.value = pipeline_description

    graph.Root.value.Children.value.append(camera_transform)
    camera_transform.Children.value.append(camera_distance)
    camera_distance.Children.value.append(camera_x_rot)
    camera_x_rot.Children.value.append(camera_y_rot)
    camera_y_rot.Children.value.append(camera)

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

    # camera_transform.Transform.connect_from(navigator.OutTransform)

    capture_script = CaptureScript()
    capture_script.my_constructor(graph, navigator)
    capture_script.set_camera(camera, camera_transform, camera_distance,
                            camera_x_rot, camera_y_rot, photo_model_center)
    capture_script.Button0.connect_from(navigator.Mouse.ButtonLeft)
    capture_script.Button1.connect_from(navigator.Mouse.ButtonRight)
    capture_script.R_Key.connect_from(navigator.Keyboard.KeyR)
    capture_script.T_Key.connect_from(navigator.Keyboard.KeyT)
    capture_script.I_Key.connect_from(navigator.Keyboard.KeyI)
    capture_script.K_Key.connect_from(navigator.Keyboard.KeyK)
    capture_script.C_Key.connect_from(navigator.Keyboard.KeyC)
    capture_script.V_Key.connect_from(navigator.Keyboard.KeyV)
    capture_script.M_Key.connect_from(navigator.Keyboard.KeyM)

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

'''
def main():

path = os.path.dirname(os.path.realpath(file))
file_name = path + '/test_screen_shot'

for i in range(5):
bash_command = "env DISPLAY=:0.0 import -window ROOT " + file_name + str(i) + ".png"
process = subprocess.Popen(bash_command.split(), stdout=subprocess.PIPE)
output, error = process.communicate()
print(i, error)
time.sleep(1)


if name == 'main':
main()

'''