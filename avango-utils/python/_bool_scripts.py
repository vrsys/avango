# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# AVANGO is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# AVANGO is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
##########################################################################

from _nodes import *

import avango
import avango.script

class BoolXBase(avango.script.Script):
    "Base for sequential operations on bools"
    
    BaseFieldName = avango.SFString()
    NumFieldsIn = avango.SFInt() 
    
    Output = avango.SFBool()
    
    def __init__(self):
        self.super(BoolXBase).__init__()  
        
        self.Output.value = False      
        self.BaseFieldName.value = "Input"
        self.NumFieldsIn.value = 0
        
        self._actual_id = 0
    
    def add_and_connect_bool_field(self,field):
        field_name = self.BaseFieldName.value + str(self._actual_id)
        
        if self.has_field(field_name):
            return
        
        #create and add the new field
        self.add_and_init_field(avango.SFBool(), field_name, False)
        
        #connect the field with the given field
        getattr(self, field_name).connect_from(field)
        
        self._actual_id += 1
        self.NumFieldsIn.value = self._actual_id
       
    def evaluate(self):
        self.on_calculate()
        
    def on_calculate(self):
        pass
    
    
class BoolXOr(BoolXBase):
    
    def on_calculate(self):
        result = False
        for field_id in range(0,self._actual_id):
            field_name = self.BaseFieldName.value + str(field_id)
            field = self.get_field(field_name)
            if not field:
                continue
            
            if field.value:
                result = True
                break
         
        self.Output.value = result
        
        
class BoolXAnd(BoolXBase):
    
    def on_calculate(self):
        result = True
        for field_id in range(0,self._actual_id):
            field_name = self.BaseFieldName.value + str(field_id)
            field = self.get_field(field_name)
            if not field:
                continue
            
            if not field.value:
                result = False
                break
         
        self.Output.value = result
        
          
def make_boolX_or(input_list):
    boolX_or = nodes.BoolXOr()
    for input in input_list:
        boolX_or.add_and_connect_bool_field(input)
    return boolX_or.Output

def make_boolX_and(input_list):
    boolX_or = nodes.BoolXAnd()
    for input in input_list:
        boolX_or.add_and_connect_bool_field(input)
    return boolX_or.Output
            
def make_bool2_or(input1,input2):
    bool2_or = nodes.Bool2Or()
    bool2_or.Input1.connect_from(input1)
    bool2_or.Input2.connect_from(input2)
    return bool2_or.Output
            
def make_bool2_and(input1,input2):
    bool2_and = nodes.Bool2And()
    bool2_and.Input1.connect_from(input1)
    bool2_and.Input2.connect_from(input2)
    return bool2_and.Output

def make_bool3_and(input1,input2,input3):
    bool3_and = nodes.Bool3And()
    bool3_and.Input1.connect_from(input1)
    bool3_and.Input2.connect_from(input2)
    bool3_and.Input3.connect_from(input3)
    return bool3_and.Output

def make_bool3_or(input1,input2,input3):
    bool3_and = nodes.Bool3Or()
    bool3_and.Input1.connect_from(input1)
    bool3_and.Input2.connect_from(input2)
    bool3_and.Input3.connect_from(input3)
    return bool3_and.Output