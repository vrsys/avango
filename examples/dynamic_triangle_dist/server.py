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
from avango.script import field_has_changed
import avango.gua
import avango.gua.lod

from examples_common.GuaVE import GuaVE
from LocalizedImage import LocalizedImageQuad
# from PhotoProjection import PhotoProjection
from MyProjector import MyProjector

import math
import random
#avango.enable_logging(4, "server.log")

nettrans = avango.gua.nodes.NetTransform(Name="net",
                                         # specify role, ip, and port
                                         Groupname="AVSERVER|127.0.0.1|7432")

## print the subgraph under a given node to the console
def print_graph(root_node):
    stack = [(root_node, 0)]
    while stack:
        node, level = stack.pop()
        print("│   " * level + "├── {0} <{1}>".format(
          node.Name.value, node.__class__.__name__))
        stack.extend(
          [(child, level + 1) for child in reversed(node.Children.value)])


class UpdateDynamicTriangleScript(avango.script.Script):
    def __init__(self):
        self.super(UpdateDynamicTriangleScript).__init__()
        self.StartCounting = False
        self.FrameCount = 0

    def set_dynamic_triangle_node(self, dynamic_triangle_node):
        self.node_to_update = dynamic_triangle_node
        self.always_evaluate(True)
        self.StartCounting = True

    def evaluate(self):
        if self.StartCounting == True:  
            self.FrameCount += 1   

            self.node_to_update.start_vertex_list()
            # quad_transform = avango.gua.make_trans_mat(0.5 * math.cos(self.FrameCount/100), 0.3*math.sin(self.FrameCount/100), 2.0)
            quad_transform = avango.gua.make_trans_mat(0.0, 0.0, 2.0)

            pos = quad_transform * avango.gua.Vec3( quad_size, quad_size, 0.0)
            uv  = avango.gua.Vec2(1.0, 0.0)
            dynamic_tri_node.enqueue_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)

            pos = quad_transform * avango.gua.Vec3(-quad_size, -quad_size, 0.0)
            uv  = avango.gua.Vec2(0.0, 1.0)
            dynamic_tri_node.enqueue_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)

            pos = quad_transform * avango.gua.Vec3( quad_size, -quad_size, 0.0)
            uv  = avango.gua.Vec2(1.0, 1.0)
            dynamic_tri_node.enqueue_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)

            pos = quad_transform * avango.gua.Vec3( quad_size, quad_size, 0.0)
            uv  = avango.gua.Vec2(1.0, 0.0)
            dynamic_tri_node.enqueue_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)

            pos = quad_transform * avango.gua.Vec3(-quad_size, quad_size, 0.0)
            uv  = avango.gua.Vec2(0.0, 0.0)
            dynamic_tri_node.enqueue_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)

            pos = quad_transform * avango.gua.Vec3(-quad_size, -quad_size, 0.0)
            uv  = avango.gua.Vec2(0.0, 1.0)
            dynamic_tri_node.enqueue_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)
            
            self.node_to_update.end_vertex_list()


