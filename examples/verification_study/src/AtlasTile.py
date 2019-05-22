import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import avango.gua.lod

from src.PhotoProjection import PhotoProjection



class AtlasTile:

    def __init__(self, graph, dt_node, quad_id, trans_mat, rows, cols):
        self.dt_node = dt_node
        self.graph = graph
        self.id = quad_id

        self.rows = rows
        self.cols = cols

        self.x = self.id % cols
        self.y = self.id // cols

        self.png_width = 122880
        self.png_height = 64512

        self.aspect_ratio = 9.0 / 16.0
        self.tile_scale = 0.2
        self.tile_width = 0.3 * self.tile_scale
        self.tile_height = self.tile_width * self.aspect_ratio
        self.tile_cam_distance = -0.8 * self.tile_scale

        self.padding_width = 960
        self.padding_height = 510

        self.x_min = 0
        self.x_max = 1.0

        self.y_min = 1.0
        self.y_max = 0.0

        self.transform = trans_mat # * avango.gua.make_rot_mat(180,0,0,1.0)
        print(self.transform.get_scale())
        self.rotation = avango.gua.make_rot_mat(self.transform.get_rotate_scale_corrected())
        self.position = avango.gua.Vec3(self.transform.get_translate()[0], 
                                        self.transform.get_translate()[1], 
                                        self.transform.get_translate()[2])
        _rot_mat = avango.gua.make_rot_mat(self.transform.get_rotate_scale_corrected())
        _abs_dir = _rot_mat * avango.gua.Vec3(0.0,0.0,-1.0)
        self.direction = avango.gua.Vec3(_abs_dir.x,_abs_dir.y,_abs_dir.z) # cast to vec3
        
        self.frustum = None
        self.quad_vertices = []
        self.min_uv = avango.gua.Vec2(0.0, 0.0)
        self.max_uv = avango.gua.Vec2(1.0, 1.0)

        self.create_quad()
        self.init_camera_setup()
        self.screen_transform = avango.gua.make_trans_mat(self.position) * self.rotation * avango.gua.make_trans_mat(0.0, 0.0, self.tile_cam_distance)

    def init_camera_setup(self):
        group_node = avango.gua.nodes.TransformNode(Name = "quad_"+str(self.id))
        self.graph.Root.value.Children.value.append(group_node)

        screen_transform = avango.gua.make_trans_mat(self.position) * self.rotation * avango.gua.make_trans_mat(0.0, 0.0, self.tile_cam_distance) # *\
        
        screen = avango.gua.nodes.ScreenNode(
          Name = "dummyscreen"+str(self.id),
          Width = self.tile_width,
          Height = self.tile_height,
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

    def create_quad(self):

        self.t_w  = (self.png_width + self.padding_width) / 2**17 / self.cols
        self.t_h  = (self.png_height + self.padding_height) / 2**17 / self.rows
        
        self.x_min = self.x * (self.t_w)
        self.x_max = self.x_min + (self.t_w)

        self.y_min = 1 - (self.y * self.t_h)
        self.y_max = 1 - (self.y * self.t_h + self.t_h)

        pos = self.transform * avango.gua.Vec3( self.tile_width, self.tile_height, self.tile_cam_distance)
        uv  = avango.gua.Vec2(self.x_max, self.y_min)
        # dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
        t1_v1 = AtlasTileVertex(self.dt_node, self.id * 6, pos, uv)
        
        pos = self.transform * avango.gua.Vec3(-self.tile_width, -self.tile_height, self.tile_cam_distance)
        uv  = avango.gua.Vec2(self.x_min, self.y_max)
        # dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
        t1_v2 = AtlasTileVertex(self.dt_node, self.id * 6 + 1, pos, uv)
        
        pos = self.transform * avango.gua.Vec3(self.tile_width, -self.tile_height, self.tile_cam_distance)
        uv  = avango.gua.Vec2(self.x_max, self.y_max)
        # dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
        t1_v3 = AtlasTileVertex(self.dt_node, self.id * 6 + 2, pos, uv)

        pos = self.transform * avango.gua.Vec3( self.tile_width, self.tile_height, self.tile_cam_distance)
        uv  = avango.gua.Vec2(self.x_max, self.y_min)
        # dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
        t2_v4 = AtlasTileVertex(self.dt_node, self.id * 6 + 3, pos, uv)
        
        pos = self.transform * avango.gua.Vec3(-self.tile_width, self.tile_height, self.tile_cam_distance)
        uv  = avango.gua.Vec2(self.x_min, self.y_min)
        # dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
        t2_v5 = AtlasTileVertex(self.dt_node, self.id * 6 + 4, pos, uv)

        pos = self.transform * avango.gua.Vec3(-self.tile_width, -self.tile_height, self.tile_cam_distance)
        uv  = avango.gua.Vec2(self.x_min, self.y_max)
        # dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
        t2_v6 = AtlasTileVertex(self.dt_node, self.id * 6 + 5, pos, uv)

        # pos = self.transform * avango.gua.Vec3( self.tile_width, self.tile_height, self.tile_cam_distance)
        # uv  = avango.gua.Vec2(self.x_max, self.y_min)
        # # dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
        # t1_v1 = AtlasTileVertex(self.dt_node, self.id * 6, pos, uv)
        
        # pos = self.transform * avango.gua.Vec3(-self.tile_width, -self.tile_height, self.tile_cam_distance)
        # uv  = avango.gua.Vec2(self.x_min, self.y_max)
        # # dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
        # t1_v2 = AtlasTileVertex(self.dt_node, self.id * 6 + 1, pos, uv)
        
        # pos = self.transform * avango.gua.Vec3(self.tile_width, -self.tile_height, self.tile_cam_distance)
        # uv  = avango.gua.Vec2(self.x_max, self.y_max)
        # # dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
        # t1_v3 = AtlasTileVertex(self.dt_node, self.id * 6 + 2, pos, uv)

        # pos = self.transform * avango.gua.Vec3( self.tile_width, self.tile_height, self.tile_cam_distance)
        # uv  = avango.gua.Vec2(self.x_max, self.y_min)
        # # dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
        # t2_v4 = AtlasTileVertex(self.dt_node, self.id * 6 + 3, pos, uv)
        
        # pos = self.transform * avango.gua.Vec3(-self.tile_width, self.tile_height, self.tile_cam_distance)
        # uv  = avango.gua.Vec2(self.x_min, self.y_min)
        # # dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
        # t2_v5 = AtlasTileVertex(self.dt_node, self.id * 6 + 4, pos, uv)

        # pos = self.transform * avango.gua.Vec3(-self.tile_width, -self.tile_height, self.tile_cam_distance)
        # uv  = avango.gua.Vec2(self.x_min, self.y_max)
        # # dt_node.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
        # t2_v6 = AtlasTileVertex(self.dt_node, self.id * 6 + 5, pos, uv)

        self.min_uv = avango.gua.Vec2(self.x_max, self.y_min)
        self.max_uv = avango.gua.Vec2(self.x_min, self.y_max)
        self.quad_vertices = [t1_v1, t1_v2, t1_v3, t2_v4, t2_v5, t2_v6]



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