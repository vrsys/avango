#!/usr/bin/python

## @file
# Contains classes KeyboardInput.

### import avango-guacamole libraries
import avango
import avango.daemon


### import framework libraries

### import python libraries
import builtins

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
    KeySpace = avango.SFBool()

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
        # self.KeyZ.value = self.device_sensor.Button5.value
        self.KeySpace.value = self.device_sensor.Button5.value
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
        # self.KeySpace.value = self.device_sensor.Button32.value


class KeyboardInput:

    ## constructor
    def __init__(self, DEVICE_STATION=""):


        ### variables ###
        self.keybinding_dict = {}


        ### resources ###
        self.keyboard_sensor0 = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
        self.keyboard_sensor0.Station.value = DEVICE_STATION

        self.keyboard_sensor1 = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
        self.keyboard_sensor1.Station.value = DEVICE_STATION

        ### set initial states ###
        self.init_keybinding_dictionary()

        self.KeyboardDevice



    def init_keybinding_dictionary(self):
        ## first part of keyboard bindings
        self.keybinding_dict["KEY_A"] = (0,0) # sensor-ID & button-ID
        self.keybinding_dict["KEY_B"] = (0,1)
        self.keybinding_dict["KEY_C"] = (0,2)
        self.keybinding_dict["KEY_D"] = (0,3)
        self.keybinding_dict["KEY_E"] = (0,4)
        self.keybinding_dict["KEY_F"] = (0,5)
        self.keybinding_dict["KEY_G"] = (0,6)
        self.keybinding_dict["KEY_H"] = (0,7)
        self.keybinding_dict["KEY_I"] = (0,8)
        self.keybinding_dict["KEY_J"] = (0,9)
        self.keybinding_dict["KEY_K"] = (0,10)
        self.keybinding_dict["KEY_L"] = (0,11)
        self.keybinding_dict["KEY_M"] = (0,12)
        self.keybinding_dict["KEY_N"] = (0,13)
        self.keybinding_dict["KEY_O"] = (0,14)
        self.keybinding_dict["KEY_P"] = (0,15)
        self.keybinding_dict["KEY_Q"] = (0,16)
        self.keybinding_dict["KEY_R"] = (0,17)
        self.keybinding_dict["KEY_S"] = (0,18)
        self.keybinding_dict["KEY_T"] = (0,19)
        self.keybinding_dict["KEY_U"] = (0,20)
        self.keybinding_dict["KEY_V"] = (0,21)
        self.keybinding_dict["KEY_W"] = (0,22)
        self.keybinding_dict["KEY_X"] = (0,23)
        self.keybinding_dict["KEY_Y"] = (0,24)
        self.keybinding_dict["KEY_Z"] = (0,25)
        self.keybinding_dict["KEY_SPACE"] = (0,26)
        self.keybinding_dict["KEY_ENTER"] = (0,27)
        self.keybinding_dict["KEY_LEFT"] = (0,28)
        self.keybinding_dict["KEY_RIGHT"] = (0,29)
        self.keybinding_dict["KEY_UP"] = (0,30)
        self.keybinding_dict["KEY_DOWN"] = (0,31)

        ## second part of keyboard bindings
        self.keybinding_dict["KEY_0"] = (1,0) # sensor-ID & button-ID
        self.keybinding_dict["KEY_1"] = (1,1)
        self.keybinding_dict["KEY_2"] = (1,2)
        self.keybinding_dict["KEY_3"] = (1,3)
        self.keybinding_dict["KEY_4"] = (1,4)
        self.keybinding_dict["KEY_5"] = (1,5)
        self.keybinding_dict["KEY_6"] = (1,6)
        self.keybinding_dict["KEY_7"] = (1,7)
        self.keybinding_dict["KEY_8"] = (1,8)
        self.keybinding_dict["KEY_9"] = (1,9)
        self.keybinding_dict["KEY_F1"] = (1,10)
        self.keybinding_dict["KEY_F2"] = (1,11)
        self.keybinding_dict["KEY_F3"] = (1,12)
        self.keybinding_dict["KEY_F4"] = (1,13)
        self.keybinding_dict["KEY_F5"] = (1,14)
        self.keybinding_dict["KEY_F6"] = (1,15)
        self.keybinding_dict["KEY_F7"] = (1,16)
        self.keybinding_dict["KEY_F8"] = (1,17)
        self.keybinding_dict["KEY_F9"] = (1,18)
        self.keybinding_dict["KEY_F10"] = (1,19)
        self.keybinding_dict["KEY_F11"] = (1,20)
        self.keybinding_dict["KEY_F12"] = (1,21)
        self.keybinding_dict["KEY_INSERT"] = (1,22)
        self.keybinding_dict["KEY_DELETE"] = (1,23)
        self.keybinding_dict["KEY_HOME"] = (1,24)
        self.keybinding_dict["KEY_END"] = (1,25)
        self.keybinding_dict["KEY_PAGEUP"] = (1,26)
        self.keybinding_dict["KEY_PAGEDOWN"] = (1,27)


    def get_field_by_name(self, STRING):
        if self.keybinding_dict.__contains__(STRING) == True:
            _tupel = self.keybinding_dict[STRING]
            _sensor_id = _tupel[0]
            _button_id = _tupel[1]

            _command_string = "self.keyboard_sensor{0}.Button{1}".format(_sensor_id, _button_id)
            return eval(_command_string)
