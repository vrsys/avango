#!/usr/bin/python

## @file
# Contains classes SpheronInput and DualSpheronInput and DualSpheronInputScript.

### import avango-guacamole libraries
import avango
import avango.gua
#from avango.script import field_has_changed


### import framework libraries
from src.Input import MultiDofInput

### import python libraries
# ...


class SpheronInput(MultiDofInput):

    ### constructor
    def __init__( self
                , DEVICE_STATION = ""
                , TRANSLATION_FACTOR = 1.0
                , ROTATION_FACTOR = 1.0
                , SCALE_FACTOR = 1.0
                ):


        # call base class constructor
        MultiDofInput.__init__( self
                              , DEVICE_STATION = DEVICE_STATION
                              , TRANSLATION_FACTOR = TRANSLATION_FACTOR
                              , ROTATION_FACTOR = ROTATION_FACTOR
                              , SCALE_FACTOR = SCALE_FACTOR
                              )        
        

    ### instance functions ####

    ## override respective base class function
    def update_input(self):
        # read and filter values
        self.dofs = [0.0,0.0,0.0,0.0,0.0,0.0,0.0]

        _x = self.get_value0()
        _y = self.get_value2() * -1.0
        _z = self.get_value1()
        _rx = self.get_value3() * -1.0
        _ry1 = self.get_value4() * -1.0
        _rz = self.get_value5() * -1.0
        _ry2 = self.get_value6() * -1.0

        #print("X", _x)
        #print("Y", _y)
        #print("Z", _z)
        #print(self.device_sensor.Value0.value, self.device_sensor.Station.value)

        self.set_and_filter_dof(0, _x, 0.0, -1.0, 1.0, 0, 0)
        self.set_and_filter_dof(1, _y, 0.0, -0.027, 0.28, 0, 0)
        self.set_and_filter_dof(2, _z, 0.0, -1.0, 1.0, 0, 0)
        self.set_and_filter_dof(3, _rx, 0.0, 150.0, 150.0, 0, 0)
        self.set_and_filter_dof(4, _ry1, 0.0, 150.0, 150.0, 0, 0)
        self.set_and_filter_dof(5, _rz, 0.0, 150.0, 150.0, 0, 0)
        self.set_and_filter_dof(4, _ry2, 0.0, -1.0, 1.0, 0, 0)

        self.dofs[0] *= self.translation_factor
        self.dofs[1] *= self.translation_factor
        self.dofs[2] *= self.translation_factor
        self.dofs[3] *= self.rotation_factor
        self.dofs[4] *= self.rotation_factor
        self.dofs[5] *= self.rotation_factor
        self.dofs[6] *= self.scale_factor

        # read buttons
        _button0 = self.get_button0() # left Spheron key
        _button1 = self.get_button1() # right Spheron key

        self.set_dof(6, _button0 * 1.0)
        self.set_dof(6, _button1 * -1.0)

        self.script.mf_dof.value = self.dofs # forward input once (if values have changed)


    def get_button3_field(self):  
        return None

    def get_button4_field(self):
        return None


class OldSpheronInput(MultiDofInput):

    ### constructor
    def __init__( self
                , DEVICE_STATION = ""
                , TRANSLATION_FACTOR = 1.0
                , ROTATION_FACTOR = 10.0
                , SCALE_FACTOR = 0.5
                ):


        # call base class constructor
        MultiDofInput.__init__( self
                              , DEVICE_STATION = DEVICE_STATION
                              , TRANSLATION_FACTOR = TRANSLATION_FACTOR
                              , ROTATION_FACTOR = ROTATION_FACTOR
                              , SCALE_FACTOR = SCALE_FACTOR
                              )        
        
        ### instance variables ###
        self.y_init_flag = False
        self.rx_init_flag = False
        self.ry_init_flag = False
        self.rz_init_flag = False                
        
        ### resources ###
        self.button_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
        self.button_sensor.Station.value = "device-old-spheron-buttons"
        

    ### functions ####

    ## override respective base class function
    def update_input(self):
        # read and filter values
        self.dofs = [0.0,0.0,0.0,0.0,0.0,0.0,0.0]

        _x = self.get_value0()
        _y = self.get_value1()
        _z = self.get_value2()
        _rx = self.get_value3()
        _ry = self.get_value4()
        _rz = self.get_value5()


        if self.y_init_flag == False and _y != 0.0:
            self.y_init_flag = True
            
        if self.rx_init_flag == False and _rx != 0.0:
            self.rx_init_flag = True
        if self.ry_init_flag == False and _ry != 0.0:
            self.ry_init_flag = True
        if self.rz_init_flag == False and _rz != 0.0:
            self.rz_init_flag = True

        #print("X", _x)
        #print("Y", _y)
        #print("Z", _z)
        #print("RX", _rx)
        #print("RY", _ry)

        self.set_and_filter_dof(0, _x, -0.007599108852446079, -0.0138, 0.00091555, 2, 2)
        if self.y_init_flag == True:
            self.set_and_filter_dof(1, _y, -0.006958220154047012, -0.01358073, 0.0016479, 10, 10)
        self.set_and_filter_dof(2, _z, -0.007812738418579102, -0.015045, 0.0006103, 2, 2)
        if self.rx_init_flag == True:
            self.set_and_filter_dof(3, _rx, -0.00787377543747425, -0.0095, -0.006, 0, 0)
        if self.ry_init_flag == True:
            self.set_and_filter_dof(4, _ry, -0.00787377543747425, -0.0095, -0.006, 0, 0)
        if self.rz_init_flag == True:
            self.set_and_filter_dof(5, _rz, -0.00787377543747425, -0.0095, -0.006, 0, 0)

        # read buttons
        '''
        _button0 = self.get_button0() # left Spheron key
        _button2 = self.get_button2() # right Spheron key

        self.set_dof(6, _button0 * 1.0)
        self.set_dof(6, _button2 * -1.0)
        '''

        self.dofs[0] *= self.translation_factor
        self.dofs[1] *= self.translation_factor
        self.dofs[2] *= self.translation_factor
        self.dofs[3] *= self.rotation_factor
        self.dofs[4] *= self.rotation_factor
        self.dofs[5] *= self.rotation_factor
        self.dofs[6] *= self.scale_factor
        

        self.script.mf_dof.value = self.dofs # forward input once (if values have changed)




    def get_button0_field(self):  
        return self.button_sensor.Button0

    def get_button0(self):  
        return self.button_sensor.Button0.value

    def get_button1_field(self):  
        return self.button_sensor.Button1

    def get_button1(self):  
        return self.button_sensor.Button1.value

    def get_button2_field(self):  
        return self.button_sensor.Button2

    def get_button2(self):  
        return self.button_sensor.Button2.value

    def get_button3_field(self):  
        return None

    def get_button4_field(self):
        return None



