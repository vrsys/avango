import avango
import avango.script
import avango.gua
from avango.script import field_has_changed

import random
import datetime
import math
import src.provenance_utils as pu
import time
import json
import os
import csv


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
        
        self.error_indicator_locations = []
        # /home/senu8384/Desktop/master-thesis/avango/examples/q_study/real_head_marker_list.json
        # with open('/home/senu8384/Desktop/master-thesis/avango/examples/q_study/real_wappen_marker_list.json', 'r') as marker_location_f:
        with open('/home/senu8384/Desktop/master-thesis/avango/examples/q_study/real_head_marker_list.json', 'r') as marker_location_f:
        # with open('src/study_files/markers.json', 'r') as marker_location_f:
            locas = json.load(marker_location_f)

            locations = locas['markers']            
            n = len(locas['markers'])
            
            for i in range(n):
                # print(' \n')
                marker = locations[i]
                marker_matrix = avango.gua.from_list(marker['mat'])
                self.error_indicator_locations.append(marker_matrix)
            print('Loaded',len(self.error_indicator_locations),'markers')
        study_dict = None
        with open(study_meta_path, 'r') as f:
            study_dict = json.load(f)


        self.study_part = study_dict['part']
        self.study_condition = study_dict['condition']
        self.study_geo = study_dict['geometry']
        self.geometry_version = study_dict['version']
        self.geometry_trail = study_dict['trail']
        print('init study part: ', self.study_part, self.study_group, self.study_condition, self.study_geo, self.geometry_version, self.geometry_trail)

        self.indicator_transform = avango.gua.nodes.TransformNode(Name='indicator_offset',
                                                                  Transform=avango.gua.make_trans_mat(0.0,0.0,0.0))
        self.marker_mat = avango.gua.nodes.Material()
        marker_color = avango.gua.Vec4(1.0, 0.1, 0.1, 0.2)
        # sphere_color.normalize()
        self.marker_mat.set_uniform("Color", marker_color)
        self.marker_mat.set_uniform("Roughness", 1.0)
        self.marker_mat.set_uniform("Emissivity", 1.0)
        self.marker_mat.EnableBackfaceCulling.value = False

        self.indicator = avango.gua.nodes.TriMeshLoader().create_geometry_from_file(
            "error_indicator", "data/objects/marker.obj", self.marker_mat,
            avango.gua.LoaderFlags.DEFAULTS)

        self.sign = avango.gua.nodes.TriMeshLoader().create_geometry_from_file(
            "study_sign", "data/objects/plane2.obj",
            avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS)
        self.sign.Transform.value = avango.gua.make_trans_mat(1.5,1.0,0.0) * \
                                    avango.gua.make_rot_mat(90.0,1.0,0.0,0.0) *\
                                    avango.gua.make_rot_mat(-90.0,0.0,1.0,0.0) * avango.gua.make_scale_mat(0.25,1.0,0.5)
        self.sign.Material.value.set_uniform("ColorMap","data/textures/NavigationSign2.png")
        self.sign.Material.value.set_uniform("Emissivity",1.0)
        self.graph.Root.value.Children.value.append(self.indicator_transform)

        self.navigation_sign_path = "data/textures/NavigationSign2.png"
        self.start_sign_path = "data/textures/StartSign2.png"

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
        self.times = []

        self.next_pressed = False
        self.prev_pressed = False
        self.indicator_pressed = False
        self.keyboard_button_pressed = False

        # self.error_indicator_locations = []
        # self.read_indicator_locations()

        self.indicator.Transform.value = self.error_indicator_locations[self.indicator_id] *  avango.gua.make_trans_mat(0.0,-0.0,0.00)*\
                                         avango.gua.make_scale_mat(0.1,0.1,0.1) * avango.gua.make_rot_mat(90.0,1.0,0.0,0.0)
        self.indicator_transform.Children.value.append(self.indicator)

    def write_csv_file(self):
        folder_name = 'src/study_files/user_data_' + str(self.user_id)
        folder_ok = False
        try:
            # Create target Directory
            os.mkdir(folder_name)
            print("Directory " , folder_name ,  " Created ") 
            folder_ok = True
        except FileExistsError:
            print("Directory " , folder_name ,  " already exists")
            folder_ok = True

        if folder_ok:
            file_name = folder_name+'/part_' + str(self.study_part) + '_' + self.study_geo + '_v' + str(self.geometry_version) + '_trail_' + str(self.geometry_trail) + '.csv'
            with open(file_name, 'w+') as csvfile:
                date=datetime.datetime.now()
            # with open(file_name, 'w+', newline='') as csvfile:  
            # with open('names.csv', 'w', newline='') as csvfile:
                fieldnames = ['date','user_id', 'part', 'group', 'geometry', 'version', 'trail', 'condition', 'feature', 'feature_value', 'time']
                writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

                writer.writeheader()
                for idx, t in enumerate(self.times):
                    writer.writerow( {'date':date,'user_id':self.user_id, 'part':self.study_part, 'group':self.study_group, 'geometry':self.study_geo,
                                      'version':self.geometry_version, 'trail':self.geometry_trail, 'condition':self.study_condition, 'feature':idx, 'feature_value':'True', 'time':self.times[idx]})
                    

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
            
        elif self.state == 1: # stops pause state -> starts navigation state
            print('PAUSE STATE - relax')
            self.state = 2
            self.sign.Material.value.set_uniform("ColorMap", self.start_sign_path)
            

        elif self.state == 2: # stops navigation state -> starts verification state
            self.screen.Children.value.remove(self.sign)
            print('Marker', self.indicator_id, '/', len(self.error_indicator_locations))
            self.indicator_transform.Children.value.remove(self.indicator)
            self.state = 3
            self.start_time = time.time()

        elif self.state == 3: # stops verification state -> starts pause state
            elapsed_time = time.time() - self.start_time
            self.times.append(elapsed_time)
            print('Elapsed Time in sec?',elapsed_time)
            if self.indicator_id < len(self.error_indicator_locations) - 1:
                self.indicator_id += 1
                self.state = 1
                self.screen.Children.value.append(self.sign)
                self.sign.Material.value.set_uniform("ColorMap","data/textures/NavigationSign2.png")
                self.indicator.Transform.value = self.error_indicator_locations[self.indicator_id] *  avango.gua.make_trans_mat(0.0,-0.0,0.00)*\
                                         avango.gua.make_scale_mat(0.1,0.1,0.1) * avango.gua.make_rot_mat(90.0,1.0,0.0,0.0)
                self.indicator_transform.Children.value.append(self.indicator)
            else:
                print('ID TOO BIG')
                self.state = 4 # STUDY IS OVER

        elif self.state == 4: # stops pause state -> starts navigation state
            print('Study is over')
            self.write_csv_file()

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