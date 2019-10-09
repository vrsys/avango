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
    self.projection_lense_mat = avango.gua.make_identity_mat()
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
    self.keep_perspective = True
    self.sliding_window = []
    self.frozen = 0
    self.center = avango.gua.make_trans_mat(0.0, 0.0, 0.0)
    self.calculated = False
    self.state = 0
    self.indicate = False
    # 0 Linse aus, Widget aus
    # 1 Linse an
    # 2 Kugel am Strahl
    # 3 Kugel in der Welt
    # 4 Kugel in der Welt, Linse an
    
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
    self.direction_transform = avango.gua.nodes.TransformNode(Name='direction_transform')
    self.frame_scale = avango.gua.nodes.TransformNode(Name='frame_scale')
    self.frame_scale.Transform.value = avango.gua.make_scale_mat(0.2)

    self.direction_indicator = self.loader.create_geometry_from_file(
        "direction_indicator", "data/objects/marker.obj",
        self.frame_mat, avango.gua.LoaderFlags.DEFAULTS)
    self.direction_transform.Transform.value = avango.gua.make_trans_mat(0.0,0.21,0.0)
    self.direction_indicator.Transform.value = avango.gua.make_scale_mat(0.005,0.2,0.005) * avango.gua.make_rot_mat(-90.0, 0.0, 1.0, 0.0)
    self.direction_transform.Children.value.append(self.direction_indicator)

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

    self.sphere = self.loader.create_geometry_from_file(
        "sphere", "data/objects/sphere3.obj",
        self.frame_mat, avango.gua.LoaderFlags.DEFAULTS)
    self.sphere.Transform.value = avango.gua.make_trans_mat(0.0,0.2,0.0) * avango.gua.make_scale_mat(0.1, 0.1, 0.1)
    self.indicators = []
    self.direction_indicators = []

    self.cam_indicator_mat = avango.gua.nodes.Material()
    sphere_color = avango.gua.Vec4(0.1, 0.1, 1.0, 0.5)
    # sphere_color.normalize()
    self.cam_indicator_mat.set_uniform("Color", sphere_color)
    self.cam_indicator_mat.set_uniform("Roughness", 1.0)
    self.cam_indicator_mat.set_uniform("Emissivity", 1.0)

    self.high_light_mat = avango.gua.nodes.Material()
    sphere_color = avango.gua.Vec4(0.5, 0.1, 0.7, 0.9)
    # sphere_color.normalize()
    self.high_light_mat.set_uniform("Color", sphere_color)
    self.high_light_mat.set_uniform("Roughness", 1.0)
    self.high_light_mat.set_uniform("Emissivity", 1.0)

    self.invis = avango.gua.nodes.Material()
    sphere_color = avango.gua.Vec4(0.1, 0.1, 0.1, 0.0)
    # sphere_color.normalize()
    self.invis.set_uniform("Color", sphere_color)
    self.invis.set_uniform("Roughness", 1.0)
    self.invis.set_uniform("Emissivity", 1.0)

    self.green_material = avango.gua.nodes.Material()
    sphere_color = avango.gua.Vec4(0.1, 0.9, 0.1, 0.5)
    # sphere_color.normalize()
    self.green_material.set_uniform("Color", sphere_color)
    self.green_material.set_uniform("Roughness", 1.0)
    self.green_material.set_uniform("Emissivity", 1.0)

    self.dir_indicator_mat = avango.gua.nodes.Material()
    sphere_color = avango.gua.Vec4(0.5, 0.9, 0.5, 0.5)
    sphere_color.normalize()
    self.dir_indicator_mat.set_uniform("Color", sphere_color)
    self.dir_indicator_mat.set_uniform("Roughness", 1.0)
    self.dir_indicator_mat.set_uniform("Emissivity", 1.0)

    self.camera_indicator = self.loader.create_geometry_from_file(
      "cam_indicator", "data/objects/cube.obj",
      self.cam_indicator_mat, avango.gua.LoaderFlags.DEFAULTS)

    # self.orientation_frame = 
    self.turn_off()

    
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
    self.projection_lense_mat = self.projection_lense.Transform.value
    print('################ PROJECTION LENSE MAT ', self.projection_lense_mat)

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

  def get_heatmap_material(self, alpha):
    mat = avango.gua.nodes.Material()
    col = map_from_to(alpha, 0, 180, 0.5, 0.0)
    rand_color = avango.gua.Vec4(0.2, col+0.5, 0.2, 1.0)
    mat.set_uniform("Color", rand_color)
    mat.set_uniform("Roughness", 1.0)
    mat.set_uniform("Emissivity", 1.0)

    return mat

  def turn_on(self):
    print('Lens turned on')
    self.always_evaluate(True)
    # self.button0_pressed = True
    self.Material.value.set_uniform("my_color", avango.gua.Vec4(0.2,0.2,0.2,0.85))
    self.freeze_flag = False
    self.keep_perspective = True
    self.frame_transform.Children.value.append(self.frame_scale)
      
  def turn_off(self):
    print('Lens turned off')
    self.Material.value.set_uniform("my_color", avango.gua.Vec4(0.2,0.2,0.2,0.0))
    self.freeze_flag = True
    self.always_evaluate(False)
    self.frozen = 0
    self.frame_transform.Children.value.remove(self.frame_scale)
    
      
  def update_perspective(self, freeze_flag=False):
    if self.last_lense_pos:
      if self.keep_perspective:
        closest_id = self.old_closest_id 
      else:
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

      # self.screen.Width.value = self.localized_image_list[closest_id].img_w_half * (1 / tile_scale)
      self.screen.Width.value = self.localized_image_list[closest_id].img_w_half / 3.00 * (1 / tile_scale)
      
      # self.screen.Height.value = self.localized_image_list[closest_id].img_h_half * (1 / tile_scale)
      self.screen.Height.value = self.localized_image_list[closest_id].img_h_half / 3.00 * (1 / tile_scale)

      self.last_lense_pos = self.tracked_node.WorldTransform.value.get_translate()
      if self.indicate:
        self.direction_indicators[self.old_closest_id].Material.value = self.green_material
        self.direction_indicators[closest_id].Material.value = self.high_light_mat
      self.old_closest_id = closest_id
      self.Transform.value = self.localized_image_list[closest_id].transform

      lense_mat = self.tracked_node.WorldTransform.value
      
      lense_rot = avango.gua.make_rot_mat(lense_mat.get_rotate_scale_corrected()) 
      self.frame_transform.Transform.value =  self.center * avango.gua.make_inverse_mat(lense_rot) * self.localized_image_list[closest_id].rotation
      # self.direction_transform.Transform.value =  self.center * avango.gua.make_inverse_mat(lense_rot) * avango.gua.make_rot_mat(_rot_mat)
    else:
      self.last_lense_pos = self.tracked_node.WorldTransform.value.get_translate()

  def evaluate(self):
    if self.state == 1 or self.state == 4:
      if self.freeze_flag:
        # self.Transform.value = self.projection_lense.WorldTransform.value * self.offset
        self.Transform.value = self.tracked_node.WorldTransform.value * self.offset
      else:
        self.update_perspective()

    if self.state == 3 or self.state == 4:
      # calc lense indicator
      lense_pos = self.tracked_node.WorldTransform.value.get_translate()
      C = self.sphere.WorldTransform.value.get_translate()
      r = 0.1
      P = lense_pos
      U = C - P # direction vector
      U.normalize()
      _rot_mat = avango.gua.make_rot_mat(self.tracked_node.WorldTransform.value.get_rotate_scale_corrected())
      B = _rot_mat * avango.gua.Vec3(0.0,0.0,-1.0)
      B = avango.gua.Vec3(B.x,B.y,B.z) # 
      _dot = U.x*B.x + U.y*B.y + U.z*B.z
      _dot = 1 if _dot >= 1 else _dot
      _dot = -1 if _dot <= -1 else _dot
      alpha = math.degrees(math.acos(_dot))
      if alpha <= 180:
        Q = P - C
        a = 1
        dotUQ = U.x*Q.x + U.y*Q.y + U.z*Q.z
        b = 2*dotUQ
        dotQQ = Q.x*Q.x + Q.y*Q.y + Q.z*Q.z
        c = dotQQ - r*r;
        d = b*b - 4*a*c;  # discriminant of quadratic
        
        if d >= 0:
          sol1 = (-b- math.sqrt(d))/(2*a)    
          if sol1 >= 0:
            P1 = avango.gua.Vec3(P.x + sol1*U.x, P.y + sol1*U.y, P.z + sol1*U.z); # first intersection
            self.camera_indicator.Transform.value = avango.gua.make_trans_mat(P1) * avango.gua.make_scale_mat(0.005)

      # calculate indicators
      
      if self.calculated == False:
        if self.indicate:
          C = self.sphere.WorldTransform.value.get_translate()
          r = 0.1
          for idx, ind in enumerate(self.direction_indicators):
            # self.graph.Root.value.Children.value.remove(ind)
            # P = self.indicators[idx].WorldTransform.value.get_translate()
            P = self.indicators[idx].get_translate()
            U = C - P # direction vector
            U.normalize()
            # self.tracked_node.WorldTransform.value
            _rot_mat = avango.gua.make_rot_mat(self.tracked_node.WorldTransform.value.get_rotate_scale_corrected())
            B = _rot_mat * avango.gua.Vec3(0.0,0.0,-1.0)
            B = avango.gua.Vec3(B.x,B.y,B.z) # 
            _dot = U.x*B.x + U.y*B.y + U.z*B.z
            _dot = 1 if _dot >= 1 else _dot
            _dot = -1 if _dot <= -1 else _dot
            alpha = math.degrees(math.acos(_dot))
            if alpha <= 180:
              Q = P - C
              a = 1
              dotUQ = U.x*Q.x + U.y*Q.y + U.z*Q.z
              b = 2*dotUQ
              dotQQ = Q.x*Q.x + Q.y*Q.y + Q.z*Q.z
              c = dotQQ - r*r;
              d = b*b - 4*a*c;  # discriminant of quadratic
              
              if d >= 0:
                # then solutions are real, so there are intersections
                sol1 = (-b- math.sqrt(d))/(2*a)    
                if sol1 >= 0:
                  P1 = avango.gua.Vec3(P.x + sol1*U.x, P.y + sol1*U.y, P.z + sol1*U.z); # first intersection
                  
                  # self.graph.Root.value.Children.value.append(ind)
                  ind.Transform.value = avango.gua.make_trans_mat(P1) * avango.gua.make_scale_mat(0.005, 0.005, 0.005)
          self.calculated = True

  def toggle_indicators(self, flag):

    if flag:
      print('Show indicators', len(self.localized_image_list))
      self.indicate = True
      self.indicate_perspectives = True
      self.graph.Root.value.Children.value.append(self.camera_indicator)
      self.camera_indicator.Transform.value = avango.gua.make_scale_mat(0.005)
      for i in self.localized_image_list:
        pos = i.position
        image_mat = i.transform
        indicator_mat =  image_mat * avango.gua.make_scale_mat(0.01, 0.01, 0.1)
        
        dir_indicator = self.loader.create_geometry_from_file(
            "dir_indicator_" + str(len(self.indicators)), "data/objects/cube.obj",
            self.green_material, avango.gua.LoaderFlags.DEFAULTS)
        self.direction_indicators.append(dir_indicator)
        self.graph.Root.value.Children.value.append(dir_indicator)
        self.indicators.append(indicator_mat)
      self.always_evaluate(True)
    else:
      print('Dont show indicators')
      self.indicate = False
      self.indicate_perspectives = False
      self.graph.Root.value.Children.value.remove(self.camera_indicator)
      
      for i in self.direction_indicators:
        self.graph.Root.value.Children.value.remove(i)
      self.indicators = []
      self.direction_indicators = []


  @field_has_changed(Button0)
  def button0_changed(self):
    # if right mouse pressed
    if self.Button0.value:
      # print('TASTE 0')
      if self.button0_pressed == False:
        if self.state == 0:
          self.turn_on()
          self.state = 1
        elif self.state == 3:
          self.turn_on()
          self.state = 4

        print('keep perspective', self.old_closest_id)
        self.keep_perspective = False

      self.button0_pressed = True
      
    else:
      print('update perspectives')
      self.button0_pressed = False
      self.keep_perspective = True

  @field_has_changed(Button1)
  def button1_changed(self):
    if self.Button1.value:
      if self.state == 1:
        if self.freeze_flag == True:
          self.freeze_flag = False
          self.Material.value.set_uniform("my_color", avango.gua.Vec4(0.2,0.2,0.2,0.85))
          self.projection_lense.Transform.value = avango.gua.make_identity_mat()
        elif self.freeze_flag == False:
          self.freeze_flag = True
          self.Material.value.set_uniform("my_color", avango.gua.Vec4(0.2,0.2,0.2,1.0))
          lense_mat = self.tracked_node.WorldTransform.value
          lense_rot = avango.gua.make_rot_mat(lense_mat.get_rotate_scale_corrected()) 
          self.projection_lense.Transform.value = avango.gua.make_inverse_mat(lense_rot) * self.localized_image_list[self.old_closest_id].rotation
           # self.localized_image_list[closest_id].rotation
          
        self.always_evaluate(True)
      elif self.state == 4:
        if self.indicate:
          self.indicate = False
          # self.sphere.Material.value = self.invis
          self.graph.Root.value.Children.value.remove(self.sphere)
          self.camera_indicator.Material.value = self.invis
          for i in self.direction_indicators:
            i.Material.value = self.invis
        elif self.indicate == False:
          self.indicate = True
          self.graph.Root.value.Children.value.append(self.sphere)
          # self.sphere.Material.value = self.frame_mat
          self.camera_indicator.Material.value = self.cam_indicator_mat
          for i in self.direction_indicators:
            i.Material.value = self.green_material

      self.button1_pressed = True
    else:
      if self.button1_pressed:
        if self.state == 1:
         
          lense_mat = self.tracked_node.WorldTransform.value
          projector_mat = self.localized_image_list[self.old_closest_id].transform
          self.offset = avango.gua.make_inverse_mat(lense_mat) * projector_mat
          self.always_evaluate(True)

          self.Transform.value = self.tracked_node.WorldTransform.value * self.offset
      self.button1_pressed = False

  @field_has_changed(Button2)
  def button2_changed(self):
    if self.Button2.value:
      print('Lens Button 2')
      # print('TASTE 1')
      if self.button2_pressed == False:
        if self.state == 0:
          self.sphere.Transform.value = avango.gua.make_trans_mat(0.0,0.2,0.0) * avango.gua.make_scale_mat(0.1, 0.1, 0.1)
          self.direction_transform.Children.value.append(self.sphere)
          self.state = 2
        elif self.state == 1:
          self.turn_off()
          self.state = 0
        elif self.state == 2:
          self.direction_transform.Children.value.remove(self.sphere)
          # self.sphere.Transform.value = avango.gua.make_trans_mat(self.direction_transform.WorldTransform.value.get_translate()) * avango.gua.make_trans_mat(0.0,0.2,0.0) * avango.gua.make_scale_mat(0.1, 0.1, 0.1)
          self.sphere.Transform.value = self.direction_transform.WorldTransform.value * avango.gua.make_trans_mat(0.0,0.2,0.0) * avango.gua.make_scale_mat(0.1, 0.1, 0.1)
          self.graph.Root.value.Children.value.append(self.sphere)
          self.state = 3
          self.toggle_indicators(True)
        elif self.state == 3:
          self.graph.Root.value.Children.value.remove(self.sphere)
          self.sphere.Transform.value = avango.gua.make_trans_mat(0.0,0.2,0.0) * avango.gua.make_scale_mat(0.1, 0.1, 0.1)
          self.graph.Root.value.Children.value.remove(self.camera_indicator)
          self.state = 0
          self.always_evaluate(True)
          self.toggle_indicators(False)
          self.calculated = False
        elif self.state == 4:
          self.graph.Root.value.Children.value.remove(self.sphere)
          self.sphere.Transform.value = avango.gua.make_trans_mat(0.0,0.2,0.0) * avango.gua.make_scale_mat(0.1, 0.1, 0.1)
          self.graph.Root.value.Children.value.remove(self.camera_indicator)
          self.state = 0
          self.turn_off()
          self.toggle_indicators(False)
          self.calculated = False

      self.button2_pressed = True
      
    else:
      self.button2_pressed = False

