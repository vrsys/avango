import avango
import avango.script
from avango.script import field_has_changed

class CharacterControl(avango.script.Script):

  TimeIn = avango.SFFloat()
  
  _last_animation = None
  _current_animation = None

  _animations_kd = []
  _animations_ku = []
  _transformations = []
  _translations = {}

  _delta_transformation = avango.gua.make_identity_mat()

  _current_translation = avango.gua.Vec3(0.0,0.0,0.0)
  _last_translation = avango.gua.Vec3(0.0,0.0,0.0)

  # wall detection
  _scene_graph = None
  _ray = avango.gua.nodes.RayNode() 
  _wall_detected_height = 0.0
  _wall_detected_front = False
  _wall_detected_back = False
  _wall_detect_idle = None


  def __init__(self):

    self.super(CharacterControl).__init__()

  def my_constructor(self, character_node, navigation_node, application_window):
    # character node (bob)
    self._character = character_node
    # navigation node (bob_nav)
    self._navigation = navigation_node
    # window (for key evaluations)
    #self._window = application_window


    def handle_key(ascii, unknown , event , unknown2):
      
      def switch_animation(animation, blending_duration):

        self._character.BlendingDuration.value = blending_duration
        self._last_animation = self._character.Animation.value
        self._character.Animation.value = animation
        self._current_animation = animation

        if self._last_animation in self._translations:
          self._last_translation = self._translations[self._last_animation]
        else:
         self._last_translation =  avango.gua.Vec3(0.0,0.0,0.0)

        if self._current_animation in self._translations:
          self._current_translation = self._translations[self._current_animation]
        else:
         self._current_translation =  avango.gua.Vec3(0.0,0.0,0.0)


      # animation trigger key down
      for a in self._animations_kd:
        if ascii == a[0] and event == 1 and self._character.Animation.value == a[1]:

          switch_animation(a[2],a[3])


      # animation trigger key up
      for a in self._animations_ku:
        if ascii == a[0] and event == 0 and self._character.Animation.value == a[1]:
          
          switch_animation(a[2],a[3])
      

      # transformation trigger
      for t in self._transformations:
        if ascii == t[0] and event == 1:
          self._delta_transformation = self._delta_transformation * t[1]
        if ascii == t[0] and event == 0:
          self._delta_transformation = self._delta_transformation * avango.gua.make_inverse_mat(t[1])


    application_window.on_key_press(handle_key)
		

  def bind_transformation(self, key_nr, transform_matrix):
  	self._transformations.append((key_nr, transform_matrix))

  def key_down(self, key_nr, current_animation, next_animation, blend_duration = 0.5):
    self._animations_kd.append((key_nr, current_animation,next_animation, blend_duration))

  def key_up(self, key_nr, current_animation, next_animation, blend_duration = 0.5):
    self._animations_ku.append((key_nr, current_animation,next_animation, blend_duration))

  def bind_translation(self, animation, translation_vec):
    self._translations[animation] = translation_vec

  def activate_wall_detection(self, height, idle_animation, scene_graph):
    self._wall_detected_height = height
    self._wall_detect_idle = idle_animation
    self._scene_graph = scene_graph


  @field_has_changed(TimeIn)
  def update(self):

    # adapt to changes from outside
    if self._current_animation != self._character.Animation.value:
      self._last_animation = self._current_animation
      self._current_animation = self._character.Animation.value
      if self._last_animation in self._translations:
        self._last_translation = self._translations[self._last_animation]
      else:
       self._last_translation =  avango.gua.Vec3(0.0,0.0,0.0)

      if self._current_animation in self._translations:
        self._current_translation = self._translations[self._current_animation]
      else:
       self._current_translation =  avango.gua.Vec3(0.0,0.0,0.0)

    # add transformation delta from keys
    self._navigation.Transform.value = self._navigation.Transform.value * self._delta_transformation
    
    # blend transformations binded to animations
    blendFact = self._character.BlendingFactor.value

    trans_vec = (self._last_translation * (1-blendFact) ) + (self._current_translation * blendFact)

    if (trans_vec.z >= 0.0 and not self._wall_detected_front) or (trans_vec.z <= 0.0 and not self._wall_detected_back):
      self._navigation.Transform.value = self._navigation.Transform.value * avango.gua.make_trans_mat(trans_vec)
    elif self._character.Animation.value != self._wall_detect_idle:
      self._character.Animation.value = self._wall_detect_idle

    

    #wall detection
    if self._scene_graph:
      #wall detection forwards:
      in_translation = self._character.WorldTransform.value.get_translate()
      in_translation.y += self._wall_detected_height
      char_rot = self._character.WorldTransform.value.get_rotate_scale_corrected()
      self._ray.Transform.value = avango.gua.make_trans_mat(in_translation) *\
                                   avango.gua.make_rot_mat(char_rot) *\
                                   avango.gua.make_rot_mat(-90, 1, 0, 0) *\
                                   avango.gua.make_scale_mat(1.0, 1.0, 0.075)

      results = self._scene_graph.ray_test(
                                       self._ray,
                                       avango.gua.PickingOptions.PICK_ONLY_FIRST_OBJECT |
                                       avango.gua.PickingOptions.GET_POSITIONS)

      self._wall_detected_front = len(results.value) > 0

      #wall detection backwards:
      self._ray.Transform.value = avango.gua.make_trans_mat(in_translation) *\
                                   avango.gua.make_rot_mat(char_rot) *\
                                   avango.gua.make_rot_mat(90, 1, 0, 0) *\
                                   avango.gua.make_scale_mat(1.0, 1.0, 0.075)

      results = self._scene_graph.ray_test(
                                       self._ray,
                                       avango.gua.PickingOptions.PICK_ONLY_FIRST_OBJECT |
                                       avango.gua.PickingOptions.GET_POSITIONS)

      self._wall_detected_back = len(results.value) > 0



    

