import avango
import avango.script
import avango.gua
from avango.script import field_has_changed


## print the subgraph under a given node to the console
def print_graph(root_node):
    stack = [(root_node, 0)]
    while stack:
        node, level = stack.pop()
        print("│   " * level + "├── {0} <{1}>".format(node.Name.value, node.__class__.__name__))
        stack.extend([(child, level + 1) for child in reversed(node.Children.value)])


class MultiWindowVisualizer(avango.script.Script):

    def __init__(self):
        self.super(MultiWindowVisualizer).__init__()
        self.always_evaluate(True)
        self.is_initialized = False

    def my_constructor(self, parent_node, atlas_path, images, width, height, rows=3, columns=4):

        self.parent_node = parent_node
        self.atlas_path = atlas_path
        self.width = width
        self.height = height
        self.rows = rows
        self.columns = columns
        self.views = rows* columns
        self.distribute = False

         # Create loaders
        self.dynamic_tri_loader = avango.gua.nodes.DynamicTriangleLoader()
        self.aux_loader = avango.gua.lod.nodes.Aux()
        
        self.dynamic_triangle_node = self.dynamic_tri_loader.create_empty_geometry(
            "multi_window_vis", 
            "multi_window_vis.lob", 
            avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.MAKE_PICKABLE)
        self.view_num = 0
        self.atlas_tiles_num = 0
        self.atlas = None
        self.vt_mat = avango.gua.nodes.Material()
        self.vt_mat.set_uniform("vt_images", atlas_path)
        self.vt_mat.EnableVirtualTexturing.value = True
        self.dynamic_triangle_node.Material.value = self.vt_mat
        self.dynamic_triangle_node.Material.value.set_uniform("Metalness", 0.0)
        self.dynamic_triangle_node.Material.value.set_uniform("Emissivity", 1.0)
        self.dynamic_triangle_node.Material.value.set_uniform("Roughness", 1.0)
        # self.dynamic_triangle_node.Material.value.EnableVirtualTexturing.value = True

        self.images = images
        self.dynamic_quads = []
        self.selected_images = self.images[0:self.views]
        for idx, img in enumerate(self.selected_images):

            x = (self.width/2) - (idx // self.rows * (self.width/self.columns)) - (self.width/self.columns/2)
            y = self.height/2 - (idx % self.rows * (self.height/self.rows)) - (self.height/self.rows/2)
            transform = avango.gua.make_trans_mat(x, y, 0.0)

            w_half = (self.width/self.columns) / 2
            h_half = (self.height/self.rows) / 2

            pos = transform * avango.gua.Vec3(w_half, h_half, 0.0)
            uv  = avango.gua.Vec2(img.tile_pos_x, img.tile_pos_y)
            t1_v1 = ImageVertex(self.dynamic_triangle_node, idx * 6, pos, uv)
            
            pos = transform * avango.gua.Vec3(-w_half, -h_half, 0.0)
            uv  = avango.gua.Vec2(img.tile_pos_x + img.tile_w, img.tile_pos_y + img.tile_h)
            t1_v2 = ImageVertex(self.dynamic_triangle_node, idx * 6 + 1, pos, uv)
            
            pos = transform * avango.gua.Vec3(w_half, -h_half, 0.0)
            uv  = avango.gua.Vec2(img.tile_pos_x, img.tile_pos_y + img.tile_h)
            t1_v3 = ImageVertex(self.dynamic_triangle_node, idx * 6 + 2, pos, uv)
            
            pos = transform * avango.gua.Vec3(w_half, h_half, 0.0)
            uv  = avango.gua.Vec2(img.tile_pos_x, img.tile_pos_y)
            t2_v4 = ImageVertex(self.dynamic_triangle_node, idx * 6 + 3, pos, uv)
            
            pos = transform * avango.gua.Vec3(-w_half, h_half, 0.0)
            uv  = avango.gua.Vec2(img.tile_pos_x + img.tile_w, img.tile_pos_y )
            t2_v5 = ImageVertex(self.dynamic_triangle_node, idx * 6 + 4, pos, uv)

            pos = transform * avango.gua.Vec3(-w_half, -h_half, 0.0)
            uv  = avango.gua.Vec2(img.tile_pos_x + img.tile_w, img.tile_pos_y + img.tile_h)
            t2_v6 = ImageVertex(self.dynamic_triangle_node, idx * 6 + 5, pos, uv)

            self.min_uv = avango.gua.Vec2(img.tile_pos_x, img.tile_pos_y)
            self.max_uv = avango.gua.Vec2(img.tile_pos_x + img.tile_w, img.tile_pos_y + img.tile_h)

            self.quad_vertices = [t1_v1, t1_v2, t1_v3, t2_v4, t2_v5, t2_v6]
            self.dynamic_quads.append(self.quad_vertices)

        self.parent_node.Children.value.append(self.dynamic_triangle_node)

    def set_image_list(self, images):
        self.images = images

    def get_node(self):
        return self.dynamic_triangle_node

    def get_material(self):
        return self.vt_mat

    def evaluate(self):
        if self.distribute:
            # print('uuuu')
            self.dynamic_triangle_node.start_vertex_list()

            for quad in self.dynamic_quads:
                # quad_transform = avango.gua.make_trans_mat(0.5 * math.cos(self.FrameCount/100), 0.3*math.sin(self.FrameCount/100), 2.0)
                for vertex in quad:
                    # print(vertex.uv)
                    # new_u = vertex.uv[0] + 0.001
                    # new_v = vertex.uv[0] + 0.001
                    vertex.enqueue()
                    #vertex.enqueue_with_new_uv(avango.gua.Vec2(new_u, new_v))
            
            self.dynamic_triangle_node.end_vertex_list()

    def update_images(self, image_id_list, tex_coords):
        print('update', len(tex_coords), len(image_id_list), len(self.dynamic_quads))
        for idx, img_id in enumerate(image_id_list):
            # print(tex_coords[idx])
            
            uv1  = avango.gua.Vec2(tex_coords[idx][0].x, tex_coords[idx][0].y)
            self.dynamic_quads[idx][0].update(uv=uv1)
            uv2  = avango.gua.Vec2(tex_coords[idx][1].x, tex_coords[idx][1].y)
            self.dynamic_quads[idx][1].update(uv=uv2)
            uv3  = avango.gua.Vec2(tex_coords[idx][0].x, tex_coords[idx][1].y)
            self.dynamic_quads[idx][2].update(uv=uv3)
            uv4  = avango.gua.Vec2(tex_coords[idx][0].x, tex_coords[idx][0].y)
            self.dynamic_quads[idx][3].update(uv=uv4)
            uv5  = avango.gua.Vec2(tex_coords[idx][1].x, tex_coords[idx][0].y)
            self.dynamic_quads[idx][4].update(uv=uv5)
            uv6  = avango.gua.Vec2(tex_coords[idx][1].x, tex_coords[idx][1].y)
            self.dynamic_quads[idx][5].update(uv=uv6)


class ImageVertex:
    def __init__(self, node, vertex_id, pos, uv):
        self.vertex_id = vertex_id
        self.dt_node = node 
        self.color = (1.0, 0.0, 0.0, 1.0)
        self.pos = pos
        self.uv = uv
        self.dt_node.push_vertex(self.pos.x, self.pos.y, self.pos.z, 1.0, 0.0, 0.0, 1.0, self.uv.x, self.uv.y)
        # self.dt_node.push_vertex(*self.pos, 1.0, 0.0, 0.0, 1.0, *self.uv)
        # print()

    def update(self, pos=None, uv=None):
        if pos:
            self.pos = pos
        if uv:
            # print('changed uv', uv)
            self.uv = uv


    def enqueue(self):
        self.dt_node.enqueue_vertex(self.pos.x, self.pos.y, self.pos.z, 1.0, 0.0, 0.0, 1.0, self.uv.x, self.uv.y)

    def enqueue_with_new_uv(self, uv):
        self.uv = uv
        self.dt_node.enqueue_vertex(self.pos.x, self.pos.y, self.pos.z, 1.0, 0.0, 0.0, 1.0, self.uv.x, self.uv.y)
