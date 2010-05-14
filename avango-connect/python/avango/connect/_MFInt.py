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

import avango
from _registry import _register_field

class MFIntDescriptor(object):
    'Simple stream support for MFInt'

    key = "MFInt"

    def write(self, field, hout):
        if field.value:
            hout.write('\x00')
        values = []
        for v in field.value:
            values.append(str(v))
        hout.write('\x00'.join(values))

    def read(self, line):
        field = avango.MFInt()
        field.value = [int(x) for x in line]
        return field

_register_field(avango.MFInt, MFIntDescriptor())
