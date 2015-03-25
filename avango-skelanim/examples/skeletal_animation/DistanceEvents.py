import avango
import avango.script
from avango.script import field_has_changed
import math

class DistanceEvents(avango.script.Script):

  DistanceToGround = avango.SFFloat()
  
  _character_control = None
  _smaller_than = []
  _bigger_than = []

  def my_constructor(self, character_control):
    self._character_control = character_control

  def smaller_than(self, distance_to_ground, current_animation, next_animation, blending_duration = 0.5, loop_mode = True):
    self._smaller_than.append((distance_to_ground,current_animation,next_animation,blending_duration,loop_mode))

  def bigger_than(self, distance_to_ground, current_animation, next_animation, blending_duration = 0.5, loop_mode = True):
    self._bigger_than.append((distance_to_ground,current_animation,next_animation,blending_duration,loop_mode))

  @field_has_changed(DistanceToGround)
  def distance_check(self):
    
    for st in self._smaller_than:
      if math.fabs(self.DistanceToGround.value) < st[0] and self._character_control.get_current_animation() == st[1]:
        self._character_control.switch_animation(st[2],st[3],st[4])
    for st in self._bigger_than:
      if math.fabs(self.DistanceToGround.value) > st[0] and self._character_control.get_current_animation() == st[1]:
        self._character_control.switch_animation(st[2],st[3],st[4])