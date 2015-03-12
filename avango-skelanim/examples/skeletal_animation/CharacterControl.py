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

  def key_down(self, key_nr, path_to_current_animation, path_to_next_animation, blend_duration = 0.5):
    self._animations_kd.append((key_nr, path_to_current_animation,path_to_next_animation, blend_duration))

  def key_up(self, key_nr, path_to_current_animation, path_to_next_animation, blend_duration = 0.5):
    self._animations_ku.append((key_nr, path_to_current_animation,path_to_next_animation, blend_duration))

  def bind_translation(self, path_to_animation, translation_vec):
    self._translations[path_to_animation] = translation_vec 


  @field_has_changed(TimeIn)
  def update(self):

    # adapt to changes from outside
    if self._current_animation != self._character.Animation.value:
      self._last_animation = self._current_animation
      self._current_animation = self._character.Animation.value

    # add transformation delta from keys
    self._navigation.Transform.value = self._navigation.Transform.value * self._delta_transformation
    
    # blend transformations binded to animations
    blendFact = self._character.BlendingFactor.value

    trans_vec = (self._last_translation * (1-blendFact) ) + (self._current_translation * blendFact)

    self._navigation.Transform.value = self._navigation.Transform.value * avango.gua.make_trans_mat(trans_vec)

