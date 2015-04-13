import avango
import avango.script
from avango.gua.skelanim.AnimationControl import *
import avango.daemon
import math

class CharacterControl(avango.script.Script):

  XBOX_X = avango.SFFloat()
  XBOX_Y = avango.SFFloat()
  XBOX_BTN_A = avango.SFFloat()
  XBOX_BTN_X = avango.SFFloat()


  def __init__(self):

    self.super(CharacterControl).__init__()

    # animations
    self._animation_control = AnimationControl()
    self._queued_animations = []
    self._on_animation_end = {}

    # keys
    self._pressed_keys = []
    self._animations_kd = []
    self._animations_ku = []
    
    # transformations
    self._transformations = []
    self._delta_transformations = []
    self._translations = {}
    self._current_translation = avango.gua.Vec3(0.0,0.0,0.0)
    self._last_translation = avango.gua.Vec3(0.0,0.0,0.0)

    # wall detection
    self._scene_graph = None
    self._ray = avango.gua.nodes.Ray()
    self._wall_detect_height = 0.0
    self._wall_detect_offset = 0.0
    self._wall_detected = False
    self._wall_detect_idle = None

    self._xbox_events = []
    self._xbox_animation_speeds = {}


  def my_constructor(self, character_node, navigation_node, application_window):
    # character node (bob)
    self._character = character_node
    # navigation node (bob_nav)
    self._navigation = navigation_node
    # window (for key evaluations)
    #self._window = application_window

    self._animation_control.my_constructor(character_node)

    self.always_evaluate(True)

    application_window.on_key_press(self._handle_key)
    
  def bind_transformation(self, key_nr, current_animation_name, transform_matrix):
    delta_list_id = len(self._delta_transformations)
    self._delta_transformations.append(avango.gua.make_identity_mat())
    self._transformations.append((key_nr, current_animation_name, delta_list_id, transform_matrix))

  def on_key_down(self, key_nr, current_animation_name, next_animation_config, blend_duration = 0.5):
    self._animations_kd.append((key_nr, current_animation_name,next_animation_config, blend_duration))

  def on_key_up(self, key_nr, current_animation_name, next_animation_config, blend_duration = 0.5):
    self._animations_ku.append((key_nr, current_animation_name,next_animation_config, blend_duration))

  def on_animation_end(self, animation_name, next_animation_config, blend_duration = 0.5):
    self._on_animation_end[animation_name] = (next_animation_config,blend_duration)

  def bind_translation(self, animation_name, translation_vec):
    self._translations[animation_name] = translation_vec

  def activate_wall_detection(self, dir_offset, height_offset, idle_animation_name, scene_graph):
    self._wall_detect_offset = dir_offset
    self._wall_detect_height = height_offset
    self._wall_detect_idle = AnimationConfig(idle_animation_name)
    self._scene_graph = scene_graph

  def queue_animation(self, animation_config, blend_duration = 0.5):
    self._queued_animations.append((animation_config,blend_duration))

  def get_current_animation(self):
    return self._animation_control.get_current_animation()

  def xbox_override_key(self, field, key, threshold = 0.0, multiplier = 1.0):
    self._xbox_events.append((field,key,threshold,multiplier))

  def xbox_animation_speed(self, field, animation_name):
    self._xbox_animation_speeds[animation_name] = field

  def evaluate(self):

    self._check_xbox_overrides()

    self._check_animation_loop()

    # add transformation delta from keys
    for mat in self._delta_transformations:
      self._navigation.Transform.value = self._navigation.Transform.value * mat
    
    self._blend_translations()
    
  def switch_animation(self, animation_config):

    # look for already pressed keys with propreitary current animation
    # abort blending if already pressed key changed state
    if self._check_pressed_keys(animation_config.name):
        return

    next_animation_blending = self._check_on_animation_end(animation_config.name)
    
    self._animation_control.switch_to(animation_config)
    
    self._apply_animation_changes(next_animation_blending)

  def blend_animation(self, animation_config, blending_duration = 0.5):

    # look for already pressed keys with propreitary current animation
    # abort blending if already pressed key changed state
    if self._check_pressed_keys(animation_config.name):
        return

    if blending_duration < 0.001:
      self.switch_animation(animation_config)
      return

    next_animation_blending = self._check_on_animation_end(animation_config.name)
    
    self._animation_control.blend_to(animation_config, blending_duration)
    
    self._apply_animation_changes(next_animation_blending)


  def _handle_key(self, ascii, unknown , event , unknown2, animation_name = None):

    # additional animation parameter for proprietary animation states
    if animation_name is None:
      animation_name = self._animation_control.get_current_animation()

    # animation trigger key down
    if event == 1:
      if ascii not in self._pressed_keys:
        self._pressed_keys.append(ascii)
      for a in self._animations_kd:
        if ascii == a[0] and animation_name == a[1]:

          self.blend_animation(a[2],a[3])

    # animation trigger key up
    if event == 0:
      if ascii in self._pressed_keys:
        self._pressed_keys.remove(ascii)
      for a in self._animations_ku:
        if ascii == a[0] and animation_name == a[1]:

          self.blend_animation(a[2],a[3])

    # transformation trigger
    for t in self._transformations:
      if ascii == t[0] and event == 1 and animation_name == t[1]:
        self._delta_transformations[t[2]] = t[3]
      if ascii == t[0] and event == 0 and animation_name== t[1]:
        self._delta_transformations[t[2]] = avango.gua.make_identity_mat()

  def _blend_translations(self):

    # blend transformations binded to animations
    blendFact = self._animation_control.get_blending_factor()

    trans_vec = (self._last_translation * (1-blendFact) ) + (self._current_translation * blendFact)

    #xbox_controller:
    if math.fabs(self.XBOX_Y.value)>0.0:
      trans_vec.z *= math.fabs(self.XBOX_Y.value)
    if math.fabs(self.XBOX_X.value)>0.0:
      trans_vec.x *= math.fabs(self.XBOX_X.value)
    
    # translation delta in world coordinate system:
    before_trans = self._navigation.WorldTransform.value.get_translate()
    new_navigation_transform = self._navigation.WorldTransform.value * avango.gua.make_trans_mat(trans_vec)
    after_trans = new_navigation_transform.get_translate()
    delta_trans = after_trans - before_trans

    if not self._wall_detection(delta_trans):
      self._navigation.Transform.value = self._navigation.Transform.value * avango.gua.make_trans_mat(trans_vec)

    elif self._animation_control.get_current_animation() != self._wall_detect_idle:
      self._pressed_keys = []
      self.blend_animation(self._wall_detect_idle)

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
      self.blend_animation(queued[0],queued[1])

  def _check_on_animation_end(self, animation_name):

    next_animation_blending = None
    if animation_name in self._on_animation_end:
      next_animation_blending = self._on_animation_end[animation_name]
    return next_animation_blending

  def _check_pressed_keys(self, next_animation_name):

    current_animation = self._animation_control.get_current_animation()
    for ascii in self._pressed_keys:
      self._handle_key(ascii,None,1,None,next_animation_name)
      if current_animation != self._animation_control.get_current_animation():
        return True
    return False

  def _check_xbox_overrides(self):

    for tup in self._xbox_events:
      if (tup[0].value*tup[3]) > (tup[2]*tup[3]):
        if not tup[1] in self._pressed_keys:
          self._handle_key(tup[1],None,1,None)
      elif tup[1] in self._pressed_keys:
        self._handle_key(tup[1],None,0,None)

    cur_anim = self._animation_control.get_current_animation()
    if cur_anim in self._xbox_animation_speeds:
      self._animation_control._current_animation.speed = math.fabs(self._xbox_animation_speeds[cur_anim].value)

  def _apply_animation_changes(self, next_animation_blending):

    if self._animation_control.get_last_animation() in self._translations:
      self._last_translation = self._translations[self._animation_control.get_last_animation()]
    else:
     self._last_translation =  avango.gua.Vec3(0.0,0.0,0.0)

    if self._animation_control.get_current_animation() in self._translations:
      self._current_translation = self._translations[self._animation_control.get_current_animation()]
    else:
     self._current_translation =  avango.gua.Vec3(0.0,0.0,0.0)


    # queue next animation
    if next_animation_blending != None:
      self.queue_animation(next_animation_blending[0],next_animation_blending[1])

    for index in range(0,len(self._delta_transformations)):
      self._delta_transformations[index] = avango.gua.make_identity_mat()