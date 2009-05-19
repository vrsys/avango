# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of Avango.                                           #
#                                                                        #
# Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# Avango is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# Avango is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with Avango. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
# Avango is a trademark owned by FhG.                                    #
#                                                                        #
##########################################################################

import avango.osg
from _registry import _register_field

class SFMatrixDescriptor(object):
    'Simple stream support for SFMatrix'

    key = "SFMatrix"

    def write(self, field, hout):
        if field.value:
            hout.write('\x00')
        value = []
        v = field.value
        value.append(str(v.get_element(0,0)))
        value.append(str(v.get_element(0,1)))
        value.append(str(v.get_element(0,2)))
        value.append(str(v.get_element(0,3)))
        value.append(str(v.get_element(1,0)))
        value.append(str(v.get_element(1,1)))
        value.append(str(v.get_element(1,2)))
        value.append(str(v.get_element(1,3)))
        value.append(str(v.get_element(2,0)))
        value.append(str(v.get_element(2,1)))
        value.append(str(v.get_element(2,2)))
        value.append(str(v.get_element(2,3)))
        value.append(str(v.get_element(3,0)))
        value.append(str(v.get_element(3,1)))
        value.append(str(v.get_element(3,2)))
        value.append(str(v.get_element(3,3)))
        hout.write('\x00'.join(value))

    def read(self, line):
        field = avango.osg.SFMatrix()
        for x in zip(line[::16], line[1::16], line[2::16], line[3::16],
                     line[4::16], line[5::16], line[6::16], line[7::16],
                     line[8::16], line[9::16], line[10::16], line[11::16],
                     line[12::16], line[13::16], line[14::16], line[15::16]):
            mat = avango.osg.Matrix()
            mat.set_element(0,0, float(x[0]))
            mat.set_element(0,1, float(x[1]))
            mat.set_element(0,2, float(x[2]))
            mat.set_element(0,3, float(x[3]))
            mat.set_element(1,0, float(x[4]))
            mat.set_element(1,1, float(x[5]))
            mat.set_element(1,2, float(x[6]))
            mat.set_element(1,3, float(x[7]))
            mat.set_element(2,0, float(x[8]))
            mat.set_element(2,1, float(x[9]))
            mat.set_element(2,2, float(x[10]))
            mat.set_element(2,3, float(x[11]))
            mat.set_element(3,0, float(x[12]))
            mat.set_element(3,1, float(x[13]))
            mat.set_element(3,2, float(x[14]))
            mat.set_element(3,3, float(x[15]))
        field.value = mat
        return field

_register_field(avango.osg.SFMatrix, SFMatrixDescriptor())
