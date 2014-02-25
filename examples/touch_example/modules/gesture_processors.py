# import guacamole libraries
import avango
import avango.gua

from gesture import Gesture

class GestureProcessor(avango.script.Script):

  def __init__(self):
    self.super(GestureProcessor).__init__()
    self.__successor = None;

  def process (self, gesture):
    if self.__successor:
      self.__successor.process(gesture)

  def followed_by(self, successor):
    self.__successor = successor
    return self.__successor


class TapProcessor(GestureProcessor):

  def __init__(self):
    self.super(TapProcessor).__init__()

  def process (self, gesture):
    self.super(GestureProcessor).process(gesture)
