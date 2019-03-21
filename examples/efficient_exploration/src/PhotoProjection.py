import avango
import avango.script
import avango.gua
from avango.script import field_has_changed

import random
import math
import src.provenance_utils as pu
import time


class PhotoProjection(avango.script.Script):

  Material = avango.gua.SFMaterial()
  Transform = avango.gua.SFMatrix4()
  Graph = avango.gua.SFSceneGraph()
  Button0 = avango.SFBool()
  Button1 = avango.SFBool()

  Texture = avango.SFString()

  def __init__(self):
    self.super(PhotoProjection).__init__()
    self.always_evaluate(False)
    self.is_initialized = False

  def my_constructor(self):
    self.indicator = None
    loader = avango.gua.nodes.TriMeshLoader()

    self.localized_image_list = []
    self.position_list = []
    self.projection_lense = None
    self.lense_parent_node = None
    self.show_lense = False
    self.min_tex_coords = avango.gua.Vec2(0.0, 0.0)
    self.max_tex_coords = avango.gua.Vec2(1.0, 1.0)
    self.last_lense_pos = None
    self.scene_graph = None
    self.old_closest_id = 0
    self.button0_pressed = False
    # self.offset = avango.gua.Vec3(0.0,0.0,0.0)
    self.offset = avango.gua.make_identity_mat()
    
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
    print('scene graph got set')
    self.scene_graph = graph

  def set_projection_lense(self, obj, parent_node):
    if self.projection_lense is not None:
      self.projection_lense.Material.disconnect_from(self.Material)
    self.projection_lense = obj
    self.lense_parent_node = parent_node
    if self.projection_lense and self.lense_parent_node:
      self.show_lense = True
    self.projection_lense.Material.connect_from(self.Material)

  def set_localized_image_list(self, localized_image_list):
    self.localized_image_list = localized_image_list
    self.position_list = [li.position for li in self.localized_image_list]
    print(len(self.localized_image_list), len(self.position_list))
    self.min_tex_coords = self.localized_image_list[self.old_closest_id].min_uv
    self.max_tex_coords = self.localized_image_list[self.old_closest_id].max_uv

  def find_orthogonal_image_plane_view(self):
    closest_id = None
    # get direction vector of hendheld lense
    pos = self.Transform2.value.get_translate()

    # _rot_mat = avango.gua.make_rot_mat(self.Transform2.value.get_rotate_scale_corrected()) * avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0)
    _rot_mat = avango.gua.make_rot_mat(self.Transform2.value.get_rotate_scale_corrected())
    _abs_dir = _rot_mat * avango.gua.Vec3(0.0,0.0,-1.0)
    _abs_dir = avango.gua.Vec3(_abs_dir.x,_abs_dir.y,_abs_dir.z) # cast to vec3

    angle_list = []
    # get angles between hendheld lense and images
    for img in self.localized_image_list:
      dot = _abs_dir.x*img.direction.x + _abs_dir.y*img.direction.y + _abs_dir.z*img.direction.z
      # make sure values are in range for acos
      dot = 1 if dot >= 1 else dot
      dot = -1 if dot <= -1 else dot
      a = math.acos(dot)
      # filter by angle
      if a < 1:
        tup = (img, a)
        # if angle between image nad quad is small show img direction indicator 
        # img.set_selected(False, True)
        angle_list.append(tup)
      else:
        # if image and handheld quad angle is too big dont show indicator
        pass
    angle_list.sort(key=lambda tup: tup[1])

    lense_center_pos = self.Transform2.value.get_translate()
    for tup in angle_list:
      if tup[0].frustum.contains(lense_center_pos):
        # TODO: Might be good ideas:
        # - adapt frustum size based on distance of hendheld lense 
        # - check if the corners of the quad are in the frustum

        # print('in frustum')
        closest_id = tup[0].id
        return closest_id
      else:
        closest_id = None

    # if hendheld is in no frustum, get images just by angle
    if closest_id is None:
      # print('not in any frustum')
      if len(angle_list) > 0:
        closest_id = angle_list[0][0].id
      else: 
        print('NO view')
        closest_id = 0

    return closest_id

  def find_closest_perspective(self):
    closest_id = None

    # get direction vector of hendheld lense
    # pos = self.Transform2.value.get_translate()
    lense_pos= self.projection_lense.WorldTransform.value.get_translate()
    # print(lense_pos)
    # inv *avango.osg.make_inverse_mat(self.HeadTransform.value)
    # _rot_mat = avango.gua.make_rot_mat(self.Transform2.value.get_rotate_scale_corrected()) * avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0)
    _rot_mat = avango.gua.make_rot_mat(self.projection_lense.WorldTransform.value.get_rotate_scale_corrected())
    _abs_dir = _rot_mat * avango.gua.Vec3(0.0,0.0,-1.0)
    _abs_dir = avango.gua.Vec3(_abs_dir.x,_abs_dir.y,_abs_dir.z) # cast to vec3
    # print(_abs_dir)

    angle_list = []

    for img in self.localized_image_list:
      dir_vec = lense_pos - img.position
      dir_vec.normalize()
      dot = _abs_dir.x*dir_vec.x + _abs_dir.y*dir_vec.y + _abs_dir.z*dir_vec.z
      # make sure values are in range for acos
      dot = 1 if dot >= 1 else dot
      dot = -1 if dot <= -1 else dot
      a = math.acos(dot)
      # print(img.id, a)
      # filter by angle
      if a < 10:
        t = (img, a)
        # if angle between image nad quad is small show img direction indicator 
        # img.set_selected(False, True)
        angle_list.append(t)

    angle_list.sort(key=lambda tup: tup[1])

    for tup in angle_list:

      if tup[0].frustum.contains(lense_pos):
        # TODO: Might be good ideas:
        # - adapt frustum size based on distance of hendheld lense 
        # - check if the corners of the quad are in the frustum

        # print('in frustum')
        closest_id = tup[0].id
        print('in frus', closest_id)
        return closest_id
      else:
        closest_id = None

    # if hendheld is in no frustum, get images just by angle
    if closest_id is None:
      if len(angle_list) > 0:
        closest_id = angle_list[0][0].id
      else: 
        print('NO view')
        closest_id = 0

    print('image id :', closest_id)
    return closest_id

  def find_closest_view(self):
    closest_id = None

    # get direction vector of hendheld lense
    pos = self.Transform2.value.get_translate()
    # inv *avango.osg.make_inverse_mat(self.HeadTransform.value)
    # _rot_mat = avango.gua.make_rot_mat(self.Transform2.value.get_rotate_scale_corrected()) * avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0)
    _rot_mat = avango.gua.make_rot_mat(self.Transform2.value.get_rotate_scale_corrected())
    _abs_dir = _rot_mat * avango.gua.Vec3(0.0,0.0,-1.0)
    _abs_dir = avango.gua.Vec3(_abs_dir.x,_abs_dir.y,_abs_dir.z) # cast to vec3

    angle_list = []
    angle_list2 = []
    
    # get angles between hendheld lense and images
    for img in self.localized_image_list:
      dot = _abs_dir.x*img.direction.x + _abs_dir.y*img.direction.y + _abs_dir.z*img.direction.z
      # make sure values are in range for acos
      dot = 1 if dot >= 1 else dot
      dot = -1 if dot <= -1 else dot
      a = math.acos(dot)
      # filter by angle
      if a < 10:
        tup = (img, a)
        # if angle between image nad quad is small show img direction indicator 
        # img.set_selected(False, True)
        angle_list.append(tup)

    for tup in angle_list:
      dir_vec = pos - tup[0].position
      dot = _abs_dir.x*dir_vec.x + _abs_dir.y*dir_vec.y + _abs_dir.z*dir_vec.z
      # make sure values are in range for acos
      dot = 1 if dot >= 1 else dot
      dot = -1 if dot <= -1 else dot
      a = math.acos(dot)
      # filter by angle
      if a < 1:
        t = (tup[0], a)
        # if angle between image nad quad is small show img direction indicator 
        # img.set_selected(False, True)
        angle_list2.append(t)

    angle_list2.sort(key=lambda tup: tup[1])

    lense_center_pos = self.Transform2.value.get_translate()
    for tup in angle_list2:

      if tup[0].frustum.contains(lense_center_pos):
        # TODO: Might be good ideas:
        # - adapt frustum size based on distance of hendheld lense 
        # - check if the corners of the quad are in the frustum

        # print('in frustum')
        closest_id = tup[0].id
        return closest_id
      else:
        closest_id = None

    # if hendheld is in no frustum, get images just by angle
    if closest_id is None:
      # print('not in any frustum')
      if len(angle_list2) > 0:
        closest_id = angle_list2[0][0].id
      else: 
        print('NO view')
        closest_id = 0

    return closest_id


  @field_has_changed(Transform)
  def update_matrices(self):
    # if self.button0_pressed:
    # self.group_node.Transform.value = self.Transform.value
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

  @field_has_changed(Texture)
  def update_texture(self):
    self.Material.value.set_uniform("projective_texture", self.Texture.value)
    self.Material.value.set_uniform("Emissivity", 1.0)
    self.Material.value.EnableVirtualTexturing.value = True
    # self.Material.value.EnableBackfaceCulling.value = True

  @field_has_changed(Button0)
  def button0_changed(self):
    # pass
    if self.Button0.value:
      self.update_perspective()

      self.button0_pressed = True
      # self.projection_lense.Material.connect_from(self.Material)
      print('left pressed')
    else:
      if self.button0_pressed:
        # lense_pos = self.projection_lense.WorldTransform.value.get_translate()
        # projector_pos1 = self.localized_image_list[self.old_closest_id].transform.get_translate()
        # projector_pos = self.Transform.value.get_translate()
        lense_mat = self.projection_lense.WorldTransform.value
        projector_mat = self.localized_image_list[self.old_closest_id].transform
        # projector_pos = self.Transform.value.get_translate()

        # print('proj pos list ',projector_pos1)
        # print('proj pos trans',projector_pos)

        # self.offset = projector_pos - lense_pos
        self.offset = avango.gua.make_inverse_mat(lense_mat) * projector_mat
        print(self.offset)



      # if self.button0_pressed:
      # lense_pos = self.projection_lense.WorldTransform.value.get_translate()
      # projector_pos = self.localized_image_list[self.old_closest_id].transform.get_translate()
      # self.offset = projector_pos - lense_pos 
      # print('b released')
      # TODO: create offset and move projector according to object position
      lense_pos = self.projection_lense.WorldTransform.value.get_translate()
      lense_rot = self.projection_lense.WorldTransform.value.get_rotate_scale_corrected()
      lense_scl = self.projection_lense.WorldTransform.value.get_scale()
      
      # self.Transform.value = avango.gua.make_trans_mat(lense_pos) * avango.gua.make_trans_mat(self.offset) *\ 
      #                        avango.gua.make_rot_mat(lense_rot) * avango.gua.make_rot_mat(180,0,0,1) *\ 
      #                        avango.gua.make_scale_mat(lense_scl) 
      # self.Transform.value = avango.gua.make_trans_mat(lense_pos) * avango.gua.make_rot_mat(lense_rot) *\
      #                        avango.gua.make_trans_mat(self.offset) #* avango.gua.make_scale_mat(lense_scl) 
      self.Transform.value = self.projection_lense.WorldTransform.value * self.offset
      # self.Transform.value = avango.gua.make_inverse_mat(self.projection_lense.WorldTransform.value) * avango.gua.make_trans_mat(self.offset)

      self.freeze_projection = True
      # if self.projection_lense:
      #   self.projection_lense.Material.disconnect_from(self.Material)
      self.button0_pressed = False
      # print('b not pressed')

  @field_has_changed(Button1)
  def button1_changed(self):
    # if right mouse pressed
    if self.Button1.value:
      if self.button1_pressed == False:
        if self.show_lense:  
          self.lense_parent_node.Children.value.remove(self.projection_lense)
          self.show_lense = False
        else:
          self.lense_parent_node.Children.value.append(self.projection_lense)
          self.show_lense = True

      self.button1_pressed = True
      
    else:
      self.button1_pressed = False
      
      
      
  def update_perspective(self):
    if self.last_lense_pos:
      # if self.button0_pressed:
      # if (self.Transform2.value.get_translate().x != self.last_lense_pos.x
      #  or self.Transform2.value.get_translate().y != self.last_lense_pos.y 
      #  or self.Transform2.value.get_translate().z != self.last_lense_pos.z):
       
      # closest_id = self.find_closest_view()
      closest_id = self.find_closest_perspective()
      
      # self.localized_image_list[self.old_closest_id].set_selected(False, False)
      self.min_tex_coords = self.localized_image_list[closest_id].min_uv
      self.max_tex_coords = self.localized_image_list[closest_id].max_uv
      self.screen.Width.value = self.localized_image_list[closest_id].img_w_half * 2
      self.screen.Height.value = self.localized_image_list[closest_id].img_h_half * 2
      # self.localized_image_list[closest_id].set_selected(True, True)
      self.last_lense_pos = self.projection_lense.WorldTransform.value.get_translate()
      self.old_closest_id = closest_id
      self.Transform.value = self.localized_image_list[closest_id].transform
      
      # self.localized_image_list[self.old_closest_id].set_selected(True, True)
    else:
      # self.last_lense_pos = self.Transform2.value.get_translate()
      self.last_lense_pos = self.projection_lense.WorldTransform.value.get_translate()

  def evaluate(self):
    pass

    # _rot_mat = avango.gua.make_rot_mat(mat.get_rotate_scale_corrected())
    # _abs_dir = _rot_mat * avango.gua.Vec3(0.0,0.0,-1.0)
    # _abs_dir = avango.gua.Vec3(_abs_dir.x,_abs_dir.y,_abs_dir.z) # cast to vec3

    # _vec1.normalize()
    # _vec2.normalize()
    # axis = vec1.cross(vec2)
    # angle = math.acos(vec1*vec2)

