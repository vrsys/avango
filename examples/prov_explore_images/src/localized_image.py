import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import avango.gua.lod

from src.vtprojector import VTProjector, AutoVTProjector


class LocalizedImageController:

    def __init__(self, graph, parent, aux_path, atlas_path):
        self.graph = graph
        self.parent = parent
        self.aux_path = aux_path

        # Create loaders
        self.dynamic_tri_loader = avango.gua.nodes.DynamicTriangleLoader()
        self.aux_loader = avango.gua.lod.nodes.Aux()
        
        self.dynamic_triangle_node = None
        self.view_num = 0
        self.atlas_tiles_num = 0
        self.atlas = None
        self.vt_mat = avango.gua.nodes.Material()
        # # self.vt_mat.set_uniform("vt_images", "/home/ephtron/Documents/master-render-files/salem/salem.atlas")
        self.vt_mat.set_uniform("vt_images", atlas_path)

        self.localized_images = []

        self.setup_localized_images()

        self.vtprojector = AutoVTProjector()
        self.vtprojector.my_constructor()
        self.vtprojector.set_localized_image_list(self.localized_images)
        # self.vtprojector.set_transform(self.parent.Transform.value)
        self.vtprojector.Graph.value = graph
        # self.vtprojector.Texture.value = "data/textures/smiley.jpg"
        # vtprojector.Texture.value = "/home/ephtron/Documents/master-render-files/salem/salem.atlas"
        self.vtprojector.Texture.value = atlas_path

        graph.Root.value.Children.value.append(self.vtprojector.group_node)
        # self.parent.Children.value.append(self.vtprojector.group_node)

    def setup_localized_images(self):

        self.dynamic_triangle_node = self.dynamic_tri_loader.create_empty_geometry(
            "dynamic_triangle_node", 
            "empty_name_1.lob", 
            avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.MAKE_PICKABLE)

        self.dynamic_triangle_node.Material.value = self.vt_mat
        self.dynamic_triangle_node.Material.value.set_uniform("Metalness", 0.0)
        self.dynamic_triangle_node.Material.value.set_uniform("Emissivity", 1.0)
        self.dynamic_triangle_node.Material.value.set_uniform("Roughness", 1.0)
        # self.dynamic_triangle_node.Material.value.set_uniform("vt_images", "/home/ephtron/Documents/master-render-files/salem/salem.atlas")
        self.dynamic_triangle_node.Material.value.EnableVirtualTexturing.value = True

        self.aux_loader.load_aux_file(self.aux_path);
        self.view_num = self.aux_loader.get_num_views()
        self.atlas_tiles_num = self.aux_loader.get_num_atlas_tiles()
        self.atlas = self.aux_loader.get_atlas()
        # fallback_mat = avango.gua.create_material(avango.gua.MaterialCapabilities.COLOR_VALUE)
        for quad_id in range(4):
        # for quad_id in range(self.view_num):
            self.create_localized_quad(quad_id)
            # pass

        self.parent.Children.value.append(self.dynamic_triangle_node)

    def create_localized_quad(self, quad_id):
        view = self.aux_loader.get_view(quad_id)
        atlas_tile = self.aux_loader.get_atlas_tile(quad_id)
        atlas = self.aux_loader.get_atlas()
        quad = LocalizedImageQuad(self.graph, self.dynamic_triangle_node, quad_id, view, atlas_tile, atlas)
        self.localized_images.append(quad)

    def get_projector(self):
        return self.vtprojector


