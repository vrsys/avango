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


class PropertyModifierInt(avango.script.Script):

    Enable = avango.SFBool()
    TimeIn = avango.SFDouble()

    Property = avango.SFInt()
    PropertyMin = avango.SFInt()
    PropertyMax = avango.SFInt()


    PropertyStepSize = avango.SFInt()
    PropertyInc = avango.SFBool()
    PropertyDec = avango.SFBool()
    TriggerTimeDelta = avango.SFFloat()

    Triggered = avango.SFBool()

    def __init__(self):
        self.super(PropertyModifierInt).__init__()

        self.last_trigger_time = self.TimeIn.value
        self.Enable.value = False

        self.Property.value = 0
        self.PropertyMin.value = 0
        self.PropertyMax.value = 100000

        self.PropertyStepSize.value = 1
        self.PropertyInc.value = False
        self.PropertyDec.value = False
        self.TriggerTimeDelta.value = 0.05

        self.always_evaluate(True)

    def evaluate(self):

        if (not self.PropertyInc.value and not self.PropertyDec.value) or not self.Enable.value:
            return

        time = self.TimeIn.value
        time_delta = time - self.last_trigger_time
        if time_delta > self.TriggerTimeDelta.value:

            if self.PropertyInc.value:
                new_val = self.Property.value+self.PropertyStepSize.value
                if new_val <= self.PropertyMax.value:
                    self.Property.value = new_val
                    self.Triggered.value = True
                self.last_trigger_time = time

            elif self.PropertyDec.value:
                new_val = self.Property.value-self.PropertyStepSize.value
                if new_val >= self.PropertyMin.value:
                    self.Property.value = new_val
                    self.Triggered.value = True
                self.last_trigger_time = time
