#!/usr/bin/python

## @file
# Contains class SteeringNavigation.

### import avango-guacamole libraries
import avango
import avango.gua
import avango.script
from avango.script import field_has_changed

### import framework libraries
from src.Navigation import Navigation
#from lib.Intersection import *
#import lib.Utilities


### import python libraries
import math
import time
import builtins
       

class SteeringNavigationScript(avango.script.Script):

    ### input fields ###

    ## @var mf_dof
    # The relative input values of the device.
    mf_dof = avango.MFFloat()


    ## @var sf_groundfollowing_trigger
    # Boolean field to indicate if the change of the dof mode is to be triggered.
    sf_groundfollowing_trigger = avango.SFBool()

    ### Default constructor.
    def __init__(self):
        self.super(SteeringNavigationScript).__init__()

        ### external references ###
        self.CLASS = None


    def my_constructor(self, CLASS):
        self.CLASS = CLASS


    ### callbacks ###
    @field_has_changed(mf_dof)
    def mf_dof_changed(self):
        if self.CLASS is not None and len(self.mf_dof.value) == 7:
            self.CLASS.map_input(
                self.mf_dof.value[0],
                self.mf_dof.value[1],
                self.mf_dof.value[2],
                self.mf_dof.value[3],
                self.mf_dof.value[4],
                self.mf_dof.value[5],
                self.mf_dof.value[6],
                )


    ## Evaluated when value changes.
    @field_has_changed(sf_groundfollowing_trigger)
    def sf_groundfollowing_trigger_changed(self):
        if self.CLASS is not None and self.sf_groundfollowing_trigger.value == True: # button pressed
            #self.CLASS.toggle_groundfollowing()
            self.CLASS.toggle_nav_mode()

    

