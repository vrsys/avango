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
import avango.menu.Preferences
import avango.tools
import avango.script
from avango.script import field_has_changed

class PlacementBase(avango.script.Script):
    MenuSize = avango.SFFloat()
    MinDistance = avango.SFFloat()
    MaxDistance = avango.SFFloat()
    ObjectDistance = avango.SFFloat()
    FixedRoll = avango.SFBool()
    EnableRayBasedPlacement = avango.SFBool()
    InterpolationDistance = avango.SFFloat()
    SceneRoot = avango.osg.SFNode()
    ScaleDebugSpheres = avango.SFFloat()

    _Trigger = avango.SFBool()

    def __init__(self):
        self.MenuSize.connect_from(avango.menu.Preferences.placement.MenuSize)
        self.MinDistance.connect_from(avango.menu.Preferences.placement.MinDistance)
        self.MaxDistance.connect_from(avango.menu.Preferences.placement.MaxDistance)
        self.ObjectDistance.connect_from(avango.menu.Preferences.placement.ObjectDistance)
        self.FixedRoll.connect_from(avango.menu.Preferences.placement.FixedRoll)
        self.EnableRayBasedPlacement.connect_from(avango.menu.Preferences.placement.EnableRayBasedPlacement)
        self.InterpolationDistance.connect_from(avango.menu.Preferences.placement.InterpolationDistance)
        self.ScaleDebugSpheres.connect_from(avango.menu.Preferences.placement.ScaleDebugSpheres)
        self.menu_position = avango.osg.Vec3(0,0,0)
        self.menu_scale = self.MenuSize.value
        self.HeadTransform = None
        self.position_dirty = False
        self._Trigger.value = False
        self.valid = False
        self.intersector = avango.tools.nodes.PickSelector()
        self.intersector.RootNode.connect_from(self.SceneRoot)
        self.intersector.CreateIntersections.value = True
        self.debug_group = avango.osg.nodes.Group()

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
        self.eye_to_object = self.hit_position - self.head_position
        self.object_distance = self.eye_to_object.length()
        self.eye_to_object_normalized = self.eye_to_object / self.object_distance
        self.valid = True
        self._Trigger.touch()

    def set_head_transform(self, field):
        '''
        Set the Matrix field, that represents the position and orientation of the viewer
        '''
        self.HeadTransform = field

    @field_has_changed(_Trigger)
    def trigger_touched(self):
        self.position_dirty = True

    def evaluate(self):
        if self.position_dirty and self.valid:
            self.evaluate_dependency(self.panel)
            self.update_position()
            self.position_dirty = False

    def update_position(self):
        # before standard-placement, determine a "standard" position for the menu.
        self.pre_placement()

        # standard position, scale and rotation:
        # panel is positioned in front of clicked point, facing the viewer
        self.standard_placement()
        self.rotate()
        self.scale()

        # move menu to screen, if it is clipped by screen edges at standard position
        self.move_to_screen()
        self.rotate()
        self.scale()

        # move menu a little bit closer to the viewer to avoid "sticking" to an object
        self.gap_to_object()
        self.scale()

        # use intersect rays to move menu in front of nearest object
        fraction = 1
        if self.EnableRayBasedPlacement.value:
            fraction = self.ray_based_placement()

        # if menu has been moved by ray based placement, move it closer again
        if abs(fraction-1) > 0.0001:
            self.gap_to_object()

        # update scaling
        self.scale()

        # finally, calculate and set root matrix
        self.bake_placement()

    def standard_placement(self):
        max_distance = min(self.MaxDistance.value, self.screen_distance(self.head_position, self.eye_to_object_normalized))
        menu_distance = max(min(max_distance, self.object_distance), self.MinDistance.value)
        self.menu_position = self.head_position + menu_distance * self.eye_to_object_normalized

        # place a yellow sphere at hit point
        if avango.menu.Preferences.enable_debug_output and self.SceneRoot.value is not None:
            self.add_debug_sphere(self.hit_position, avango.osg.Vec4(1,1,0,1), 0.003)

    def rotate(self):
        if self.FixedRoll.value:
            eye_to_menu = self.menu_position - self.head_position
            eye_to_menu.normalize()
            self.rotation = avango.osg.make_rot_mat(avango.osg.Vec3(0,0,-1), eye_to_menu)
        else:
            self.rotation = self.head_rotation

    def gap_to_object(self):
        eye_to_menu = self.menu_position - self.head_position
        menu_distance = eye_to_menu.length() - self.ObjectDistance.value
        menu_distance = max(menu_distance, self.MinDistance.value)
        self.menu_position = self.head_position + menu_distance * eye_to_menu / eye_to_menu.length()

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
        # calculate menu corner positions
        panel_corners = self.get_absolute_menu_corner_positions()

        # interpolate left and right edges, from top to bottom
        left_points = self.interpolate_positions(panel_corners[0], panel_corners[2])
        right_points = self.interpolate_positions(panel_corners[1], panel_corners[3])

        # now interpolate between interpolated points from left to right
        points = []
        for (left_point, right_point) in zip(left_points, right_points):
            points +=  self.interpolate_positions(left_point, right_point)

        if avango.menu.Preferences.enable_debug_output:
            print "Number of interpolated points for ray based placement:", len(points)

        # hide panel to prevent picking the panel itself
        self.panel.switch.VisibleChild.value = -1
        self.evaluate_dependency(self.panel.switch)

        self.intersector.PickRayTransform.value = avango.osg.make_trans_mat(self.head_position)
        fraction = 1
        for point in points:
            direction = point - self.head_position
            eye_point_distance = direction.length()
            direction.normalize()
            self.intersector.PickRayLength.value = eye_point_distance
            self.intersector.PickRayDirection.value = direction
            holder = self.intersector.pick()

            if len(holder)>0:
                intersect_point = holder[0].Intersection.value.Point.value
                eye_intersect_distance = (intersect_point - self.head_position).length()
                fraction = min(fraction, eye_intersect_distance / eye_point_distance)

                # place a red sphere at intersection point
                if avango.menu.Preferences.enable_debug_output:
                    self.add_debug_sphere(intersect_point, avango.osg.Vec4(1,0,0,1), 0.002)

            # place a blue sphere at target point
            if avango.menu.Preferences.enable_debug_output:
                self.add_debug_sphere(point, avango.osg.Vec4(0.2,0.2,1,1), 0.002)

        # revert panel visibility state
        self.panel.switch.VisibleChild.value = self.panel._VisibleChild.value

        if avango.menu.Preferences.enable_debug_output:
            print "Scale menu distance by:", fraction

        # scale menu distance
        new_menu_distance = (fraction * (self.menu_position - self.head_position)).length()
        eye_to_menu_normalized = self.menu_position - self.head_position
        eye_to_menu_normalized.normalize()
        self.menu_position = self.head_position + new_menu_distance * eye_to_menu_normalized

        return fraction

    def bake_placement(self):
        # first rotation matrix is a workaround for polygon offset problem
        self.root.Matrix.value = avango.osg.make_rot_mat(0.01, 1.0, 0.0, 0.0) \
                                 * self.rotation \
                                 * avango.osg.make_scale_mat(self.menu_scale) \
                                 * avango.osg.make_trans_mat(self.menu_position)

    def get_absolute_menu_corner_positions(self):
        half_width = self.panel.PanelWidth.value / 2
        height = self.panel.PanelHeight.value
        menu_transform = self.rotation \
                         * avango.osg.make_scale_mat(self.menu_scale) \
                         * avango.osg.make_trans_mat(self.menu_position)
        return [avango.osg.Vec3(-half_width,0,0) * menu_transform,
                avango.osg.Vec3(half_width,0,0) * menu_transform,
                avango.osg.Vec3(-half_width,-height,0) * menu_transform,
                avango.osg.Vec3(half_width,-height,0) * menu_transform
                ]

    def interpolate_positions(self, pos1, pos2):
        vec = (pos2 - pos1)
        count = max(1, int(vec.length() / self.InterpolationDistance.value / self.menu_scale))
        return [pos1 + (float(i) / count) * vec for i in range(0,count+1)]

    def add_debug_sphere(self, position, color, radius):
        s = avango.osg.nodes.Sphere(Radius=radius * self.ScaleDebugSpheres.value,
                                    Color=color,
                                    Matrix=avango.osg.make_trans_mat(position),
                                    DetailRatio=0.1
                                    )
        s.add_field(avango.SFUInt(), "PickMask")
        s.PickMask.value = 0
        self.debug_group.Children.value.append(s)

    def cleanup(self):
        self.intersector.disconnect_all_fields()
        self.disconnect_all_fields()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "PlacementBase deleted"
