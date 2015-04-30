import avango
import avango.script
from avango.script import field_has_changed
import avango.gua.skelanim
from examples_common.GuaVE import GuaVE

from avango.gua.skelanim.GroundFollowing import GroundFollowing
from avango.gua.skelanim.CharacterControl import CharacterControl
from avango.gua.skelanim.CameraControl import CameraControl
from avango.gua.skelanim.DistanceEvents import DistanceEvents
from avango.gua.skelanim.AnimationControl import AnimationConfig
from CharacterSettings import *

camera_controls = []
character_controls = []
ground_followings = []
current_character = 0
device_sensor = None
screen = None


class XBOX_Listener(avango.script.Script):

    XBOX_BTN_B = avango.SFFloat()
    XBOX_BTN_Y = avango.SFFloat()

    def __init__(self):

        self.super(XBOX_Listener).__init__()

    @field_has_changed(XBOX_BTN_B)
    def xbox_button_b_changed(self):
        if self.XBOX_BTN_B.value:
            next_character()

    @field_has_changed(XBOX_BTN_Y)
    def xbox_button_y_changed(self):
        if self.XBOX_BTN_Y.value:
            next_character(-1)


def next_character(add=1):

    global current_character
    global camera_controls
    global camera_controls
    global device_sensor
    global screen

    if(device_sensor is not None and screen is not None and
       len(character_controls) > 0 and len(camera_controls) > 0):
        character_controls[current_character].listen_keyboard(False)
        camera_controls[current_character].listen_mouse(False)
        camera_control_xbox_disconnect(camera_controls[current_character],
                                       device_sensor)
        current_character = (current_character + add) % len(camera_controls)
        character_controls[current_character].listen_keyboard(True)
        camera_controls[current_character].listen_mouse(True)
        camera_control_xbox_connect(camera_controls[current_character],
                                    device_sensor)
        screen.Transform.disconnect()
        screen.Transform.connect_from(
            camera_controls[current_character].OutTransform)


### CAMERA CONTROL VIA XBOX CONTROLLER:
def camera_control_xbox_connect(camera_control, device_sensor):
    # optional / additional xbox controller settings:
    camera_control.XBOX_X.connect_from(device_sensor.Value2)
    camera_control.XBOX_Y.connect_from(device_sensor.Value3)
    camera_control.XBOX_LZ.connect_from(device_sensor.Value4)
    camera_control.XBOX_RZ.connect_from(device_sensor.Value5)


def camera_control_xbox_disconnect(camera_control, device_sensor):
    # optional / additional xbox controller settings:
    camera_control.XBOX_X.disconnect()
    camera_control.XBOX_Y.disconnect()
    camera_control.XBOX_LZ.disconnect()
    camera_control.XBOX_RZ.disconnect()