class DualSpheronInputScript(avango.script.Script):

    ### internal fields ###

    ## @var mf_dof
    # The input values of the input device.
    mf_dof = avango.MFFloat()
    mf_dof.value = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] # init 7 channels: [trans_x, trans_y, trans_z, pitch, head, roll, scale]


    ### Default constructor.
    def __init__(self):
        self.super(DualSpheronInputScript).__init__()

        self.CLASS = None


    def my_constructor(self, CLASS):
        ### references ###
        self.CLASS = CLASS
    
        self.always_evaluate(True)


    ### callback functions ###
    def evaluate(self):
        self.CLASS.update_input()



class DualSpheronInput:

    ### constructor
    def __init__(self,
        DEVICE_STATION1 = "",
        DEVICE_STATION2 = "",
        TRANSLATION_FACTOR = 1.0,
        ROTATION_FACTOR = 1.0,
        SCALE_FACTOR = 1.0,
        ):


        ### instance variables ###
        self.translation_factor = TRANSLATION_FACTOR
        self.rotation_factor = ROTATION_FACTOR
        self.scale_factor = SCALE_FACTOR

        ### resources ###

        ## @var device_sensor
        # Device sensor for the device's inputs.
        self.device_sensor1 = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService()) # right joystick sensor
        self.device_sensor1.Station.value = DEVICE_STATION1

        self.device_sensor2 = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService()) # left joystick sensor
        self.device_sensor2.Station.value = DEVICE_STATION2

        ## init script
        self.script = DualSpheronInputScript()
        self.script.my_constructor(self)
        

    ### functions ####

    def update_input(self):
        # read and filter values
        self.dofs = [0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]

        ## left spheron joystick
        _x_left = self.device_sensor2.Value0.value
        _y_left = self.device_sensor2.Value1.value
        _z_left = self.device_sensor2.Value2.value
        _rx_left = self.device_sensor2.Value3.value
        _ry_left = self.device_sensor2.Value4.value
        _rz_left = self.device_sensor2.Value5.value

        _throttle_left = self.device_sensor2.Value6.value

        #print(_x_left, _y_left, _z_left, _throttle_left)
        #print(_rx_left)
        
        self.set_and_filter_dof(7, _x_left, 0.0, -1.0, 1.0, 0, 0)
        self.set_and_filter_dof(8, _y_left, 0.0, -0.135, 0.18, 0, 0)
        self.set_and_filter_dof(9, _z_left, 0.0, -1.0, 1.0, 0, 0)
        self.set_and_filter_dof(10, _rx_left, 0.0, -1000.0, 1000.0, 0, 0)
        self.set_and_filter_dof(11, _ry_left, 0.0, -1000.0, 1000.0, 0, 0)
        self.set_and_filter_dof(12, _rz_left, 0.0, -1000.0, 1000.0, 0, 0)
        self.set_and_filter_dof(13, _throttle_left, 0.0, -1.0, 1.0, 0, 0)


        self.dofs[7] *= self.translation_factor * -1.0
        self.dofs[8] *= self.translation_factor * -1.0
        self.dofs[9] *= self.translation_factor
        self.dofs[10] *= self.rotation_factor
        self.dofs[11] *= self.rotation_factor
        self.dofs[12] *= self.rotation_factor * -1.0
        self.dofs[13] *= self.scale_factor


        ## right spheron joystick
        _x_right = self.device_sensor1.Value0.value
        _y_right = self.device_sensor1.Value1.value
        _z_right = self.device_sensor1.Value2.value
        #_rx_right = self.device_sensor1.Value3.value
        #_ry_right = self.device_sensor1.Value4.value
        #_rz_right = self.device_sensor1.Value5.value

        _throttle_right = self.device_sensor1.Value6.value

        #print(_y_right)

        self.set_and_filter_dof(0, _x_right, 0.0, -1.0, 1.0, 0, 0)
        self.set_and_filter_dof(1, _y_right, 0.0, -0.09, 0.13, 0, 0)
        self.set_and_filter_dof(2, _z_right, 0.0, -1.0, 1.0, 0, 0)
        ##self.set_and_filter_dof(3, _rx_right, 0.0, 150.0, 150.0, 0, 0)
        ##self.set_and_filter_dof(4, _ry_right, 0.0, 150.0, 150.0, 0, 0)
        ##self.set_and_filter_dof(5, _rz_right, 0.0, 150.0, 150.0, 0, 0)
        self.set_and_filter_dof(6, _throttle_right, 0.0, -1.0, 1.0, 0, 0)


        self.dofs[0] *= self.translation_factor
        self.dofs[1] *= self.translation_factor * -1.0
        self.dofs[2] *= self.translation_factor * -1.0
        #self.dofs[3] *= self.rotation_factor
        #self.dofs[4] *= self.rotation_factor
        #self.dofs[5] *= self.rotation_factor
        self.dofs[6] *= self.scale_factor

        self.script.mf_dof.value = self.dofs # forward input once (if values have changed)

        #print(self.device_sensor1.Button0.value, self.device_sensor1.Button1.value, self.device_sensor1.Button2.value, self.device_sensor1.Button3.value, self.device_sensor1.Button4.value)

    ## Map an input value to a certain interval.
    # @param VALUE The value to be mapped.
    # @param OFFSET The offset to be applied to VALUE, MIN and MAX.
    # @param MIN The minimum value of the old interval.
    # @param MAX The maximum value of the old interval.
    # @param NEG_THRESHOLD The negative threshold to be used.
    # @param POS_THRESHOLD The positive threshold to be used.
    def filter_channel(self, VALUE, OFFSET, MIN, MAX, NEG_THRESHOLD, POS_THRESHOLD):
        VALUE = VALUE - OFFSET
        MIN = MIN - OFFSET
        MAX = MAX - OFFSET

        #print "+", VALUE, MAX, POS_THRESHOLD
        #print "-", VALUE, MIN, NEG_THRESHOLD

        if VALUE > 0:
            _pos = MAX * POS_THRESHOLD * 0.01

            if VALUE > _pos: # above positive threshold
                VALUE = min( (VALUE - _pos) / (MAX - _pos), 1.0) # normalize interval

            else: # beneath positive threshold
                VALUE = 0

        elif VALUE < 0:
            _neg = MIN * NEG_THRESHOLD * 0.01

            if VALUE < _neg:
               VALUE = max( (VALUE - _neg) / abs(MIN - _neg), -1.0)

            else: # above negative threshold
                VALUE = 0

        return VALUE


    ## Sets a specific degree of freedom to a value which is filtered before.
    # @param ID ID Number of the degree of freedom to be set.
    # @param VALUE The value to be filtered.
    # @param OFFSET The offset to be applied to VALUE, MIN and MAX.
    # @param MIN The minimum value of the old interval.
    # @param MAX The maximum value of the old interval.
    # @param NEG_THRESHOLD The negative threshold to be used.
    # @param POS_THRESHOLD The positive threshold to be used.
    def set_and_filter_dof(self, ID, VALUE, OFFSET, MIN, MAX, NEG_THRESHOLD, POS_THRESHOLD):
        self.dofs[ID] += self.filter_channel(VALUE, OFFSET, MIN, MAX, NEG_THRESHOLD, POS_THRESHOLD)


    ## Sets a specific degree of freedom to a value.
    # @param ID ID Number of the degree of freedom to be set.
    # @param VALUE The value to be set.
    def set_dof(self, ID, VALUE):
        self.dofs[ID] += VALUE


    def get_dofs(self):
        return self.script.mf_dof.value


    def get_dofs_field(self):
        return self.script.mf_dof


    def get_button0(self):
        return self.device_sensor1.Button0.value


    def get_button0_field(self):
      return self.device_sensor1.Button0


    def get_button1(self):
        return self.device_sensor1.Button1.value


    def get_button1_field(self):
        return self.device_sensor1.Button1
   

    def get_button2(self):
       return self.device_sensor1.Button2.value


    def get_button2_field(self): 
        return self.device_sensor1.Button2


    def get_button3(self):
       return self.device_sensor1.Button3.value


    def get_button3_field(self): 
        return self.device_sensor1.Button3


    def get_button4(self):
       return self.device_sensor1.Button4.value


    def get_button4_field(self): 
        return self.device_sensor1.Button4
