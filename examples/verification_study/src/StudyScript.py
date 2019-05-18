import avango
import avango.script
import avango.gua
from avango.script import field_has_changed

import random
import math
import src.provenance_utils as pu
import time


class StudyScript(avango.script.Script):

    Button0 = avango.SFBool()
    NextButton = avango.SFBool()
    PrevButton = avango.SFBool()
    IndicatorButton = avango.SFBool()

    def __init__(self):
        self.super(StudyScript).__init__()
        self.always_evaluate(False)

        self.is_initialized = False

    def my_constructor(self, graph):
        self.graph = graph
        self.indicator_transform = avango.gua.nodes.TransformNode(Name='indicator_offset',
                                                                  Transform=avango.gua.make_trans_mat(0.0,0.1,0.0))
        self.indicator = avango.gua.nodes.TriMeshLoader().create_geometry_from_file(
            "error_indicator", "data/objects/cube.obj",
            avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS)
        self.graph.Root.value.Children.value.append(self.indicator_transform)
        self.indicator_id = 0
        self.show_indicator = False

        self.next_pressed = False
        self.prev_pressed = False
        self.indicator_pressed = False

        self.error_indicator_locations = []
        self.read_indicator_locations()

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

        
    @field_has_changed(IndicatorButton)
    def indicator_key_changed(self):
        if self.IndicatorButton.value:
            if self.indicator_pressed == False:
                if self.show_indicator:
                    print('Off')
                    self.show_indicator = False
                    self.indicator_transform.Children.value.remove(self.indicator)
                else:
                    print('On')
                    self.show_indicator = True
                    self.indicator.Transform.value = self.error_indicator_locations[self.indicator_id] *\
                                                     avango.gua.make_scale_mat(0.1,0.1,0.3)
                    self.indicator_transform.Children.value.append(self.indicator)
                self.indicator_pressed = True
            else:
                self.indicator_pressed = False


    @field_has_changed(NextButton)
    def next_key_changed(self):
        if self.NextButton.value:
            if self.next_pressed == False:
                print('id', self.indicator_id)
                if self.indicator_id < len(self.error_indicator_locations) - 1:

                    self.indicator_id += 1
                    print('id ++', self.indicator_id)
                else:
                    self.indicator_id = 0
                    print('id r+', self.indicator_id)
                    
                self.indicator.Transform.value = self.error_indicator_locations[self.indicator_id] *\
                                                     avango.gua.make_scale_mat(0.1,0.1,0.3)
                self.next_pressed = True
            else:
                self.next_pressed = False


    @field_has_changed(PrevButton)
    def prev_key_changed(self):
        if self.PrevButton.value:
            if self.prev_pressed == False:
                print('id', self.indicator_id)
                if self.indicator_id > 0:
                    self.indicator_id -= 1
                    print('id --', self.indicator_id)
                else:
                    self.indicator_id = len(self.error_indicator_locations) - 1
                    print('id r-', self.indicator_id)

                self.indicator.Transform.value = self.error_indicator_locations[self.indicator_id]*\
                                                     avango.gua.make_scale_mat(0.1,0.1,0.3)
                self.prev_pressed = True
            else:
                self.prev_pressed = False
