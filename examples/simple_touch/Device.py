#!/usr/bin/python

import avango
import avango.daemon
import avango.script
from avango.script import field_has_changed

class TouchCursor(avango.script.Script):
  # input fields
  CursorID = avango.SFInt();
  PosX = avango.SFFloat()
  PosY = avango.SFFloat()
  SpeedX = avango.SFFloat()
  SpeedY = avango.SFFloat()
  MotionSpeed = avango.SFFloat()
  MotionAcceleration = avango.SFFloat()
  IsMoving = avango.SFBool()
  State = avango.SFFloat()
  SessionID = avango.SFFloat()

  def __init__(self):
    # call parent constructor
    self.super(TouchCursor).__init__()

    # create device sensor providing input data
    self.device_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())

    # initialize field values
    self.PosX.value = -1
    self.PosY.value = -1
    self.SpeedX.value = -1
    self.SpeedY.value = -1

    # setup field connections to retrieve values
    self.PosX.connect_from(self.device_sensor.Value0)
    self.PosY.connect_from(self.device_sensor.Value1)
    self.SpeedX.connect_from(self.device_sensor.Value2)
    self.SpeedY.connect_from(self.device_sensor.Value3)
    self.MotionSpeed.connect_from(self.device_sensor.Value4)
    self.MotionAcceleration.connect_from(self.device_sensor.Value5)
    self.IsMoving.connect_from(self.device_sensor.Value6)
    self.State.connect_from(self.device_sensor.Value7)
    self.SessionID.connect_from(self.device_sensor.Value8)

  # set the device sensor's station when the id is set
  @field_has_changed(CursorID)
  def set_station(self):
    self.device_sensor.Station.value = "gua-finger" + str(self.CursorID.value)

  # called whenever any of the input fields changed
  def evaluate(self):
    # print id and position values
    print "{0}: ({1}, {2})".format(self.CursorID.value, self.PosX.value, self.PosY.value)


# creates a list of TouchCursors
class TouchDevice(avango.script.Script):
  TouchCursors = []

  def __init__(self):
    self.super(TouchDevice).__init__()
    for i in range(0, 20):
      self.TouchCursors.append(TouchCursor(CursorID = i))
