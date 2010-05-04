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
from avango.script import field_has_changed

class ImmediateEdgeTrigger(avango.script.Script):
    """
    Triggers immediately (as soon as the field has been changed) on 
    the edge transitions of the value of the Trigger field.
    """
    
    Trigger = avango.SFBool()

    def __init__(self):
        self.super(ImmediateEdgeTrigger).__init__()
        
        self._last_trigger = False

    @field_has_changed(Trigger)
    def trigger_changed(self):
        callback = None
        if self.Trigger.value:
            if not self._last_trigger:
                self._last_trigger = True
                self.on_down_transition()
        else:
            if self._last_trigger:
                self._last_trigger = False
                self.on_up_transition()

    def on_up_transition(self):
        pass
    def on_down_transition(self):
        pass



class EdgeTrigger(avango.script.Script):
    "Triggers on the edge transitions of the value of the Trigger field."
    Trigger = avango.SFBool()

    def __init__(self):
        self.super(EdgeTrigger).__init__()
        
        self._last_trigger = False

    def evaluate(self):
        callback = None
        if self.Trigger.value:
            if not self._last_trigger:
                self._last_trigger = True
                self.on_down_transition()
        else:
            if self._last_trigger:
                self._last_trigger = False
                self.on_up_transition()
            
    def on_up_transition(self):
        pass
    def on_down_transition(self):
        pass