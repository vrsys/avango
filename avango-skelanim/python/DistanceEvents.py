import avango
import avango.script
from avango.script import field_has_changed
import math

class DistanceEvents(avango.script.Script):

  DistanceToGround = avango.SFFloat()
  
  def __init__(self):

    self.super(DistanceEvents).__init__()

    self._character_control = None
    self._smaller_than = []
    self._bigger_than = []

  def my_constructor(self, character_control):
    self._character_control = character_control

  def smaller_than(self, distance_to_ground, current_animation_name, next_animation_config, blending_duration = 0.5):
    self._smaller_than.append((distance_to_ground,current_animation_name,next_animation_config,blending_duration))

  def bigger_than(self, distance_to_ground, current_animation_name, next_animation_config, blending_duration = 0.5):
    self._bigger_than.append((distance_to_ground,current_animation_name,next_animation_config,blending_duration))

  @field_has_changed(DistanceToGround)
  def distance_check(self):
    
    for st in self._smaller_than:
      if math.fabs(self.DistanceToGround.value) < st[0] and self._character_control.get_current_animation() == st[1]:
        self._character_control.blend_animation(st[2],st[3])
    for bt in self._bigger_than:
      if math.fabs(self.DistanceToGround.value) > bt[0] and self._character_control.get_current_animation() == bt[1]:
        self._character_control.blend_animation(bt[2],bt[3])