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
'''
A distributed viewer setup: This Python scripts starts an
avango.osg.simpleviewer to load a given geometry. Any client connected to the
group "testgroup" should receive this model. (see also simpleviewer-clnt.py)
'''

import avango
import avango.script
import avango.gua
from avango.script import field_has_changed

import examples_common.navigator

from examples_common.GuaVE import GuaVE

MemoryController = avango.gua.nodes.NamedSharedMemoryController()
MemoryController.add_memory_segment("DEPTH_FEEDBACK_SEGMENT", 1000)


MemoryController.construct_named_atomic_int("DEPTH_FEEDBACK_SEGMENT", "DEPTH_BUFFER_RES_X")
MemoryController.set_value_for_named_object("DEPTH_BUFFER_RES_X", 0)

MemoryController.construct_named_atomic_int("DEPTH_FEEDBACK_SEGMENT", "DEPTH_BUFFER_RES_Y")
MemoryController.set_value_for_named_object("DEPTH_BUFFER_RES_Y", 0)

MemoryController.construct_named_atomic_int("DEPTH_FEEDBACK_SEGMENT", "DEPTH_FEEDBACK_SEMAPHOR")
MemoryController.set_value_for_named_object("DEPTH_FEEDBACK_SEMAPHOR", 99)



print("Successfully created shared memory segment!")