class LocalizedImageQuad:
    def __init__(self, graph, node, quad_id, view, atlas_tile, atlas):
        self.node = node
        self.id = quad_id
        self.view = view
        self.transform = avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * self.view.get_transform()
        # HARDCODED TRANSFORM TODO
        # transform_pos = avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * self.transform.get_translate()
        # transforma = avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * self.transform
        # transform_pos = self.node.Transform.value * self.transform.get_translate()
        self.position = avango.gua.Vec3(self.transform.get_translate()[0], 
                                        self.transform.get_translate()[1], 
                                        self.transform.get_translate()[2])
        loader = avango.gua.nodes.TriMeshLoader()
        self.indicator = loader.create_geometry_from_file("boob_", "data/objects/cube.obj")
        self.indicator.Material.value = self.set_selected(False)
        self.indicator.Transform.value = self.transform * \
                                 avango.gua.make_scale_mat(0.001, 0.001, 10.0)
        graph.Root.value.Children.value.append(self.indicator)
        
        self.atlas_tile = atlas_tile
        self.atlas = atlas
        self.quad_vertices = []
        self.min_uv = avango.gua.Vec2(0.0,0.0)
        self.max_uv = avango.gua.Vec2(1.0,1.0)

        self.setup()
        self.create_quad()

    def set_selected(self, flag):
        mat = avango.gua.nodes.Material()

        unselected = avango.gua.Vec4(0.2, 0.2, 1.0, 1.0)
        # unselected.normalize()
        selected = avango.gua.Vec4(1.0, 0.1, 0.1, 1.0)
        # selected.normalize()

        if flag:
            mat.set_uniform("Color", selected)
        else :
            mat.set_uniform("Color", unselected)
        mat.set_uniform("Roughness", 1.0)
        mat.set_uniform("Emissivity", 1.0)
        mat.set_uniform("Metalness", 0.0)
        self.indicator.Material.value = mat
        return mat

    def setup(self):
        self.aspect_ratio = self.view.get_image_height() / self.view.get_image_width()
        print('aspect_ratio', self.aspect_ratio)

        # focal_length = view.get_focal_length() // Problem: Return 0 carl said not perfect yet
        self.focal_length = 0.1
        self.img_w_half = self.focal_length * 0.5
        self.img_h_half = self.img_w_half * self.aspect_ratio
        print('img_w_half', self.img_w_half)
        print('img_h_half', self.img_h_half)
        
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
        transform = self.view.get_transform()

        pos = transform * avango.gua.Vec3(self.img_w_half, self.img_h_half, -self.focal_length)
        uv  = avango.gua.Vec2(self.tile_pos_x, self.tile_pos_y)
        t1_v1 = LocalizedImageVertex(self.node, self.id * 6, pos, uv)
        self.min_uv = uv
        print('p1', uv)
        pos = transform * avango.gua.Vec3(-self.img_w_half, -self.img_h_half, -self.focal_length)
        uv  = avango.gua.Vec2(self.tile_pos_x + self.tile_w, self.tile_pos_y + self.tile_h)
        t1_v2 = LocalizedImageVertex(self.node, self.id * 6 + 1, pos, uv)
        self.max_uv = uv
        print('p2', uv)

        pos = transform * avango.gua.Vec3(-self.img_w_half, self.img_h_half, -self.focal_length)
        uv  = avango.gua.Vec2(self.tile_pos_x + self.tile_w, self.tile_pos_y)
        t1_v3 = LocalizedImageVertex(self.node, self.id * 6 + 2, pos, uv)
        
        print('p3', uv)
        pos = transform * avango.gua.Vec3(self.img_w_half, self.img_h_half, -self.focal_length)
        uv  = avango.gua.Vec2(self.tile_pos_x, self.tile_pos_y)
        t2_v4 = LocalizedImageVertex(self.node, self.id * 6 + 3, pos, uv)
        print('p4', uv)
        pos = transform * avango.gua.Vec3(self.img_w_half, -self.img_h_half, -self.focal_length)
        uv  = avango.gua.Vec2(self.tile_pos_x, self.tile_pos_y + self.tile_h)
        t2_v5 = LocalizedImageVertex(self.node, self.id * 6 + 4, pos, uv)
        print('p5', uv)
        pos = transform * avango.gua.Vec3(-self.img_w_half, -self.img_h_half, -self.focal_length)
        uv  = avango.gua.Vec2(self.tile_pos_x + self.tile_w, self.tile_pos_y + self.tile_h)
        t2_v6 = LocalizedImageVertex(self.node, self.id * 6 + 5, pos, uv)
        print('p6', uv)

        # pos = transform * avango.gua.Vec3(-self.img_w_half, self.img_h_half, -self.focal_length)
        # uv  = avango.gua.Vec2(self.tile_pos_x + self.tile_w, self.tile_pos_y)
        # t1_v1 = LocalizedImageVertex(self.node, self.id * 6, pos, uv)
        # self.max_uv = uv
        # print('p1', uv)

        # pos = transform * avango.gua.Vec3(self.img_w_half, -self.img_h_half, -self.focal_length)
        # uv  = avango.gua.Vec2(self.tile_pos_x, self.tile_pos_y + self.tile_h)
        # t1_v2 = LocalizedImageVertex(self.node, self.id * 6 + 1, pos, uv)
        # self.min_uv = uv
        # print('p2', uv)
        # pos = transform * avango.gua.Vec3(-self.img_w_half, -self.img_h_half, -self.focal_length)
        # uv  = avango.gua.Vec2(self.tile_pos_x + self.tile_w, self.tile_pos_y + self.tile_h)
        # t1_v3 = LocalizedImageVertex(self.node, self.id * 6 + 2, pos, uv)
        
        # print('p3', uv)
        # pos = transform * avango.gua.Vec3(self.img_w_half, self.img_h_half, -self.focal_length)
        # uv  = avango.gua.Vec2(self.tile_pos_x, self.tile_pos_y)
        # t2_v4 = LocalizedImageVertex(self.node, self.id * 6 + 3, pos, uv)
        
        # print('p4', uv)
        # pos = transform * avango.gua.Vec3(self.img_w_half, -self.img_h_half, -self.focal_length)
        # uv  = avango.gua.Vec2(self.tile_pos_x, self.tile_pos_y + self.tile_h)
        # t2_v5 = LocalizedImageVertex(self.node, self.id * 6 + 4, pos, uv)
        # print('p5', uv)
        # pos = transform * avango.gua.Vec3(-self.img_w_half, self.img_h_half, -self.focal_length)
        # uv  = avango.gua.Vec2(self.tile_pos_x + self.tile_w, self.tile_pos_y)
        # t2_v6 = LocalizedImageVertex(self.node, self.id * 6 + 5, pos, uv)
        # print('p6', uv)
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
        print('image quad ', self.id, self.min_uv, self.max_uv)


