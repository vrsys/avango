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

def normalize(min_in, max_in, min_out, max_out, val_in):
    if min_in < 0:
        val_in += (-min_in)
        val_in /= max_in+(-min_in)
    else:
        val_in -= min_in
        val_in /= max_in-min_in

    return min_out + val_in*(max_out-min_out)

class FloatNormalization(avango.script.Script):
    MinIn = avango.SFFloat()
    MaxIn = avango.SFFloat()

    MinOut = avango.SFFloat()
    MaxOut = avango.SFFloat()

    ValueIn = avango.SFFloat()
    ValueOut = avango.SFFloat()

    def __init__(self):
        self.super(FloatNormalization).__init__()
        
        self.MinIn.value = -1.0
        self.MaxIn.value = 1.0

        self.MinOut.value = 0.0
        self.MaxOut.value = 1.0
        
        self.Name.value = "FloatNormalization"

    def evaluate(self):
        val_in = self.ValueIn.value
        min_in = self.MinIn.value
        max_in = self.MaxIn.value
        min_out = self.MinOut.value
        max_out = self.MaxOut.value

        assert(min_in < max_in)
        assert(min_out < max_out)
        assert(val_in >= min_in)
        assert(val_in <= max_in)

        self.ValueOut.value = normalize(min_in, max_in, min_out, max_out, val_in)

def make_float_normalization(min_in, max_in, min_out, max_out, value_in_field):
    normalization = avango.utils.nodes.FloatNormalization()
    normalization.MinIn.value = min_in
    normalization.MaxIn.value = max_in
    normalization.MinOut.value = min_out
    normalization.MaxOut.value = max_out
    normalization.ValueIn.connect_from(value_in_field)
    return normalization.ValueOut
