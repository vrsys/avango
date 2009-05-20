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

class SFVec4Descriptor(object):
    'Simple stream support for SFVec4'

    key = "SFVec4"

    def write(self, field, hout):
        if field.value:
            hout.write('\x00')
        value = []
        v = field.value
        value.append(str(v.x))
        value.append(str(v.y))
        value.append(str(v.z))
        value.append(str(v.w))
        hout.write('\x00'.join(value))

    def read(self, line):
        field = avango.osg.SFVec4()
        for x in zip(line[::4], line[1::4], line[2::4], line[3::4]):
            vec = avango.osg.Vec4(float(x[0]), float(x[1]), float(x[2]), float(x[3]))
        field.value = vec 
        return field

_register_field(avango.osg.SFVec4, SFVec4Descriptor())

