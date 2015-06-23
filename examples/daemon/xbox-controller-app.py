import avango
import avango.daemon
import avango.gua
import avango.script
from examples_common.GuaVE import GuaVE

from random import random


class RandomColor(avango.script.Script):

    # input field
    Trigger = avango.SFBool()

    # output field
    ColorOut = avango.gua.SFColor()

    def evaluate(self):
        if self.Trigger.value:  # only update color on button down
            self.ColorOut.value = avango.gua.Color(
                0.5 * random(), 0.5 * random(), 0.5 * random()
                )


class DeviceRotate(avango.script.Script):

    # input fields from device
    RotX = avango.SFFloat()
    RotY = avango.SFFloat()
    RotZ = avango.SFFloat()

    # output field: rotation matrix
    MatrixOut = avango.gua.SFMatrix4()

    def evaluate(self):
        self.MatrixOut.value = \
            avango.gua.make_rot_mat(self.RotX.value * 90.0, 1.0, 0.0, 0.0) * \
            avango.gua.make_rot_mat(self.RotY.value * 90.0, 0.0, 1.0, 0.0) * \
            avango.gua.make_rot_mat(self.RotZ.value * 90.0, 0.0, 0.0, 1.0)


def start():
    # setup scenegraph
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
    loader = avango.gua.nodes.TriMeshLoader()

    left_monkey = loader.create_geometry_from_file(
        "monkey",
        "data/objects/monkey.obj",
        avango.gua.LoaderFlags.NORMALIZE_SCALE)

    right_monkey = loader.create_geometry_from_file(
        "monkey",
        "data/objects/monkey.obj",
        avango.gua.LoaderFlags.NORMALIZE_SCALE)

    left_monkey.Material.value.set_uniform(
        "Color", avango.gua.Vec4(1.0, 0.766, 0.336, 1.0))
    left_monkey.Material.value.set_uniform("Roughness", 0.3)
    left_monkey.Material.value.set_uniform("Metalness", 1.0)

    right_monkey.Material.value.set_uniform(
        "Color", avango.gua.Vec4(1.0, 0.266, 0.136, 1.0))
    right_monkey.Material.value.set_uniform("Roughness", 0.6)
    right_monkey.Material.value.set_uniform("Metalness", 0.0)

    left_rot_transform = avango.gua.nodes.TransformNode(
        Children=[left_monkey]
        )
    left_pos_transform = avango.gua.nodes.TransformNode(
        Transform=avango.gua.make_trans_mat(-0.5, 0.0, 0.0),
        Children=[left_rot_transform]
        )
    right_rot_transform = avango.gua.nodes.TransformNode(
        Children=[right_monkey]
        )
    right_pos_transform = avango.gua.nodes.TransformNode(
        Transform=avango.gua.make_trans_mat(0.5, 0.0, 0.0),
        Children=[right_rot_transform]
        )

    light = avango.gua.nodes.LightNode(
        Type=avango.gua.LightType.POINT,
        Name="light",
        Color=avango.gua.Color(1.0, 1.0, 1.0),
        Brightness=100.0,
        Transform=(avango.gua.make_trans_mat(1, 1, 5) *
                   avango.gua.make_scale_mat(30, 30, 30))
        )

    size = avango.gua.Vec2ui(1024, 768)

    window = avango.gua.nodes.GlfwWindow(
        Size=size,
        LeftResolution=size
        )

    avango.gua.register_window("window", window)

    cam = avango.gua.nodes.CameraNode(
        LeftScreenPath="/screen",
        SceneGraph="scenegraph",
        Resolution=size,
        OutputWindowName="window",
        Transform=avango.gua.make_trans_mat(0.0, 0.0, 3.5)
        )

    res_pass = avango.gua.nodes.ResolvePassDescription()
    res_pass.EnableSSAO.value = True
    res_pass.SSAOIntensity.value = 4.0
    res_pass.SSAOFalloff.value = 10.0
    res_pass.SSAORadius.value = 7.0

    #res_pass.EnableScreenSpaceShadow.value = True

    res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1, 0.1, 0.1)
    res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
    res_pass.Exposure.value = 1.0
    res_pass.BackgroundColor.value = avango.gua.Color(0.45, 0.5, 0.6)

    anti_aliasing = avango.gua.nodes.SSAAPassDescription()

    pipeline_description = avango.gua.nodes.PipelineDescription(
        Passes=[
            avango.gua.nodes.TriMeshPassDescription(),
            avango.gua.nodes.LightVisibilityPassDescription(),
            res_pass,
            anti_aliasing,
            ])

    cam.PipelineDescription.value = pipeline_description

    screen = avango.gua.nodes.ScreenNode(
        Name="screen",
        Width=2,
        Height=1.5,
        Children=[cam]
        )

    graph.Root.value.Children.value = [
        left_pos_transform,
        right_pos_transform,
        light,
        screen
        ]

    #setup viewer
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    # device sensor listening to the daemon values
    device = avango.daemon.nodes.DeviceSensor(
        DeviceService=avango.daemon.DeviceService()
        )
    # station name determines which device is used
    device.Station.value = "gua-device-xbox-0"  # 0 for first xbox controller

    # field container to set rotation of left monkey
    left_rotation_updater = DeviceRotate()
    left_rotation_updater.RotY.connect_from(device.Value0)
    left_rotation_updater.RotX.connect_from(device.Value1)
    left_rot_transform.Transform.connect_from(left_rotation_updater.MatrixOut)

    # field container to set rotation of right monkey
    right_rotation = DeviceRotate()
    right_rotation.RotY.connect_from(device.Value2)
    right_rotation.RotX.connect_from(device.Value3)
    right_rot_transform.Transform.connect_from(right_rotation.MatrixOut)

    # all buttons change the background color
    bgcolor_updater = RandomColor()
    bgcolor_updater.Trigger.connect_from(device.Button0)
    bgcolor_updater.Trigger.connect_from(device.Button1)
    bgcolor_updater.Trigger.connect_from(device.Button2)
    bgcolor_updater.Trigger.connect_from(device.Button3)
    bgcolor_updater.Trigger.connect_from(device.Button4)
    bgcolor_updater.Trigger.connect_from(device.Button5)
    bgcolor_updater.Trigger.connect_from(device.Button6)
    bgcolor_updater.Trigger.connect_from(device.Button7)
    bgcolor_updater.Trigger.connect_from(device.Button8)
    bgcolor_updater.Trigger.connect_from(device.Button9)
    res_pass.BackgroundColor.connect_from(bgcolor_updater.ColorOut)

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()


if __name__ == '__main__':
    start()
