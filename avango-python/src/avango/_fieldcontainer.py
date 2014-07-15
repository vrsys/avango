# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
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

from ._avango import *


def FieldContainer_getattr(self, name):
    field = self._get_field(name)
    if field is None:
        raise AttributeError('Unknown field %s' % name)
    return field

def FieldContainer_setattr(self, name, value):
    if self._get_field(name) is not None:
        raise AttributeError("Fields can not be set")
    return super(FieldContainer, self).__setattr__(name, value)

def FieldContainer_get_values(container):

    class Values(object):
        def __getattr__(self, name):
            return getattr(container, name).value
        def __setattr__(self, name, value):
            getattr(container, name).value = value

    return Values()

def FieldContainer_add_and_init_field(self, field, name, value):
    self.add_field(field, name)
    getattr(self, name).value = value


FieldContainer.__getattr__ = FieldContainer_getattr
FieldContainer.__setattr__ = FieldContainer_setattr
FieldContainer.get_values = FieldContainer_get_values
FieldContainer.add_and_init_field = FieldContainer_add_and_init_field
