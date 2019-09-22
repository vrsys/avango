#!/usr/bin/python

## @file
# Contains classes DisplayGroup and VirtualDisplayGroup.

### import avango-guacamole libraries
import avango
import avango.gua
from avango.script import field_has_changed


# import framework libraries
# ...

### import python libraries
# ...

       

class MatrixInput:

    ### class variables ###

    ## @var number_of_instances
    # Number of Mouse instances that have already been created. Used for assigning unique IDs.
    number_of_instances = 0


    ### constructor
    def __init__( self
                , DEFAULT_MAT = avango.gua.make_identity_mat()              
                ):


        ## @var id
        # Identification number of this user.
        self.id = MatrixInput.number_of_instances
        MatrixInput.number_of_instances += 1 

        ### instance variables ###

        self.default_mat = DEFAULT_MAT


    ### functions ###
    def get_matrix(self):
        raise NotImplementedError("To be implemented by a subclass.")   


    def get_matrix_field(self):
        raise NotImplementedError("To be implemented by a subclass.")   


    def get_button0(self):
        raise NotImplementedError("To be implemented by a subclass.")   


    def get_button0_field(self):
        raise NotImplementedError("To be implemented by a subclass.")   


    def get_button1(self):
        raise NotImplementedError("To be implemented by a subclass.")   


    def get_button1_field(self):
        raise NotImplementedError("To be implemented by a subclass.")   





class MultiDofInputScript(avango.script.Script):

    ### internal fields ###

    ## @var mf_dof
    # The input values of the input device.
    mf_dof = avango.MFFloat()
    mf_dof.value = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] # init 7 channels: [trans_x, trans_y, trans_z, pitch, head, roll, scale]

    '''
    ## @var sf_mat
    # Tracking position and rotation.
    sf_device_mat = avango.gua.SFMatrix4()
    sf_device_mat.value = avango.gua.make_identity_mat()
    '''

    '''
    ## values
    sf_value0 = avango.SFFloat()
    sf_value1 = avango.SFFloat()
    sf_value2 = avango.SFFloat()
    sf_value3 = avango.SFFloat()
    sf_value4 = avango.SFFloat()
    sf_value5 = avango.SFFloat()
    sf_value6 = avango.SFFloat()            

    ## buttons
    sf_button0 = avango.SFBool()
    sf_button1 = avango.SFBool()
    sf_button2 = avango.SFBool()
    '''

    ### Default constructor.
    def __init__(self):
        self.super(MultiDofInputScript).__init__()

        self.CLASS = None


    def my_constructor(self, CLASS):

        ### references ###    
        self.CLASS = CLASS

        self.always_evaluate(True)
        '''
        self.sf_value0.connect_from(self.CLASS.get_value0_field())
        self.sf_value1.connect_from(self.CLASS.get_value1_field())
        self.sf_value2.connect_from(self.CLASS.get_value2_field())
        self.sf_value3.connect_from(self.CLASS.get_value3_field())
        self.sf_value4.connect_from(self.CLASS.get_value4_field())
        self.sf_value5.connect_from(self.CLASS.get_value5_field())
        self.sf_value6.connect_from(self.CLASS.get_value6_field())
        '''


    ### callback functions ###
    def evaluate(self):
        self.CLASS.update_input()

