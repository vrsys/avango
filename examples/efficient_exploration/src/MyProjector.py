import avango
import avango.script
import avango.gua
from avango.script import field_has_changed

import random
import math
import src.provenance_utils as pu
import time


class MyProjector(avango.script.Script):

  Material = avango.gua.SFMaterial()
  Transform = avango.gua.SFMatrix4()
  Graph = avango.gua.SFSceneGraph()
  Texture = avango.SFString()

  def __init__(self):
    self.super(MyProjector).__init__()
    self.always_evaluate(True)

    self.is_initialized = False

  def my_constructor(self, name):
    self.indicator = None
    loader = avango.gua.nodes.TriMeshLoader()

    self.name = name

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
    self.new_id = 1
    self.button0_pressed = False
    self.offset = avango.gua.make_identity_mat()
    self.selected_photo_id = 0
    self.frame_count = 0
    
    self.group_name = "projector_group_" + self.name
    self.group_node = avango.gua.nodes.TransformNode(Name = self.group_name)
    self.group_node.Transform.connect_from(self.Transform)

    self.geometry = loader.create_geometry_from_file("projector_geometry", "data/objects/projector.obj", 
      avango.gua.LoaderFlags.NORMALIZE_SCALE |
      avango.gua.LoaderFlags.NORMALIZE_POSITION | 
      avango.gua.LoaderFlags.LOAD_MATERIALS)
    self.geometry.Transform.value = avango.gua.make_scale_mat(0.1)
    self.group_node.Children.value.append(self.geometry)

    self.screen = avango.gua.nodes.ScreenNode(
      Name = "screen1",
      Width = 0.5,
      Height = 0.5,
      Transform = avango.gua.make_trans_mat(0.0, 0.0, -0.1)
    )
    self.group_node.Children.value.append(self.screen)
    path_str = "/net/multi_view_trans_node/"
    self.cam = avango.gua.nodes.CameraNode(
      LeftScreenPath = path_str+self.group_name+"/screen1",
      # LeftScreenPath = "/"+self.group_name+"/screen1",
      # LeftScreenPath = "scene_trans/projector_group/screen1",
      SceneGraph = "scenegraph",
    )
    self.group_node.Children.value.append(self.cam)

    self.Material.value = avango.gua.nodes.Material(
      ShaderName = "proj_mat"+self.name
    )

    self.Graph.value = avango.gua.nodes.SceneGraph(Name = "dummy"+self.name)

    proj_mat_desc = avango.gua.nodes.MaterialShaderDescription()
    proj_mat_desc.load_from_file("data/materials/Projective_VT_Material.gmd")

    # proj_mat_desc.EnableVirtualTexturing.value = True
    avango.gua.register_material_shader(proj_mat_desc, "proj_mat"+self.name)
    
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

  def get_new_perspective(self):
    return self.new_id

  def set_image_id(self, i_id):
    self.new_id = i_id

  def set_offset(self, offset):
    self.offset = offset

  @field_has_changed(Transform)
  def update_matrices(self):
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

  @field_has_changed(Texture)
  def update_texture(self):
    self.Material.value.set_uniform("projective_texture", self.Texture.value)
    self.Material.value.set_uniform("Emissivity", 1.0)
    self.Material.value.EnableVirtualTexturing.value = True
    # self.Material.value.EnableBackfaceCulling.value = True

  # @field_has_changed(Button0)
  def button0_changed(self):
    # pass
    # if self.Button0.value:
    self.update_perspective()
      
  def change_photo_projection(self, image_id ):
    print('change id', image_id)
    self.set_image_id(image_id)
    self.update_perspective()
    self.button0_changed()
    print('update')
    
      
  def update_perspective(self):
    if self.last_lense_pos:

      closest_id = self.get_new_perspective()
      print("#####################", closest_id)
      
      # self.min_tex_coords = self.localized_image_list[closest_id].min_uv
      # self.max_tex_coords = self.localized_image_list[closest_id].max_uv
      self.min_tex_coords = self.localized_image_list[closest_id].max_uv
      self.max_tex_coords = self.localized_image_list[closest_id].min_uv
      self.screen.Width.value = self.localized_image_list[closest_id].img_w_half * 1.8
      self.screen.Height.value = self.localized_image_list[closest_id].img_h_half * 1.8
      # self.localized_image_list[closest_id].set_selected(True, True)
      self.last_lense_pos = self.projection_lense.WorldTransform.value.get_translate()
      self.old_closest_id = closest_id
      # self.Transform.value = self.localized_image_list[closest_id].transform
      self.update_matrices()
    else:
      # self.last_lense_pos = self.Transform2.value.get_translate()
      self.last_lense_pos = self.projection_lense.WorldTransform.value.get_translate()

  def evaluate(self):
    pass
    # self.frame_count += 1
    # self.frame_count = self.frame_count % 120
    # if self.frame_count == 0:
    #   self.selected_photo_id += 1
    #   self.set_image_id(self.selected_photo_id)
    #   self.button0_changed()
    #   print('update')
    
