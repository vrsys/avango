import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import avango.gua.lod

from src.PhotoProjection import PhotoProjection



class AtlasTile:

    def __init__(self, graph, dt_node, quad_id, view, atlas_tile, atlas):
        self.dt_node = dt_node
        self.graph = graph
        self.id = quad_id
        self.view = view
        self.atlas_tile = atlas_tile
        self.atlas = atlas

        # self.transform = self.view.get_transform()
        self.transform = avango.gua.make_trans_mat(0.0, -1.000, 0.1) * avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * self.view.get_transform()

        self.rotation = avango.gua.make_rot_mat(self.transform.get_rotate_scale_corrected())

        self.position = avango.gua.Vec3(self.transform.get_translate()[0], 
                                        self.transform.get_translate()[1], 
                                        self.transform.get_translate()[2])
        _rot_mat = avango.gua.make_rot_mat(self.transform.get_rotate_scale_corrected())
        _abs_dir = _rot_mat * avango.gua.Vec3(0.0,0.0,-1.0)
        self.direction = avango.gua.Vec3(_abs_dir.x,_abs_dir.y,_abs_dir.z) # cast to vec3
        
        self.frustum = None
        self.indicator = None

        self.quad_vertices = []
        self.min_uv = avango.gua.Vec2(0.0, 0.0)
        self.max_uv = avango.gua.Vec2(1.0, 1.0)

        self.setup()
        self.create_quad()
        self.init_camera_setup()

        self.screen_transform = avango.gua.make_trans_mat(self.position) * self.rotation * avango.gua.make_trans_mat(0.0, 0.0, -0.1) *\
            avango.gua.make_rot_mat(90.0, 1.0, 0.0, 0.0) * avango.gua.make_scale_mat(self.img_w_half, self.img_h_half, self.img_h_half)
        # self.frustum = avango.gua.make_perspective_frustum(self.transform, screen_transform, 0.05, 3.0)

    def init_camera_setup(self):
        group_node = avango.gua.nodes.TransformNode(Name = "quad_"+str(self.id))
        self.graph.Root.value.Children.value.append(group_node)
        self.tile_cam_distance = -0.1 # TODO FOCAL LENGTH
        screen_transform = avango.gua.make_trans_mat(self.position) * self.rotation * avango.gua.make_trans_mat(0.0, 0.0, self.tile_cam_distance) # *\
        
        screen = avango.gua.nodes.ScreenNode(
          Name = "dummyscreen"+str(self.id),
          Width = self.tile_w,
          Height = self.tile_h,
          Transform = screen_transform
        )
        group_node.Children.value.append(screen)

        cam = avango.gua.nodes.CameraNode(
          LeftScreenPath = screen.Path.value,
          RightScreenPath = screen.Path.value,
          SceneGraph = "scenegraph",
          Transform = self.transform
        )
        group_node.Children.value.append(cam)

        self.frustum = cam.get_frustum(self.graph, avango.gua.CameraMode.CENTER)
        group_node.Children.value.remove(cam)
        del cam
        group_node.Children.value.remove(screen)
        del screen
        self.graph.Root.value.Children.value.remove(group_node)
        del group_node


    def setup(self):
        self.aspect_ratio = self.view.get_image_height() / self.view.get_image_width()
        # print('aspect_ratio', self.aspect_ratio)
        # opening_ratio_x = self.view.get_focal_value_x() / self.view.get_image_width()
        self.opening_ratio_x = 6114.757 / 4910.0 # hardcoded - adjust from aux file TODO 1.245368024

        # focal_length = view.get_focal_length() // Problem: Return 0 carl said not perfect yet
        self.focal_length = 0.1
        # self.img_w_half = self.focal_length / self.opening_ratio_x
        self.img_w_half = self.focal_length * 0.5
        self.img_h_half = self.img_w_half * self.aspect_ratio
        

        # opening_ratio_y = self.view.get_focal_value_y() / self.view.get_image_height()

        self.atlas_width  = self.atlas.get_width()
        self.atlas_height = self.atlas.get_height()
        # print('AAAAATLASS', self.atlas_width, self.atlas_height)

        # scale factor from image space to vt atlas space
        # factor = get_atlas_scale_factor();
        # self.factor = 0.950787 # low size res
        self.factor = 0.594242 # half size res
        # self.factor = factor = get_atlas_scale_factor();

        self.tile_h = self.atlas_tile.get_width() / self.atlas_width * self.factor
        self.tile_w = self.atlas_tile.get_width() / self.atlas_height * self.factor
        # print("tile_width:", self.tile_h, self.tile_w)
        # 0.04527557142857143 0.03020421952639752

        self.tile_pos_x = self.atlas_tile.get_x() / self.atlas_height * self.factor
        self.tile_pos_y = self.atlas_tile.get_y() / self.atlas_tile.get_height() * self.tile_h + (1 - self.factor)

        print('x min max y min max', self.tile_pos_x, self.tile_pos_x + self.tile_w,
              self.tile_pos_y, self.tile_pos_y + self.tile_h)

    def create_quad(self):
        transform = self.view.get_transform() #* avango.gua.make_rot_mat(180.0, 0.0, 1.0,0.0)

        pos = transform * avango.gua.Vec3(self.img_w_half, self.img_h_half, -self.focal_length - 0.02)
        uv  = avango.gua.Vec2(self.tile_pos_x, self.tile_pos_y)
        # uv  = avango.gua.Vec2(0.0, 0.0)
        t1_v1 = AtlasTileVertex(self.dt_node, self.id * 6, pos, uv)
        
        pos = transform * avango.gua.Vec3(-self.img_w_half, -self.img_h_half, -self.focal_length - 0.02)
        uv  = avango.gua.Vec2(self.tile_pos_x + self.tile_w, self.tile_pos_y + self.tile_h)
        # uv  = avango.gua.Vec2(1.0,1.0)
        t1_v2 = AtlasTileVertex(self.dt_node, self.id * 6 + 1, pos, uv)
        
        pos = transform * avango.gua.Vec3(self.img_w_half, -self.img_h_half, -self.focal_length - 0.02)
        uv  = avango.gua.Vec2(self.tile_pos_x, self.tile_pos_y + self.tile_h)
        # uv  = avango.gua.Vec2(0.0,1.0)
        t1_v3 = AtlasTileVertex(self.dt_node, self.id * 6 + 2, pos, uv)
        
        pos = transform * avango.gua.Vec3(self.img_w_half, self.img_h_half, -self.focal_length - 0.02)
        uv  = avango.gua.Vec2(self.tile_pos_x, self.tile_pos_y)
        # uv  = avango.gua.Vec2(0.0,0.0)
        t2_v4 = AtlasTileVertex(self.dt_node, self.id * 6 + 3, pos, uv)
        
        pos = transform * avango.gua.Vec3(-self.img_w_half, self.img_h_half, -self.focal_length- 0.02)
        uv  = avango.gua.Vec2(self.tile_pos_x + self.tile_w, self.tile_pos_y )
        # uv  = avango.gua.Vec2(1.0,0.0)
        t2_v5 = AtlasTileVertex(self.dt_node, self.id * 6 + 4, pos, uv)

        pos = transform * avango.gua.Vec3(-self.img_w_half, -self.img_h_half, -self.focal_length- 0.02 )
        uv  = avango.gua.Vec2(self.tile_pos_x + self.tile_w, self.tile_pos_y + self.tile_h)
        # uv  = avango.gua.Vec2(1.0,1.0)
        t2_v6 = AtlasTileVertex(self.dt_node, self.id * 6 + 5, pos, uv)

        self.min_uv = avango.gua.Vec2(self.tile_pos_x, self.tile_pos_y)
        self.max_uv = avango.gua.Vec2(self.tile_pos_x + self.tile_w, self.tile_pos_y + self.tile_h)
        print(self.id, 'min uv', self.min_uv, 'max uv', self.max_uv)
        self.quad_vertices = [t1_v1, t1_v2, t1_v3, t2_v4, t2_v5, t2_v6]


