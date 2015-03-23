import avango
import avango.script
from AnimationControl import *

class CharacterControl(avango.script.Script):

  _animation_control = AnimationControl()

  _queued_animations = []
  _play_once = {}

  _animations_kd = []
  _animations_ku = []
  _transformations = []
  _translations = {}

  _delta_transformation = avango.gua.make_identity_mat()

  _current_translation = avango.gua.Vec3(0.0,0.0,0.0)
  _last_translation = avango.gua.Vec3(0.0,0.0,0.0)


  # wall detection
  _scene_graph = None
  _ray = avango.gua.nodes.Ray()
  _wall_detect_height = 0.0
  _wall_detect_offset = 0.0
  _wall_detected = False
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

    self._animation_control.my_constructor(character_node)

    self.always_evaluate(True)



    def handle_key(ascii, unknown , event , unknown2):

      # animation trigger key down
      for a in self._animations_kd:
        if ascii == a[0] and event == 1 and self._animation_control.get_current_animation() == a[1]:

          self._switch_animation(a[2],a[3])

      # animation trigger key up
      for a in self._animations_ku:
        if ascii == a[0] and event == 0 and self._animation_control.get_current_animation() == a[1]:
          
          self._switch_animation(a[2],a[3])
      

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

  def activate_wall_detection(self, dir_offset, height_offset, idle_animation, scene_graph):
    self._wall_detect_offset = dir_offset
    self._wall_detect_height = height_offset
    self._wall_detect_idle = idle_animation
    self._scene_graph = scene_graph


  def queue_animation(self, animation, blend_duration = 0.5):
    self._queued_animations.append((animation,blend_duration))

  def play_once(self, animation, next_animation, blend_duration = 0.5):
    self._play_once[animation] = (next_animation,blend_duration)

  def get_current_animation(self):
    return self._animation_control.get_current_animation()

  def evaluate(self):

    self._check_animation_loop()
    
    #self._check_animation_changes()

    # add transformation delta from keys
    self._navigation.Transform.value = self._navigation.Transform.value * self._delta_transformation
    
    self._blend_translations()
    

  def _switch_animation(self, animation, blending_duration = 0.5, loop_mode = True):

    loop_mode_tmp = loop_mode
    next_animation = None
    # check play once:
    if animation in self._play_once:
      loop_mode_tmp = False
      next_animation = self._play_once[animation]
    
    self._animation_control.blend_to(animation, blending_duration, loop_mode_tmp)
    
    if self._animation_control.get_last_animation() in self._translations:
      self._last_translation = self._translations[self._animation_control.get_last_animation()]
    else:
     self._last_translation =  avango.gua.Vec3(0.0,0.0,0.0)

    if self._animation_control.get_current_animation() in self._translations:
      self._current_translation = self._translations[self._animation_control.get_current_animation()]
    else:
     self._current_translation =  avango.gua.Vec3(0.0,0.0,0.0)


    # queue next animation if play once
    if next_animation != None:
      self.queue_animation(next_animation[0],next_animation[1])
    
    # clear queue when looped animation is triggered
    if loop_mode_tmp:
      self._queued_animations = []


  '''def _check_animation_changes(self):

    # adapt to changes from outside
    if self._current_animation != self._character.Animation.value:
      self._animation_control._last_animation = self._current_animation
      self._current_animation = self._character.Animation.value
      if self._animation_control._last_animation in self._translations:
        self._last_translation = self._translations[self._animation_control._last_animation]
      else:
       self._last_translation =  avango.gua.Vec3(0.0,0.0,0.0)

      if self._current_animation in self._translations:
        self._current_translation = self._translations[self._current_animation]
      else:
       self._current_translation =  avango.gua.Vec3(0.0,0.0,0.0)'''


  def _blend_translations(self):

    # blend transformations binded to animations
    blendFact = self._animation_control.get_blending_factor()

    trans_vec = (self._last_translation * (1-blendFact) ) + (self._current_translation * blendFact)
    
    # translation delta in world coordinate system:
    before_trans = self._navigation.WorldTransform.value.get_translate()
    new_navigation_transform = self._navigation.WorldTransform.value * avango.gua.make_trans_mat(trans_vec)
    after_trans = new_navigation_transform.get_translate()
    delta_trans = after_trans - before_trans

    if not self._wall_detection(delta_trans):

      self._navigation.Transform.value = self._navigation.Transform.value * avango.gua.make_trans_mat(trans_vec)

    elif self._animation_control.get_current_animation() != self._wall_detect_idle:

      ##self._animation_control.blend_to(self._wall_detect_idle)
      self._switch_animation(self._wall_detect_idle)

    

  def _wall_detection(self, delta_translation):

    #wall detection
    if self._scene_graph and delta_translation.length() > 0.00001:

      delta_norm = avango.gua.Vec3(delta_translation)
      delta_norm.normalize()

      in_translation = self._character.WorldTransform.value.get_translate() + (delta_norm * self._wall_detect_offset)
      in_translation.y += self._wall_detect_height
      self._ray.Origin.value = in_translation
      self._ray.Direction.value = delta_translation

      results = self._scene_graph.ray_test(self._ray,0)

      return len(results.value)>0
   
    else:

      return False



  def _check_animation_loop(self):

    if not self._animation_control.is_looping() and len(self._queued_animations)>0:

      queued = self._queued_animations.pop(0)

      self._switch_animation(queued[0],queued[1],False)

