import avango
import avango.script
import avango.gua
from avango.script import field_has_changed

import random
import datetime
# from datetime import datetime
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
    PhotoButton = avango.SFBool()
    IndicatorButton = avango.SFBool()
    StudyStateButton = avango.SFBool()
    StudyStateKeyboardButton = avango.SFBool()
    StudyStateCorrectButton = avango.SFBool()
    StudyStateFalseButton = avango.SFBool()

    def __init__(self):
        self.super(StudyScript).__init__()
        self.always_evaluate(False)

        self.is_initialized = False

    def my_constructor(self, graph, user_id, study_task, study_part, study_condition, study_geo, study_geo_version, marker_path, wall_perspectives_path, feature_values_path, study_user_two):

        self.graph = graph
        self.user_id = user_id
        self.study_user_two = study_user_two
        self.study_task = study_task
        self.study_part = study_part
        self.study_condition = study_condition
        self.study_geo = study_geo
        self.study_geo_version = study_geo_version 
        self.marker_path = marker_path
        self.wall_perspectives_path = wall_perspectives_path
        self.feature_values_path = feature_values_path
        self.study_start_time = datetime.datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
        self.study_group = 'A' if self.user_id % 2 == 0 else "B"
        self.study_block = self.study_part % 3
        
        self.error_indicator_locations = []
        self.wall_perspectives = []
        self.wall_centers = []
        self.feature_values = []

        with open(self.marker_path, 'r') as marker_location_f:
            locas = json.load(marker_location_f)

            locations = locas['markers']            
            n = len(locas['markers'])
            
            for i in range(n):
                # print(' \n')
                marker = locations[i]
                marker_matrix = avango.gua.from_list(marker['mat'])
                self.error_indicator_locations.append(marker_matrix)
            print('Loaded',len(self.error_indicator_locations),'markers')

        if self.wall_perspectives_path:
            with open(self.wall_perspectives_path, 'r') as wall_perspectives_f:
                persps = json.load(wall_perspectives_f)
                perspectives = persps['perspectives']            
                n = len(persps['perspectives'])
                print('length of perspectives', n)
                for i in range(n):
                    pers = perspectives[i]
                    
                    _perspective_ids_per_feature = []
                    _centers_per_feature = []
                    for i, idx in enumerate(pers['perspective_ids']):
                        _perspective_ids_per_feature.append(idx)
                    for k, center in enumerate(pers['center_uvs']):
                        _centers_per_feature.append(avango.gua.Vec2(center['u'], center['v']) )

                    self.wall_perspectives.append(_perspective_ids_per_feature)
                    self.wall_centers.append(_centers_per_feature)
        with open(self.feature_values_path, 'r') as truths:
            print(truths)
            vals = json.load(truths)
            if vals['geo'] == self.study_geo:
                print('GEO CORRECT')
            if vals['version'] == self.study_geo_version:
                print('VERSION CORRECT')
            for v in vals['feature_values']:
                self.feature_values.append(v)
            print('LENGTH', len(self.feature_values), self.feature_values)
                    
        
        print('init study part: ', self.study_part, self.study_group, self.study_condition, self.study_geo, self.study_geo_version)

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
        self.sign.Material.value.set_uniform("ColorMap","data/textures/StartStudy.png")
        self.sign.Material.value.set_uniform("Emissivity",1.0)
        self.graph.Root.value.Children.value.append(self.indicator_transform)

        self.navigation_sign_path = "data/textures/NavigationSign2.png"
        self.start_sign_path = "data/textures/StartSign2.png"
        self.start_study_sign_path = "data/textures/StartStudy.png"

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
        self.time_stamps = []
        self.user_answers = []
        self.locks = []
        self.perspective_picker = None
        self.navigation = None
        self.wall = None
        self.question_flag = False
        self.viewing_setup = None

        self.image_updater = None
        self.perspectives_per_feature = []
        self.perspectives_coords = []

        self.next_pressed = False
        self.prev_pressed = False
        self.photo_button_pressed = False
        self.indicator_pressed = False
        self.keyboard_button_pressed = False
        self.correct_button_pressed = False
        self.false_button_pressed = False
        self.answer = None

        self.indicator.Transform.value = self.error_indicator_locations[self.indicator_id] *  avango.gua.make_trans_mat(0.0,-0.0,0.00)*\
                                         avango.gua.make_scale_mat(0.1,0.1,0.1) * avango.gua.make_rot_mat(90.0,1.0,0.0,0.0)
        self.indicator_transform.Children.value.append(self.indicator)

    def set_picker_and_navigation(self, perspective_picker, navigation):
        # function to set picker and navigation so that we can get timings from the picker e.g how long did search for a perspective
        # needs to be set in order to disable navigation or trigger some stuff
        self.perspective_picker = perspective_picker
        self.navigation = navigation

    def set_wall(self, wall_viz):
        self.wall = wall_viz


    def write_csv_file(self):
        
        folder_name = 'src/study_files/user_data_' + str(self.user_id) #+ folder_fix
        if self.study_task == 'real':
            folder_name = folder_name + '_' + str(self.study_user_two) + '_real'
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
            # print(file_fix)
            part = str(self.study_part)
            task = str(self.study_task)
            vers = str(self.study_geo_version)
            file_name = folder_name + '/part_' + part + '_task_' + task + '_' + self.study_geo + '_v' + vers + '_' + self.study_start_time + '.csv'
            # file_name = folder_name + '/part_' + part + '_task_' + task + '_' + self.study_geo + '_v' + vers +'.csv'
            with open(file_name, 'w+') as csvfile:
                date=datetime.datetime.now()
                self.time_stamps.append(date)
                print('time stamps length ',len(self.time_stamps))
            # with open(file_name, 'w+', newline='') as csvfile:  
            # with open('names.csv', 'w', newline='') as csvfile:
                fieldnames = ['date','user_id', 'task', 'part', 'group', 'block', 'geometry', 'version', 'condition', 'trail', 'feature_value', 'time', 'frozen', 'answer', 'user_two_id']

                writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
                user_two_id = -1 if self.study_user_two is None else self.study_user_two
                writer.writeheader()
                for idx, t in enumerate(self.times):
                    writer.writerow( {'date':self.time_stamps[idx],
                                      'user_id':self.user_id,
                                      'task':self.study_task,
                                      'part':self.study_part,
                                      'group':self.study_group,
                                      'block':self.study_block,
                                      'geometry':self.study_geo,
                                      'version':self.study_geo_version,
                                      'condition':self.study_condition,
                                      'trail':idx,
                                      'feature_value':self.feature_values[idx],
                                      'time':self.times[idx],
                                      'frozen': self.locks[idx],
                                      'answer': self.user_answers[idx],
                                      'user_two_id': user_two_id
                                      })
                    

    def write_perspectives(self):
        file_name = 'wall_perspectives_' + self.study_geo + '_task_' + self.study_task + '.json'
        # print('!!!!!!!!!!!!!!!!!!!!!!!',self.perspectives_coords[0][0][0])
        with open(file_name, 'w') as jsonfile:  
                json_data = {}
                json_data['perspectives'] = [] 
                for idx, per_list in enumerate(self.perspectives_per_feature):
                    uv_coords = []
                    for k, item in enumerate(self.perspectives_coords[idx]):
                        uv_coords.append({
                            'c_id':k,
                            'u':item.x,
                            'v':item.y,
                            })
                    json_data['perspectives'].append({
                        'feature_id': idx,
                        'perspective_ids': per_list,
                        'center_uvs': uv_coords
                    })
                json.dump(json_data, jsonfile)
        
    def set_screen(self, screen):
        self.screen = screen
        if self.state == 0:

            self.screen.Children.value.append(self.sign)
        # TODO: APPEND SIGN WITH INSTRUCTION in state 0

    def set_image_updater(self, updater):
        self.image_updater = updater

    def change_study_state(self, answer=2):
        if self.state == 0:
            if self.perspective_picker == None:
                print("SET PICKER")

            self.sign.Material.value.set_uniform("ColorMap", self.navigation_sign_path)
            self.state = 1
            
        elif self.state == 1: # stops pause state -> starts navigation state
            print('PAUSE STATE - relax')
            self.state = 2
            self.sign.Material.value.set_uniform("ColorMap", self.start_sign_path)

        elif self.state == 2: # stops navigation state -> starts verification state
            self.screen.Children.value.remove(self.sign)
            print('Marker', self.indicator_id, '/', len(self.error_indicator_locations))
            
            self.state = 3
            self.start_time = time.time()
            if self.study_condition == 'lense':
                self.perspective_picker.turn_on()
                self.indicator_transform.Children.value.remove(self.indicator)

            elif self.study_condition == 'wall':
                print('wall')
                self.wall.show(True)
                self.update_textures(self.indicator_id)
            elif self.study_condition == 'demo':
                print('demo')
                self.perspective_picker.turn_on()
                self.wall.show(True)
                self.update_textures(self.indicator_id)

        elif self.state == 3: # stops verification state -> starts pause state
            elapsed_time = time.time() - self.start_time
            
            self.times.append(elapsed_time)
            self.locks.append(self.perspective_picker.frozen)
            # self.perspectives_per_feature.append(self.image_updater.last_relevants)
            # self.perspectives_coords.append(self.image_updater.last_centers)
            # print(self.perspectives_coords)
            # print(self.perspectives_per_feature)
            # self.write_perspectives()
            # self.search_times.append(perspective_search_time)
            print('Elapsed Time in sec?',elapsed_time)
            self.wall.show(False)

            self.state = 4
            self.screen.Children.value.append(self.sign)
                
            self.sign.Material.value.set_uniform("ColorMap","/home/senu8384/Desktop/master-thesis/data/CorrectQuestion.png")
            
                
            self.perspective_picker.turn_off()
                

        elif self.state == 4:

            if self.answer is not None:
                if self.answer == True:
                    if self.indicator_id < len(self.error_indicator_locations) - 1:
                        self.indicator_id += 1
                        self.user_answers.append(self.answer)
                        self.state = 1
                        if self.study_task == 'demo':
                            print('dont save demo')
                        else:
                            self.write_csv_file()
                        self.sign.Material.value.set_uniform("ColorMap","data/textures/NavigationSign2.png")
                        self.answer = None
                        self.indicator_transform.Children.value.remove(self.indicator)
                        self.indicator.Transform.value = self.error_indicator_locations[self.indicator_id] *  avango.gua.make_trans_mat(0.0,-0.0,0.00)*\
                                                 avango.gua.make_scale_mat(0.1,0.1,0.1) * avango.gua.make_rot_mat(90.0,1.0,0.0,0.0)
                        self.indicator_transform.Children.value.append(self.indicator)
                    else:
                        print('ID TOO BIG')
                        self.user_answers.append(self.answer)
                        if self.study_task == 'demo':
                            self.indicator_id = 0
                            self.state = 1
                        else:
                            self.state = 5 # STUDY IS OVER
                        

                elif self.answer == False:
                    if self.indicator_id < len(self.error_indicator_locations) - 1:
                        self.indicator_id += 1
                        self.user_answers.append(self.answer)
                        print(len(self.user_answers))
                        
                        self.state = 1
                        if self.study_task == 'demo':
                            print('dont save demo')    
                        else:
                            self.write_csv_file()
                        self.sign.Material.value.set_uniform("ColorMap","data/textures/NavigationSign2.png")
                        self.answer = None
                        self.indicator_transform.Children.value.remove(self.indicator)
                        self.indicator.Transform.value = self.error_indicator_locations[self.indicator_id] *  avango.gua.make_trans_mat(0.0,-0.0,0.00)*\
                                                 avango.gua.make_scale_mat(0.1,0.1,0.1) * avango.gua.make_rot_mat(90.0,1.0,0.0,0.0)
                        self.indicator_transform.Children.value.append(self.indicator)
                    else:
                        print('ID TOO BIG')
                        self.user_answers.append(self.answer)
                        if self.study_task == 'demo':
                            self.indicator_id = 0
                            self.state = 1
                        else:
                            self.state = 5 # STUDY IS OVER
            else:
                print('Give answer.')
            


        elif self.state == 5: # stops pause state -> starts navigation state
            print('Study is over')
            if self.answer != None:
                self.write_csv_file()
            self.answer = None
            self.sign.Material.value.set_uniform("ColorMap",self.start_study_sign_path)
    def set_viewing_setup(self, viewing_setup):
        self.viewing_setup = viewing_setup

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
            self.change_study_state()

    @field_has_changed(StudyStateKeyboardButton)
    def study_state_changed_by_keyboard(self):
        if self.StudyStateKeyboardButton.value:
            if self.keyboard_button_pressed == False:

                self.change_study_state()
                self.keyboard_button_pressed = True
        else:
            self.keyboard_button_pressed = False

    @field_has_changed(StudyStateCorrectButton)
    def study_state_changed_by_correct_button(self):
        if self.StudyStateCorrectButton.value:
            if self.correct_button_pressed == False:
                if self.state == 4:
                    self.answer = True
                    self.sign.Material.value.set_uniform("ColorMap","/home/senu8384/Desktop/master-thesis/data/CorrectSign.png")
                self.correct_button_pressed = True
        else:
            self.correct_button_pressed = False

    @field_has_changed(StudyStateFalseButton)
    def study_state_changed_by_false_button(self):
        if self.StudyStateFalseButton.value:
            if self.false_button_pressed == False:
                if self.state == 4:
                    self.answer = False
                    self.sign.Material.value.set_uniform("ColorMap","/home/senu8384/Desktop/master-thesis/data/WrongSign.png")
                self.false_button_pressed = True
        else:
            self.false_button_pressed = False

    @field_has_changed(PhotoButton)
    def study_photo_button_changed(self):
        if self.PhotoButton.value:
            if self.photo_button_pressed == False:
                if self.state == 4:
                    self.answer = False
                    self.sign.Material.value.set_uniform("ColorMap","/home/senu8384/Desktop/master-thesis/data/WrongSign.png")
                self.photo_button_pressed = True
        else:
            self.false_button_pressed = False
            self.photo_button_pressed