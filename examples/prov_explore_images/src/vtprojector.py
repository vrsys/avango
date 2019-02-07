import avango
import avango.script
import avango.gua
from avango.script import field_has_changed

import random
import math
import src.provenance_utils as pu


class AutoVTProjector(avango.script.Script):

  Material = avango.gua.SFMaterial()
  Transform = avango.gua.SFMatrix4()
  Transform2 = avango.gua.SFMatrix4()
  Graph = avango.gua.SFSceneGraph()

  Texture = avango.SFString()

  def __init__(self):
    self.super(AutoVTProjector).__init__()
    self.always_evaluate(False)
    self.is_initialized = False

  def my_constructor(self):
    loader = avango.gua.nodes.TriMeshLoader()

    self.localized_image_list = []
    self.position_list = []
    self.min_tex_coords = avango.gua.Vec2(0.0, 0.0)
    self.max_tex_coords = avango.gua.Vec2(1.0, 1.0)
    self.last_lense_pos = None
    self.scene_graph = None
    self.old_closest_id = 0

    self.group_node = avango.gua.nodes.TransformNode(Name = "projector_group")
    self.group_node.Transform.connect_from(self.Transform)

    self.geometry = loader.create_geometry_from_file("projector_geometry", "data/objects/projector.obj", 
      avango.gua.LoaderFlags.NORMALIZE_SCALE |
      avango.gua.LoaderFlags.NORMALIZE_POSITION | 
      avango.gua.LoaderFlags.LOAD_MATERIALS)
    self.geometry.Transform.value = avango.gua.make_scale_mat(0.1)
    # self.group_node.Children.value.append(self.geometry)

    self.screen = avango.gua.nodes.ScreenNode(
      Name = "screen1",
      Width = 0.5,
      Height = 0.5,
      Transform = avango.gua.make_trans_mat(0.0, 0.0, -0.1)
    )
    self.group_node.Children.value.append(self.screen)

    self.cam = avango.gua.nodes.CameraNode(
      LeftScreenPath = "projector_group/screen1",
      # LeftScreenPath = "scene_trans/projector_group/screen1",
      SceneGraph = "scenegraph",
    )
    self.group_node.Children.value.append(self.cam)

    self.Material.value = avango.gua.nodes.Material(
      ShaderName = "proj_mat"
    )

    self.Graph.value = avango.gua.nodes.SceneGraph(Name = "dummy")

    proj_mat_desc = avango.gua.nodes.MaterialShaderDescription()
    proj_mat_desc.load_from_file("data/materials/Projective_VT_Material.gmd")

    # proj_mat_desc.EnableVirtualTexturing.value = True
    avango.gua.register_material_shader(proj_mat_desc, "proj_mat")
    

  def set_scenegraph(self, graph):
    self.scene_graph = graph

  def set_localized_image_list(self, localized_image_list):
    self.localized_image_list = localized_image_list
    self.position_list = [li.position for li in self.localized_image_list]
    print(len(self.localized_image_list), len(self.position_list))
    self.min_tex_coords = self.localized_image_list[self.old_closest_id].min_uv
    self.max_tex_coords = self.localized_image_list[self.old_closest_id].max_uv

  def find_closest_view(self, criteria):

    pos = self.Transform2.value.get_translate()
    _rot_mat = avango.gua.make_rot_mat(self.Transform2.value.get_rotate_scale_corrected())
    _abs_dir = _rot_mat * avango.gua.Vec3(0.0,0.0,-1.0)
    _abs_dir = avango.gua.Vec3(_abs_dir.x,_abs_dir.y,_abs_dir.z) # cast to vec3


    distance = 10000.0
    angle = 1000.0
    closest_id = None
    image_pos = None
    best_size = 3
    best_distance_ids = []
    best_angle_ids = []
    best_mix_ids = []

    angle_list = []
    for img in self.localized_image_list:
      dot = _abs_dir.x*img.direction.x + _abs_dir.y*img.direction.y + _abs_dir.z*img.direction.z
      dot = 1 if dot >= 1 else dot
      dot = -1 if dot <= -1 else dot
      # print(dot)
      a = math.acos(dot)
      if a < 1:
        tup = (img, a)
        angle_list.append(tup)


    # go through all views and find localized image
    for i_id, img in enumerate(self.localized_image_list):
        new_distance = pu.distance(img.position, pos)
        # new_angle = math.cos(_abs_dir.x*img.direction.x + _abs_dir.y*img.direction.y + _abs_dir.z*img.direction.z)
        # print(new_distance)
        if new_distance < distance:
            distance = new_distance
            if len(best_distance_ids) > 3:
                del best_distance_ids[-1]
                e = (i_id, new_distance)
                best_distance_ids.append(e)
                best_distance_ids.sort(key=lambda tup: tup[1])
                distance = best_distance_ids[-1][1]
            else:
                best_distance_ids.append((i_id, new_distance))
                best_distance_ids.sort(key=lambda tup: tup[1])
                distance = best_distance_ids[-1][1]

    for tu in best_distance_ids:
        img_id = tu[0]
        img = self.localized_image_list[img_id]
        new_angle = math.cos(_abs_dir.x*img.direction.x + _abs_dir.y*img.direction.y + _abs_dir.z*img.direction.z)
        if new_angle < angle:
            angle = new_angle
            closest_id = img_id
    # closest_id = 0

    # if distance > new_distance and new_angle < angle:
    #     best_mix_ids.append(i_id)
    #     closest_id = i_id

    # if new_distance < distance:
    #     distance = new_distance
    #     best_distance_ids.append((i_id, new_distance))

    # if new_angle < angle:
    #     angle = new_angle
    #     best_angle_ids.append((i_id, new_angle))

    # print('@@@@@@@@@@@')
    # print('mix len', best_mix_ids)
    # print('dist len', best_distance_ids)
    # print('angl len', best_angle_ids)

    # closest_id = i_id

    # for i_id, img in enumerate(self.localized_image_list):
    #     # print(img.direction)
    #     # print(_abs_dir)
    #     # new_angle = math.acos(_abs_dir*img.direction)
    #     new_angle = math.acos(_abs_dir.x*img.direction.x + 
    #       _abs_dir.y*img.direction.y +
    #       _abs_dir.z*img.direction.z)

    #     if new_angle < angle:
    #         angle = new_angle
    #         print(angle)
    # _vec1.normalize()
    # _vec2.normalize()
    # axis = vec1.cross(vec2)
    # angle = math.acos(vec1*vec2)

    return closest_id, angle_list


  @field_has_changed(Transform)
  def update_matrices(self):
    self.group_node.Transform.value = self.Transform.value
    # print('changed', self.Transform.value, self.group_node.Transform.value)
    frustum = self.cam.get_frustum(self.Graph.value, avango.gua.CameraMode.CENTER)

    projection_matrix = frustum.ProjectionMatrix.value
    view_matrix = frustum.ViewMatrix.value
    # frustum.contains(pos)
    
    self.Material.value.set_uniform("projective_texture_matrix", projection_matrix)
    # self.Material.value.set_uniform("projective_texture", self.Texture.value)
    self.Material.value.set_uniform("view_texture_matrix", view_matrix) 
    self.Material.value.set_uniform("Emissivity", 1.0)
    self.Material.value.set_uniform("Roughness", 1.0)
    self.Material.value.set_uniform("Metalness", 0.0)
    self.Material.value.set_uniform("view_port_min", avango.gua.Vec2(self.min_tex_coords))
    self.Material.value.set_uniform("view_port_max", avango.gua.Vec2(self.max_tex_coords))

  @field_has_changed(Transform2)
  def updated_lens_position(self):
    if self.last_lense_pos:
      if (self.Transform2.value.get_translate().x != self.last_lense_pos.x
       or self.Transform2.value.get_translate().y != self.last_lense_pos.y 
       or self.Transform2.value.get_translate().z != self.last_lense_pos.z):
        # pos = self.Transform2.value.get_translate()
        # look_at_mat = avango.gua.make_look_at_mat()
        closest_id, l = self.find_closest_view('angle')
        # distance = 10000.0
        # closest_id = None
        # image_pos = None
        # # go through all views and find localized image
        # for i_id, i_pos in enumerate(self.position_list):
        #   new_distance = pu.distance(i_pos, pos)

        #   if distance > new_distance:
        #     distance = new_distance
        #     closest_id = i_id
        #     image_pos = i_pos

        if closest_id != self.old_closest_id:
          self.localized_image_list[self.old_closest_id].set_selected(False, False)

          self.Transform.value = self.localized_image_list[closest_id].transform
          self.min_tex_coords = self.localized_image_list[closest_id].min_uv
          self.max_tex_coords = self.localized_image_list[closest_id].max_uv
          self.screen.Width.value = self.localized_image_list[closest_id].img_w_half 
          self.screen.Height.value = self.localized_image_list[closest_id].img_h_half 
          
          self.localized_image_list[closest_id].set_selected(True, True)
          self.last_lense_pos = self.Transform2.value.get_translate()
          self.old_closest_id = closest_id

        for i, img in enumerate(self.localized_image_list):
          if i == self.old_closest_id:
            pass
          # elif i in l:
          #   img.set_selected(False, True)
          # else:
          #   img.set_selected(False, False)
    else:
      self.last_lense_pos = self.Transform2.value.get_translate()

  @field_has_changed(Texture)
  def update_texture(self):
    print('updating tex')
    self.Material.value.set_uniform("projective_texture", self.Texture.value)
    self.Material.value.set_uniform("Emissivity", 1.0)
    self.Material.value.EnableVirtualTexturing.value = True

  def evaluate(self):
    pass

    # _rot_mat = avango.gua.make_rot_mat(mat.get_rotate_scale_corrected())
    # _abs_dir = _rot_mat * avango.gua.Vec3(0.0,0.0,-1.0)
    # _abs_dir = avango.gua.Vec3(_abs_dir.x,_abs_dir.y,_abs_dir.z) # cast to vec3

    # _vec1.normalize()
    # _vec2.normalize()
    # axis = vec1.cross(vec2)
    # angle = math.acos(vec1*vec2)

