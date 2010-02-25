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

import avango.display
import math

class iCone(avango.display.Display):

    def __init__(self, inspector, options):
        super(iCone, self).__init__("iCone", inspector)

        self._eye_vec = avango.osg.Vec3(0., 1.7, 0.)

        common_transform = avango.osg.make_rot_mat(math.radians(-4.43), 1, 0, 0) \
                         * avango.osg.make_trans_mat(0, 1.390, -2.818)

        transforms = [
            common_transform * avango.osg.make_rot_mat(math.radians(84.135), 0, 1, 0),
            common_transform * avango.osg.make_rot_mat(math.radians(28.045), 0, 1, 0),
            common_transform * avango.osg.make_rot_mat(math.radians(-28.045), 0, 1, 0),
            common_transform * avango.osg.make_rot_mat(math.radians(-84.135), 0, 1, 0),
        ]
        print str(transforms)
        self._cone_windows = []
        for i in range(0,len(transforms)):
            window = self.make_window(0, 0, 1440, 1320, 3.540, 2.830, True, ":0.%i"%i)
            self._cone_windows.append((window, transforms[i]))

    def make_view(self, subdisplay):
        display_view = avango.display.nodes.View()

        for window, transform in self._cone_windows:
            camera, view = self.make_camera(display_view, 0.03, window)
            camera.EyeTransform.value = avango.osg.make_trans_mat(self._eye_vec)
            camera.ScreenTransform.value = transform
            self.add_view(view)

        return display_view
