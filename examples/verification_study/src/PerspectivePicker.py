import avango
import avango.script
import avango.gua
from avango.script import field_has_changed

import random
import math
import src.provenance_utils as pu
import time


class PerspectivePicker(avango.script.Script):

  Button0 = avango.SFBool()

  def __init__(self):
    self.super(PerspectivePicker).__init__()
    self.always_evaluate(False)

    self.is_initialized = False

  def my_constructor(self):

    self.localized_image_list = []
    self.position_list = []
    self.projection_lense = None
    self.lense_parent_node = None
    self.min_tex_coords = avango.gua.Vec2(0.0, 0.0)
    self.max_tex_coords = avango.gua.Vec2(1.0, 1.0)
    self.old_closest_id = 0
    self.button0_pressed = False
    # self.offset = avango.gua.Vec3(0.0,0.0,0.0)
    self.offset = avango.gua.make_identity_mat()
    self.relevant_perspectives = []
    self.perspective_offsets = []
    self.texture_coordinates = []
    self.visualizer = None
    self.mode = 'texture' # 'projector'


  def set_projection_lense(self, obj, parent_node):
    self.projection_lense = obj
    self.lense_parent_node = parent_node

  def set_localized_image_list(self, localized_image_list):
    self.localized_image_list = localized_image_list
    self.position_list = [li.position for li in self.localized_image_list]
    
  def find_closest_perspective(self):
    print('find')
    closest_id = None

    # get direction vector of hendheld lense
    lense_pos = self.projection_lense.WorldTransform.value.get_translate()
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
      # filter by angle
      if math.degrees(a) < 25:
  
        print(img.id, math.degrees(a))
        t = (img, math.degrees(a))
        # if angle between image nad quad is small show img direction indicator 
        # img.set_selected(False, True)
        angle_list.append(t)

    angle_list.sort(key=lambda tup: tup[1])
    self.relevant_perspectives = []
    self.perspective_offsets = []
    self.texture_coordinates = []

    for tup in angle_list:
      
      if tup[0].frustum.contains(lense_pos):
        # TODO: Might be good ideas:
        # - adapt frustum size based on distance of hendheld lense 
        # - check if the corners of the quad are in the frustum
        _img = tup[0]

        # step 1: get vector between camera and lense
        # lense_pos = lense_trans.WorldTransform.value.get_translate()
        line_p = _img.position
        line_d = lense_pos - line_p
        print(lense_pos)
        line_d.normalize()

        # step 2: direction vector of the camera
        plane_p = _img.screen_transform.get_translate()
        _rot_mat = avango.gua.make_rot_mat(_img.transform.get_rotate_scale_corrected())
        plane_n = _rot_mat * avango.gua.Vec3(0.0,0.0,-1.0)
        plane_n = avango.gua.Vec3(plane_n.x,plane_n.y,plane_n.z) # cast to vec3

        pnpp_dot = plane_n.x*plane_p.x + plane_n.y*plane_p.y + plane_n.z*plane_p.z


        pnlp_dot = plane_n.x*line_p.x + plane_n.y*line_p.y + plane_n.z*line_p.z


        pnld_dot = plane_n.x*line_d.x + plane_n.y*line_d.y + plane_n.z*line_d.z


        t = (pnpp_dot - pnlp_dot) / pnld_dot;

        intersection_pos =  line_p + (line_d * t);

        x_values = [plane_p.x - _img.tile_width/2, plane_p.x + _img.tile_width/2]
        y_values = [plane_p.y - _img.tile_height/2, plane_p.y + _img.tile_height/2]

        ratio = 4.07 / 2.30

        u_coord = map_from_to(intersection_pos.x, x_values[0], x_values[1], _img.min_uv.x, _img.max_uv.x)
        v_coord = map_from_to(intersection_pos.y, y_values[0], y_values[1], _img.min_uv.y, _img.max_uv.y)

        if u_coord > _img.min_uv.x and u_coord < _img.max_uv.x:
          print('yes')
        zoom_factor = 1.5
        max_uv = avango.gua.Vec2(u_coord - (_img.t_w / 2 / zoom_factor) , v_coord - (_img.t_w / 2 / zoom_factor))
        # min_uv = avango.gua.Vec2(u_coord - (_img.tile_w / 2 / zoom_factor) , v_coord - (_img.tile_h / 2 / zoom_factor))
        min_uv = avango.gua.Vec2(u_coord + (_img.t_w / 2 / zoom_factor) , v_coord + (_img.t_w / 2 / zoom_factor))
        # max_uv = avango.gua.Vec2(u_coord + (_img.tile_w / 2 / zoom_factor) , v_coord + (_img.tile_h / 2 / zoom_factor))
        min_max_coords = [min_uv, max_uv]

        closest_id = tup[0].id
        lense_mat = self.projection_lense.WorldTransform.value
        projector_mat = self.localized_image_list[closest_id].transform
        offset = avango.gua.make_inverse_mat(lense_mat) * projector_mat

        self.relevant_perspectives.append(closest_id)
        self.perspective_offsets.append(offset)
        self.texture_coordinates.append(min_max_coords)
      else:
        closest_id = None
    if closest_id != None:
      return self.relevant_perspectives[0]

    # if hendheld is in no frustum, get images just by angle
    if closest_id is None:
      if len(angle_list) > 0:
        closest_id = angle_list[0][0].id
      else: 
        print('NO view')
        closest_id = 0

    print('image id :', closest_id)
    return closest_id

  @field_has_changed(Button0)
  def button0_changed(self):
    if self.Button0.value:
      print('update perspective')
      if self.mode is 'texture':
        self.update_textures()
      if self.mode is 'projector':
        self.update_perspective()
      print(self.relevant_perspectives[0:16])
      self.button0_pressed = True
    else:
      self.button0_pressed = False
      
  def update_perspective(self):
    closest_id = self.find_closest_perspective()
      
    self.old_closest_id = closest_id
    if self.visualizer:
      # print(self.relevant_perspectives[0:8])
      # print(self.perspective_offsets[0])
      self.visualizer.update_images(self.relevant_perspectives[0:self.visualizer.views],
                                    self.perspective_offsets[0:self.visualizer.views])
    else:
      print('No projective visualizer is set.')


  def update_textures(self):
    closest_id = self.find_closest_perspective()
      
    self.old_closest_id = closest_id
    if self.visualizer:
      self.visualizer.update_images(self.relevant_perspectives[0:self.visualizer.views], 
                                    self.texture_coordinates[0:self.visualizer.views])
    else:
      print('No texture visualizer is set.')

  def set_visualizer(self, visualizer, mode):
    self.visualizer = visualizer
    if mode is 'texture' or mode is 'projector':
      self.mode = mode

  def evaluate(self):
    pass

def map_from_to(x,a,b,c,d):
  y=(x-a)/(b-a)*(d-c)+c
  return y