def most_frequent(List): 
  return max(set(List), key = List.count)

def map_from_to(x,a,b,c,d):
  y=(x-a)/(b-a)*(d-c)+c
  return y


# dir_vec = lense_mat.get_translate() - self.localized_image_list[closest_id].position
# dir_vec.normalize()
# # mat = avango.make_trans_mat(dir_vec)

# #two random 3D vectors
# p0 = lense_mat.get_translate()
# p1 = self.localized_image_list[closest_id].position
# #calculate cross and dot products
# v = p0.cross(p1)
# print(v)
# D = dot(p0, p1) ;
# NP0 = norm(p0) ; # used for scaling
# if ~all(C==0) # check for colinearity    
#     Z = [0 -C(3) C(2); C(3) 0 -C(1); -C(2) C(1) 0] ; 
#     R = (eye(3) + Z + Z^2 * (1-D)/(norm(C)^2)) / NP0^2 ; # rotation matrix
# else
#     R = sign(D) * (norm(p1) / NP0) ; # orientation and scaling
# end
# #R is the rotation matrix from p0 to p1, so that (except for round-off errors) ...
# R * p0      #... equals p1 
# inv(R) * p1 # ... equals p0

# # mat = avango.gua.make_identity_mat()
# _rot_mat = mat * avango.gua.Vec3(dir_vec.x,dir_vec.y,dir_vec.z)
# _rot_mat = lense_mat.get_rotate_scale_corrected()