#!/usr/bin/python3
# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# AVANGO is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# AVANGO is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
##########################################################################

'''
Learn how to setup and use DeviceDaemon and DeviceService to readout input data
from a HIDInput device.

This example manipulates the position of a displayed monkey
based on the output the device specified in mouse-daemon.py generates.
'''
import avango
import avango.script
import avango.gua
from examples_common.GuaVE import GuaVE


class MouseMove(avango.script.Script):

    # input field
    Resolution = avango.gua.SFVec2ui()

    # output field
    MatrixOut = avango.gua.SFMatrix4()
    MatrixOut.value = avango.gua.make_identity_mat()

    def on_move_cursor(self, mouse_pos):
        monkey_pos = avango.gua.Vec3(0.0, 0.0, 0.0)
        cursor_pos = avango.gua.Vec3(
            mouse_pos.x / self.Resolution.value.x - 0.5,
            mouse_pos.y / self.Resolution.value.y - 0.5,
            3.5)
        self.MatrixOut.value = avango.gua.make_look_at_mat_inv(
            monkey_pos, cursor_pos, avango.gua.Vec3(0.0, 1.0, 0.0)
            ) * avango.gua.make_rot_mat(180.0, 0.0, 1.0, 0.0)


def start():
    # setup scenegraph
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
    loader = avango.gua.nodes.TriMeshLoader()

    monkey = loader.create_geometry_from_file(
        "monkey",
        "data/objects/monkey.obj",
        avango.gua.LoaderFlags.NORMALIZE_SCALE)

    monkey.Material.value.set_uniform(
        "Color",
        avango.gua.Vec4(1.0, 0.766, 0.336, 1.0))
    monkey.Material.value.set_uniform("Roughness", 0.3)
    monkey.Material.value.set_uniform("Metalness", 1.0)

    transform = avango.gua.nodes.TransformNode(
        Children=[monkey]
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

    graph.Root.value.Children.value = [transform, light, screen]

    #setup viewer
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    # make monkey look at the mouse cursor
    mouse_move = MouseMove()
    mouse_move.Resolution.connect_from(window.LeftResolution)
    window.on_move_cursor(mouse_move.on_move_cursor)
    transform.Transform.connect_from(mouse_move.MatrixOut)

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()


if __name__ == '__main__':
    start()
