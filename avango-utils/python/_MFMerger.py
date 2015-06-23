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

import avango.script

class MFMerger(avango.script.Script):

    def __init__(self):
        self.super(MFMerger).__init__()
        self._initialized = False
        
        self.Name.value = "MFMerger"

    def init(self, sftype, mftype):
        self._num_inputs = 0
        self._field_type = sftype
        self.add_field(mftype(), "Output")
        self._initialized = True

    def evaluate(self):
        if not self._initialized:
            return

        result = []
        for i in range(0, self._num_inputs):
            value = self.get_input(i).value
            if value:
                result.append(value)
        self.Output.value = result

    def get_input(self, index):
        if index >= self._num_inputs:
            return None
        return getattr(self, self.get_input_name(index))

    def get_input_name(self, index):
        return 'Input%i' % index

    def add_input(self):
        result = self._num_inputs
        self._num_inputs += 1

        self.add_field(self._field_type(), self.get_input_name(result))

        return result

def make_mf_merger(sftype, mftype):
    merger = MFMerger()
    merger.init(sftype, mftype)
    return merger
