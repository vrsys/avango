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
import avango.tools
import avango.script
from avango.script import field_has_changed

class SliderMouseInteractor(avango.script.Script):
    '''
    Calculate the movement in X direction of the intersection between device pickray
    and an infinite slider plane. The movement is scaled by slider length and mapped
    to slider value range.

    This interaction is dependent on slider distance and width and best suited for
    use as mouse input on 2D screens.
    '''
    Layouter = avango.script.SFObject()
    TargetHolder = avango.tools.SFTargetHolder()
    DeviceMatrix = avango.osg.SFMatrix()

    def __init__(self):
        self.pickray_direction = None
        self.slider_plane = None
        self.slider_length = None
        self.value_range = None
        self.start_value = None
        self.initial_hit_point_plane_coord = None
        self.slider_abs_inverted = avango.osg.Matrix()
        self.valid = False

    @field_has_changed(TargetHolder)
    def target_holder_changed(self):
        if self.TargetHolder.value is not None:
            # first frame of slider interaction
            self.DeviceMatrix.connect_from(self.TargetHolder.value.Creator.value.PickRayTransform)
            self.pickray_direction = self.TargetHolder.value.Creator.value.PickRayDirection.value
            initial_hit_point = self.TargetHolder.value.Intersection.value.Point.value

            # get absolute transform of slider position/orientation/scale and store the inverse for later use
            caller = self.Layouter.value.proxy.geode.AvangoMenuBehavior.value.Caller.value
            slider_abs = self.Layouter.value.root.get_absolute_transform(caller)
            self.slider_abs_inverted.invert(slider_abs)

            # create infinite plane
            slider_normal = avango.osg.transform3x3(avango.osg.Vec3(0,0,1), slider_abs)
            self.slider_plane = avango.osg.Plane(slider_normal, initial_hit_point)

            # calculate the hit point in plane coordinate system
            self.initial_hit_point_plane_coord = initial_hit_point * self.slider_abs_inverted

            # we need those values for scaling the movement to slider range
            self.slider_length = self.Layouter.value.slider.background_width
            self.value_range = self.Layouter.value.slider.Max.value - self.Layouter.value.slider.Min.value
            self.start_value = self.Layouter.value.slider.Value.value

            self.valid = True

        else:
            # last frame of slider interaction
            self.valid = False
            self.DeviceMatrix.disconnect()

    @field_has_changed(DeviceMatrix)
    def device_matrix_changed(self):
        if not self.valid:
            return

        # calculate actual pickray-plane intersection in plane coordinate system
        device_rotate = self.DeviceMatrix.value.get_rotate()
        device_direction = self.pickray_direction * device_rotate
        ray_hit_point = self.slider_plane.intersect(self.DeviceMatrix.value.get_translate(), device_direction)
        ray_hit_point_plane_coords = ray_hit_point * self.slider_abs_inverted

        # calculate moved distance in X direction on plane (=slider axis) and scale movement
        # to slider length and value range
        move_distance = (ray_hit_point_plane_coords - self.initial_hit_point_plane_coord).x
        value_distance = move_distance / self.slider_length * self.value_range

        # finally, clamp new slider value to Min/Max and set new value if changed from old one
        # The slider geometry will change automatically via field connections
        new_value = max(self.Layouter.value.slider.Min.value,
                        min(self.Layouter.value.slider.Max.value, self.start_value + value_distance))
        old_value = self.Layouter.value.Widget.value.Value.value
        if old_value!=new_value:
            self.Layouter.value.Widget.value.Value.value = new_value

    def cleanup(self):
        self.disconnect_all_fields()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "SliderMouseInteractor deleted"
