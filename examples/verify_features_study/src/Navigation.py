#!/usr/bin/python

## @file
# Contains base class Navigation.

### import avango-guacamole libraries
import avango
import avango.gua
from avango.script import field_has_changed



class NavigationScript(avango.script.Script):

    ### input fields ###
    mf_dof = avango.MFFloat()


    ### output fields ###

    ## @var sf_platform_mat
    # Combined matrix of translation, rotation and scaling of the viewing platform
    sf_platform_mat = avango.gua.SFMatrix4()
    sf_platform_mat.value = avango.gua.make_identity_mat()

    sf_reference_mat = avango.gua.SFMatrix4()
    sf_reference_mat.value = avango.gua.make_identity_mat()


    ### Default constructor.
    def __init__(self):
        self.super(NavigationScript).__init__()

        self.CLASS = None

    def my_constructor(self, CLASS):
        # references
        self.CLASS = CLASS
        


## Base class. Not to be instantiated.
class Navigation:

    ### static class variables ###

    ## @var number_of_instances
    # Number of Navigation instances already created. Used for trace material assignment.
    number_of_instances = 0

    ## Base constructor.
    def __init__(self,
        NAME = "",
        MATRIX = avango.gua.make_identity_mat(),
        SCALE = 1.0,
        DEFAULT_REFERENCE_MAT = avango.gua.make_identity_mat(),
        REFERENCE_TRACKING_STATION = None,
        TRANSMITTER_OFFSET = avango.gua.make_identity_mat(),
        RECEIVER_OFFSET = avango.gua.make_identity_mat(),        
        ):


        self.id = Navigation.number_of_instances
        Navigation.number_of_instances += 1
        

        ### parameters ###
        self.color = avango.gua.Color(1.0,1.0,1.0) # is set later


        ### variables ###

        self.name = NAME

        ## @var home_nav_mat
        # Initial position matrix of the navigation.
        self.home_nav_mat = MATRIX

        ## @var home_nav_scale
        # Initial scaling factor of the navigation.
        self.home_nav_scale = SCALE

        ## @var nav_mat
        # Matrix representing the current translation and rotation of the navigation in the scene.
        self.nav_mat = MATRIX

        ## @var nav_scale
        # The current scaling factor of this navigation platform.
        self.nav_scale = SCALE

        self.default_reference_mat = DEFAULT_REFERENCE_MAT
        self.reference_tracking_station = REFERENCE_TRACKING_STATION
        self.transmitter_offset = TRANSMITTER_OFFSET
        self.receiver_offset = RECEIVER_OFFSET
            

        ### resources ###
        self.bc_script = NavigationScript()
        self.bc_script.my_constructor(self)

        if self.reference_tracking_station is not None:
            ## init reference tracking sensor
            self.reference_tracking_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
            self.reference_tracking_sensor.Station.value = self.reference_tracking_station
            self.reference_tracking_sensor.TransmitterOffset.value = self.transmitter_offset
            self.reference_tracking_sensor.ReceiverOffset.value = self.receiver_offset

            self.bc_script.sf_reference_mat.disconnect()
            self.bc_script.sf_reference_mat.connect_from(self.reference_tracking_sensor.Matrix)

        else:
            self.set_reference_matrix(self.default_reference_mat)


        ### set initial states ###
        self.set_nav_mat(MATRIX)
        self.set_nav_scale(SCALE)


    ### functions ###

    def get_name(self):
        return self.name

    def get_reference_mat(self):  
        return self.bc_script.sf_reference_mat.value


    def set_reference_matrix(self, MATRIX):
        self.bc_script.sf_reference_mat.value = MATRIX


    def get_default_reference_matrix(self):
        return self.default_reference_mat


    def set_default_reference_matrix(self, MATRIX):
        self.default_reference_mat = MATRIX


    def get_platform_mat(self):  
        return self.bc_script.sf_platform_mat.value

    def get_platform_matrix_field(self):  
        return self.bc_script.sf_platform_mat

    def set_platform_matrix(self, MAT4):
        self.bc_script.sf_platform_mat.value = MAT4


    def set_home_nav_mat(self, MATRIX):
        self.home_nav_mat = MATRIX

    def get_home_nav_mat(self):
        return self.home_nav_mat
        
        
    def set_home_nav_scale(self, SCALE):
        self.home_nav_scale = SCALE

    def get_home_nav_scale(self):
        return self.home_nav_scale


    def get_nav_mat(self):
        return self.nav_mat
        
    def set_nav_mat(self, MATRIX):
        self.nav_mat = MATRIX

        # update viewing platform
        self.set_platform_matrix(self.nav_mat * avango.gua.make_scale_mat(self.nav_scale))
       

    def get_nav_scale(self):
        return self.nav_scale

    def set_nav_scale(self, SCALE):
        self.nav_scale = SCALE

        # update viewing platform
        self.set_platform_matrix(self.nav_mat * avango.gua.make_scale_mat(self.nav_scale))


    def set_color(self, COLOR):
        self.color = COLOR
        


    ## Resets the navigation's matrix to the initial value.
    def reset(self):
        self.set_nav_mat(self.get_home_nav_mat())
        self.set_nav_scale(self.get_home_nav_scale())
