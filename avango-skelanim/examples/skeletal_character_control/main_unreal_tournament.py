import avango
import avango.script
import avango.gua.skelanim
from examples_common.GuaVE import GuaVE

from avango.gua.skelanim.GroundFollowing import GroundFollowing
from avango.gua.skelanim.CharacterControl import CharacterControl
from avango.gua.skelanim.CameraControl import CameraControl
from avango.gua.skelanim.DistanceEvents import DistanceEvents
from avango.gua.skelanim.AnimationControl import AnimationConfig
from CharacterSettings import *


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

    plane = tri_mesh_loader.create_geometry_from_file(
        "cube",
        "/opt/project_animation/demo/data/objects/cube2.obj",
        avango.gua.LoaderFlags.NORMALIZE_POSITION
        | avango.gua.LoaderFlags.NORMALIZE_SCALE
        | avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY
        | avango.gua.LoaderFlags.MAKE_PICKABLE
        | avango.gua.LoaderFlags.LOAD_MATERIALS)

    plane.Transform.value *= \
        avango.gua.make_scale_mat(10.0, 0.01, 10.0) *  \
        avango.gua.make_trans_mat(0, -3, 0)

    plane.Material.value.set_uniform(
        "NormalMap",
        "/opt/project_animation/demo/data/objects/glass_2_3_nm.TGA")

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

    screen = avango.gua.nodes.ScreenNode(Name="screen", Width=0.8, Height=0.45)
    screen.Children.value = [cam]
    screen.Transform.value = avango.gua.make_trans_mat(0, 0.1, -2)

    graph.Root.value.Children.value = [screen, plane, sunlight]

    avango.gua.register_window("window", window)

    #setup viewer
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]
    window.CursorMode.value = avango.gua.CursorMode.DISABLED

    #XBOX Controller
    device_sensor = avango.daemon.nodes.DeviceSensor(
        DeviceService=avango.daemon.DeviceService())
    device_sensor.Station.value = "device-xbox-1"

    #unreal tournament character
    skel_mesh_loader = avango.gua.skelanim.nodes.SkeletalAnimationLoader()

    #scenegraph setup:
    #root    ->   bob_ground    ->        bob_nav    ->        bob
    #             ground following        character control    geometry
    bob_ground = avango.gua.nodes.TransformNode(Name="bob_ground")
    graph.Root.value.Children.value.append(bob_ground)
    bob_nav = avango.gua.nodes.TransformNode(Name="bob_nav")
    #set initial position of character here:
    #bob_nav.Transform.value = avango.gua.make_trans_mat(0.0,0.05,0.0)

    bob = skel_mesh_loader.create_geometry_from_file(
        "bob",
        "/opt/project_animation/Assets/UnrealTournament/Characters/" +
        "Necris_Male/necris_male_ut4_SKELMESH.FBX",
        avango.gua.LoaderFlags.LOAD_MATERIALS
        | avango.gua.LoaderFlags.NORMALIZE_SCALE)

    bob.Transform.value = \
        bob.Transform.value * \
        avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * \
        avango.gua.make_scale_mat(0.02, 0.02, 0.02)

    #load animations from character settings
    load_unreal_animations1(bob)

    bob_ground.Children.value = [bob_nav]
    bob_nav.Children.value = [bob]

    #character control
    character_control = CharacterControl()
    character_control.my_constructor(bob, bob_nav,
                                     AnimationConfig("idle"), window)
    #override keyboard events with xbox controller events:
    #apply_character_control_settings1(character_control, device_sensor)
    apply_character_control_settings1(character_control)
    #wall detection
    character_control.activate_wall_detection(0.0075, 0.009, "idle", graph)

    #camera control
    camera_control = CameraControl()
    camera_control.my_constructor(bob, window, -17.0)
    screen.Transform.connect_from(camera_control.OutTransform)
    #camera_control_xbox_connect(camera_control, device_sensor)

    #ground following
    ground_following = GroundFollowing(
        SceneGraph=graph,
        OffsetToGround=0.01,
        MaxDistanceToGround=100.0
    )
    ground_following.my_constructor(gravity=-0.00005)
    ground_following.InTransform.connect_from(bob.WorldTransform)
    bob_ground.Transform.connect_from(ground_following.OutTransform)

    #distance events
    distance_events = DistanceEvents()
    distance_events.my_constructor(character_control)
    apply_distance_events(distance_events, ground_following)

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()

if __name__ == '__main__':
    start()
