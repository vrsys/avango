
import avango
import avango.daemon
import math

from avango.script import field_has_changed


class TouchCursor(avango.script.Script):

    CursorID = avango.SFInt()

    PosX = avango.SFFloat()
    PosY = avango.SFFloat()

    SpeedX = avango.SFFloat()
    SpeedY = avango.SFFloat()

    MotionSpeed = avango.SFFloat()
    MotionAcceleration = avango.SFFloat()

    IsMoving = avango.SFBool()
    State = avango.SFInt()

    SessionID = avango.SFInt()

    def __init__(self):
        self.super(TouchCursor).__init__()

        self.__device_sensor = avango.daemon.nodes.DeviceSensor(
            DeviceService=avango.daemon.DeviceService())

        self.PosX.value = -1
        self.PosY.value = -1

        self.always_evaluate(True)

    @field_has_changed(CursorID)
    def set_station(self):
        self.__device_sensor.Station.value = "gua-finger" + str(
            self.CursorID.value)

    def evaluate(self):

        self.PosX.value = self.__device_sensor.Value0.value
        self.PosY.value = self.__device_sensor.Value1.value

        self.SpeedX.value = self.__device_sensor.Value2.value
        self.SpeedY.value = self.__device_sensor.Value3.value

        self.MotionSpeed.value = self.__device_sensor.Value4.value
        self.MotionAcceleration.value = self.__device_sensor.Value5.value

        self.IsMoving.value = bool(self.__device_sensor.Value6.value)
        self.State.value = int(self.__device_sensor.Value7.value)

        self.SessionID.value = int(self.__device_sensor.Value8.value)


class TouchDevice(avango.script.Script):

    TouchCursors = []

    def __init__(self):
        self.super(TouchDevice).__init__()

        for i in range(0, 20):
            self.TouchCursors.append(TouchCursor(CursorID=i))


class MouseDevice(avango.script.Script):

    RelX = avango.SFFloat()
    RelY = avango.SFFloat()

    ButtonLeft = avango.SFBool()
    ButtonRight = avango.SFBool()

    def __init__(self):
        self.super(MouseDevice).__init__()

        self.device_sensor = avango.daemon.nodes.DeviceSensor(
            DeviceService=avango.daemon.DeviceService())
        self.device_sensor.Station.value = "gua-device-mouse"

        self.always_evaluate(True)

    def evaluate(self):

        self.RelX.value = 0.0
        self.RelY.value = 0.0

        rel_x = self.device_sensor.Value0.value
        if rel_x >= 2 or rel_x <= -2:
            self.RelX.value = rel_x

        rel_y = self.device_sensor.Value1.value
        if rel_y >= 2 or rel_y <= -2:
            self.RelY.value = rel_y

        self.ButtonLeft.value = self.device_sensor.Button0.value
        self.ButtonRight.value = self.device_sensor.Button1.value


