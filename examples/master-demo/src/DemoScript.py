import avango
import avango.script
import avango.gua
from avango.script import field_has_changed

import random
import math
import src.provenance_utils as pu
import time
import os



class DemoScript(avango.script.Script):

    Button0 = avango.SFBool()
    Button1 = avango.SFBool()
    Button2 = avango.SFBool()
    NextButton = avango.SFBool()
    PrevButton = avango.SFBool()
    PhotoButton = avango.SFBool()
    IndicatorButton = avango.SFBool()
    StudyStateButton = avango.SFBool()
    StudyStateKeyboardButton = avango.SFBool()
    StudyStateCorrectButton = avango.SFBool()
    StudyStateFalseButton = avango.SFBool()

    def __init__(self):
        self.super(DemoScript).__init__()
        self.always_evaluate(False)

        self.is_initialized = False

    def my_constructor(self, plod_node, atlas_tiles_node):

        # self.wall_perspectives = []
        # self.wall_centers = []

        # if self.wall_perspectives_path:
        #     with open(self.wall_perspectives_path, 'r') as wall_perspectives_f:
                
        #             for i, idx in enumerate(pers['perspective_ids']):
        #                 _perspective_ids_per_feature.append(idx)
        #             for k, center in enumerate(pers['center_uvs']):
        #                 _centers_per_feature.append(avango.gua.Vec2(center['u'], center['v']) )

        #             self.wall_perspectives.append(_perspective_ids_per_feature)
        #             self.wall_centers.append(_centers_per_feature)

        # self.indicator_transform = avango.gua.nodes.TransformNode(Name='indicator_offset',
        #                                                           Transform=avango.gua.make_trans_mat(0.0,0.0,0.0))
        # self.marker_mat = avango.gua.nodes.Material()
        # marker_color = avango.gua.Vec4(1.0, 0.1, 0.1, 0.2)
        # # sphere_color.normalize()
        # self.marker_mat.set_uniform("Color", marker_color)
        # self.marker_mat.set_uniform("Roughness", 1.0)
        # self.marker_mat.set_uniform("Emissivity", 1.0)
        # self.marker_mat.EnableBackfaceCulling.value = False

        # self.indicator = avango.gua.nodes.TriMeshLoader().create_geometry_from_file(
        #     "error_indicator", "data/objects/marker.obj", self.marker_mat,
        #     avango.gua.LoaderFlags.DEFAULTS)

        # states of the study: 0 = START STUDY"
        #                      1 = NAVIGATION STATE: User has to navigate to the marker
        #                      2 = VERIFICATION STATE: Time is calculated
        #                      3 = PAUSE STATE: Stop calculating time
        #                      4 = STUDY IS OVER
        self.plod_node = plod_node
        self.atlas_tiles_node = atlas_tiles_node
        self.show_images = True
        self.indicator_id = 0
        self.state = 0
        self.screen = None
        self.perspective_picker = None
        self.navigation = None
        self.wall = None
        self.viewing_setup = None

        self.image_updater = None
        self.perspectives_per_feature = []
        self.perspectives_coords = []

        self.next_pressed = False
        self.button0_pressed = False
        self.button1_pressed = False
        self.button2_pressed = False
        self.prev_pressed = False
        self.photo_button_pressed = False
        self.indicator_pressed = False
        self.keyboard_button_pressed = False
        self.correct_button_pressed = False
        self.false_button_pressed = False
        self.answer = None

        # self.indicator.Transform.value = self.error_indicator_locations[self.indicator_id] *  avango.gua.make_trans_mat(0.0,-0.0,0.00)*\
        #                                  avango.gua.make_scale_mat(0.1,0.1,0.1) * avango.gua.make_rot_mat(90.0,1.0,0.0,0.0)
        # self.indicator_transform.Children.value.append(self.indicator)

    def set_picker_and_navigation(self, perspective_picker, navigation):
        # function to set picker and navigation so that we can get timings from the picker e.g how long did search for a perspective
        # needs to be set in order to disable navigation or trigger some stuff
        self.perspective_picker = perspective_picker
        # self.perspective_picker.turn_on()
        self.navigation = navigation

    def set_wall(self, wall_viz):
        # Set wall to visualize images at the tiled display wall
        self.wall = wall_viz

    def set_screen(self, screen):
        # Set screen to append HUD infos
        self.screen = screen
        # if self.state == 0:

            # self.screen.Children.value.append(self.sign)

    def set_image_updater(self, updater):
        self.image_updater = updater

    def set_viewing_setup(self, viewing_setup):
        self.viewing_setup = viewing_setup

    def demo_state(self, state):
        if state==0:
            if self.perspective_picker == None:
                print('perspective_picker is None')
            else:
                self.perspective_picker.turn_on()

        # elif state==1:
            # self.perspective_picker

    def update_textures(self, feature_id):

        if self.wall:
            print(self.wall_perspectives[feature_id][0:8])
            print(self.wall_centers[feature_id][0:8])

            zoom_factor = 3
            wall_ratio1 = 4.07 / 2.3
            wall_ratio1 = 2.035 / 1.15
            wall_ratio2 = 2.3 / 4.07
            width = 0.033215967814127605
            texture_coordinates = []
            for vec in self.wall_centers[feature_id]:
                max_uv = avango.gua.Vec2(vec.x - (width * wall_ratio1/ 2 / zoom_factor), vec.y - (width/ 2 / zoom_factor))
                # min_uv = avango.gua.Vec2(u_coord - (_img.tile_w / 2 / zoom_factor) , v_coord - (_img.tile_h / 2 / zoom_factor))
                min_uv = avango.gua.Vec2(vec.x + (width * wall_ratio1/ 2 / zoom_factor), vec.y + (width / 2 / zoom_factor))
                # max_uv = avango.gua.Vec2(u_coord + (_img.tile_w / 2 / zoom_factor) , v_coord + (_img.tile_h / 2 / zoom_factor))
                min_max_coords = [min_uv, max_uv]
                texture_coordinates.append(min_max_coords)
            # self.wall_perspectives = []
            # self.wall_centers = []
            # self.last_relevants = self.wall_perspectives[0:8]
            # self.last_centers = self.wall_centers[0:8]
            # self.visualizer.update_images(self.relevant_perspectives[0:self.visualizer.views], 
                                        # self.texture_coordinates[0:self.visualizer.views])
            self.wall.update_images(self.wall_perspectives[feature_id][0:self.wall.views], 
                                    texture_coordinates[0:self.wall.views])
        else:
            print('No texture visualizer is set.')

    @field_has_changed(StudyStateButton)
    def study_state_changed(self):
        print(self.StudyStateButton.value)
        if self.StudyStateButton.value:
            print('TASTE')
            # if self.show_images:
            #     self.plod_node.Tags = ['client']
            #     self.show_images = False
            # elif self.show_images == False:
            #     self.plod_node.Tags = ['client']
            #     self.show_images = True


            # self.change_study_state()

    @field_has_changed(StudyStateKeyboardButton)
    def study_state_changed_by_keyboard(self):
        if self.StudyStateKeyboardButton.value:
            if self.keyboard_button_pressed == False:
                print('Space pressed - toggle images')

                if self.show_images:
                    self.atlas_tiles_node.Tags.value = ['client']
                    self.show_images = False
                elif self.show_images == False:
                    self.atlas_tiles_node.Tags.value = []
                    self.show_images = True
                self.keyboard_button_pressed = True
        else:
            self.keyboard_button_pressed = False

    @field_has_changed(StudyStateCorrectButton)
    def study_state_changed_by_correct_button(self):
        if self.StudyStateCorrectButton.value:
            if self.correct_button_pressed == False:
                if self.state == 4:
                   print('correct button')
                   
                self.correct_button_pressed = True
        else:
            self.correct_button_pressed = False

    @field_has_changed(StudyStateFalseButton)
    def study_state_changed_by_false_button(self):
        if self.StudyStateFalseButton.value:
            if self.false_button_pressed == False:
                if self.state == 4:
                    self.answer = False
                    print('false button')
                self.false_button_pressed = True
        else:
            self.false_button_pressed = False

    @field_has_changed(PhotoButton)
    def study_photo_button_changed(self):
        if self.PhotoButton.value:
            if self.photo_button_pressed == False:
                if self.state == 4:
                    self.answer = False
                    print('kp photo button')
                self.photo_button_pressed = True
        else:
            self.false_button_pressed = False
            self.photo_button_pressed

    @field_has_changed(Button0)
    def study_button0_changed(self):
        if self.Button0.value:
            if self.button0_pressed == False:    
                print('button 0')
                self.always_evaluate(True)
                
                self.button0_pressed = True
        else:
            self.button0_pressed = False
            print('release')
            self.always_evaluate(False)


    @field_has_changed(Button1)
    def study_button1_changed(self):
        if self.Button1.value:
            if self.button1_pressed == False:    
                print('button 1')
                self.button1_pressed = True
        else:
            self.button1_pressed = False
            print('release')

    @field_has_changed(Button2)
    def study_button2_changed(self):
        if self.Button2.value:
            if self.button2_pressed == False:    
                print('button 2')
                self.button2_pressed = True
        else:
            self.button2_pressed = False
            print('release')

    # def evaluate(self):
    #     if self.button0_pressed:
    #         print('test')