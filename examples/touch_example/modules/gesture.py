# import guacamole libraries
import avango
import avango.gua

from finger import Finger

class Gesture(avango.script.Script):

  Fingers = []

  Properties = dict()

  def __init__(self):
    self.super(Gesture).__init__()
