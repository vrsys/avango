#!/usr/bin/python

## @file
# Contains class SpheronNavigation.

### import avango-guacamole libraries
import avango
import avango.gua
import avango.script
from avango.script import field_has_changed

### import framework libraries
from src.Navigation import Navigation


### import python libraries
import math
import time
import builtins
 
      

class SpheronNavigationScript(avango.script.Script):

    ### input fields ###

    ## @var mf_dof
    # The relative input values of the device.
    mf_dof = avango.MFFloat()

    ## @var sf_right_joystick_button
    # Boolean field to indicate if the change of the dof mode is to be triggered.
    sf_right_joystick_button = avango.SFBool()
 
    ## @var sf_left_joystick_button
    # Boolean field to indicate if the change of the dof mode is to be triggered.
    sf_left_joystick_button = avango.SFBool()
 
 
    ### Default constructor.
    def __init__(self):
        self.super(SpheronNavigationScript).__init__()

        self.CLASS = None


    def my_constructor(self, CLASS):
        ### references ###
        self.CLASS = CLASS


    ### callbacks ###
    @field_has_changed(mf_dof)
    def mf_dof_changed(self):
        if self.CLASS is not None:
            self.CLASS.map_input(
                self.mf_dof.value[0],
                self.mf_dof.value[1],
                self.mf_dof.value[2],
                self.mf_dof.value[3],
                self.mf_dof.value[4],
                self.mf_dof.value[5],
                self.mf_dof.value[6],
                self.mf_dof.value[7],
                self.mf_dof.value[8],
                self.mf_dof.value[9],
                self.mf_dof.value[10],
                self.mf_dof.value[11],
                self.mf_dof.value[12],
                self.mf_dof.value[13],
                )            

    ## Evaluated when value changes.
    @field_has_changed(sf_right_joystick_button)
    def sf_right_joystick_button_changed(self):
        if self.CLASS is not None and self.sf_right_joystick_button.value == True: # button pressed
            self.CLASS.toggle_nav_mode()            
 
    ## Evaluated when value changes.
    @field_has_changed(sf_left_joystick_button)
    def sf_left_joystick_button_changed(self):       
        if self.CLASS is not None and self.sf_left_joystick_button.value == True: # button pressed
            self.CLASS.toggle_nav_mode()
            


