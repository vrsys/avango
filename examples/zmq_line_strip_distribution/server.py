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

from examples_common.GuaVE import GuaVE

import math
import random
#avango.enable_logging(4, "server.log")

nettrans = avango.gua.nodes.NetTransform(Name="net",
                                         # specify role, ip, and port
                                         Groupname="AVSERVER|127.0.0.1|7432")


class CreateSpiralPerFrameScript(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.gua.SFMatrix4()

    def __init__(self):
        self.super(CreateSpiralPerFrameScript).__init__()
        self.FrameCount = 0
        self.StartCounting = False
        self.CountForward = True


    def set_line_strip_node(self, line_strip_node):
        self.node_to_update = line_strip_node
        self.always_evaluate(True)
        self.StartCounting = True

    def evaluate(self):
        if self.StartCounting == True:
            if self.CountForward:
                self.FrameCount += 1
            else:
                self.FrameCount -= 1

            if self.FrameCount >= 0:
                

                """ 
                    LINE DATA DISTRIBUTION STEP 1
                    =============================
                Start a new vertex list for the node with
                 .start_vertex_list()

                method

                This clears the data of the node on the 
                server side and keeps the server and client
                sides consistent
                """
                self.node_to_update.start_vertex_list()
   
                # create FrameCount vertices for the spiral
                for i in range(self.FrameCount):
                    b = 0.1;
                    a = 6.28;
                    spiral_float_parameter = i * 0.1;

                    norm_pos_x = b * spiral_float_parameter * math.cos(spiral_float_parameter + a);
                    norm_pos_y = spiral_float_parameter * 0.05;
                    norm_pos_z = b * spiral_float_parameter * math.sin(spiral_float_parameter + a);

                    vertex_x = norm_pos_x
                    vertex_y = norm_pos_y
                    vertex_z = norm_pos_z
                    col_r = random.uniform(0.0, 1.0)
                    col_g = random.uniform(0.0, 1.0)
                    col_b = random.uniform(0.0, 1.0)
                    thickness = 0.02

                    """ 
                    LINE DATA DISTRIBUTION STEP 2
                    =============================
                    Enqueue vertices to the vertex list with the
                     .enqueue_vertex(float pos_x, float pos_y, float pos_z,
                                     float col_r, float col_g, float col_b,
                                     float thickness)

                    method

                        
                    the thickness only plays a role for the line width in volumetric mode

                    the field ScreenSpaceLineWidth plays a role for the line width in non-volumetric mode

                    This clears the data of the node on the 
                    server side and keeps the server and client
                    sides consistent
                    """
                    self.node_to_update.enqueue_vertex(norm_pos_x/5.0, norm_pos_y / 5.0 - 1.0, norm_pos_z/5.0, col_r, col_g, col_b, thickness)

                    #attributes for rendering volumetric and linesize in non_volumetric node
                    self.node_to_update.RenderVolumetric.value = False if self.FrameCount % 60 < 30 else True
                    self.node_to_update.ScreenSpaceLineWidth.value = 2.0
                
                    """ 
                    LINE DATA DISTRIBUTION STEP 3
                    =============================
                    End the vertex list for the node with
                     .end_vertex_list()

                    method

                    This replaces all vertices for the server side node with
                    the ones defined in the list and sends this state to the
                    client afterwards
                    server side and keeps the server and client
                    sides consistent
                    """
                self.node_to_update.end_vertex_list()

            #switch spiral movement
            if self.CountForward == True and self.FrameCount > 1300:
                self.CountForward = False

            elif self.CountForward == False and self.FrameCount < 1:
                self.CountForward = True

    #@field_has_changed(TimeIn)
    #def update(self):
    #    self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value * 2.0,
    #                                                   0.0, 1.0, 0.0)



def make_node_distributable(node):
    for child in node.Children.value:
        make_node_distributable(child)
    nettrans.distribute_object(node)


def make_material_distributable(mat):
    nettrans.distribute_object(mat)

# setup scenegraph
graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
loader = avango.gua.nodes.TriMeshLoader()
line_loader = avango.gua.nodes.LineStripLoader()

monkey = loader.create_geometry_from_file(
    "monkey", "../simple_example/data/objects/monkey.obj")

monkey.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 0.766, 0.336,
                                                            1.0))
monkey.Material.value.set_uniform("Roughness", 0.3)
monkey.Material.value.set_uniform("Metalness", 1.0)


# empty nodes are used for adding geometry dynamically
line_strip_geode = line_loader.create_empty_geometry("lines", "empty.lob")
line_strip_geode.RenderVolumetric.value = False


mat_desc = avango.gua.nodes.MaterialShaderDescription()
mat_desc.load_from_file("data/materials/SimpleMaterial.gmd")
avango.gua.register_material_shader(mat_desc, "mat")

mat = avango.gua.nodes.Material(ShaderName="mat")

nettrans.distribute_object(mat)



light = avango.gua.nodes.LightNode(Type=avango.gua.LightType.POINT,
                                   Name="light",
                                   Color=avango.gua.Color(1.0, 1.0, 1.0),
                                   Brightness=100.0,
                                   Transform=avango.gua.make_trans_mat(1, 1, 5)
                                   * avango.gua.make_scale_mat(30, 30, 30))

monkey_transform = avango.gua.nodes.TransformNode(Name="monkey_transform")
monkey_transform.Transform.value = avango.gua.make_trans_mat(-0.00, 0.0, -0.5) * avango.gua.make_scale_mat(0.7)
monkey_transform.Children.value = [monkey]

group = avango.gua.nodes.TransformNode(Name="group")
group.Children.value = [line_strip_geode, monkey_transform, light]

screen = avango.gua.nodes.ScreenNode(Name="screen", Width=4, Height=3)

size = avango.gua.Vec2ui(800, 600)

line_pass = avango.gua.nodes.LineStripPassDescription()
tri_pass = avango.gua.nodes.TriMeshPassDescription()
tquad_pass = avango.gua.nodes.TexturedQuadPassDescription()
lvis_pass = avango.gua.nodes.LightVisibilityPassDescription()
res_pass = avango.gua.nodes.ResolvePassDescription()
res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
tscreenspace_pass = avango.gua.nodes.TexturedScreenSpaceQuadPassDescription()

pipeline_description = avango.gua.nodes.PipelineDescription(
    EnableABuffer=True,
    Passes=[line_pass, tri_pass, tquad_pass, lvis_pass, res_pass, tscreenspace_pass])

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

nettrans.distribute_object(line_pass)
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

spiral_creator = CreateSpiralPerFrameScript()

#add linestrip to the spiral creator to create new vertices every frame
spiral_creator.set_line_strip_node(line_strip_geode)


guaVE = GuaVE()
guaVE.start(locals(), globals())

viewer.run()