def start():

    # setup scenegraph
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")

    #environment:
    tri_mesh_loader = avango.gua.nodes.TriMeshLoader()

    tower = tri_mesh_loader.create_geometry_from_file(
        "tower",
        "/opt/project_animation/highrise/tower.fbx",
        avango.gua.LoaderFlags.MAKE_PICKABLE
        | avango.gua.LoaderFlags.LOAD_MATERIALS)

    tower.Transform.value = \
        tower.Transform.value * \
        avango.gua.make_rot_mat(235.0, 0.0, 1.0, 0.0)

    environment = tri_mesh_loader.create_geometry_from_file(
        "environment",
        "/opt/project_animation/highrise/environment.fbx",
        avango.gua.LoaderFlags.MAKE_PICKABLE
        | avango.gua.LoaderFlags.LOAD_MATERIALS)

    environment.Transform.value = \
        environment.Transform.value * \
        avango.gua.make_scale_mat(0.1) * \
        avango.gua.make_rot_mat(235.0, 0.0, 1.0, 0.0)

    sunlight = avango.gua.nodes.LightNode(
        Type=avango.gua.LightType.SUN,
        Name="light",
        Color=avango.gua.Color(245.0/255.0, 246.0/255.0, 178.0/255.0),
        Brightness=5.0,
        Transform=(avango.gua.make_rot_mat(119.5, 0.0, 1.0, 0.0) *
                   avango.gua.make_rot_mat(-10, 1.0, 0.0, 0.0))
    )

    #view setup:
    size = avango.gua.Vec2ui(2560, 1440)

    window = avango.gua.nodes.GlfwWindow(
        Size=size,
        LeftResolution=size
    )
    window.CursorMode.value = 2
    window.EnableFullscreen.value = True

    def handle_key(ascii, unknown, event, unknown2):

        global character_controls
        global ground_followings

        if(ascii == 82 and
           event == 1 and
           len(character_controls) > 0 and
           len(ground_followings) > 0):

            character_controls[current_character].reset_transform()
            ground_followings[current_character].reset_transform()

        if ascii == 257 and event == 1:

            next_character()

    window.on_key_press(handle_key)

    cam = avango.gua.nodes.CameraNode(
        LeftScreenPath="/screen",
        SceneGraph="scenegraph",
        Resolution=size,
        OutputWindowName="window")

    #render pipeline
    pipeline_description = avango.gua.nodes.PipelineDescription(
        Passes=[
            avango.gua.nodes.TriMeshPassDescription(),
            avango.gua.nodes.LightVisibilityPassDescription(),
            avango.gua.skelanim.nodes.SkeletalAnimationPassDescription(),
            avango.gua.nodes.ResolvePassDescription(),
            avango.gua.nodes.SSAAPassDescription(),
            ])

    pipeline_description.Passes.value[3].EnableSSAO.value = True
    pipeline_description.Passes.value[3].SSAORadius.value = 2.0
    pipeline_description.Passes.value[3].SSAOIntensity.value = 2.0
    pipeline_description.Passes.value[3].BackgroundMode.value = 1
    pipeline_description.Passes.value[3].BackgroundTexture.value = \
        "/opt/avango/master/examples/picking/data/textures/skymap.jpg"
    pipeline_description.Passes.value[3].ToneMappingMode.value = \
        avango.gua.ToneMappingMode.LINEAR
    #pipeline_description.EnableABuffer.value = True

    cam.PipelineDescription.value = pipeline_description
    cam.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 0.4)
    cam.FarClip.value = 300
    cam.NearClip.value = 0.01

    global screen
    screen = avango.gua.nodes.ScreenNode(Name="screen", Width=0.8, Height=0.45)
    screen.Children.value = [cam]
    screen.Transform.value = avango.gua.make_trans_mat(0, 0.1, -2)

    graph.Root.value.Children.value = [screen, tower, environment, sunlight]

    avango.gua.register_window("window", window)

    #setup viewer
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]
    window.CursorMode.value = avango.gua.CursorMode.DISABLED

    # XBOX Controller
    global device_sensor
    device_sensor = avango.daemon.nodes.DeviceSensor(
        DeviceService=avango.daemon.DeviceService())
    device_sensor.Station.value = "device-xbox-1"

    xbox_listener = XBOX_Listener()
    xbox_listener.XBOX_BTN_B.connect_from(device_sensor.Button1)
    xbox_listener.XBOX_BTN_Y.connect_from(device_sensor.Button3)

    #unreal tournament character
    skel_mesh_loader = avango.gua.skelanim.nodes.SkeletalAnimationLoader()

    # #scenegraph setup:
    # #root    ->   bob_ground    ->        bob_nav    ->        bob
    # #             ground following        character control    geometry
    # bob_ground = avango.gua.nodes.TransformNode(Name="bob_ground")
    # graph.Root.value.Children.value.append(bob_ground)
    # bob_nav = avango.gua.nodes.TransformNode(Name="bob_nav")
    # #set initial position of character here:
    # #bob_nav.Transform.value = avango.gua.make_trans_mat(0.0,0.05,0.0)
    # bob = skel_mesh_loader.create_geometry_from_file(
    #     "bob",
    #     "/opt/project_animation/Assets/UnrealTournament/Characters/" +
    #     "Necris_Male/necris_male_ut4_SKELMESH.FBX",
    #     avango.gua.LoaderFlags.LOAD_MATERIALS
    #     | avango.gua.LoaderFlags.NORMALIZE_SCALE)
    # bob.Transform.value = \
    #     bob.Transform.value * \
    #     avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * \
    #     avango.gua.make_scale_mat(0.02, 0.02, 0.02)
    # #load animations from character settings
    # load_unreal_animations1(bob)

    # bob_ground.Children.value = [bob_nav]
    # bob_nav.Children.value = [bob]
    # #character control
    # character_control = CharacterControl()
    # character_control.my_constructor(bob, bob_nav,
    #                                  AnimationConfig("idle"), window)
    # #override keyboard events with xbox controller events:
    # #apply_character_control_settings1(character_control, device_sensor)
    # apply_character_control_settings1(character_control)
    # #wall detection
    # character_control.activate_wall_detection(0.0075, 0.009, "idle", graph)
    # character_controls.append(character_control)
    # #camera control
    # camera_control = CameraControl()
    # camera_control.my_constructor(bob, window, -17.0)
    # camera_controls.append(camera_control)
    # #ground following
    # ground_following = GroundFollowing(
    #     SceneGraph=graph,
    #     OffsetToGround=0.01,
    #     MaxDistanceToGround=100.0
    # )
    # ground_following.my_constructor(gravity=-0.000075)
    # ground_following.InTransform.connect_from(bob.WorldTransform)
    # ground_followings.append(ganfault_ground_following)
    # bob_ground.Transform.connect_from(ground_following.OutTransform)
    # graph.Root.value.Children.value.append(bob_ground)
    # #distance events
    # distance_events = DistanceEvents()
    # distance_events.my_constructor(character_control)
    # apply_distance_events(distance_events, ground_following)

    ganfault_nav = avango.gua.nodes.TransformNode(Name="ganfault_nav")
    ganfault_ground = avango.gua.nodes.TransformNode(Name="ganfault_ground")
    ganfault = skel_mesh_loader.create_geometry_from_file(
        "ganfault",
        "/opt/project_animation/Assets/Mixamo/" +
        "Ganfault/Mixamo_Ganfault_Aure.FBX",
        avango.gua.LoaderFlags.LOAD_MATERIALS
        | avango.gua.LoaderFlags.NORMALIZE_SCALE)
    ganfault.Transform.value = \
        ganfault.Transform.value * \
        avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * \
        avango.gua.make_scale_mat(0.02, 0.02, 0.02)
    load_mixamo_animations_ganfault(ganfault)
    ganfault_nav.Transform.value =  \
        ganfault_nav.Transform.value * \
        avango.gua.make_trans_mat(-0.26, 0.012, 0.376)
    ganfault_ground.Children.value = [ganfault_nav]
    ganfault_nav.Children.value = [ganfault]
    ganfault_character_control = CharacterControl()
    ganfault_character_control.my_constructor(ganfault, ganfault_nav,
                                              AnimationConfig("idle"),
                                              window)
    #apply_character_control_settings_mixamo(ganfault_character_control,
    #                                        device_sensor)
    apply_character_control_settings_mixamo(ganfault_character_control)
    character_controls.append(ganfault_character_control)
    #ganfault_character_control.listen_keyboard(False)
    ganfault_character_control.activate_wall_detection(0.0075,
                                                       0.009,
                                                       "idle",
                                                       graph)
    ganfault_camera_control = CameraControl()
    ganfault_camera_control.my_constructor(ganfault, window, -17.0)
    camera_controls.append(ganfault_camera_control)
    #ganfault_camera_control.listen_mouse(False)
    ganfault_ground_following = GroundFollowing(
        SceneGraph=graph,
        OffsetToGround=0.01,
        MaxDistanceToGround=100.0
    )
    ganfault_ground_following.my_constructor(gravity=-0.000075)
    ganfault_ground_following.InTransform.connect_from(
        ganfault.WorldTransform)
    ganfault_ground.Transform.connect_from(
        ganfault_ground_following.OutTransform)
    ground_followings.append(ganfault_ground_following)
    graph.Root.value.Children.value.append(ganfault_ground)

    kachujin_nav = avango.gua.nodes.TransformNode(Name="kachujin_nav")
    kachujin_ground = avango.gua.nodes.TransformNode(Name="kachujin_ground")
    kachujin = skel_mesh_loader.create_geometry_from_file(
        "kachujin",
        "/opt/project_animation/Assets/Mixamo/Kachujin/Kachujin_G_Rosales.FBX",
        avango.gua.LoaderFlags.LOAD_MATERIALS
        | avango.gua.LoaderFlags.NORMALIZE_SCALE)
    kachujin.Transform.value = \
        kachujin.Transform.value * \
        avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * \
        avango.gua.make_scale_mat(0.02, 0.02, 0.02)
    load_mixamo_animations_kachujin(kachujin)
    kachujin_nav.Transform.value = \
        kachujin_nav.Transform.value * \
        avango.gua.make_trans_mat(-0.576, 0.087, 0.813)
    kachujin_ground.Children.value = [kachujin_nav]
    kachujin_nav.Children.value = [kachujin]
    kachujin_character_control = CharacterControl()
    kachujin_character_control.my_constructor(kachujin, kachujin_nav,
                                              AnimationConfig("idle"), window)
    apply_character_control_settings_mixamo(kachujin_character_control)
    #apply_character_control_settings_mixamo(kachujin_character_control,
    #                                        device_sensor)
    character_controls.append(kachujin_character_control)
    kachujin_character_control.listen_keyboard(False)
    kachujin_character_control.activate_wall_detection(0.0075,
                                                       0.009,
                                                       "idle",
                                                       graph)
    kachujin_camera_control = CameraControl()
    kachujin_camera_control.my_constructor(kachujin, window, -17.0)
    camera_controls.append(kachujin_camera_control)
    kachujin_camera_control.listen_mouse(False)
    kachujin_ground_following = GroundFollowing(
        SceneGraph=graph,
        OffsetToGround=0.01,
        MaxDistanceToGround=100.0
    )
    kachujin_ground_following.my_constructor(gravity=-0.000075)
    kachujin_ground_following.InTransform.connect_from(kachujin.WorldTransform)
    kachujin_ground.Transform.connect_from(
        kachujin_ground_following.OutTransform)
    ground_followings.append(kachujin_ground_following)
    graph.Root.value.Children.value.append(kachujin_ground)

    maria_nav = avango.gua.nodes.TransformNode(Name="maria_nav")
    maria_ground = avango.gua.nodes.TransformNode(Name="maria_ground")
    maria = skel_mesh_loader.create_geometry_from_file(
        "maria",
        "/opt/project_animation/Assets/Mixamo/Maria/Maximo_Maria.FBX",
        avango.gua.LoaderFlags.LOAD_MATERIALS
        | avango.gua.LoaderFlags.NORMALIZE_SCALE)
    maria.Transform.value = \
        maria.Transform.value * \
        avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * \
        avango.gua.make_scale_mat(0.02, 0.02, 0.02)
    load_mixamo_animations_maria(maria)
    maria_nav.Transform.value = \
        maria_nav.Transform.value * \
        avango.gua.make_trans_mat(-0.927, 0.0762, 0.127)
    maria_ground.Children.value = [maria_nav]
    maria_nav.Children.value = [maria]
    maria_character_control = CharacterControl()
    maria_character_control.my_constructor(maria, maria_nav,
                                           AnimationConfig("idle"), window)
    apply_character_control_settings_mixamo(maria_character_control)
    #apply_character_control_settings_mixamo(maria_character_control,
    #                                        device_sensor)
    character_controls.append(maria_character_control)
    maria_character_control.listen_keyboard(False)
    maria_character_control.activate_wall_detection(0.0075,
                                                    0.009,
                                                    "idle",
                                                    graph)
    maria_camera_control = CameraControl()
    maria_camera_control.my_constructor(maria, window, -17.0)
    camera_controls.append(maria_camera_control)
    maria_camera_control.listen_mouse(False)
    maria_ground_following = GroundFollowing(
        SceneGraph=graph,
        OffsetToGround=0.01,
        MaxDistanceToGround=100.0
    )
    maria_ground_following.my_constructor(gravity=-0.000075)
    maria_ground_following.InTransform.connect_from(maria.WorldTransform)
    maria_ground.Transform.connect_from(maria_ground_following.OutTransform)
    ground_followings.append(maria_ground_following)
    graph.Root.value.Children.value.append(maria_ground)

    maw_nav = avango.gua.nodes.TransformNode(Name="maw_nav")
    maw_ground = avango.gua.nodes.TransformNode(Name="maw_ground")
    maw = skel_mesh_loader.create_geometry_from_file(
        "maw",
        "/opt/project_animation/Assets/Mixamo/Maw/Maw_J_Laygo.FBX",
        avango.gua.LoaderFlags.LOAD_MATERIALS
        | avango.gua.LoaderFlags.NORMALIZE_SCALE)
    maw.Transform.value = \
        maw.Transform.value * \
        avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * \
        avango.gua.make_scale_mat(0.04, 0.04, 0.04)
    load_mixamo_animations_maw(maw)
    maw_nav.Transform.value = \
        maw_nav.Transform.value * \
        avango.gua.make_trans_mat(-0.05, 0.055, 0.081)
    maw_ground.Children.value = [maw_nav]
    maw_nav.Children.value = [maw]
    maw_character_control = CharacterControl()
    maw_character_control.my_constructor(maw, maw_nav,
                                         AnimationConfig("idle"), window)
    apply_character_control_settings_mixamo(maw_character_control)
    #apply_character_control_settings_mixamo(maw_character_control,
    #                                        device_sensor)
    character_controls.append(maw_character_control)
    maw_character_control.listen_keyboard(False)
    maw_character_control.activate_wall_detection(0.0075,
                                                  0.009,
                                                  "idle",
                                                  graph)
    maw_camera_control = CameraControl()
    maw_camera_control.my_constructor(maw, window, -9.0)
    camera_controls.append(maw_camera_control)
    maw_camera_control.listen_mouse(False)
    maw_ground_following = GroundFollowing(
        SceneGraph=graph,
        OffsetToGround=0.01,
        MaxDistanceToGround=100.0
    )
    maw_ground_following.my_constructor(gravity=-0.000075)
    maw_ground_following.InTransform.connect_from(maw.WorldTransform)
    maw_ground.Transform.connect_from(maw_ground_following.OutTransform)
    ground_followings.append(maw_ground_following)
    graph.Root.value.Children.value.append(maw_ground)

    vampire_nav = avango.gua.nodes.TransformNode(Name="vampire_nav")
    vampire_ground = avango.gua.nodes.TransformNode(Name="vampire_ground")
    vampire = skel_mesh_loader.create_geometry_from_file(
        "vampire",
        "/opt/project_animation/Assets/Mixamo/Vampire/Vampire_A_Lusth.FBX",
        avango.gua.LoaderFlags.LOAD_MATERIALS
        | avango.gua.LoaderFlags.NORMALIZE_SCALE)
    vampire.Transform.value = \
        vampire.Transform.value * \
        avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * \
        avango.gua.make_scale_mat(0.02, 0.02, 0.02)
    load_mixamo_animations_vampire(vampire)
    vampire_nav.Transform.value = \
        vampire_nav.Transform.value * \
        avango.gua.make_trans_mat(0.163, 0.077, 0.992)
    vampire_ground.Children.value = [vampire_nav]
    vampire_nav.Children.value = [vampire]
    vampire_character_control = CharacterControl()
    vampire_character_control.my_constructor(vampire, vampire_nav,
                                             AnimationConfig("idle"), window)
    apply_character_control_settings_mixamo(vampire_character_control)
    #apply_character_control_settings_mixamo(vampire_character_control,
    #                                        device_sensor)
    character_controls.append(vampire_character_control)
    vampire_character_control.listen_keyboard(False)
    vampire_character_control.activate_wall_detection(0.0075,
                                                      0.009,
                                                      "idle",
                                                      graph)
    vampire_camera_control = CameraControl()
    vampire_camera_control.my_constructor(vampire, window, -17.0)
    camera_controls.append(vampire_camera_control)
    vampire_camera_control.listen_mouse(False)
    vampire_ground_following = GroundFollowing(
        SceneGraph=graph,
        OffsetToGround=0.01,
        MaxDistanceToGround=100.0
    )
    vampire_ground_following.my_constructor(gravity=-0.000075)
    vampire_ground_following.InTransform.connect_from(vampire.WorldTransform)
    vampire_ground.Transform.connect_from(
        vampire_ground_following.OutTransform)
    ground_followings.append(vampire_ground_following)
    graph.Root.value.Children.value.append(vampire_ground)

    if len(camera_controls) > 0:
        camera_control_xbox_connect(camera_controls[0], device_sensor)
        screen.Transform.connect_from(camera_controls[0].OutTransform)

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()

if __name__ == '__main__':
    start()