class UpdateImagesScript(avango.script.Script):
    def __init__(self):
        self.super(UpdateImagesScript).__init__()
        self.StartCounting = False
        self.FrameCount = 0

    def set_dynamic_triangle_node(self, dynamic_triangle_node, image_list):
        self.node_to_update = dynamic_triangle_node
        self.always_evaluate(True)
        self.image_list = image_list
        self.StartCounting = True

    def evaluate(self):
        if self.StartCounting == True:  
            self.FrameCount += 1   

            self.node_to_update.start_vertex_list()

            for img in self.image_list:
                # quad_transform = avango.gua.make_trans_mat(0.5 * math.cos(self.FrameCount/100), 0.3*math.sin(self.FrameCount/100), 2.0)
                img_transform = img.transform

                pos = img_transform * avango.gua.Vec3( img.img_w_half, img.img_h_half, 0.0)
                uv  = avango.gua.Vec2(img.tile_pos_x, img.tile_pos_y)
                self.node_to_update.enqueue_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)

                pos = img_transform * avango.gua.Vec3(-img.img_w_half, -img.img_h_half, 0.0)
                uv  = avango.gua.Vec2(img.tile_pos_x + img.tile_w, img.tile_pos_y + img.tile_h)
                self.node_to_update.enqueue_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)

                pos = img_transform * avango.gua.Vec3( img.img_w_half, -img.img_h_half, 0.0)
                uv  = avango.gua.Vec2(img.tile_pos_x, img.tile_pos_y + img.tile_h)
                self.node_to_update.enqueue_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)

                pos = img_transform * avango.gua.Vec3( img.img_w_half, img.img_h_half, 0.0)
                uv  = avango.gua.Vec2(img.tile_pos_x, img.tile_pos_y)
                self.node_to_update.enqueue_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)

                pos = img_transform * avango.gua.Vec3(-img.img_w_half, img.img_h_half, 0.0)
                uv  = avango.gua.Vec2(img.tile_pos_x + img.tile_w, img.tile_pos_y)
                self.node_to_update.enqueue_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)

                pos = img_transform * avango.gua.Vec3(-img.img_w_half, -img.img_h_half, 0.0)
                uv  = avango.gua.Vec2(img.tile_pos_x + img.tile_w, img.tile_pos_y + img.tile_h)
                self.node_to_update.enqueue_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)
            
            self.node_to_update.end_vertex_list()



def make_node_distributable(node):
    for child in node.Children.value:
        make_node_distributable(child)
    nettrans.distribute_object(node)


def make_material_distributable(mat):
    nettrans.distribute_object(mat)

aux_path = "/home/ephtron/Documents/master-render-files/salem/salem_atlas.aux"
atlas_path = "/home/ephtron/Documents/master-render-files/salem/salem.atlas"

# setup scenegraph
graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
loader = avango.gua.nodes.TriMeshLoader()
dynamic_tri_loader = avango.gua.nodes.DynamicTriangleLoader()
aux_loader = avango.gua.lod.nodes.Aux()
trans_node = avango.gua.nodes.TransformNode(Name="scene_trans")
trans_node.Transform.value = avango.gua.make_trans_mat(0, 0.0, 0) * \
                             avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0)
# nettrans.Children.value.append(trans_node)

view_num = 0
atlas_tiles_num = 0
atlas = None
vt_mat = avango.gua.nodes.Material()
vt_mat.set_uniform("vt_images", atlas_path)
vt_mat.EnableVirtualTexturing.value = True

localized_images = []

localized_images_node = dynamic_tri_loader.create_empty_geometry(
        "LozalizedImages", 
        "localized_images.lob", 
        avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.MAKE_PICKABLE)

localized_images_node.Material.value = vt_mat
localized_images_node.Material.value.set_uniform("Metalness", 0.0)
localized_images_node.Material.value.set_uniform("Emissivity", 1.0)
localized_images_node.Material.value.set_uniform("Roughness", 1.0)
localized_images_node.Material.value.set_uniform("vt_images", atlas_path)
localized_images_node.Material.value.EnableVirtualTexturing.value = True
print('VT MAT', vt_mat)
# localized_images_node.Material.value.EnableBackfaceCulling.value = False
trans_node.Children.value.append(localized_images_node)

group = avango.gua.nodes.TransformNode(Name="group")


aux_loader.load_aux_file(aux_path);
view_num = aux_loader.get_num_views()
atlas_tiles_num = aux_loader.get_num_atlas_tiles()
atlas = aux_loader.get_atlas()

# for quad_id in range(5):
for quad_id in range(view_num):
    # create_localized_quad(quad_id)
    view = aux_loader.get_view(quad_id)
    atlas_tile = aux_loader.get_atlas_tile(quad_id)
    atlas = aux_loader.get_atlas()
    quad = LocalizedImageQuad(graph, localized_images_node, quad_id, view, atlas_tile, atlas)
    localized_images.append(quad)

monkey = loader.create_geometry_from_file(
    "monkey", "../simple_example/data/objects/monkey.obj")

monkey.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 0.766, 0.336, 1.0))
monkey.Material.value.set_uniform("Roughness", 0.3)
monkey.Material.value.set_uniform("Metalness", 1.0)