class MultiDofInput:

    ### class variables ###

    ## @var number_of_instances
    # Number of Mouse instances that have already been created. Used for assigning unique IDs.
    number_of_instances = 0


    ### constructor
    def __init__( self
              , DEVICE_STATION = ""
              , TRANSLATION_FACTOR = 0.0
              , ROTATION_FACTOR = 1.0
              , SCALE_FACTOR = 1.0
              ):


        if DEVICE_STATION == "":
            print("Mouse not initialized: DEVICE_STATION missing")
            return

        ## @var id
        # Identification number of this user.
        self.id = MultiDofInput.number_of_instances
        MultiDofInput.number_of_instances += 1 


        ### parameters ###

        self.device_station = DEVICE_STATION

        ## @var translation_factor
        # Factor to modify the device's translation input.
        self.translation_factor = TRANSLATION_FACTOR

        ## @var rotation_factor
        # Factor to modify the device's rotation input.
        self.rotation_factor = ROTATION_FACTOR

        self.scale_factor = SCALE_FACTOR


        ### variables ###

        ## @var dofs
        # Temporary list of degrees of freedom to process input bindings.
        self.dofs = [0.0,0.0,0.0,0.0,0.0,0.0,0.0]


        ### resources ###

        ## @var device_sensor
        # Device sensor for the device's inputs.
        self.device_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
        print('DEVICE STATION, self.id ', self.device_station)
        self.device_sensor.Station.value = self.device_station


        self.script = MultiDofInputScript()
        self.script.my_constructor(self)


    ### functions ###
    def get_value0(self): 
        return self.device_sensor.Value0.value


    def get_value0_field(self): 
        return self.device_sensor.Value0
  

    def get_value1(self): 
        return self.device_sensor.Value1.value


    def get_value1_field(self): 
        return self.device_sensor.Value1


    def get_value2(self):  
        return self.device_sensor.Value2.value


    def get_value2_field(self):  
        return self.device_sensor.Value2


    def get_value3(self):  
        return self.device_sensor.Value3.value


    def get_value3_field(self):  
        return self.device_sensor.Value3


    def get_value4(self):  
        return self.device_sensor.Value4.value


    def get_value4_field(self):  
        return self.device_sensor.Value4


    def get_value5(self):  
        return self.device_sensor.Value5.value


    def get_value5_field(self):  
        return self.device_sensor.Value5
     

    def get_value6(self):  
        return self.device_sensor.Value6.value


    def get_value6_field(self):  
        return self.device_sensor.Value6


    def get_dofs(self):
        return self.script.mf_dof.value


    def get_dofs_field(self):
        return self.script.mf_dof


    def get_button0(self):  
        return self.device_sensor.Button0.value


    def get_button0_field(self): 
        return self.device_sensor.Button0


    def get_button1(self):  
        return self.device_sensor.Button1.value


    def get_button1_field(self): 
        return self.device_sensor.Button1
   

    def get_button2(self): 
        return self.device_sensor.Button2.value


    def get_button2_field(self): 
        return self.device_sensor.Button2

  
    def update_input(self):
        raise NotImplementedError("To be implemented by a subclass.")
  


    ## Map an input value to a certain interval.
    # @param VALUE The value to be mapped.
    # @param OFFSET The offset to be applied to VALUE, MIN and MAX.
    # @param MIN The minimum value of the old interval.
    # @param MAX The maximum value of the old interval.
    # @param NEG_THRESHOLD The negative threshold to be used.
    # @param POS_THRESHOLD The positive threshold to be used.
    def filter_channel(self, VALUE, OFFSET, MIN, MAX, NEG_THRESHOLD, POS_THRESHOLD):
        VALUE -= OFFSET
        MIN -= OFFSET
        MAX -= OFFSET

        #print("+", VALUE, MAX, POS_THRESHOLD)
        #print("-", VALUE, MIN, NEG_THRESHOLD)

        if VALUE > 0:
            _pos = MAX * POS_THRESHOLD * 0.01

            if VALUE > _pos: # above positive threshold
                VALUE = min( (VALUE - _pos) / (MAX - _pos), 1.0) # normalize interval

            else: # below positive threshold
                VALUE = 0


        elif VALUE < 0:
            _neg = MIN * abs(NEG_THRESHOLD) * 0.01

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
    def set_and_filter_dof(self, ID = None, VALUE = 0.0, OFFSET = 0.0, MIN = -1.0, MAX = 1.0, NEG_THRESHOLD = 0.0, POS_THRESHOLD = 0.0):
        self.dofs[ID] += self.filter_channel(VALUE, OFFSET, MIN, MAX, NEG_THRESHOLD, POS_THRESHOLD)


    ## Sets a specific degree of freedom to a value.
    # @param ID ID Number of the degree of freedom to be set.
    # @param VALUE The value to be set.
    def set_dof(self, ID, VALUE):
        self.dofs[ID] += VALUE
    
  
