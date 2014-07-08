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
import avango.gua

class LoadFileScaler(avango.script.Script):
    LoadFile = avango.osg.SFLoadFile()
    LoadFileFinished = avango.SFString()
    Radius = avango.SFFloat()

    MatrixOut = avango.osg.SFMatrix()
    Trigger = avango.SFBool()

    def __init__(self):
        self.super(LoadFileScaler).__init__()

        self.bounding_box = None
        self.Radius.value = 1.0
        self.__file_loaded = False
        
        self.Name.value = "LoadFileScaler"

    @field_has_changed(LoadFileFinished)
    def loading_finished(self):

        if self.LoadFile.value.Filename.value == self.LoadFileFinished.value:
            self.__file_loaded = True

    def evaluate(self):
        if self.__file_loaded:
            self.MatrixOut.value = self.calc_scale_matrix()
            self.Trigger.value = True
            self.__file_loaded = False

    def calc_scale_matrix(self):
        self.bounding_box = avango.osg.calc_bounding_box(self.LoadFile.value)
        width =  self.bounding_box.x_max() - self.bounding_box.x_min()
        height = self.bounding_box.y_max() - self.bounding_box.y_min()
        depth =  self.bounding_box.z_max() - self.bounding_box.z_min()
        max_extend = max(max(width,height),depth)
        scaleFactor = 2.0 / (max_extend);
        scaleFactor *= self.Radius.value;
        return avango.osg.make_scale_mat(scaleFactor,scaleFactor,scaleFactor)
    
