import avango
import avango.script
import avango.gua
from avango.script import field_has_changed

import random
import math
import src.provenance_utils as pu
import time
import json


class StudyScript(avango.script.Script):

    Button0 = avango.SFBool()
    NextButton = avango.SFBool()
    PrevButton = avango.SFBool()
    IndicatorButton = avango.SFBool()
    StudyStateButton = avango.SFBool()
    StudyStateKeyboardButton = avango.SFBool()

    def __init__(self):
        self.super(StudyScript).__init__()
        self.always_evaluate(False)

        self.is_initialized = False

    def my_constructor(self, graph, user_id, study_meta_path):

        self.graph = graph
        self.study_meta_path = study_meta_path

        self.user_id = user_id
        self.study_group = 'A' if self.user_id % 2 == 0 else "B"
        try:
            with open(study_meta_path, 'r') as f:
                study_dict = json.load(f)
           
        except FileNotFoundError:
            print('File does not exist.')

        self.study_part = study_dict['part']
        self.study_condition = study_dict['condition']
        self.study_geo = study_dict['geometry']
        self.geometry_version = study_dict['version']
        self.geometry_trial = study_dict['trail']
        print('init study', self.study_part, self.study_group, self.study_condition, self.study_geo, self.geometry_version, self.geometry_trial)
        # self.study_feature
        # self.study_feature_value


        self.indicator_transform = avango.gua.nodes.TransformNode(Name='indicator_offset',
                                                                  Transform=avango.gua.make_trans_mat(0.0,0.1,0.0)
                                                                  )
        self.indicator = avango.gua.nodes.TriMeshLoader().create_geometry_from_file(
            "error_indicator", "data/objects/cube.obj",
            avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS)

        self.sign = avango.gua.nodes.TriMeshLoader().create_geometry_from_file(
            "study_sign", "data/objects/plane2.obj",
            avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS)
        self.sign.Transform.value = avango.gua.make_trans_mat(0.0,1.0,0.0) * \
                                    avango.gua.make_rot_mat(90.0,1.0,0.0,0.0) *\
                                    avango.gua.make_rot_mat(-90.0,0.0,1.0,0.0) * avango.gua.make_scale_mat(0.5,1.0,1.0)
        self.sign.Material.value.set_uniform("ColorMap","data/textures/NavigationSign2.png")
        self.sign.Material.value.set_uniform("Emissivity",1.0)
        self.graph.Root.value.Children.value.append(self.indicator_transform)

        self.navigation_sign_path = "data/textures/NavigationSign2.png"
        self.start_sign_path = "data/textures/NavigationSign2.png"

        # states of the study: 0 = START STUDY"
        #                      1 = NAVIGATION STATE: User has to navigate to the marker
        #                      2 = VERIFICATION STATE: Time is calculated
        #                      3 = PAUSE STATE: Stop calculating time
        #                      4 = STUDY IS OVER
        
        self.indicator_id = 0
        self.state = 0
        self.show_indicator = False
        self.screen = None
        self.start_time = None

        self.next_pressed = False
        self.prev_pressed = False
        self.indicator_pressed = False
        self.keyboard_button_pressed = False

        self.error_indicator_locations = []
        self.read_indicator_locations()

        self.indicator.Transform.value = self.error_indicator_locations[self.indicator_id] *\
                                                     avango.gua.make_scale_mat(0.1,0.1,0.3)
        self.indicator_transform.Children.value.append(self.indicator)

    def set_screen(self, screen):
        self.screen = screen
        if self.state == 0:

            self.screen.Children.value.append(self.sign)
        # TODO: APPEND SIGN WITH INSTRUCTION in state 0


    def read_indicator_locations(self):
        location_data = []
        try:
            with open('/home/senu8384/Desktop/master-thesis/data/study/25-locations.lst') as f:
                for line in f:
                    location_data.append(line.replace('\n', '').replace('[', '').replace(']', ''))

                for item in location_data:
                    items = [float(x) for x in item.split(' ')]
                    mat = avango.gua.from_list(items)
                    self.error_indicator_locations.append(mat)    
        except FileNotFoundError:
            print('File does not exist.')

    def change_study_state(self):
        if self.state == 0:
            print('STARTING STUDY WITH MARKER 0')
            self.state = 1

        elif self.state == 1: # stops navigation state -> starts verification state
            self.screen.Children.value.remove(self.sign)
            print('Marker', self.indicator_id, '/', len(self.error_indicator_locations))
            self.indicator_transform.Children.value.remove(self.indicator)
            self.state = 2
            self.start_time = time.time()

        elif self.state == 2: # stops verification state -> starts pause state
            elapsed_time = time.time() - self.start_time
            print('Elapsed Time in sec?',elapsed_time)
            if self.indicator_id < len(self.error_indicator_locations) - 1:
                self.indicator_id += 1
                self.state = 3
                self.screen.Children.value.append(self.sign)
            else:
                print('ID TOO BIG')
                self.state = 4 # STUDY IS OVER

        elif self.state == 3: # stops pause state -> starts navigation state
            print('PAUSE STATE - relax')
            self.state = 1
            self.indicator.Transform.value = self.error_indicator_locations[self.indicator_id] *\
                                                 avango.gua.make_scale_mat(0.1,0.1,0.3)
            self.indicator_transform.Children.value.append(self.indicator)

        elif self.state == 4: # stops pause state -> starts navigation state
            print('Study is over')

    


    @field_has_changed(StudyStateButton)
    def study_state_changed(self):
        print(self.StudyStateButton.value)
        if self.StudyStateButton.value:
            self.change_study_state()

    @field_has_changed(StudyStateKeyboardButton)
    def study_state_changed_by_keyboard(self):
        if self.StudyStateKeyboardButton.value:
            if self.keyboard_button_pressed == False:

                self.change_study_state()
                self.keyboard_button_pressed = True
        else:
            self.keyboard_button_pressed = False