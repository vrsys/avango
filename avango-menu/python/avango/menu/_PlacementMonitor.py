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

import avango.osg
import avango.script
from avango.script import field_has_changed
import avango.menu.Preferences
from _PlacementBase import *

class PlacementMonitor(PlacementBase):

    _Trigger = avango.SFBool()
    ScreenWidth = avango.SFInt()
    ScreenHeight = avango.SFInt()

    def place(self, root, panel, hit_position):
        if self.HeadTransform is None:
            print "Placement: Please set HeadTransform before placing a panel"
            return
        if self.EnableRayBasedPlacement.value and self.SceneRoot.value is None:
            print "Placement: Please set SceneRoot before placing a panel with enabled ray based placement"
            return

        if avango.menu.Preferences.enable_debug_output:
            if self.debug_group not in self.SceneRoot.value.Children.value:
                self.SceneRoot.value.Children.value.append(self.debug_group)
            self.debug_group.Children.value = []

        self.root = root
        self.panel = panel
        self.head_position = self.HeadTransform.value.get_translate()
        self.head_rotation = avango.osg.make_rot_mat(self.HeadTransform.value.get_rotate())
        self.rotation = avango.osg.Matrix()
        self.hit_position = hit_position
        self.eye_to_object = self.hit_position * avango.osg.make_inverse_mat(self.HeadTransform.value)
        self.object_distance = self.eye_to_object.length()
        self.eye_to_object_normalized = self.eye_to_object / self.object_distance
        self.valid = True
        self._Trigger.touch()

    def standard_placement(self):
        # place menu at MinDistance which is ideally the distance to the near clipping plane
        menu_distance = self.eye_to_object * ((self.MinDistance.value + 0.001)/-self.eye_to_object.z)
        self.menu_position = menu_distance * self.HeadTransform.value

        # place a yellow sphere at hit point and green sphere at view point
        if avango.menu.Preferences.enable_debug_output and self.SceneRoot.value is not None:
            self.add_debug_sphere(self.hit_position, avango.osg.Vec4(1,1,0,1), 0.003)
            self.add_debug_sphere(self.head_position, avango.osg.Vec4(0,1,0,1), 0.003)

    def gap_to_object(self):
        pass

    def pre_placement(self):
        '''
        Determine a "standard" placement for the menu. Normally, the standard position is the clicked point.
        If this point is not visible (e.g. user has clicked at the floor or above screen), there should be a
        proper alternative position, e.g. a fixed position or in front of the head (if user is facing the
        screen!). This off-screen-click handling can be implemented in a derived class, that is specialized
        for a particular screen type (like monitor, twoview, icone, workbench...)
        '''
        pass

    def scale(self):
        '''
        Calculate scale with respect to screen distance and render size
        Since this depends on the screen, it should be implemented in a derived class,
        that is specialized for a particular screen type (like monitor, twoview, icone, workbench...)
        This basic scaling only depends on menu distance.
        '''
        self.menu_scale = self.MenuSize.value * (self.menu_position - self.head_position).length()
        menu_distance = self.eye_to_object * ((self.MinDistance.value + (self.menu_scale * self.MenuSize.value))/-self.eye_to_object.z)
        self.menu_position = menu_distance * self.HeadTransform.value

    def move_to_screen(self):
        '''
        Move panel to screen
        This method can be implemented in a derived class, that is specialized for
        a particular screen type (like monitor, twoview, icone, workbench...)
        '''
        pass

    def screen_distance(self, position, direection):
        '''
        Intersect the given line (position, direction) with the screen and return the distance.
        Limit the menu distance to screen distance to make sure, that the menu is
        placed within the screen plane. This reduces the paralax movement on screen
        and reduces eye strain because of divergency and focus.
        Since this depends on the screen, it should be implemented in a derived class,
        that is specialized for a particular screen type (like monitor, twoview, icone, workbench...)
        '''
        return 9999

    def ray_based_placement(self):
        return 1

    def cleanup(self):
        self.intersector.disconnect_all_fields()
        self.disconnect_all_fields()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "PlacementBase deleted"