class LocalizedImageVertex:
    def __init__(self, node, vertex_id, pos, uv):
        self.vertex_id = vertex_id
        self.node = node 
        self.color = (1.0, 0.0, 0.0, 1.0)
        self.pos = pos
        self.uv = uv
        self.node.push_vertex(self.pos.x, self.pos.y, self.pos.z, 1.0, 0.0, 0.0, 1.0, self.uv.x, self.uv.y)
        # self.node.push_vertex(*self.pos, 1.0, 0.0, 0.0, 1.0, *self.uv)
        # print()

    def update(self, pos=None, uv=None):
        pass
    


# def get_atlas_scale_factor():
    
    # atlas_width  = atlas.get_width()
    # atlas_height = atlas.get_height()
    # auto atlas = new vt::pre::AtlasFile(settings_.atlas_file_.c_str());
    # uint64_t image_width    = atlas->getImageWidth();
    # uint64_t image_height   = atlas->getImageHeight();

    # // tile's width and height without padding
    # uint64_t tile_inner_width  = atlas->getInnerTileWidth();
    # uint64_t tile_inner_height = atlas->getInnerTileHeight();

    # // Quadtree depth counter, ranges from 0 to depth-1
    # uint64_t depth = atlas->getDepth();

    # double factor_u  = (double) image_width  / (tile_inner_width  * std::pow(2, depth-1));
    # double factor_v  = (double) image_height / (tile_inner_height * std::pow(2, depth-1));

    # return std::max(factor_u, factor_v);
