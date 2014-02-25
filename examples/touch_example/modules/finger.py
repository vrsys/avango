# import guacamole libraries
import avango
import avango.gua

from examples_common.device import TouchDevice
from examples_common.device import TouchCursor

class Finger(avango.script.Script):

  Cursor = TouchCursor()

  Properties = dict()

  def __init__(self):
    self.super(Finger).__init__()

    timer = avango.nodes.TimeSensor()
    self.__creation_time = timer.Time.value

  def get_creation_time(self):
    return self.__creation_time