class SteeringNavigation(Navigation):

    ### constructor
    def __init__(self,
        MATRIX = avango.gua.make_identity_mat(),
        SCALE = 1.0,
        DEFAULT_REFERENCE_MAT = avango.gua.make_identity_mat(),
        REFERENCE_TRACKING_STATION = None,
        GF_START_HEIGHT = 1.5, # in meter
        GF_PICK_LENGTH = 100.0, # in meter              
        GF_PICK_DIRECTION = avango.gua.Vec3(0.0, -1.0, 0.0),
        INITIAL_MODE = 0, # 6DoF navigation mode        
        ):

        self.nav_mode = 0
         
        # call base class constructor
        Navigation.__init__(self,
            NAME = "SteeringNavigation",
            MATRIX = MATRIX,
            SCALE = SCALE,
            DEFAULT_REFERENCE_MAT = DEFAULT_REFERENCE_MAT,
            REFERENCE_TRACKING_STATION = REFERENCE_TRACKING_STATION,            
            )
    
        ### external references ###
        self.INPUT = None


        ### parameters ###

        ## @var min_scale
        # The minimum scaling factor that can be applied.
        self.min_scale = 0.0004

        ## @var max_scale
        # The maximum scaling factor that can be applied.
        self.max_scale = 10000.0
        
        ## @var scale_stop_duration
        # Time how long a scaling process is stopped at a fixed step in seconds.
        self.scale_stop_duration = 1.0 # in seconds   
       
        self.reference_center_size = 0.08 # in meter
    

        ## groundfollowing parameters
        self.gf_start_height = GF_START_HEIGHT
        self.gf_pick_length = GF_PICK_LENGTH
        self.gf_pick_direction = GF_PICK_DIRECTION

        ## @var gf_compensation_factor
        # Scaling factor used for the modification of up and down vectors.
        self.gf_compensation_factor = 0.17 # in %

        self.gf_max_fall_velocity = 10.0 # in meter/sec
        
        ## @var initial_fall_velocity
        # The starting velocity when the user is falling in meters per second. Is increased the longer the falling process goes on.
        self.gf_initial_fall_velocity = 1.0 # in meter/sec



        ### variables ###

        ## @var nav_mode
        # value to indicate if the user is navigation mode: 0 = 6DoF navigation; 1 = ground-based movement (incl. GF); 2 = 4DOF navigation (no roll & pitch); 3 = horizon allignment animation; 4 = object-based
        self.nav_mode = 0

        ## @var scale_stop_time
        # Time at which a scaling process stopped at a fixed step.
        self.scale_stop_time = None
    
    
        self.override_reference_pos = avango.gua.Vec3()
    

        ## groundfollowing variables
        self.falling = False
        self.gf_fall_velocity = self.gf_initial_fall_velocity
        
    
        #### resources ###            
        '''
        self.intersection = AdvancedPlodIntersection(
            PICK_LENGTH = self.gf_pick_length,
            PICK_DIRECTION = self.gf_pick_direction,
            BLACK_LIST = ["invisible", "unpickable", "gf_unpickable"],
            PLOD_SPLAT_INTERSECTION_FLAG = False, # use (faster) BVH pick in point-based scenes
            #PLOD_SPLAT_INTERSECTION_FLAG = True, # use (slower but more precise) splat-based pick in point-based scenes (default)
            )
        '''

        
        ## init script
        self.script = SteeringNavigationScript()
        self.script.my_constructor(self)
    

        ### set initial states ###
        self.reset()
        self.set_nav_mode(INITIAL_MODE)
                


    ### functions ###
    def assign_input(self, INPUT):
        self.INPUT = INPUT # update reference

        self.script.mf_dof.disconnect()
        self.script.mf_dof.connect_from(INPUT.get_dofs_field())

        '''
        if self.INPUT.get_button0_field() is not None:
            self.script.sf_groundfollowing_trigger.disconnect()
            self.script.sf_groundfollowing_trigger.connect_from(INPUT.get_button0_field())
            #self.script.sf_groundfollowing_trigger.connect_from(INPUT.get_button1_field()) # for spacemouse (left-side button)
        '''
        
        

    def set_nav_mode(self, INT):
        self.nav_mode = INT
        #print("nav mode", self.nav_mode)

        ## update reference center visibility        
        #if self.nav_mode == 4: # object-based mode
        #    self.reference_center_geometry.Tags.value = [] # set visible
        #else:
        #    self.reference_center_geometry.Tags.value = ["invisible"] # set invisible
                            
        

    def enable_override_reference_position(self, BOOL):
        if BOOL == True:
            self.set_nav_mode(4) # switch to object-based navigation mode
        else:
            self.set_nav_mode(0) # switch to 6DoF navigation mode


    def set_override_reference_position(self, POS):
        _pos = avango.gua.make_inverse_mat(Navigation.get_platform_mat(self)) * POS # reference position (global) in platform coordinate system
        self.override_reference_pos = avango.gua.Vec3(_pos.x, _pos.y, _pos.z) # cast to Vec3


    def toggle_nav_mode(self):
        if self.nav_mode == 0: # 6DoF navigation mode
            self.set_nav_mode(3) # switch to horizon allignment animation (and then groundfollowing)
        elif self.nav_mode == 1: # groundfollwing navigation mode
            self.set_nav_mode(2) # switch to 4DoF (upright) navigation mode
        elif self.nav_mode == 2: # 4DoF (upright)
            self.set_nav_mode(0) # switch to 6DoF navigation mode
        


    '''
    def toggle_groundfollowing(self):
        if self.nav_mode == 0: # 6DoF navigation mode
            self.set_nav_mode(3) # switch to horizon allignment animation (and then groundfollowing)
        elif self.nav_mode == 1: # groundfollwing navigation mode
            self.set_nav_mode(0) # switch to 6DoF navigation mode
            
        self.update_gui()
    '''

    def map_input(self, X, Y, Z, RX, RY, RZ, S):
        ## map inputs
        self.map_scale_input(S)
        self.map_movement_input(X, Y, Z, RX, RY, RZ)
      
        if self.nav_mode == 1: # in groundfollowing navigation mode
            return
            #self.map_groundfollowing_input()
            
        elif self.nav_mode == 3: # in horizon allignment animation
            _new_mat = self.matrixAnimator.get_matrix()        
            Navigation.set_nav_mat(self, _new_mat)
   
            if self.matrixAnimator.is_active() == False: # animation is finished
                self.set_nav_mode(1) # switch to groundfollowing navigation mode
            
        '''
        # update reference center visualization
        if self.nav_mode == 4: # object-based mode uses override reference position
            self.reference_center_geometry.Transform.value = \
                avango.gua.make_trans_mat(self.override_reference_pos) * \
                avango.gua.make_scale_mat(self.reference_center_size)
        else:
            self.reference_center_geometry.Transform.value = \
                Navigation.get_reference_mat(self) * \
                avango.gua.make_scale_mat(self.reference_center_size)
        '''


    def map_movement_input(self, X, Y, Z, RX, RY, RZ):
        ## get info
        #_fps_scale_factor = builtins.APPLICATION.get_application_fps_scale_factor()
        _fps_scale_factor = 1.0

        _nav_mat = Navigation.get_nav_mat(self)
        _nav_scale = Navigation.get_nav_scale(self)

        if self.nav_mode == 4: # object-based mode
            _ref_center = self.override_reference_pos
        else:
            _ref_center = Navigation.get_reference_mat(self).get_translate()
    
        
        ## map translation input
        _trans_input = 0.0

        if self.nav_mode == 0 or self.nav_mode == 2: # 6DoF navigation mode or 4DoF navigation mode
            _trans_vec = avango.gua.Vec3(X, Y, Z)
            _trans_input = _trans_vec.length()

        elif self.nav_mode == 1: # groundfollowing navigation mode
            _trans_vec = avango.gua.Vec3(X, 0.0, Z)
            _trans_input = _trans_vec.length()

        elif self.nav_mode == 4: # object-based navigation mode
            #_trans_vec = avango.gua.Vec3(_ref_center.x, _ref_center.y, _ref_center.z)
            #_trans_input = Z * -1.0
            _trans_vec = avango.gua.Vec3(X, Y, Z) * -1.0
            _trans_input = _trans_vec.length()
                
        
        if _trans_input != 0.0: # guard
            _trans_vec.normalize()
            _trans_vec *= math.pow(_trans_input, 3) * _fps_scale_factor # transfer function

            _nav_mat = \
                _nav_mat * \
                avango.gua.make_trans_mat(_trans_vec * _nav_scale)

            if self.nav_mode == 4: # object-based mode
                self.override_reference_pos -= _trans_vec # inverse movement of reference point (in platform coordinate system)        

        ## map rotation input
        _rot_input = 0.0

        if self.nav_mode == 0: # 6DoF navigation mode
            _rot_vec = avango.gua.Vec3(RX, RY, RZ)
            _rot_input = _rot_vec.length()

        elif self.nav_mode == 1 or self.nav_mode == 2: # groundfollowing navigation mode or 4DoF navigation mode
            _rot_vec = avango.gua.Vec3(0.0, RY, 0.0)
            _rot_input = _rot_vec.length()

        elif self.nav_mode == 4: # object-based mode            
            _rot_vec = avango.gua.Vec3(RX, RY, RZ) * -1.0            
            _rot_input = _rot_vec.length()

                    
        if _rot_input > 0.0: # guard
            _rot_vec.normalize()
            _rot_vec *= math.pow(_rot_input, 3) * _fps_scale_factor # transfer function

            _nav_mat = \
                _nav_mat * \
                avango.gua.make_trans_mat(_ref_center * _nav_scale) * \
                avango.gua.make_rot_mat(_rot_vec.y, 0, 1, 0) * \
                avango.gua.make_rot_mat(_rot_vec.x, 1, 0, 0) * \
                avango.gua.make_rot_mat(_rot_vec.z, 0, 0, 1) * \
                avango.gua.make_trans_mat(_ref_center * _nav_scale * -1)

        Navigation.set_nav_mat(self, _nav_mat) # apply (new) navigation matrix


    ## Applies a new scaling to this input mapping.
    # @param SCALE The new scaling factor to be applied.
    def map_scale_input(self, SCALE_INPUT):  
        if SCALE_INPUT == 0.0:
            return
          
        #_fps_scale_factor = builtins.APPLICATION.get_application_fps_scale_factor()
        _fps_scale_factor = 1.0
  
        _old_scale = Navigation.get_nav_scale(self)
        _new_scale = _old_scale * (1.0 + SCALE_INPUT * 0.02 * _fps_scale_factor)
        _new_scale = max(min(_new_scale, self.max_scale), self.min_scale) # clamp to min-max range

        
        if self.scale_stop_duration == 0.0:
            Navigation.set_nav_scale(self, _new_scale) # directly apply new scale       
            return
        
        if self.scale_stop_time is not None: # in stop time intervall
            if (time.time() - self.scale_stop_time) > self.scale_stop_duration:
                self.scale_stop_time = None

            else: # in stop time intervall # abort scaling
                return
          
        _old_scale = round(_old_scale, 6)
        _new_scale = round(_new_scale, 6)
                
        # auto pause at dedicated scale levels
        if (_old_scale < 1000.0 and _new_scale > 1000.0) or (_new_scale < 1000.0 and _old_scale > 1000.0):
            #print("snap 1000:1")
            _new_scale = 1000.0
            self.scale_stop_time = time.time()
          
        elif (_old_scale < 100.0 and _new_scale > 100.0) or (_new_scale < 100.0 and _old_scale > 100.0):
            #print("snap 100:1")
            _new_scale = 100.0
            self.scale_stop_time = time.time()
                
        elif (_old_scale < 10.0 and _new_scale > 10.0) or (_new_scale < 10.0 and _old_scale > 10.0):
            #print("snap 10:1")
            _new_scale = 10.0
            self.scale_stop_time = time.time()
        
        elif (_old_scale < 1.0 and _new_scale > 1.0) or (_new_scale < 1.0 and _old_scale > 1.0):
            #print("snap 1:1")
            _new_scale = 1.0
            self.scale_stop_time = time.time()

        elif (_old_scale < 0.1 and _new_scale > 0.1) or (_new_scale < 0.1 and _old_scale > 0.1):
            #print("snap 1:10")
            _new_scale = 0.1
            self.scale_stop_time = time.time()

        elif (_old_scale < 0.01 and _new_scale > 0.01) or (_new_scale < 0.01 and _old_scale > 0.01):
            #print("snap 1:100")
            _new_scale = 0.01
            self.scale_stop_time = time.time()

        elif (_old_scale < 0.001 and _new_scale > 0.001) or (_new_scale < 0.001 and _old_scale > 0.001):
            #print("snap 1:1000")
            _new_scale = 0.001
            self.scale_stop_time = time.time()

        
        # scale relative to a reference point
        if self.nav_mode == 4: # object-based mode
            _ref_center = self.override_reference_pos
        else:
            _ref_center = Navigation.get_reference_mat(self).get_translate()


        _pos1 = _ref_center * _old_scale
        _pos2 = _ref_center * _new_scale

        _vec = _pos1 - _pos2

        _new_mat = \
            Navigation.get_nav_mat(self) * \
            avango.gua.make_trans_mat(_vec)
    
        Navigation.set_nav_mat(self, _new_mat)
        
        Navigation.set_nav_scale(self, _new_scale) # apply new scale


    def map_groundfollowing_input(self):
        _nav_mat = Navigation.get_nav_mat(self)
        _nav_scale = Navigation.get_nav_scale(self)   

        ## calc ground intersection
        _gf_start_pos = Navigation.get_reference_mat(self).get_translate()
        _gf_start_pos.y = self.gf_start_height
        _gf_start_mat = Navigation.get_platform_mat(self) * avango.gua.make_trans_mat(_gf_start_pos)
        _gf_start_mat = avango.gua.make_trans_mat(_gf_start_mat.get_translate()) * avango.gua.make_scale_mat(_gf_start_mat.get_scale())
        
        self.intersection.set_pick_mat(_gf_start_mat)
        self.intersection.compute_intersection()
        
        _pick_result = self.intersection.get_first_pick_result()
        if _pick_result is not None: # intersection found
            #_fps_scale_factor = builtins.APPLICATION.get_application_fps_scale_factor()
            _fps_scale_factor = 1.0
                   
            _distance = _pick_result[5]

            # compare distance to ground and gf_start_height
            _difference = _distance - self.gf_start_height
            _difference = round(_difference, 3)

            #print("difference", _difference)
        
            if _difference < 0: # below ground --> move upwards
                # end falling when necessary
                if self.falling == True:
                    self.falling = False
                    self.gf_fall_velocity = self.gf_initial_fall_velocity 
              
                # move upwards
                _up_vec = avango.gua.Vec3(0.0, _difference * -1.0 * self.gf_compensation_factor * _fps_scale_factor * _nav_scale, 0.0)
                _nav_mat = avango.gua.make_trans_mat(_up_vec) * _nav_mat

                Navigation.set_nav_mat(self, _nav_mat)

            elif _difference > 0: # above ground              
                if _difference > self.gf_start_height: # falling
                    # make player fall down faster every frame
                    self.falling = True
                    self.gf_fall_velocity = min(self.gf_fall_velocity + 0.1 * _fps_scale_factor * _nav_scale, self.gf_max_fall_velocity * _nav_scale)
                                
                    _fall_vec = avango.gua.Vec3(0.0, -(self.gf_fall_velocity / 60.0) * _fps_scale_factor, 0.0)
                    #print(_fall_vec)
                    _nav_mat = avango.gua.make_trans_mat(_fall_vec) * _nav_mat

                    Navigation.set_nav_mat(self, _nav_mat)
                
                else: # climb down                
                    # end falling when necessary
                    if self.falling == True:
                        self.falling = False
                        self.gf_fall_velocity = self.gf_initial_fall_velocity 

                    # move platform downwards
                    _down_vec = avango.gua.Vec3(0.0, _difference * -1.0 * self.gf_compensation_factor * _fps_scale_factor * _nav_scale, 0.0)
                    _nav_mat = avango.gua.make_trans_mat(_down_vec) * _nav_mat
                
                    Navigation.set_nav_mat(self, _nav_mat)