monkey_transform = avango.gua.nodes.TransformNode(Name="monkey_transform")
monkey_transform.Transform.value = avango.gua.make_trans_mat(-0.00, 0.0, -0.5) * avango.gua.make_scale_mat(0.7)
monkey_transform.Children.value = [monkey]

group.Children.value.append(monkey_transform)


# empty nodes are used for adding geometry dynamically
dynamic_tri_node = dynamic_tri_loader.create_empty_geometry("plane", "empty.lob")
dynamic_tri_node.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 0.766, 0.336, 1.0))
dynamic_tri_node.Material.value.set_uniform("Emissivity", 1.0)
dynamic_tri_node.Material.value.set_uniform("Metalness", 0.0)
quad_transform = avango.gua.make_trans_mat(0.0, 0.0, 2.0)
quad_size = 0.2

pos = quad_transform * avango.gua.Vec3( quad_size, quad_size, 0.0)
uv  = avango.gua.Vec2(1.0, 0.0)
dynamic_tri_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, int(uv.x), int(uv.y) )

pos = quad_transform * avango.gua.Vec3(-quad_size, -quad_size, 0.0)
uv  = avango.gua.Vec2(0.0, 1.0)
dynamic_tri_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, int(uv.x), int(uv.y))

pos = quad_transform * avango.gua.Vec3( quad_size, -quad_size, 0.0)
uv  = avango.gua.Vec2(1.0, 1.0)
dynamic_tri_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, int(uv.x), int(uv.y))

pos = quad_transform * avango.gua.Vec3( quad_size, quad_size, 0.0)
uv  = avango.gua.Vec2(1.0, 0.0)
dynamic_tri_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)

pos = quad_transform * avango.gua.Vec3(-quad_size, quad_size, 0.0)
uv  = avango.gua.Vec2(0.0, 0.0)
dynamic_tri_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)

pos = quad_transform * avango.gua.Vec3(-quad_size, -quad_size, 0.0)
uv  = avango.gua.Vec2(0.0, 1.0)
dynamic_tri_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)
group.Children.value.append(dynamic_tri_node)
nettrans.Children.value.append(group)


graph.Root.value.Children.value = [nettrans]

projector3 = MyProjector()
projector3.my_constructor("p"+str(1))

group.Children.value.append(projector3.group_node)
projector3.set_localized_image_list(localized_images)
projector3.Graph.value = graph
projector3.Texture.value = atlas_path
projector3.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 3.0)
projector3.set_projection_lense(dynamic_tri_node, group)
dynamic_tri_node.Material.connect_from(projector3.Material)

projector3.change_photo_projection(1)
print_graph(graph.Root.value)
mat_desc = avango.gua.nodes.MaterialShaderDescription()
mat_desc.load_from_file("data/materials/SimpleMaterial.gmd")
avango.gua.register_material_shader(mat_desc, "mat")

mat = avango.gua.nodes.Material(ShaderName="mat")
mat2 = projector3.Material.value
print(projector3.Material.value.ShaderName.value)
mat3 = dynamic_tri_node.Material.value
print(dynamic_tri_node.Material.value.ShaderName.value)

nettrans.distribute_object(mat)
nettrans.distribute_object(mat2)
nettrans.distribute_object(mat3)

light = avango.gua.nodes.LightNode(Type=avango.gua.LightType.POINT,
                                   Name="light",
                                   Color=avango.gua.Color(1.0, 1.0, 1.0),
                                   Brightness=100.0,
                                   Transform=avango.gua.make_trans_mat(1, 1, 5)
                                   * avango.gua.make_scale_mat(30, 30, 30))

group.Children.value.append(light)

cscreen = avango.gua.nodes.ScreenNode(Name="client_screen", Width=4, Height=3)
sscreen = avango.gua.nodes.ScreenNode(Name="server_screen", Width=4, Height=3)

size = avango.gua.Vec2ui(800, 600)

dynamic_tri_pass = avango.gua.nodes.DynamicTrianglePassDescription()
tri_pass = avango.gua.nodes.TriMeshPassDescription()
tquad_pass = avango.gua.nodes.TexturedQuadPassDescription()
lvis_pass = avango.gua.nodes.LightVisibilityPassDescription()
res_pass = avango.gua.nodes.ResolvePassDescription()
res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
tscreenspace_pass = avango.gua.nodes.TexturedScreenSpaceQuadPassDescription()

