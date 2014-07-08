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

import avango
import avango.script
from avango.script import field_has_changed
import avango.gua

class FloatXBase(avango.script.Script):
    "Base for sequential operations on floats"

    BaseFieldName = avango.SFString()
    NumFieldsOut = avango.SFInt()

    Output = avango.SFFloat()

    def __init__(self):
        self.super(FloatXBase).__init__()
        
        self.Name.value = "FloatXBase"

        self.Output.value = 0
        self.BaseFieldName.value = "Input"
        self.NumFieldsOut.value = 0

        self._actual_id = 0

    def add_and_connect_float_field(self,field):

        field_name = self.BaseFieldName.value + str(self._actual_id)
        if self.has_field(field_name):
            return
        #create and add the new field
        self.add_and_init_field(avango.SFFloat(), field_name, 0)

        #connect the field with the given field
        getattr(self, field_name).connect_from(field)

        self._actual_id += 1
        self.NumFieldsOut.value = self._actual_id

    def evaluate(self):
        self.on_calculate()

    def on_calculate(self):
        pass

class FloatXSum(FloatXBase):
    
    def __init__(self):
        self.super(FloatXSum).__init__()
        
        self.Name.value = "FloatXSum"

    def on_calculate(self):
        sum = 0
        for field_id in range(0,self._actual_id):
            field_name = self.BaseFieldName.value + str(field_id)
            field = self.get_field(field_name)
            if not field:
                continue
            sum += field.value

        self.Output.value = sum

class FloatXMin(FloatXBase):
    
    def __init__(self):
        self.super(FloatXMin).__init__()
        
        self.Name.value = "FloatXMin"

    def on_calculate(self):
        min = 1e+100 # TODO: Find a way to get the real maximum
        for field_id in range(0,self._actual_id):
            field_name = self.BaseFieldName.value + str(field_id)
            field = self.get_field(field_name)
            if not field:
                continue
            if field.value < min:
                min = field.value

        self.Output.value = min


class Float2Add(avango.script.Script):
    "Adds two float values"

    Value0 = avango.SFFloat()
    Value1 = avango.SFFloat()

    Output = avango.SFFloat()
    
    def __init__(self):
        self.super(Float2Add).__init__()
        
        self.Name.value = "Float2Add"

    def evaluate(self):
        self.Output.value = self.Value0.value + self.Value1.value

class Float4AddVec2Converter(avango.script.Script):
    "Converts four Floats into a Vec2, where vec.x = Value00 + Value01 and vec.y = Value10 + Value11"

    Value00 = avango.SFFloat()
    Value01 = avango.SFFloat()

    Value10 = avango.SFFloat()
    Value11 = avango.SFFloat()

    Output = avango.gua.SFVec2()
    
    def __init__(self):
        self.super(Float4AddVec2Converter).__init__()
        
        self.Name.value = "Float4AddVec2Converter"

    def evaluate(self):
        self.Output.value = avango.gua.Vec2(self.Value00.value+self.Value01.value, self.Value10.value+self.Value11.value)


class SFNode2MFContainerConverter(avango.script.Script):
    "Converts a SFNode to a MFNode"

    Input = avango.gua.SFNode()
    Output = avango.MFContainer()
    
    def __init__(self):
        self.super(SFNode2MFContainerConverter).__init__()
        
        self.Name.value = "SFNode2MFContainerConverter"


    def evaluate(self):
        self.Output.value = [ self.Input.value ]


class Float2Vec2Converter(avango.script.Script):
    "Converts two Floats into on Vec2"

    Value0 = avango.SFFloat()
    Value1 = avango.SFFloat()
    Output = avango.gua.SFVec2()
    
    def __init__(self):
        self.super(Float2Vec2Converter).__init__()
        
        self.Name.value = "Float2Vec2Converter"
        

    def evaluate(self):
        self.Output.value = avango.gua.Vec2(self.Value0.value, self.Value1.value)
        
        
class Vec3ToTransMatrix(avango.script.Script):
    TransVec = avango.gua.SFVec3()
    TransOffset = avango.gua.SFVec3()
    Matrix = avango.gua.SFMatrix4()
    
    def __init__(self):
        self.super(Vec3ToTransMatrix).__init__()
        
        self.Name.value = "Vec3ToTransMatrix"
    
    
    def evaluate(self):
        self.Matrix.value = avango.gua.make_trans_mat(self.TransVec.value + self.TransOffset.value)
        
def make_vec3_to_trans_matrix(vec3_field, trans_offset=avango.gua.Vec3(0,0,0)):
    converter = Vec3ToTransMatrix()
    converter.TransVec.connect_from(vec3_field)
    converter.TransOffset.value = trans_offset
    
    return converter.Matrix


class TranslationMatrixCalculator(avango.script.Script):
    MatrixFrom = avango.gua.SFMatrix4()
    MatrixTo = avango.gua.SFMatrix4()
    MatrixTransDif = avango.gua.SFMatrix4()
    
    def __init__(self):
        self.super(TranslationMatrixCalculator).__init__()
        
        self.Name.value = "TranslationMatrixCalculator"
    
    
    def evaluate(self):
        if not self.MatrixFrom.value or not self.MatrixTo.value:
            return
        self.MatrixTransDif.value = avango.gua.make_trans_mat( self.MatrixFrom.value.get_translate() - self.MatrixTo.value.get_translate() )
        

class FloatToAlphaConverter(avango.script.Script):
    ColorIn = avango.gua.SFVec4()
    Alpha = avango.SFFloat()
    Color = avango.gua.SFVec4()
    
    def __init__(self):
        self.super(FloatToAlphaConverter).__init__()
        self.__alpha_changed = False
        self.__color_in = avango.gua.Vec4(1,1,1,1)
        self.Name.value = "FloatToAlphaConverter"
        
    @field_has_changed(ColorIn)
    def color_in_changed(self):
        self.__color_in = self.ColorIn.value
    
    @field_has_changed(Alpha)
    def alpha_changed(self):
        self.__alpha_changed = True
        
    def evaluate(self):
        print "eval: " + str(self.Alpha.value)
        if self.__alpha_changed:
            
            self.Color.value = avango.gua.Vec4(self.__color_in.x, self.__color_in.y, self.__color_in.z, self.Alpha.value)
            self.__alpha_changed = False
            
