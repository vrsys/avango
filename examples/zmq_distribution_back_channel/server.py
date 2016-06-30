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
A distributed viewer setup
'''

import avango
import avango.script
import avango.gua
from avango.script import field_has_changed
import asyncio
import pickle

from examples_common.GuaVE import GuaVE

#avango.enable_logging(4, "server.log")
SERVER_IP = '127.0.0.1'

nettrans = avango.gua.nodes.NetTransform(
    Name="net",
    # specify role, ip, and port
    Groupname="AVSERVER|" + SERVER_IP + "|7432")


class TimedRotate(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.gua.SFMatrix4()

    @field_has_changed(TimeIn)
    def update(self):
        self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value * 2.0,
                                                       0.0, 1.0, 0.0)


def make_node_distributable(node):
    for child in node.Children.value:
        make_node_distributable(child)
    nettrans.distribute_object(node)

# setup scenegraph
graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
loader = avango.gua.nodes.TriMeshLoader()

gold_monkey = loader.create_geometry_from_file(
    "monkey", "../simple_example/data/objects/monkey.obj")
red_monkey = loader.create_geometry_from_file(
    "monkey", "../simple_example/data/objects/monkey.obj")

gold_monkey.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 0.766,
                                                                0.336, 1.0))
gold_monkey.Material.value.set_uniform("Roughness", 0.3)
gold_monkey.Material.value.set_uniform("Metalness", 1.0)

red_monkey.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 0.266,
                                                               0.136, 1.0))
red_monkey.Material.value.set_uniform("Roughness", 0.6)
red_monkey.Material.value.set_uniform("Metalness", 0.1)

mat_desc = avango.gua.nodes.MaterialShaderDescription()
mat_desc.load_from_file("data/materials/SimpleMaterial.gmd")
avango.gua.register_material_shader(mat_desc, "mat")

mat = avango.gua.nodes.Material(ShaderName="mat")

#gold_monkey.Material.value = mat
nettrans.distribute_object(mat)

transform1 = avango.gua.nodes.TransformNode(Children=[gold_monkey])
transform2 = avango.gua.nodes.TransformNode(
    Transform=avango.gua.make_trans_mat(-0.5, 0.0, 0),
    Children=[red_monkey])

light = avango.gua.nodes.LightNode(Type=avango.gua.LightType.POINT,
                                   Name="light",
                                   Color=avango.gua.Color(1.0, 1.0, 1.0),
                                   Brightness=100.0,
                                   Transform=avango.gua.make_trans_mat(1, 1, 5)
                                   * avango.gua.make_scale_mat(30, 30, 30))

monkey_transform1 = avango.gua.nodes.TransformNode(Name="monkey_transform1")
monkey_transform1.Transform.value = avango.gua.make_trans_mat(1.0, 0.0, 0.0)
monkey_transform1.Children.value = [gold_monkey]

monkey_transform2 = avango.gua.nodes.TransformNode(Name="monkey_transform2")
monkey_transform2.Transform.value = avango.gua.make_trans_mat(-1.0, 0.0, 0.0)
monkey_transform2.Children.value = [red_monkey]

group = avango.gua.nodes.TransformNode(Name="group")
group.Children.value = [monkey_transform1, monkey_transform2, light]

screen = avango.gua.nodes.ScreenNode(Name="screen", Width=4, Height=3)

size = avango.gua.Vec2ui(800, 600)

tri_pass = avango.gua.nodes.TriMeshPassDescription()
tquad_pass = avango.gua.nodes.TexturedQuadPassDescription()
lvis_pass = avango.gua.nodes.LightVisibilityPassDescription()
res_pass = avango.gua.nodes.ResolvePassDescription()
res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
tscreenspace_pass = avango.gua.nodes.TexturedScreenSpaceQuadPassDescription()

pipeline_description = avango.gua.nodes.PipelineDescription(
    EnableABuffer=True,
    Passes=[tri_pass, tquad_pass, lvis_pass, res_pass, tscreenspace_pass])

server_cam = avango.gua.nodes.CameraNode(
    ViewID=1,
    LeftScreenPath="/net/screen",
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="server_window",
    Transform=avango.gua.make_trans_mat(0.0, 0.0, 3.5),
    PipelineDescription=pipeline_description)

client_cam = avango.gua.nodes.CameraNode(
    ViewID=2,
    LeftScreenPath="/net/screen",
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="client_window",
    Transform=avango.gua.make_trans_mat(0.0, 0.0, 3.5),
    PipelineDescription=pipeline_description)

screen.Children.value = [client_cam, server_cam]
nettrans.Children.value = [group, screen]

graph.Root.value.Children.value = [nettrans]

make_node_distributable(group)
make_node_distributable(screen)

nettrans.distribute_object(tri_pass)
nettrans.distribute_object(tquad_pass)
nettrans.distribute_object(lvis_pass)
nettrans.distribute_object(res_pass)
nettrans.distribute_object(tscreenspace_pass)

for p in pipeline_description.Passes.value:
    nettrans.distribute_object(p)
nettrans.distribute_object(pipeline_description)

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

red_monkey.Transform.connect_from(monkey_updater.MatrixOut)


@asyncio.coroutine
def viewer_loop():
    while True:
        viewer.frame()
        yield from asyncio.sleep(0.01)


@asyncio.coroutine
def handle_matrix(reader, writer):
    print("handle_matrix")
    while not reader.at_eof():
        nr_of_bytes = 152
        #data = await reader.read(nr_of_bytes)
        serialized_matrix = yield from reader.read(nr_of_bytes)
        if serialized_matrix:
            matrix = pickle.loads(serialized_matrix)
            gold_monkey.Transform.value = avango.gua.from_list(matrix)
            writer.write(b'ok')
            yield from writer.drain()
        else:
            print("no data")
        #yield from asyncio.sleep(0.01)
    writer.close()


loop = asyncio.get_event_loop()

# schedule viewer_loop
asyncio.gather(viewer_loop())
# schedule server
asyncio.gather(asyncio.start_server(handle_matrix, SERVER_IP, 8888, loop=loop))

print("run viewer loop")
try:
    loop.run_forever()
except KeyboardInterrupt:
    pass

loop.stop()
loop.close()
