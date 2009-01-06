# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of Avango.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
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

from _registry import _registry_from_type, _registry_from_key

class NoDataAvailable(IndexError):
    pass

def write(name, field, hout):
    field_descriptor = _registry_from_type[type(field)]
    hout.write(name)
    hout.write('\x00')
    hout.write(field_descriptor.key)
    field_descriptor.write(field, hout)
    hout.write('\n')

def read(hin):
    raw_line = hin.readline()
    if raw_line == "":
        raise NoDataAvailable()
    line = raw_line[:-1].split('\x00')
    return line[0], _registry_from_key[line[1]].read(line[2:])

