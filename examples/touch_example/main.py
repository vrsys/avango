#!/usr/bin/python

import avango
import avango.script
from avango.script import field_has_changed
# import avango.gua
from examples_common.GuaVE import GuaVE
from examples_common.device import TouchDevice

class FingerPrinter(avango.script.Script):

  TouchDevice = TouchDevice()

  def __init__(self):
    self.super(FingerPrinter).__init__()

    self.always_evaluate(True)

  def evaluate(self):
    for cursor in self.TouchDevice.TouchCursors:
      print "SpeedX: " + str(cursor.SpeedX.value) + " SpeedY: " + str(cursor.SpeedY.value) +\
            " MotionSpeed: " + str(cursor.MotionSpeed.value) + " MotionAcceleration: " + str(cursor.MotionAcceleration.value)

def start():

  printer = FingerPrinter()

  while True:
    avango.evaluate()


if __name__ == '__main__':
  start()

