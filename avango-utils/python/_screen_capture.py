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
from avango.script import field_has_changed

import avango.osg
import avango.utils

class ScreenCapture(avango.script.Script):
    
    View = avango.SFObject()
    NumFrames = avango.SFInt()
    Start = avango.SFBool()
    
    OutputFolder = avango.SFString()
    BaseFilename = avango.SFString()
    
    def __init__(self):
        self.super(ScreenCapture).__init__()
        
        self.NumFrames.value = 1
        self.OutputFolder.value = "/tmp"
        self.BaseFilename.value = "screen_capture"
        
        self._running = False
        self._num_captured_frames = 0
        
    @field_has_changed(Start)
    def start_changed(self):
        if not self._running and self.Start.value and self.View.value:
            self._start_capturing()
    
    def _start_capturing(self):
        avango.utils.add_screen_capture_handler(self.View.value, self.OutputFolder.value, self.BaseFilename.value, self.NumFrames.value)
        self.always_evaluate(True)
        
    def _stop_capturing(self):
        self._num_captured_frames = 0
        self._running = False
        self.always_evaluate(False)
    
    def evaluate(self):
        
        if self._num_captured_frames == self.NumFrames.value:
            self._stop_capturing()
        self._num_captured_frames += 1    
            