# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       #
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
A distributed viewer setup: This Python scripts starts an
avango.osg.simpleviewer to load a given geometry. Any client connected to the
group "testgroup" should receive this model. (see also simpleviewer-clnt.py)
'''

import avango
import avango.script
import avango.gua
from avango.script import field_has_changed

import examples_common.navigator

from examples_common.GuaVE import GuaVE

#avango.enable_logging(4, "server.log")

nettrans = avango.gua.nodes.NetTransform(Name="net",
                                         # specify role, ip, and port
                                         Groupname="AVSERVER|141.54.147.52|7432")


class TimedRotate(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.gua.SFMatrix4()

    @field_has_changed(TimeIn)
    def update(self):
        self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value * 2.0,
                                                       0.0, 1.0, 0.0) * avango.gua.make_scale_mat(0.7)


def make_node_distributable(node):
    for child in node.Children.value:
        make_node_distributable(child)
    nettrans.distribute_object(node)


def make_material_distributable(mat):
    nettrans.distribute_object(mat)

# setup scenegraph
graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
loader = avango.gua.nodes.TriMeshLoader()

teapot = loader.create_geometry_from_file(
    "teapot", "./data/objects/teapot.obj")
teapot.Transform.value = avango.gua.make_scale_mat(0.3, 0.3, 0.3)
teapot.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 0.766, 0.336,
                                                            1.0))
teapot.Material.value.set_uniform("Roughness", 0.3)
teapot.Material.value.set_uniform("Metalness", 1.0)


mat_desc = avango.gua.nodes.MaterialShaderDescription()
mat_desc.load_from_file("data/materials/SimpleMaterial.gmd")
avango.gua.register_material_shader(mat_desc, "mat")

mat = avango.gua.nodes.Material(ShaderName="mat")

#teapot.Material.value = mat
nettrans.distribute_object(mat)

spointsloader = avango.gua.nodes.SPointsLoader()
spoints_geode = spointsloader.load("kinect", "/home/wabi7015/Programming/avango/examples/spoints/spoints_resource_file.sr")

transform1 = avango.gua.nodes.TransformNode(Children=[spoints_geode])
transform2 = avango.gua.nodes.TransformNode(
    Transform=avango.gua.make_trans_mat(-0.5, 0.0, 0),
    Children=[spoints_geode])

light = avango.gua.nodes.LightNode(Type=avango.gua.LightType.POINT,
                                   Name="light",
                                   Color=avango.gua.Color(1.0, 1.0, 1.0),
                                   Brightness=100.0,
                                   Transform=avango.gua.make_trans_mat(1, 1, 5)
                                   * avango.gua.make_scale_mat(30, 30, 30))

monkey_transform1 = avango.gua.nodes.TransformNode(Name="monkey_transform1")
monkey_transform1.Transform.value = avango.gua.make_trans_mat(1.0, 0.0, 0.0)
monkey_transform1.Children.value = [teapot]

monkey_transform2 = avango.gua.nodes.TransformNode(Name="monkey_transform2")
monkey_transform2.Transform.value = avango.gua.make_trans_mat(-1.0, 0.0, 0.0)
monkey_transform2.Children.value = [spoints_geode]

group = avango.gua.nodes.TransformNode(Name="group")
group.Children.value = [monkey_transform2, light]

screen = avango.gua.nodes.ScreenNode(Name="screen", Width=4, Height=3)

size = avango.gua.Vec2ui(800, 600)

tri_pass = avango.gua.nodes.TriMeshPassDescription()
tquad_pass = avango.gua.nodes.TexturedQuadPassDescription()
lvis_pass = avango.gua.nodes.LightVisibilityPassDescription()
res_pass = avango.gua.nodes.ResolvePassDescription()
res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
tscreenspace_pass = avango.gua.nodes.TexturedScreenSpaceQuadPassDescription()

occlusion_slave_res_pass = avango.gua.nodes.OcclusionSlaveResolvePassDescription()

#pipeline_description = avango.gua.nodes.PipelineDescription(
#    EnableABuffer=True,
#    Passes=[tri_pass, tquad_pass, lvis_pass, res_pass, tscreenspace_pass])

pipeline_description = avango.gua.nodes.PipelineDescription(
    Passes=[
        avango.gua.nodes.TriMeshPassDescription(),
        avango.gua.nodes.LightVisibilityPassDescription(),
        avango.gua.nodes.SPointsPassDescription(),
        res_pass
    ])

occlusion_slave_pipeline_description = avango.gua.nodes.PipelineDescription(
    Passes=[
        avango.gua.nodes.TriMeshPassDescription(),
        avango.gua.nodes.LightVisibilityPassDescription(),
        avango.gua.nodes.SPointsPassDescription(),
        occlusion_slave_res_pass
    ])

server_cam = avango.gua.nodes.CameraNode(
    ViewID=1,
    LeftScreenPath="/net/screen",
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="server_window",
    Transform=avango.gua.make_trans_mat(0.0, 0.0, 3.5),
    PipelineDescription=pipeline_description,
    BlackList = ["invisible"]
    )

client_cam = avango.gua.nodes.CameraNode(
    ViewID=2,
    Name="viewer_0_weimar",
    LeftScreenPath="/net/screen",
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="client_window_weimar",
    Transform=avango.gua.make_trans_mat(0.0, 0.0, 3.5),
    PipelineDescription=pipeline_description)

occlusion_slave_client_cam = avango.gua.nodes.CameraNode(
    ViewID=3,
    Name="os_weimar_v0_osaka",
    LeftScreenPath="/net/screen",
    RightScreenPath="/net/screen",
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="slave_weimar_v0_osaka",
    Transform=avango.gua.make_trans_mat(0.0, 0.0, 3.5),
    PipelineDescription=occlusion_slave_pipeline_description,
    #PipelineDescription=pipeline_description,
    EyeDistance = 0.06,
    EnableStereo = True)

screen.Children.value = [occlusion_slave_client_cam, client_cam, server_cam, monkey_transform1]
nettrans.Children.value = [group, screen]

screen.Transform.value = avango.gua.make_trans_mat(0.0, 1.0, 2.5)

navigator = examples_common.navigator.Navigator()
navigator.StartLocation.value = screen.Transform.value.get_translate()
navigator.OutTransform.connect_from(screen.Transform)

navigator.RotationSpeed.value = 0.12/3.0
navigator.MotionSpeed.value = 0.07

spoints_geode.Tags.value = ["invisible"]
#spoints_geode.IsServerResource.value = True

screen.Transform.connect_from(navigator.OutTransform)

graph.Root.value.Children.value = [nettrans]

make_node_distributable(group)
make_node_distributable(monkey_transform1)
make_node_distributable(screen)

nettrans.distribute_object(tri_pass)
nettrans.distribute_object(tquad_pass)
nettrans.distribute_object(lvis_pass)
nettrans.distribute_object(res_pass)
nettrans.distribute_object(tscreenspace_pass)
nettrans.distribute_object(occlusion_slave_res_pass)
for p in pipeline_description.Passes.value:
    nettrans.distribute_object(p)
nettrans.distribute_object(pipeline_description)

for p in occlusion_slave_pipeline_description.Passes.value:
    nettrans.distribute_object(p)
nettrans.distribute_object(occlusion_slave_pipeline_description)

# setup viewing
window = avango.gua.nodes.GlfwWindow(Size=size,
                                     LeftResolution=size,
                                     Title="server_window")

avango.gua.register_window("server_window", window)

#setup viewer
viewer = avango.gua.nodes.Viewer()
viewer.SceneGraphs.value = [graph]
viewer.Windows.value = [window]

monkey_updater = TimedRotate()

timer = avango.nodes.TimeSensor()
monkey_updater.TimeIn.connect_from(timer.Time)

teapot.Transform.connect_from(monkey_updater.MatrixOut)

guaVE = GuaVE()
guaVE.start(locals(), globals())

viewer.run()