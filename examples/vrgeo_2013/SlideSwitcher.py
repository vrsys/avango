import avango
import avango.script
from avango.script import field_has_changed
import avango.gua

import time

class SlideSwitcher(avango.script.Script):

  NextSlide = avango.SFBool()
  PreviousSlide = avango.SFBool()
  CurrentSlide = avango.SFInt()
  FirstSlide = avango.SFInt()
  LastSlide = avango.SFInt()
  SlideOffset = avango.SFFloat()

  SlideLocation = avango.gua.SFVec3()
  SlideYRotation = avango.SFFloat()

  TransitionSmoothness = avango.SFFloat()

  OutTransform = avango.gua.SFMatrix4()

  def __init__(self):
    self.super(SlideSwitcher).__init__()
    self.CurrentSlide.value = -1
    self.FirstSlide.value = 0
    self.LastSlide.value = 0
    self.SlideOffset.value = 1

    self.OutTransform.value = avango.gua.make_identity_mat()
    self.__target_transform = avango.gua.make_identity_mat()

    self.TransitionSmoothness.value = 0

    self.__next_slide_pressed = False
    self.__prev_slide_pressed = False

    self.__last_time = -1

    self.always_evaluate(True)

  @field_has_changed(FirstSlide)
  def set_first_slide(self):
    if self.CurrentSlide.value == -1:
      self.CurrentSlide.value = self.FirstSlide.value

  def increase_current_slide(self):
    self.CurrentSlide.value = min(self.LastSlide.value, self.CurrentSlide.value + 1)

  def decrease_current_slide(self):
    self.CurrentSlide.value = max(self.FirstSlide.value, self.CurrentSlide.value - 1)

  def goto_slide(self, i):
    self.CurrentSlide.value = max(self.FirstSlide.value, min(self.LastSlide.value, i))


  @field_has_changed(NextSlide)
  def next_slide_pressed(self):
    if self.NextSlide.value:
      if not self.__next_slide_pressed:
        self.increase_current_slide()
        self.__next_slide_pressed = True
    else:
      self.__next_slide_pressed = False

  @field_has_changed(PreviousSlide)
  def previous_slide_pressed(self):
    if self.PreviousSlide.value:
      if not self.__prev_slide_pressed:
        self.decrease_current_slide()
        self.__prev_slide_pressed = True
    else:
      self.__prev_slide_pressed = False


  @field_has_changed(CurrentSlide)
  def update_target_transform(self):

    loc = self.SlideLocation.value
    self.__target_transform = avango.gua.make_trans_mat(loc.x - self.CurrentSlide.value * self.SlideOffset.value,
                                                        loc.y,
                                                        loc.z) *\
                              avango.gua.make_rot_mat(self.SlideYRotation.value, 0, 1, 0)

  def evaluate(self):
    if self.__last_time != -1:
      current_time = time.time()
      frame_time = current_time - self.__last_time
      self.__last_time = current_time

      self.OutTransform.value = self.OutTransform.value * self.TransitionSmoothness.value + self.__target_transform * (1.0 - self.TransitionSmoothness.value)

    else:
      loc = self.SlideLocation.value
      self.__target_transform = avango.gua.make_trans_mat(loc.x,
                                                          loc.y,
                                                          loc.z) *\
                              avango.gua.make_rot_mat(self.SlideYRotation.value, 0, 1, 0)
      self.__last_time = time.time()
