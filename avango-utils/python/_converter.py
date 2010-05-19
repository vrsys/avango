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
import avango.osg

class FloatXBase(avango.script.Script):
    "Base for sequential operations on floats"

    BaseFieldName = avango.SFString()
    NumFieldsOut = avango.SFInt()

    Output = avango.SFFloat()

    def __init__(self):
        self.super(FloatXBase).__init__()

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

    def evaluate(self):
        self.Output.value = self.Value0.value + self.Value1.value

class Float4AddVec2Converter(avango.script.Script):
    "Converts four Floats into a Vec2, where vec.x = Value00 + Value01 and vec.y = Value10 + Value11"

    Value00 = avango.SFFloat()
    Value01 = avango.SFFloat()

    Value10 = avango.SFFloat()
    Value11 = avango.SFFloat()

    Output = avango.osg.SFVec2()

    def evaluate(self):
        self.Output.value = avango.osg.Vec2(self.Value00.value+self.Value01.value, self.Value10.value+self.Value11.value)

class SFNode2MFContainerConverter(avango.script.Script):
    "Converts a SFNode to a MFNode"

    Input = avango.osg.SFNode()
    Output = avango.MFContainer()

    def evaluate(self):
        self.Output.value = [ self.Input.value ]

class Float2Vec2Converter(avango.script.Script):
    "Converts two Floats into on Vec2"

    Value0 = avango.SFFloat()
    Value1 = avango.SFFloat()
    Output = avango.osg.SFVec2()

    def evaluate(self):
        self.Output.value = avango.osg.Vec2(self.Value0.value, self.Value1.value)
        
class Vec3ToTransMatrix(avango.script.Script):
    TransVec = avango.osg.SFVec3()
    TransOffset = avango.osg.SFVec3()
    Matrix = avango.osg.SFMatrix()
    
    def evaluate(self):
        self.Matrix.value = avango.osg.make_trans_mat(self.TransVec.value + self.TransOffset.value)
        
def make_vec3_to_trans_matrix(vec3_field, trans_offset=avango.osg.Vec3(0,0,0)):
    converter = Vec3ToTransMatrix()
    converter.TransVec.connect_from(vec3_field)
    converter.TransOffset.value = trans_offset
    
    return converter.Matrix


class TranslationMatrixCalculator(avango.script.Script):
    MatrixFrom = avango.osg.SFMatrix()
    MatrixTo = avango.osg.SFMatrix()
    MatrixTransDif = avango.osg.SFMatrix()
    
    def evaluate(self):
        if not self.MatrixFrom.value or not self.MatrixTo.value:
            return
        self.MatrixTransDif.value = avango.osg.make_trans_mat( self.MatrixFrom.value.get_translate() - self.MatrixTo.value.get_translate() )