## Representation of a steering navigation controlled by a 6-DOF device. Creates the device,
# an InputMapping instance and a GroundFollowing instance.
#
# Furthermore, this class reacts on the device's button inputs and toggles the 3-DOF (realistic) / 6-DOF (unrealistic) 
# navigation mode. When switching from unrealistic to realistic mode, an animation is triggered in which the matrix
# is rotated back in an upright position (removal of pitch and roll angle).
class SpheronNavigation(Navigation):

    ### constructor
    def __init__(self,
        MATRIX = avango.gua.make_identity_mat(),
        SCALE = 1.0,
        DEFAULT_REFERENCE_MAT = avango.gua.make_identity_mat(),
        REFERENCE_TRACKING_STATION = None,
        TRANSMITTER_OFFSET = avango.gua.make_identity_mat(),
        RECEIVER_OFFSET = avango.gua.make_identity_mat(),
        INITIAL_MODE = 0, # 6DoF navigation
        ):

        self.nav_mode = 0
         
        # call base class constructor
        Navigation.__init__(self,
            NAME = "Spheron-Navigation",
            MATRIX = MATRIX,
            SCALE = SCALE,
            DEFAULT_REFERENCE_MAT = DEFAULT_REFERENCE_MAT,
            REFERENCE_TRACKING_STATION = REFERENCE_TRACKING_STATION,
            TRANSMITTER_OFFSET = TRANSMITTER_OFFSET,
            RECEIVER_OFFSET = RECEIVER_OFFSET,
            )

    
        ### parameters ###
            
        ## scaling

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


        ### variables ###

        ## @var nav_mode
        # value to indicate if the user is navigation mode: 0 = 6DoF navigation; 1 = ground-based movement (incl. GF); 2 = 4DOF navigation (no roll & pitch); 3 = horizon allignment animation; 4 = object-based
        self.nav_mode = 0

        ## @var scale_stop_time
        # Time at which a scaling process stopped at a fixed step.
        self.scale_stop_time = None

        self.override_reference_pos = avango.gua.Vec3()        


        ## groundfollowing variables
       


        ### resources ###

        ## init script
        self.script = SpheronNavigationScript()
        self.script.my_constructor(self)
    
                  
        ### set initial states ###
        self.reset()
        self.set_nav_mode(INITIAL_MODE)


    ### functions ###

    def assign_input(self, INPUT):
        self.script.mf_dof.disconnect()
        self.script.mf_dof.connect_from(INPUT.get_dofs_field())
        
        if INPUT.get_button3_field() is not None: # right jostick button
            self.script.sf_right_joystick_button.disconnect()
            self.script.sf_right_joystick_button.connect_from(INPUT.get_button3_field())

        if INPUT.get_button4_field() is not None: # left joystick button
            self.script.sf_left_joystick_button.disconnect()
            self.script.sf_left_joystick_button.connect_from(INPUT.get_button4_field())
                  

    def enable_override_reference_position(self, BOOL):
        if BOOL == True:
            self.set_nav_mode(4) # switch to object-based navigation mode
        else:
            self.set_nav_mode(0) # switch to 6DoF navigation mode


    def set_override_reference_position(self, POS):
        _pos = avango.gua.make_inverse_mat(Navigation.get_platform_mat(self)) * POS # reference position (global) in platform coordinate system
        self.override_reference_pos = avango.gua.Vec3(_pos.x, _pos.y, _pos.z) # cast to Vec3
        
   
    def set_nav_mode(self, INT):
        self.nav_mode = INT

 

    def toggle_nav_mode(self):
        return

        if self.nav_mode == 0: # 6DoF navigation mode
            self.set_nav_mode(3) # switch to horizon allignment animation (and then groundfollowing)
        elif self.nav_mode == 1: # groundfollwing navigation mode
            self.set_nav_mode(2) # switch to 4DoF (upright) navigation mode
        elif self.nav_mode == 2: # 4DoF (upright)
            self.set_nav_mode(0) # switch to 6DoF navigation mode



    def map_input(self, X1, Y1, Z1, RX1, RY1, RZ1, T1, X2, Y2, Z2, RX2, RY2, RZ2, T2):
        X,Y,Z,RX,RY,RZ,S = 0.0,0.0,0.0,0.0,0.0,0.0,0.0

        X = X1 + X2
        Y = Y1 + Y2
        Z = Z1 + Z2
        RX = RX1 + RX2
        RY = RY1 + RY2 - T1 - T2
        RZ = RZ1 + RZ2

        if (X1 > 0.0 and X2 < 0.0): # move joysticks towards each other
            S = (abs(X1) + abs(X1)) * -1.0
            X = 0.0
        elif (X1 < 0.0 and X2 > 0.0): # move joysticks apart
            S = abs(X1) + abs(X1)
            X = 0.0


        ## map inputs
        self.map_scale_input(S)
        self.map_movement_input(X, Y, Z, RX, RY, RZ)


    def map_movement_input(self, X, Y, Z, RX, RY, RZ):
        ## get info
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
            _rot_vec *= _fps_scale_factor

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
            _fps_scale_factor = builtins.APPLICATION.get_application_fps_scale_factor()
                   
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


    def init_device_visualization(self, PARENT_NODE = None, TAG_LIST = []): # called by Platform class       
        return
        
        if PARENT_NODE is None:
            return

        self.spheron_geometry = builtins.TRIMESH_LOADER.create_geometry_from_file("spheron_geometry", "data/objects/spheron_centered.obj", avango.gua.LoaderFlags.DEFAULTS)       
        #self.spheron_geometry.Material.value.set_uniform("Color", avango.gua.Vec4(self.color.r, self.color.g, self.color.b, 0.3))
        self.spheron_geometry.Material.value.set_uniform("Color", avango.gua.Vec4(0.1, 0.1, 0.1, 1.0))
        #self.spheron_geometry.Material.value.set_uniform("Emissivity", 1.0)
        self.spheron_geometry.Material.value.set_uniform("Emissivity", 0.4)
        self.spheron_geometry.Material.value.set_uniform("Roughness", 0.5)
        self.spheron_geometry.Material.value.set_uniform("Metalness", 0.2)
        self.spheron_geometry.Material.value.EnableBackfaceCulling.value = False
        self.spheron_geometry.ShadowMode.value = avango.gua.ShadowMode.OFF
        PARENT_NODE.Children.value.append(self.spheron_geometry)
        
        if self.reference_tracking_station is None:
            self.spheron_geometry.Transform.value = Navigation.get_default_reference_matrix(self)
        else:
            self.spheron_geometry.Transform.connect_from(self.reference_tracking_sensor.Matrix) # sensor from base class
        
        for _tag in TAG_LIST:
            self.spheron_geometry.Tags.value.append(_tag)
