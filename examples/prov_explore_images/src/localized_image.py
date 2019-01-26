import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import avango.gua.lod

class LocalizedImageController:

    def __init__(self, parent, aux_path):
        self.parent = parent
        self.aux_path = aux_path

        # Create loaders
        self.dynamic_tri_loader = avango.gua.nodes.DynamicTriangleLoader()
        self.aux_loader = avango.gua.lod.nodes.Aux()
        
        self.dynamic_triangle_node = None
        self.view_num = 0
        self.atlas_tiles_num = 0
        self.atlas = None

        self.localized_images = []

        self.setup_localized_images()

    def setup_localized_images(self):

        self.dynamic_triangle_node = self.dynamic_tri_loader.create_empty_geometry(
            "dynamic_triangle_node", 
            "empty_name_1.lob", 
            avango.gua.LoaderFlags.DEFAULTS | avango.gua.lod.LoaderFlags.NORMALIZE_SCALE | 
            avango.gua.lod.LoaderFlags.NORMALIZE_POSITION | avango.gua.LoaderFlags.MAKE_PICKABLE)

        self.dynamic_triangle_node.Material.value.set_uniform("Metalness", 0.0)
        self.dynamic_triangle_node.Material.value.set_uniform("Emissivity", 1.0)
        self.dynamic_triangle_node.Material.value.set_uniform("Roughness", 1.0)
        self.dynamic_triangle_node.Material.value.set_uniform("vt_images", "/home/ephtron/Documents/master-render-files/salem/salem.atlas")
        self.dynamic_triangle_node.Material.value.EnableVirtualTexturing.value = True

        self.aux_loader.load_aux_file(self.aux_path);
        self.view_num = self.aux_loader.get_num_views()
        self.atlas_tiles_num = self.aux_loader.get_num_atlas_tiles()
        self.atlas = self.aux_loader.get_atlas()
        # fallback_mat = avango.gua.create_material(avango.gua.MaterialCapabilities.COLOR_VALUE)
        print('go')
        for quad_id in range(self.view_num):
            self.create_localized_quad(quad_id)
            # pass

        self.parent.Children.value.append(self.dynamic_triangle_node)

    def create_localized_quad(self, quad_id):
        print('creat q')
        view = self.aux_loader.get_view(quad_id)
        atlas_tile = self.aux_loader.get_atlas_tile(quad_id)
        atlas = self.aux_loader.get_atlas()
        quad = LocalizedImageQuad(self.dynamic_triangle_node, quad_id, view, atlas_tile, atlas)
        self.localized_images.append(quad)
        print('JAU')



class LocalizedImageQuad:
    def __init__(self, node, quad_id, view, atlas_tile, atlas):
        self.node = node
        self.id = quad_id
        self.view = view
        self.atlas_tile = atlas_tile
        self.atlas = atlas
        self.quad_vertices = []
        print('yes')

        self.setup()
        self.create_quad()

    def setup(self):
        print('setup liq')
        self.aspect_ratio = self.view.get_image_height() / self.view.get_image_width()
        # focal_length = view.get_focal_length() // Problem: Return 0 carl said not perfect yet
        self.focal_length = 0.1
        self.img_w_half = self.focal_length * 0.5
        self.img_h_half = self.img_w_half * self.aspect_ratio
        
        self.atlas_width  = self.atlas.get_width()
        self.atlas_height = self.atlas.get_height()

        # scale factor from image space to vt atlas space
        # float factor = get_atlas_scale_factor();
        self.factor = 0.950787

        self.tile_h = self.atlas_tile.get_width() / self.atlas_width * self.factor
        self.tile_w = self.atlas_tile.get_width() / self.atlas_height * self.factor

        self.tile_pos_x = self.atlas_tile.get_x() / self.atlas_height * self.factor
        self.tile_pos_y = self.atlas_tile.get_y() / self.atlas_tile.get_height() * self.tile_h + (1 - self.factor)

    def create_quad(self):
        print('create q')
        transform = self.view.get_transform()

        pos = transform * avango.gua.Vec3(-self.img_w_half, self.img_h_half, -self.focal_length)
        uv  = avango.gua.Vec2(self.tile_pos_x + self.tile_w, self.tile_pos_y)
        t1_v1 = LocalizedImageVertex(self.node, self.id * 6, pos, uv)

        pos = transform * avango.gua.Vec3(self.img_w_half, -self.img_h_half, -self.focal_length)
        uv  = avango.gua.Vec2(self.tile_pos_x, self.tile_pos_y + self.tile_h)
        t1_v2 = LocalizedImageVertex(self.node, self.id * 6 + 1, pos, uv)

        pos = transform * avango.gua.Vec3(-self.img_w_half, -self.img_h_half, -self.focal_length)
        uv  = avango.gua.Vec2(self.tile_pos_x + self.tile_w, self.tile_pos_y + self.tile_h)
        t1_v3 = LocalizedImageVertex(self.node, self.id * 6 + 2, pos, uv)

        pos = transform * avango.gua.Vec3(self.img_w_half, self.img_h_half, -self.focal_length)
        uv  = avango.gua.Vec2(self.tile_pos_x, self.tile_pos_y)
        t2_v4 = LocalizedImageVertex(self.node, self.id * 6 + 3, pos, uv)

        pos = transform * avango.gua.Vec3(self.img_w_half, -self.img_h_half, -self.focal_length)
        uv  = avango.gua.Vec2(self.tile_pos_x, self.tile_pos_y + self.tile_h)
        t2_v5 = LocalizedImageVertex(self.node, self.id * 6 + 4, pos, uv)

        pos = transform * avango.gua.Vec3(-self.img_w_half, self.img_h_half, -self.focal_length)
        uv  = avango.gua.Vec2(self.tile_pos_x + self.tile_w, self.tile_pos_y)
        t2_v6 = LocalizedImageVertex(self.node, self.id * 6 + 5, pos, uv)
        
        # p1_pos = view.get_transform() * avango.gua.Vec3(-img_w_half, img_h_half, -focal_length)
        # p1_uv  = avango.gua.Vec2(tile_pos_x + tile_w, tile_pos_y)
        # p2_pos = view.get_transform() * avango.gua.Vec3(img_w_half, img_h_half, -focal_length)
        # p2_uv  = avango.gua.Vec2(tile_pos_x, tile_pos_y)
        # p3_pos = view.get_transform() * avango.gua.Vec3(-img_w_half, -img_h_half, -focal_length)
        # p3_uv  = avango.gua.Vec2(tile_pos_x + tile_w, tile_pos_y + tile_h)
        # p4_pos = view.get_transform() * avango.gua.Vec3(img_w_half, -img_h_half, -focal_length)
        # p4_uv  = avango.gua.Vec2(tile_pos_x, tile_pos_y + tile_h)
        # triangle 1 = v1, v4, v3
        # triangle 2 = v2, v4, v1

        self.quad_vertices = [t1_v1, t1_v2, t1_v3, t2_v4, t2_v5, t2_v6]


class LocalizedImageVertex:
    def __init__(self, node, vertex_id, pos, uv):
        self.vertex_id = vertex_id
        self.node = node 
        self.color = (1.0, 0.0, 0.0, 1.0)
        self.pos = pos
        self.uv = uv
        # print('yes', *self.pos, *self.color, *self.uv)
        self.node.push_vertex(self.pos.x, self.pos.y, self.pos.z, 1.0, 0.0, 0.0, 1.0, self.uv.x, self.uv.y)
        # self.node.push_vertex(*self.pos, 1.0, 0.0, 0.0, 1.0, *self.uv)
        # print()

    def update(self, pos=None, uv=None):
        pass
    