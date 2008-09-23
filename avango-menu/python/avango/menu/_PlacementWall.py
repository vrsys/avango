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

class PlacementWall(PlacementBase):
    '''
    Placement for Wall-like projection displays (e.g. TwoView)

    Parameters
    ScreenWidth: width of screen [meter]
    ScreenHeight: height of screen [meter]
    ScreenCenter: center position of screen in relation to scene root [Vec3]
    ScreenPadding: safe distance to screen edges for placing the menu [meter]
    MinTextSize: minimal text rendering size in screen plane [meter]

    Screen normal: fixed to Vec3(0,0,1)
    '''
    ScreenWidth = avango.SFFloat()
    ScreenHeight = avango.SFFloat()
    ScreenCenter = avango.osg.SFVec3()
    ScreenPadding = avango.SFFloat()
    MinTextSize = avango.SFFloat()

    def __init__(self):
        self.init_super(PlacementBase)
        self.screen_dimensions_changed = False
        self.screen_plane = None
        self.ScreenWidth.connect_from(avango.menu.Preferences.placement.WallScreenWidth)
        self.ScreenHeight.connect_from(avango.menu.Preferences.placement.WallScreenHeight)
        self.ScreenCenter.connect_from(avango.menu.Preferences.placement.WallScreenCenter)
        self.ScreenPadding.connect_from(avango.menu.Preferences.placement.WallScreenPadding)
        self.MinTextSize.connect_from(avango.menu.Preferences.placement.WallMinTextSize)

    @field_has_changed(ScreenWidth)
    def screen_width_changed(self):
        self.screen_dimensions_changed = True

    @field_has_changed(ScreenHeight)
    def screen_height_changed(self):
        self.screen_dimensions_changed = True

    @field_has_changed(ScreenCenter)
    def screen_center_changed(self):
        self.screen_dimensions_changed = True

    def evaluate(self):
        if self.screen_dimensions_changed:
            self.update_screen_dimensions()
            self.screen_dimensions_changed = False
        self.super().evaluate()

    def update_screen_dimensions(self):
        x_vec = avango.osg.Vec3(1,0,0) + self.ScreenCenter.value
        y_vec = avango.osg.Vec3(0,1,0) + self.ScreenCenter.value
        self.screen_plane = avango.osg.Plane(self.ScreenCenter.value, x_vec, y_vec)

    def intersect_screen(self, point, direction):
        if direction*avango.osg.Vec3(0,0,1)>0:
            return None, None
        intersection = self.screen_plane.intersect(point, direction) - self.ScreenCenter.value
        distance = (intersection - point).length()
        return intersection, distance

    def move_to_screen(self):
        '''
        Check, if menu panel is clipped by screen edges and move the panel inside screen border.
        '''
        # calculate menu corner positions
        panel_corners = self.get_absolute_menu_corner_positions()

        half_width = self.ScreenWidth.value / 2 - self.ScreenPadding.value
        half_height = self.ScreenHeight.value / 2 - self.ScreenPadding.value

        x_correct = 0
        y_correct = 0

        for corner in panel_corners:
            intersection, distance = self.intersect_screen(self.head_position, corner-self.head_position)
            if intersection is None:
                continue

            scaling_factor = (corner - self.head_position).length() / distance

            # clamp x position
            correct = 0
            if intersection.x > half_width:
                correct = (half_width - intersection.x) * scaling_factor
            if intersection.x < -half_width:
                correct = (-half_width - intersection.x) * scaling_factor

            if abs(correct) > abs(x_correct):
                x_correct = correct

            # clamp y position
            correct = 0
            if intersection.y > half_height:
                correct = (half_height - intersection.y) * scaling_factor
            if intersection.y < -half_height:
                correct = (-half_height - intersection.y) * scaling_factor

            if abs(correct) > abs(y_correct):
                y_correct = correct

            # place a white sphere at screen intersection position
            if avango.menu.Preferences.enable_debug_output and self.SceneRoot.value is not None:
                intersection += self.ScreenCenter.value
                self.add_debug_sphere(intersection, avango.osg.Vec4(1,1,1,1), 0.008)

        # place a purple sphere at old menu position
        if avango.menu.Preferences.enable_debug_output and self.SceneRoot.value is not None:
            self.add_debug_sphere(self.menu_position, avango.osg.Vec4(1,0,1,1), 0.004)

        # move menu to visible screen space
        correction_vec = avango.osg.Vec3(x_correct, y_correct, 0)
        self.menu_position += correction_vec

        # place a green sphere at new menu position
        if avango.menu.Preferences.enable_debug_output and self.SceneRoot.value is not None:
            self.add_debug_sphere(self.menu_position, avango.osg.Vec4(0,1,0,1), 0.004)
            print "Move To Screen Correction:", correction_vec

    def screen_distance(self, position, direction):
        intersection, distance = self.intersect_screen(position, direction)
        return distance

    def pre_placement(self):
        # if clicked point is visible, do not modify standard placement
        screen_hit = self.screen_hit(self.head_position, self.eye_to_object_normalized)

        # if clicked point is not visible, place menu in view direction
        if not screen_hit:
            view_direction = avango.osg.Vec3(0,0,-1) * self.head_rotation
            view_hit = self.screen_hit(self.head_position, view_direction)

            # if user is facing the screen, place menu at intersection of screen and viewing direction
            if view_hit:
                intersection, distance = self.intersect_screen(self.head_position, view_direction)
                self.hit_position = intersection + self.ScreenCenter.value
                self.eye_to_object = self.hit_position - self.head_position
                self.object_distance = self.eye_to_object.length()
                self.eye_to_object_normalized = self.eye_to_object / self.object_distance

                # place a white sphere at new alternate menu position
                if avango.menu.Preferences.enable_debug_output and self.SceneRoot.value is not None:
                    self.add_debug_sphere(self.hit_position, avango.osg.Vec4(1,1,1,1), 0.01)

            # if viewer is not facing the screen, place the menu at screen center
            else:
                self.hit_position = self.ScreenCenter.value
                self.eye_to_object = self.hit_position - self.head_position
                self.object_distance = self.eye_to_object.length()
                self.eye_to_object_normalized = self.eye_to_object / self.object_distance

        # place a white sphere at new alternate menu position
        if avango.menu.Preferences.enable_debug_output and self.SceneRoot.value is not None:
            self.add_debug_sphere(self.hit_position, avango.osg.Vec4(1,1,1,1), 0.01)

    def screen_hit(self, position, direction):
        '''
        If line (position, direction) intersects screen within boundaries, return True. Otherwise return False.
        Since this depends on the screen, it should be implemented in a derived class,
        that is specialized for a particular screen type (like monitor, twoview, icone, workbench...)
        '''
        intersection, distance = self.intersect_screen(position, direction)
        half_width = self.ScreenWidth.value / 2
        half_height = self.ScreenHeight.value / 2
        return intersection is not None and -half_width < intersection.x < half_width and -half_height < intersection.y < half_height

    def scale(self):
        '''
        Scale menu by distance considering the text rendering size in screen plane.
        When menu size gets too small for good readability of menu labels, the menu is resized
        in a way, that the rendering resolution is high enough for text rendering.
        The minimal size of all widgets and the panel title is calculated. Then the projection
        size of the text (at menu center) is calculated and the menu is rescaled to have the
        projection size equal to MinTextSize.
        '''
        self.menu_scale = self.MenuSize.value * (self.menu_position - self.head_position).length()

        text_sizes = []
        layouters = self.panel.get_layouters()
        for layouter in layouters:
            if hasattr(layouter, "TextSize"):
                text_sizes.append(layouter.TextSize.value)
        if self.panel.Title.value != "":
            text_sizes.append(self.panel.TitleSize.value)
        min_text_size = min(text_sizes) * self.menu_scale

        panel_corners = self.get_absolute_menu_corner_positions()
        menu_center = (panel_corners[0] + panel_corners[1] + panel_corners[2] + panel_corners[3]) / 4
        panel_center_distance = (menu_center - self.head_position).length()
        screen_distance = self.screen_distance(self.head_position, menu_center - self.head_position)

        projected_text_size =  min_text_size * screen_distance / panel_center_distance

        if projected_text_size < self.MinTextSize.value:
            enlarge = self.MinTextSize.value / projected_text_size
            self.menu_scale *= enlarge

    def cleanup(self):
        self.super().cleanup()
        self.disconnect_all_fields()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "PlacementWall deleted"
