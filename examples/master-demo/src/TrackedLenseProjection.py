import avango
import avango.script
import avango.gua
from avango.script import field_has_changed

import random
import math
import src.provenance_utils as pu
import time
import json

import statistics 
from statistics import mode 


class TrackedLenseProjection(avango.script.Script):

  Material = avango.gua.SFMaterial()
  Transform = avango.gua.SFMatrix4()
  Graph = avango.gua.SFSceneGraph()
  Button0 = avango.SFBool()
  Button1 = avango.SFBool()
  Button2 = avango.SFBool()

  Texture = avango.SFString()

  def __init__(self):
    self.super(TrackedLenseProjection).__init__()
    self.always_evaluate(False)

    self.is_initialized = False

  def my_constructor(self, graph):
    self.indicator = None
    self.loader = avango.gua.nodes.TriMeshLoader()
    self.graph = graph

    self.localized_image_list = []
    self.position_list = []
    self.projection_lense = None
    self.tracked_node = None
    self.lense_parent_node = None
    self.show_lense = False
    self.min_tex_coords = avango.gua.Vec2(0.0, 0.0)
    self.max_tex_coords = avango.gua.Vec2(1.0, 1.0)
    self.last_lense_pos = None
    self.old_closest_id = 0
    self.button0_pressed = False
    self.button1_pressed = False
    self.button2_pressed = False
    self.freeze_flag = False
    self.sliding_window = []
    self.frozen = 0
    self.center = avango.gua.make_trans_mat(0.0, 0.0, 0.0)
    
    self.offset = avango.gua.make_identity_mat()
    
    
    self.group_node = avango.gua.nodes.TransformNode(Name = "projector_group")
    self.group_node.Transform.connect_from(self.Transform)

    self.geometry = self.loader.create_geometry_from_file("projector_geometry", "data/objects/projector.obj", 
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

    self.frame_mat = avango.gua.nodes.Material()
    frame_color = avango.gua.Vec4(1.0, 0.1, 0.1, 0.3)
    # sphere_color.normalize()
    self.frame_mat.set_uniform("Color", frame_color)
    self.frame_mat.set_uniform("Roughness", 1.0)
    self.frame_mat.set_uniform("Emissivity", 1.0)
    self.frame_mat.EnableBackfaceCulling.value = False

    self.frame_transform = avango.gua.nodes.TransformNode(Name='frame_transform')
    self.frame_scale = avango.gua.nodes.TransformNode(Name='frame_transform')
    self.frame_scale.Transform.value = avango.gua.make_scale_mat(0.2)

    self.frame_indicator = self.loader.create_geometry_from_file(
        "frame_indicator", "data/objects/marker.obj",
        self.frame_mat, avango.gua.LoaderFlags.DEFAULTS)
    self.frame_indicator.Transform.value = avango.gua.make_scale_mat(0.01,0.01, 0.10) * avango.gua.make_rot_mat(90.0, 1.0, 0.0, 0.0)   
    self.frame_transform.Children.value.append(self.frame_indicator)

    proj_mat_desc = avango.gua.nodes.MaterialShaderDescription()
    proj_mat_desc.load_from_file("data/materials/Projective_VT_Material.gmd")

    # proj_mat_desc.EnableVirtualTexturing.value = True
    avango.gua.register_material_shader(proj_mat_desc, "proj_mat")
    self.Material.value.set_uniform("my_color", avango.gua.Vec4(0.2,0.2,0.2,0.0))
    frame_left = self.loader.create_geometry_from_file(
        "frame_left", "data/objects/cube.obj",
        self.frame_mat, avango.gua.LoaderFlags.DEFAULTS)
    frame_left.Transform.value = avango.gua.make_trans_mat(-1.0,0.0,0.0) * avango.gua.make_scale_mat(0.01, 2.0, 0.01)
    frame_right = self.loader.create_geometry_from_file(
        "frame_left", "data/objects/cube.obj",
        self.frame_mat, avango.gua.LoaderFlags.DEFAULTS)
    frame_right.Transform.value = avango.gua.make_trans_mat(1.0,0.0,0.0) * avango.gua.make_scale_mat(0.01, 2.0, 0.01)
    frame_top = self.loader.create_geometry_from_file(
        "frame_left", "data/objects/cube.obj",
        self.frame_mat, avango.gua.LoaderFlags.DEFAULTS)
    frame_top.Transform.value = avango.gua.make_trans_mat(0.0,1.0,0.0) * avango.gua.make_scale_mat(2.0, 0.01, 0.01)
    frame_bot = self.loader.create_geometry_from_file(
        "frame_left", "data/objects/cube.obj",
        self.frame_mat, avango.gua.LoaderFlags.DEFAULTS)
    frame_bot.Transform.value = avango.gua.make_trans_mat(0.0,-1.0,0.0) * avango.gua.make_scale_mat(2.0, 0.01, 0.01)
    self.frame_transform.Children.value.append(self.frame_scale)
    self.frame_scale.Children.value.append(frame_left)
    self.frame_scale.Children.value.append(frame_right)
    self.frame_scale.Children.value.append(frame_top)
    self.frame_scale.Children.value.append(frame_bot)
    

    # self.orientation_frame = 

    
  def set_scenegraph(self, graph):
    print('scene graph got set')
    self.graph = graph

  def set_lens_center(self, center):
    self.center = center
    self.frame_transform.Transform.value = self.center

  def set_projection_lense(self, obj, parent_node, tracked_node):
    if self.projection_lense is not None:
      self.projection_lense.Material.disconnect_from(self.Material)
    self.projection_lense = obj
    self.tracked_node = tracked_node
    self.lense_parent_node = parent_node
    if self.projection_lense and self.lense_parent_node:
      self.show_lense = True
    self.projection_lense.Material.connect_from(self.Material)

  def set_localized_image_list(self, localized_image_list):
    self.localized_image_list = localized_image_list
    self.position_list = [li.position for li in self.localized_image_list]
    print('set atlas tile list',len(self.localized_image_list), len(self.position_list))
    self.min_tex_coords = self.localized_image_list[self.old_closest_id].min_uv
    self.max_tex_coords = self.localized_image_list[self.old_closest_id].max_uv


  def find_closest_perspective(self):
    closest_id = None
    # lense_mat =  avango.gua.make_inverse_mat(avango.gua.make_trans_mat(0.0,-1.445,2.0)) * self.projection_lense.WorldTransform.value 
    lense_mat =  avango.gua.make_inverse_mat(avango.gua.make_trans_mat(0.0,-1.445,2.0)) * self.tracked_node.WorldTransform.value 
    # lense_pos= self.projection_lense.WorldTransform.value.get_translate()
    lense_pos= self.tracked_node.WorldTransform.value.get_translate()
    # _rot_mat = avango.gua.make_rot_mat(self.projection_lense.WorldTransform.value.get_rotate_scale_corrected())
    _rot_mat = avango.gua.make_rot_mat(self.tracked_node.WorldTransform.value.get_rotate_scale_corrected())
    _abs_dir = _rot_mat * avango.gua.Vec3(0.0,0.0,-1.0)
    _abs_dir = avango.gua.Vec3(_abs_dir.x,_abs_dir.y,_abs_dir.z) # cast to vec3
    angle_list = []
    for img in self.localized_image_list:
      dir_vec = lense_pos - img.position
      dir_vec.normalize()
      dot = _abs_dir.x*dir_vec.x + _abs_dir.y*dir_vec.y + _abs_dir.z*dir_vec.z
      # make sure values are in range for acos
      dot = 1 if dot >= 1 else dot
      dot = -1 if dot <= -1 else dot
      a = math.acos(dot)
      # filter by angle
      if math.degrees(a) < 20:
        t = (img, a)
        angle_list.append(t)

    angle_list.sort(key=lambda tup: tup[1])
    for tup in angle_list:
      if tup[0].frustum.contains(lense_pos):
        closest_id = tup[0].id
        return closest_id
      else:
        closest_id = None

    # if hendheld is in no frustum, get images just by angle
    if closest_id is None:
      if len(angle_list) > 0:
        closest_id = angle_list[0][0].id
      else: 
        closest_id = 0

    return closest_id

  def freeze(self):
    lense_mat = self.tracked_node.WorldTransform.value
    projector_mat = self.localized_image_list[self.old_closest_id].transform
    self.Material.value.set_uniform("my_color", avango.gua.Vec4(0.2,0.2,0.2,1.0))
    # self.Material.value.set_uniform("my_color", avango.gua.Vec4(0.2,0.2,0.2,0.0))
    self.offset = avango.gua.make_inverse_mat(lense_mat) * projector_mat


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
    # self.Material.value.set_uniform("color", avango.gua.Vec4(1.0, 1.0, 1.0, 1.0) )
    self.Material.value.set_uniform("Roughness", 1.0)
    self.Material.value.set_uniform("Metalness", 0.0)
    self.Material.value.set_uniform("view_port_min", avango.gua.Vec2(self.min_tex_coords))
    self.Material.value.set_uniform("view_port_max", avango.gua.Vec2(self.max_tex_coords))
    # self.Material.value.set_uniform("my_color", avango.gua.Vec4(0.2,0.2,0.2,1.0))

  @field_has_changed(Texture)
  def update_texture(self):
    self.Material.value.set_uniform("projective_texture", self.Texture.value)
    self.Material.value.set_uniform("Emissivity", 1.0)
    self.Material.value.EnableVirtualTexturing.value = True
    # self.Material.value.EnableBackfaceCulling.value = True

  @field_has_changed(Button0)
  def button0_changed(self):
    if self.Button0.value:
      print('Lens Button 0')
      if self.freeze_flag == True:
        self.freeze_flag = False
        self.Material.value.set_uniform("my_color", avango.gua.Vec4(0.2,0.2,0.2,0.85))
      elif self.freeze_flag == False:
        self.freeze_flag = True
        self.Material.value.set_uniform("my_color", avango.gua.Vec4(0.2,0.2,0.2,1.0))
      self.always_evaluate(True)
      self.update_perspective()

      self.button0_pressed = True
      # print('TASTE 0')
      # self.projection_lense.Material.connect_from(self.Material)
    else:
      
      if self.button0_pressed:
        # self.freeze_flag = False
        self.frozen += 1
        print('frozen', self.frozen)
        # lense_mat = self.projection_lense.WorldTransform.value
        lense_mat = self.tracked_node.WorldTransform.value
        projector_mat = self.localized_image_list[self.old_closest_id].transform
        # self.Material.value.set_uniform("my_color", avango.gua.Vec4(0.2,0.2,0.2,1.0))
        # self.Material.value.set_uniform("my_color", avango.gua.Vec4(0.2,0.2,0.2,0.0))
        self.offset = avango.gua.make_inverse_mat(lense_mat) * projector_mat
        self.always_evaluate(True)
      # self.Transform.value = self.projection_lense.WorldTransform.value * self.offset
      self.Transform.value = self.tracked_node.WorldTransform.value * self.offset

      self.button0_pressed = False

  @field_has_changed(Button1)
  def button1_changed(self):
    # if right mouse pressed
    if self.Button1.value:
      print('Lens Button 1')
      # print('TASTE 1')
      if self.button1_pressed == False:
        if self.show_lense:  
          # self.set_frame()
          # self.lense_parent_node.Children.value.remove(self.projection_lense)
          self.show_lense = False
        else:
          # self.lense_parent_node.Children.value.append(self.projection_lense)
          self.show_lense = True

      self.button1_pressed = True
      
    else:
      self.button1_pressed = False

  @field_has_changed(Button2)
  def button2_changed(self):
    if self.Button2.value:
      print('Lens Button 2')
      # print('TASTE 1')
      if self.button2_pressed == False:
        if self.show_lense:  
          # self.freeze()
          self.show_lense = False
        else:
          
          self.show_lense = True

      self.button2_pressed = True
      
    else:
      self.button2_pressed = False

  def turn_on(self):
    self.always_evaluate(True)
    self.button0_pressed = True
    self.Material.value.set_uniform("my_color", avango.gua.Vec4(0.2,0.2,0.2,0.85))
    self.freeze_flag = False
      
  def turn_off(self):
    self.Material.value.set_uniform("my_color", avango.gua.Vec4(0.2,0.2,0.2,0.0))
    self.freeze_flag = True
    self.always_evaluate(False)
    self.frozen = 0
      
  def update_perspective(self, freeze_flag=False):
    if self.last_lense_pos:
      # print('update_perspective')
      # closest_id = self.find_closest_view()
      if freeze_flag == False:
        closest_id = self.find_closest_perspective()

        # sliding window approach
        window_size = 3
        if len(self.sliding_window) < window_size:
          self.sliding_window.append(closest_id)
        elif len(self.sliding_window) == window_size:
          self.sliding_window.pop(0)
          self.sliding_window.append(closest_id)

        closest_id = most_frequent(self.sliding_window)
        # print(closest_id, self.sliding_window)

      self.min_tex_coords = self.localized_image_list[closest_id].min_uv
      self.max_tex_coords = self.localized_image_list[closest_id].max_uv
    
      tile_scale = 0.2

      self.screen.Width.value = self.localized_image_list[closest_id].img_w_half / 3.00 * (1 / tile_scale)
      
      self.screen.Height.value = self.localized_image_list[closest_id].img_h_half / 3.00 * (1 / tile_scale)
      # self.localized_image_list[closest_id].set_selected(True, True)
      # self.last_lense_pos = self.projection_lense.WorldTransform.value.get_translate()
      self.last_lense_pos = self.tracked_node.WorldTransform.value.get_translate()
      self.old_closest_id = closest_id
      self.Transform.value = self.localized_image_list[closest_id].transform


      lense_mat = self.tracked_node.WorldTransform.value
      lense_rot = avango.gua.make_rot_mat(lense_mat.get_rotate_scale_corrected()) 
      self.frame_transform.Transform.value =  self.center * avango.gua.make_inverse_mat(lense_rot) * self.localized_image_list[closest_id].rotation
      
      # self.localized_image_list[self.old_closest_id].set_selected(True, True)
    else:
      # self.last_lense_pos = self.Transform2.value.get_translate()
      # self.last_lense_pos = self.projection_lense.WorldTransform.value.get_translate()
      self.last_lense_pos = self.tracked_node.WorldTransform.value.get_translate()

  def evaluate(self):
    # print(' Freeze flag ',self.freeze_flag)
    if self.freeze_flag:
      print('fro')
      # self.Transform.value = self.projection_lense.WorldTransform.value * self.offset
      self.Transform.value = self.tracked_node.WorldTransform.value * self.offset
    # elif self.keep_perspective:
    #   print()
    else:
      self.update_perspective()

def most_frequent(List): 
    return max(set(List), key = List.count) 


