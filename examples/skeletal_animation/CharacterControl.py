import avango
import avango.script
from avango.script import field_has_changed

class CharacterControl(avango.script.Script):

  TimeIn = avango.SFFloat()
  
  _last_animation = None

  _animations = []
  _transformations = []
  #_ease_in = []
  #_ease_out = []

  def __init__(self):

    self.super(CharacterControl).__init__()

  def my_constructor(self, character_node, navigation_node, application_window):
    # character node (bob)
    self._character = character_node
    # navigation node (bob_nav)
    self._navigation = navigation_node
    # window (for key evaluations)
    #self._window = application_window



    def handle_char(c):
      '''if c == 119 and self._character.Animation.value != "data/objects/marine/run.md5anim":
        self._last_animation = self._character.Animation.value
        self._character.Animation.value = "data/objects/marine/run.md5anim"
      if c == 115 and self._character.Animation.value != "data/objects/marine/fists_idle.md5anim":
        self._last_animation = self._character.Animation.value
        self._character.Animation.value = "data/objects/marine/fists_idle.md5anim"
      if c == 100:
        self._navigation.Transform.value = self._navigation.Transform.value * avango.gua.make_rot_mat(-2.0, 0.0, 1.0,0.0) 
      if c == 97:
        self._navigation.Transform.value = self._navigation.Transform.value * avango.gua.make_rot_mat(2.0, 0.0, 1.0,0.0) 
      if c == 99 and self._character.Animation.value != "data/objects/marine/crouch.md5anim":
        self._last_animation = self._character.Animation.value
        self._character.Animation.value = "data/objects/marine/crouch.md5anim"'''
      #if c == 99:
      #  self._navigation.Transform.value = self._navigation.Transform.value * avango.gua.make_trans_mat(0.0,0.5,0.0)

      # animation trigger
      for a in self._animations:
        if c == a[0] and self._character.Animation.value == a[1]:
          self._last_animation = self._character.Animation.value
          self._character.Animation.value = a[2]
      
      # transformation trigger
      for t in self._transformations:
      	if c == t[0]:
      	  self._navigation.Transform.value = self._navigation.Transform.value * t[1]



    application_window.on_char(handle_char)
		


  def bind_animation(self, key_nr, path_to_current_animation, path_to_next_animation):
  	self._animations.append((key_nr, path_to_current_animation,path_to_next_animation))

  def bind_transformation(self, key_nr, transform_matrix):
  	self._transformations.append((key_nr, transform_matrix))

  '''def ease_in(self, path_to_current_animation, path_to_next_animation, transform_matrix):
    self._ease_in.append((path_to_current_animation, path_to_next_animation, transform_matrix))

  def ease_out(self, path_to_current_animation, path_to_next_animation, transform_matrix):
    self._ease_out.append((path_to_current_animation, path_to_next_animation, transform_matrix))'''

  @field_has_changed(TimeIn)
  def update(self):


    blendFact = self._character.BlendingFactor.value

    if self._character.Animation.value == "data/objects/marine/run.md5anim":
      self._navigation.Transform.value = self._navigation.Transform.value * avango.gua.make_trans_mat(0.0, 0.0,blendFact * 0.05)
    if self._last_animation == "data/objects/marine/run.md5anim":
      self._navigation.Transform.value = self._navigation.Transform.value * avango.gua.make_trans_mat(0.0, 0.0,(1-blendFact) * 0.05)

    # WIP: scaling unique transformation matrix by blending factor not correct
    ''''for ei in self._ease_in:
    	if self._character.Animation.value == ei[1] and self._last_animation == ei[0]:
    	  self._navigation.Transform.value = self._navigation.Transform.value * ei[2] * blendFact
    	  print(self._navigation.Transform.value)'''	