class KeyboardDevice(avango.script.Script):

    KeyQ = avango.SFBool()
    KeyW = avango.SFBool()
    KeyE = avango.SFBool()
    KeyR = avango.SFBool()
    KeyT = avango.SFBool()
    KeyZ = avango.SFBool()
    KeyU = avango.SFBool()
    KeyI = avango.SFBool()
    KeyO = avango.SFBool()
    KeyP = avango.SFBool()
    KeyA = avango.SFBool()
    KeyS = avango.SFBool()
    KeyD = avango.SFBool()
    KeyF = avango.SFBool()
    KeyG = avango.SFBool()
    KeyH = avango.SFBool()
    KeyJ = avango.SFBool()
    KeyK = avango.SFBool()
    KeyL = avango.SFBool()
    KeyY = avango.SFBool()
    KeyX = avango.SFBool()
    KeyC = avango.SFBool()
    KeyV = avango.SFBool()
    KeyB = avango.SFBool()
    KeyN = avango.SFBool()
    KeyM = avango.SFBool()

    KeyUp = avango.SFBool()
    KeyDown = avango.SFBool()

    Key1 = avango.SFBool()
    Key2 = avango.SFBool()
    KeyLeft = avango.SFBool()
    KeyRight = avango.SFBool()

    def __init__(self):
        self.super(KeyboardDevice).__init__()

        self.device_sensor = avango.daemon.nodes.DeviceSensor(
            DeviceService=avango.daemon.DeviceService())
        # self.device_sensor.Station.value = "device-keyboard"
        self.device_sensor.Station.value = "gua-device-keyboard" + str(0)
        # self.device_sensor.Station.value = "gua-device-keyboard0"

        self.always_evaluate(True)

    
    def set_device_number(self, number):
        self.device_sensor = avango.daemon.nodes.DeviceSensor(
            DeviceService=avango.daemon.DeviceService())
        self.device_sensor.Station.value = "gua-device-keyboard" + str(number)
    

    def evaluate(self):
        
        self.KeyQ.value = self.device_sensor.Button0.value
        self.KeyW.value = self.device_sensor.Button1.value
        self.KeyE.value = self.device_sensor.Button2.value
        self.KeyR.value = self.device_sensor.Button3.value
        self.KeyT.value = self.device_sensor.Button4.value
        self.KeyZ.value = self.device_sensor.Button5.value
        self.KeyU.value = self.device_sensor.Button6.value
        self.KeyI.value = self.device_sensor.Button7.value
        self.KeyO.value = self.device_sensor.Button8.value
        self.KeyP.value = self.device_sensor.Button9.value
        self.KeyA.value = self.device_sensor.Button10.value
        self.KeyS.value = self.device_sensor.Button11.value
        self.KeyD.value = self.device_sensor.Button12.value
        self.KeyF.value = self.device_sensor.Button13.value
        self.KeyG.value = self.device_sensor.Button14.value
        self.KeyH.value = self.device_sensor.Button15.value
        self.KeyJ.value = self.device_sensor.Button16.value
        self.KeyK.value = self.device_sensor.Button17.value
        self.KeyL.value = self.device_sensor.Button18.value
        self.KeyY.value = self.device_sensor.Button19.value
        self.KeyX.value = self.device_sensor.Button20.value
        self.KeyC.value = self.device_sensor.Button21.value
        self.KeyV.value = self.device_sensor.Button22.value
        self.KeyB.value = self.device_sensor.Button23.value
        self.KeyN.value = self.device_sensor.Button24.value
        self.KeyM.value = self.device_sensor.Button25.value

        self.KeyUp.value = self.device_sensor.Button26.value
        self.KeyDown.value = self.device_sensor.Button27.value

        self.Key1.value = self.device_sensor.Button28.value
        self.Key2.value = self.device_sensor.Button29.value
        self.KeyLeft.value = self.device_sensor.Button30.value
        self.KeyRight.value = self.device_sensor.Button31.value

# class PointerDevice(avango.script.Script):

#   KeyUp = avango.SFBool()
#   KeyDown = avango.SFBool()

#   def __init__(self):
#     self.super(PointerDevice).__init__()

#     self.device_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
#     self.device_sensor.Station.value = "gua-device-pointer"

#     self.always_evaluate(True)

#   def evaluate(self):

#     self.KeyUp.value = self.device_sensor.Button0.value
#     self.KeyDown.value = self.device_sensor.Button1.value


class Spheron(avango.script.Script):

    RotX = avango.SFFloat()
    RotY = avango.SFFloat()
    RotZ = avango.SFFloat()

    PosX = avango.SFFloat()
    PosY = avango.SFFloat()
    PosZ = avango.SFFloat()

    def __init__(self):
        self.super(Spheron).__init__()

        self.device_sensor = avango.daemon.nodes.DeviceSensor(
            DeviceService=avango.daemon.DeviceService())
        self.device_sensor.Station.value = "gua-device-spheron"

        self.always_evaluate(True)

    def evaluate(self):

        threshold = 0.004
        x0 = -0.0079
        y0 = -0.0081
        z0 = -0.0079

        self.PosX.value = 0.0
        self.PosY.value = 0.0
        self.PosZ.value = 0.0

        if self.device_sensor.Value0.value - x0 > threshold:
            self.PosX.value = 1.0
        if self.device_sensor.Value0.value - x0 < -threshold:
            self.PosX.value = -1.0
        if self.device_sensor.Value1.value - y0 > threshold:
            self.PosY.value = 1.0
        if self.device_sensor.Value1.value - y0 < -threshold:
            self.PosY.value = -1.0
        if self.device_sensor.Value2.value - z0 > threshold:
            self.PosZ.value = 1.0
        if self.device_sensor.Value2.value - z0 < -threshold:
            self.PosZ.value = -1.0

        threshold = 0.00001
        x0 = -0.00787
        y0 = -0.00787
        z0 = -0.00787

        self.RotX.value = 0.0
        self.RotY.value = 0.0
        self.RotZ.value = 0.0

        if abs(self.device_sensor.Value3.value - x0) > threshold:
            self.RotX.value = self.device_sensor.Value3.value - x0
        if abs(self.device_sensor.Value4.value - y0) > threshold:
            self.RotY.value = self.device_sensor.Value4.value - y0
        if abs(self.device_sensor.Value5.value - z0) > threshold:
            self.RotZ.value = self.device_sensor.Value5.value - z0
