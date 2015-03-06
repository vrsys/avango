import avango
import avango.script
from avango.script import field_has_changed

class CharacterControl(avango.script.Script):

  TimeIn = avango.SFFloat()
  
  _last_animation = None

  _animations_kd = []
  _animations_ku = []
  _transformations = []
  _ease_in_translations = []
  _ease_out_translations = []


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

      # animation trigger
      for a in self._animations_kd:
        if ascii == a[0] and event == 1 and self._character.Animation.value == a[1]:
          self._last_animation = self._character.Animation.value
          self._character.Animation.value = a[2]

      for a in self._animations_ku:
        if ascii == a[0] and event == 0 and self._character.Animation.value == a[1]:
          self._last_animation = self._character.Animation.value
          self._character.Animation.value = a[2]
      
      # transformation trigger
      for t in self._transformations:
        if ascii == t[0]:
          self._navigation.Transform.value = self._navigation.Transform.value * t[1]

    application_window.on_key_press(handle_key)
		

  def bind_transformation(self, key_nr, transform_matrix):
  	self._transformations.append((key_nr, transform_matrix))

  def key_down(self, key_nr, path_to_current_animation, path_to_next_animation):
    self._animations_kd.append((key_nr, path_to_current_animation,path_to_next_animation))

  def key_up(self, key_nr, path_to_current_animation, path_to_next_animation):
    self._animations_ku.append((key_nr, path_to_current_animation,path_to_next_animation))

  def ease_in_translation(self, path_to_current_animation, path_to_next_animation, translation_vec):
    self._ease_in_translations.append((path_to_current_animation, path_to_next_animation, translation_vec))

  def ease_out_translation(self, path_to_current_animation, path_to_next_animation, translation_vec):
    self._ease_out_translations.append((path_to_current_animation, path_to_next_animation, translation_vec))


  @field_has_changed(TimeIn)
  def update(self):

    blendFact = self._character.BlendingFactor.value

    for trans in self._ease_in_translations:
      if self._character.Animation.value == trans[1] and self._last_animation == trans[0]:
        self._navigation.Transform.value = self._navigation.Transform.value * avango.gua.make_trans_mat(trans[2] * blendFact)

    for trans in self._ease_out_translations:
      if self._character.Animation.value == trans[1] and self._last_animation == trans[0]:
        self._navigation.Transform.value = self._navigation.Transform.value * avango.gua.make_trans_mat(trans[2] * (1 - blendFact))

