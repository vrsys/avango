# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       #
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

import avango.osg
from _registry import _register_field

class MFVec2Descriptor(object):
    'Simple stream support for MFVec2'

    key = "MFVec2"

    def write(self, field, hout):
        if field.value:
            hout.write('\x00')
        values = []
        for v in field.value:
            values.append(str(v.x))
            values.append(str(v.y))
        hout.write('\x00'.join(values))

    def read(self, line):
        field = avango.osg.MFVec2()
        field.value = [avango.osg.Vec2(float(x[0]), float(x[1])) for x in zip(line[::2], line[1::2])]
        return field

_register_field(avango.osg.MFVec2, MFVec2Descriptor())
