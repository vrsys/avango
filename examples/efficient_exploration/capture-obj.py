import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import avango.gua.lod
from examples_common.GuaVE import GuaVE
import examples_common.navigator
from examples_common.GuaVE import GuaVE


from src.CaptureTool import CaptureScript
from src.picker import Picker
import random
import os
import time
import math




    

def start():
    """
    Capture script starts in free mode:
    Free Mode: User can move freely, by clicking the user adds 
    """

    graph  = avango.gua.nodes.SceneGraph(Name = "scene")
    mesh_loader = avango.gua.nodes.TriMeshLoader()

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
    bb = photo_model.BoundingBox.value
    # bb.center()
    print(type(bb.center()), bb.center())
    photo_model_center = avango.gua.Vec3(0.0, 0.5, 0.0)
    # photo_model_center = bb.center()    
    #photo_model.Transform.value = avango.gua.make_trans_mat(photo_model_center) * avango.gua.make_rot_mat(90,0,1,0)*\
    #                                    avango.gua.make_rot_mat(-90,1,0,0) * avango.gua.make_scale_mat(0.00015)
    # photo_model.Transform.value = avango.gua.make_trans_mat(bb.center().x, bb.center().y, bb.center().z) * \
    # photo_model.Transform.value = avango.gua.make_trans_mat(0.0,0.4,0.0) * \
    photo_model.Transform.value = avango.gua.make_trans_mat(0.0,0.5,0.0) * \
            avango.gua.make_rot_mat(90.0,-1,0,0) * \
            avango.gua.make_rot_mat(90.0,0,0,1) * \
            avango.gua.make_scale_mat(0.0014)
            # avango.gua.make_scale_mat(0.05)
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

    # width = 6144
    width = 8192
    # width = 2560;
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
        Transform = avango.gua.make_trans_mat(0.0, 0.0, 0.0) * avango.gua.make_rot_mat(90,0,0,1.0)
    )

    screen_grab_pass = avango.gua.nodes.ScreenGrabPassDescription()
    screen_grab_pass.setOutputPrefix("/home/senu8384/Desktop/pics/image_")

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

    pipeline_description = avango.gua.nodes.PipelineDescription(
        Passes = [
            avango.gua.nodes.TriMeshPassDescription(),
            #avango.gua.nodes.SkyMapPassDescription(
            #    OutputTextureName="awesome_skymap"
            #),
            avango.gua.nodes.LightVisibilityPassDescription(),
            res_pass,
            screen_grab_pass,
        ],
        EnableABuffer = True
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
    capture_script.my_constructor(graph, navigator, sphere, screen_grab_pass)
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
    capture_script.N_Key.connect_from(navigator.Keyboard.KeyN)

    viewer = avango.gua.nodes.Viewer()
    # viewer.DesiredFPS.value = 200
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

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