###
        # self.x = self.id % cols
        # self.y = self.id // cols

        # self.png_width = 64800 #122880
        # self.png_height = 107520 # 64512

        # self.aspect_ratio = 9.0 / 16.0
        # self.tile_scale = 0.2
        # self.tile_height = 0.3 * self.tile_scale
        # self.tile_width = self.tile_height * self.aspect_ratio
        # self.tile_cam_distance = -0.8 * self.tile_scale

        # self.padding_width = (self.png_width/256) * 2 - 1
        # self.padding_height = (self.png_height/256) * 2 - 1

        # self.x_min = 0
        # self.x_max = 1.0

        # self.y_min = 0.0
        # self.y_max = 1.0

        # self.transform = trans_mat # * avango.gua.make_rot_mat(180.0, 0.0, 0.0, 1.0)
        # # print(self.transform.get_scale())
        # self.rotation = avango.gua.make_rot_mat(self.transform.get_rotate_scale_corrected())
        # self.position = avango.gua.Vec3(self.transform.get_translate()[0], 
        #                                 self.transform.get_translate()[1], 
        #                                 self.transform.get_translate()[2])
        # _rot_mat = avango.gua.make_rot_mat(self.transform.get_rotate_scale_corrected())
        # _abs_dir = _rot_mat * avango.gua.Vec3(0.0,0.0,-1.0)
        # self.direction = avango.gua.Vec3(_abs_dir.x,_abs_dir.y,_abs_dir.z) # cast to vec3
        
        # self.frustum = None
        # self.quad_vertices = []
        # self.min_uv = avango.gua.Vec2(0.0, 0.0)
        # self.max_uv = avango.gua.Vec2(1.0, 1.0)

        # self.create_quad()
        # self.init_camera_setup()
        # self.screen_transform = avango.gua.make_trans_mat(self.position) * self.rotation * avango.gua.make_trans_mat(0.0, 0.0, self.tile_cam_distance)

    

    # def create_quad(self):


    #     self.t_w  = (self.png_width + self.padding_width) / 2**17 / self.cols
    #     self.t_h  = (self.png_height + self.padding_height) / 2**17 / self.rows
        
    #     self.x_min = self.x * (self.t_w)
    #     self.x_max = self.x * (self.t_w) + self.t_w

    #     self.y_min = 1 - (self.y * self.t_h)
    #     self.y_max = 1 - (self.y * self.t_h + self.t_h)

    #     pos = self.transform * avango.gua.Vec3( self.tile_width, self.tile_height, self.tile_cam_distance)
    #     # uv  = avango.gua.Vec2(self.x_min, self.y_min)
    #     uv  = avango.gua.Vec2(self.x_max, self.y_min)
    #     # dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
    #     t1_v1 = AtlasTileVertex(self.dt_node, self.id * 6, pos, uv)
        
    #     pos = self.transform * avango.gua.Vec3(-self.tile_width, self.tile_height, self.tile_cam_distance)
    #     # uv  = avango.gua.Vec2(self.x_max, self.y_min)
    #     uv  = avango.gua.Vec2(self.x_min, self.y_min)
    #     # dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
    #     t1_v2 = AtlasTileVertex(self.dt_node, self.id * 6 + 1, pos, uv)

    #     pos = self.transform * avango.gua.Vec3(-self.tile_width, -self.tile_height, self.tile_cam_distance)
    #     # uv  = avango.gua.Vec2(self.x_max, self.y_max)
    #     uv  = avango.gua.Vec2(self.x_min, self.y_max)
    #     # dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
    #     t1_v3 = AtlasTileVertex(self.dt_node, self.id * 6 + 2, pos, uv)

    #     pos = self.transform * avango.gua.Vec3( self.tile_width, self.tile_height, self.tile_cam_distance)
    #     # uv  = avango.gua.Vec2(self.x_min, self.y_min)
    #     uv  = avango.gua.Vec2(self.x_max, self.y_min)
    #     # dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
    #     t2_v1 = AtlasTileVertex(self.dt_node, self.id * 6 + 3, pos, uv)
        
    #     pos = self.transform * avango.gua.Vec3(-self.tile_width, -self.tile_height, self.tile_cam_distance)
    #     # uv  = avango.gua.Vec2(self.x_max, self.y_max)
    #     uv  = avango.gua.Vec2(self.x_min, self.y_max)
    #     # dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
    #     t2_v2 = AtlasTileVertex(self.dt_node, self.id * 6 + 4, pos, uv)
        
    #     pos = self.transform * avango.gua.Vec3(self.tile_width, -self.tile_height, self.tile_cam_distance)
    #     # uv  = avango.gua.Vec2(self.x_min, self.y_max)
    #     uv  = avango.gua.Vec2(self.x_max, self.y_max)
    #     # dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
    #     t2_v3 = AtlasTileVertex(self.dt_node, self.id * 6 + 5, pos, uv)

    #     # swap x_max and x_min to flip projection
 
    #     self.min_uv = avango.gua.Vec2(self.x_max, self.y_min)
    #     self.max_uv = avango.gua.Vec2(self.x_min, self.y_max)
    #     self.quad_vertices = [t1_v1, t1_v2, t1_v3, t2_v1, t2_v2, t2_v3]

class AtlasTileVertex:
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
        pass