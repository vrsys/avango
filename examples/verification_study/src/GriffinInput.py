#!/usr/bin/python

## @file
# Contains classes SpacemouseInput.

### import avango-guacamole libraries
import avango
import avango.gua

### import framework libraries
from src.Input import MultiDofInput


class GriffinInput(MultiDofInput):

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

    ### functions ####

    # implement respective base-class function
    def update_input(self): # override respective base class function    
        self.dofs = [0.0,0.0,0.0,0.0,0.0,0.0,0.0]
        
        _ry = self.get_value0() 

                
        MultiDofInput.set_and_filter_dof(self, ID = 0, VALUE = _ry, MIN = -350.0, MAX = 350.0, NEG_THRESHOLD = 0, POS_THRESHOLD = 0)
        
        _button0 = self.get_button0()

        self.set_dof(6, _button0 * 1.0)

        self.dofs[0] *= self.rotation_factor
       
        self.script.mf_dof.value = self.dofs # forward input once
    
"""
class NewSpacemouseInput(MultiDofInput):

    ### constructor
    def __init__(self,
        DEVICE_STATION = "",
        TRANSLATION_FACTOR = 1.0,
        ROTATION_FACTOR = 1.0,
        SCALE_FACTOR = 1.0
        ):

    
        # call base class constructor
        MultiDofInput.__init__(self,
            DEVICE_STATION = DEVICE_STATION,
            TRANSLATION_FACTOR = TRANSLATION_FACTOR,
            ROTATION_FACTOR = ROTATION_FACTOR,
            SCALE_FACTOR = SCALE_FACTOR
            )

    ### functions ####
       
    # implement respective base-class function
    def update_input(self):
        self.dofs = [0.0,0.0,0.0,0.0,0.0,0.0,0.0]
        
        _x = self.get_value0()
        _y = self.get_value1() * -1.0
        _z = self.get_value2()
        _rx = self.get_value3()
        _ry = self.get_value4() * -1.0
        _rz = self.get_value5()
        
        #print(_rx, _x, _y, _z)
        
        ## map translation DoF
        MultiDofInput.set_and_filter_dof(self, ID = 0, VALUE = _x, MIN = -350.0, MAX = 350.0, NEG_THRESHOLD = -3, POS_THRESHOLD = 3)
        MultiDofInput.set_and_filter_dof(self, ID = 1, VALUE = _y, MIN = -350.0, MAX = 350.0, NEG_THRESHOLD = -3, POS_THRESHOLD = 3)
        MultiDofInput.set_and_filter_dof(self, ID = 2, VALUE = _z, MIN = -350.0, MAX = 350.0, NEG_THRESHOLD = -3, POS_THRESHOLD = 3)

        ## map rotation DoF
        MultiDofInput.set_and_filter_dof(self, ID = 3, VALUE = _rx, MIN = -350.0, MAX = 350.0, NEG_THRESHOLD = -5, POS_THRESHOLD = 5)
        MultiDofInput.set_and_filter_dof(self, ID = 4, VALUE = _ry, MIN = -350.0, MAX = 350.0, NEG_THRESHOLD = -5, POS_THRESHOLD = 5)
        MultiDofInput.set_and_filter_dof(self, ID = 5, VALUE = _rz, MIN = -350.0, MAX = 350.0, NEG_THRESHOLD = -5, POS_THRESHOLD = 5)

        ## map scale DoF
        self.set_dof(6, self.get_button1() * 1.0)
        self.set_dof(6, self.get_button2() * -1.0)


        ## apply isomorphic scaling factor
        self.dofs[0] *= self.translation_factor
        self.dofs[1] *= self.translation_factor
        self.dofs[2] *= self.translation_factor
        self.dofs[3] *= self.rotation_factor
        self.dofs[4] *= self.rotation_factor
        self.dofs[5] *= self.rotation_factor
        self.dofs[6] *= self.scale_factor
       
        self.script.mf_dof.value = self.dofs # forward input once
"""