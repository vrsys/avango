import avango
import avango.gua
from examples_common.device import TouchDevice
from examples_common.device import TouchCursor

class TapDetector(avango.script.Script):
  Cursor           = TouchCursor()
  TapDetected      = avango.SFBool()
  TapPosition      = avango.gua.SFVec2()
  TapTimeThreshold = avango.SFFloat() # in seconds

  def __init__(self):
    self.super(TapDetector).__init__()
    self.always_evaluate(True)

    self.TapTimeThreshold.value = 0.5

    self.__timer = avango.nodes.TimeSensor()
    self.reset()

  def reset(self):
    self.TapDetected.value = False
    self.TapPosition.value = avango.gua.Vec2(-1.0, -1.0)

    self.__touch_start_time = 0
    self.__touch_start_position = avango.gua.Vec2(-1.0, -1.0)
    self.__last_cursor_state = 4

  def evaluate(self):
    if self.__last_cursor_state == 4 and self.Cursor.State.value != 4:
      self.__touch_start_time = self.__timer.Time.value
      self.__touch_start_position = avango.gua.Vec2(self.Cursor.PosX.value,
                                                    self.Cursor.PosY.value)

    elif self.__last_cursor_state != 4 and self.Cursor.State.value == 4:
      if self.__timer.Time.value - self.__touch_start_time <= self.TapTimeThreshold.value:

        self.TapDetected.value = True
        self.TapPosition.value = self.__touch_start_position

    self.__last_cursor_state = self.Cursor.State.value

class DoubleTapDetector(avango.script.Script):
  DoubleTapPositions         = avango.gua.MFVec2()
  DoubleTapTimeThreshold     = avango.SFFloat() # in seconds
  DoubleTapDistanceThreshold = avango.SFFloat() # in NDC

  def __init__(self):
    self.super(DoubleTapDetector).__init__()
    self.always_evaluate(True)

    self.DoubleTapTimeThreshold.value = 0.5
    self.DoubleTapDistanceThreshold.value = 0.01

    self.__timer = avango.nodes.TimeSensor()
    self.__cursors = []
    self.__tap_detectors = []
    self.reset()

  def reset(self):
    self.DoubleTapPositions.value = []

    self.__first_taps = []

  def add_cursor(self, touch_cursor):
    self.__cursors.append(touch_cursor)
    tap_detector = TapDetector()
    tap_detector.Cursor = touch_cursor
    self.__tap_detectors.append(tap_detector)

  def evaluate(self):

    for tap in self.__first_taps:
      for detector in self.__tap_detectors:
        if detector.TapDetected.value == True:
          if self.__timer.Time.value - tap[0] <= self.DoubleTapTimeThreshold.value and \
             (detector.TapPosition.value - tap[1]).length() <= self.DoubleTapDistanceThreshold.value:

             self.DoubleTapPositions.value.append((detector.TapPosition.value + tap[1])/2.0)
             detector.reset()

    for detector in self.__tap_detectors:
      if detector.TapDetected.value == True:
        self.__first_taps.append((self.__timer.Time.value,
                                  detector.TapPosition.value))
        detector.reset()