pipeline_description = avango.gua.nodes.PipelineDescription(
    EnableABuffer=False,
    Passes=[dynamic_tri_pass, tri_pass, tquad_pass, lvis_pass, res_pass, tscreenspace_pass])

server_cam = avango.gua.nodes.CameraNode(
    ViewID=1,
    LeftScreenPath="/server_screen",
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="server_window",
    Transform=avango.gua.make_trans_mat(0.0, 0.0, 3.5),
    PipelineDescription=pipeline_description)

client_cam = avango.gua.nodes.CameraNode(
    Name='client_cam',
    ViewID=2,
    LeftScreenPath="/net/client_screen",
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="client_window",
    Transform=avango.gua.make_trans_mat(0.0, 0.0, 4.5),
    PipelineDescription=pipeline_description)

cscreen.Children.value = [client_cam]
sscreen.Children.value = [server_cam]

nettrans.Children.value.append(cscreen)

graph.Root.value.Children.value.append(sscreen)
# make_node_distributable(trans_node)
make_node_distributable(group)
make_node_distributable(cscreen)

nettrans.distribute_object(dynamic_tri_pass)
nettrans.distribute_object(tri_pass)
nettrans.distribute_object(tquad_pass)
nettrans.distribute_object(lvis_pass)
nettrans.distribute_object(res_pass)
nettrans.distribute_object(tscreenspace_pass)

for p in pipeline_description.Passes.value:
    nettrans.distribute_object(p)
nettrans.distribute_object(pipeline_description)

vt_backend = avango.gua.VTBackend()
vt_backend.add_camera(server_cam)
vt_backend.add_camera(client_cam)
vt_backend.start_backend()

# setup viewing
window = avango.gua.nodes.GlfwWindow(Size=size,
                                     LeftResolution=size,
                                     Title="server_window")

avango.gua.register_window("server_window", window)

#setup viewer
viewer = avango.gua.nodes.Viewer()
viewer.SceneGraphs.value = [graph]
viewer.Windows.value = [window]

update_dt = UpdateDynamicTriangleScript()

# #add linestrip to the spiral creator to create new vertices every frame
# # spiral_creator.set_line_strip_node(dynamic_tri_node)
update_dt.set_dynamic_triangle_node(dynamic_tri_node)
# dynamic_tri_node.start_vertex_list()
# # dynamic_tri_node.enqueue_vertex(norm_pos_x/5.0, norm_pos_y / 5.0 - 1.0, norm_pos_z/5.0, col_r, col_g, col_b, thickness)
# pos = quad_transform * avango.gua.Vec3( quad_size, quad_size, 0.0)
# uv  = avango.gua.Vec2(1.0, 0.0)
# dynamic_tri_node.enqueue_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)

# pos = quad_transform * avango.gua.Vec3(-quad_size, -quad_size, 0.0)
# uv  = avango.gua.Vec2(0.0, 1.0)
# dynamic_tri_node.enqueue_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)

# pos = quad_transform * avango.gua.Vec3( quad_size, -quad_size, 0.0)
# uv  = avango.gua.Vec2(1.0, 1.0)
# dynamic_tri_node.enqueue_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)

# pos = quad_transform * avango.gua.Vec3( quad_size, quad_size, 0.0)
# uv  = avango.gua.Vec2(1.0, 0.0)
# dynamic_tri_node.enqueue_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)

# pos = quad_transform * avango.gua.Vec3(-quad_size, quad_size, 0.0)
# uv  = avango.gua.Vec2(0.0, 0.0)
# dynamic_tri_node.enqueue_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)

# pos = quad_transform * avango.gua.Vec3(-quad_size, -quad_size, 0.0)
# uv  = avango.gua.Vec2(0.0, 1.0)
# dynamic_tri_node.enqueue_vertex(pos.x, pos.y, pos.z, 1.0, 0.0, 0.0, 1.0, uv.x, uv.y)
# dynamic_tri_node.end_vertex_list()
projector3.change_photo_projection(2)

guaVE = GuaVE()
guaVE.start(locals(), globals())

viewer.run()
