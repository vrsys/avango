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

import avango.osg
import avango.tools
import avango.script
from avango.script import field_has_changed
import math
import Preferences

class SliderStylusInteractor(avango.script.Script):
    '''
    Calculate the device rotation around y-axis on a plane defined by device
    position, slider position and widget orientation. The angular frame by
    frame difference is then mapped to the slider value range und results in
    a value change.

    You can find a detailed description of this slider interaction in Armin
    Dresslers diploma thesis "Benutzergerechte Menuegestaltung fuer Virtuelle
    Umgebungen im Expertenkontext".

    This interaction is independent of slider distance and width and best
    suited for use as stylus input in immersive 3D environments.
    '''
    Layouter = avango.script.SFObject()
    TargetHolder = avango.tools.SFTargetHolder()
    DeviceMatrix = avango.osg.SFMatrix()
    FullRangeAngle = avango.SFFloat()

    def __init__(self):
        self.super(SliderStylusInteractor).__init__()

        self.FullRangeAngle.connect_from(Preferences.interaction.SliderStylusInteractorFullRangeAngle)
        self.quat = avango.osg.Quat()
        self.pickray_direction = None
        self.initial_hit_point = None
        self.slider_point2 = None
        self.last_point = None
        self.last_value = None
        self.value_range = None
        self.valid = False

    @field_has_changed(TargetHolder)
    def target_holder_changed(self):
        if self.TargetHolder.value is not None:
            # first frame of slider interaction
            self.DeviceMatrix.connect_from(self.TargetHolder.value.Creator.value.PickRayTransform)
            self.pickray_direction = self.TargetHolder.value.Creator.value.PickRayDirection.value

            # use initial hit point as first point on slider widget
            self.initial_hit_point = self.TargetHolder.value.Intersection.value.Point.value
            self.last_point = self.initial_hit_point

            # get absolute transform of slider position/orientation/scale and createsecond point on slider widget
            caller = self.Layouter.value.proxy.geode.AvangoMenuBehavior.value.Caller.value
            slider_rot = self.Layouter.value.root.get_absolute_transform(caller).get_rotate()
            self.slider_point2 = self.initial_hit_point + avango.osg.Vec3(0.1,0,0) * avango.osg.make_rot_mat(slider_rot)

            # we need those values for scaling the movement to slider range
            self.value_range = self.Layouter.value.slider.Max.value - self.Layouter.value.slider.Min.value
            self.last_value = self.Layouter.value.slider.Value.value

            self.valid = True

        else:
            # last frame of slider interaction
            self.valid = False
            self.DeviceMatrix.disconnect()

    @field_has_changed(DeviceMatrix)
    def device_matrix_changed(self):
        if not self.valid:
            return

        device_abs = self.DeviceMatrix.value
        device_pos = device_abs.get_translate()
        device_rot = device_abs.get_rotate()

        # create a plane through device position and slider
        slider_plane = avango.osg.Plane(device_pos, self.initial_hit_point, self.slider_point2)

        # project last frame pick ray to plane
        last_vec = slider_plane.closest_point(self.last_point) - device_pos
        last_vec.normalize()

        # project current pickray to plane
        new_point = device_pos + self.pickray_direction * device_rot
        new_vec = slider_plane.closest_point(new_point) - device_pos
        new_vec.normalize()

        # angle between the projected vectors
        # it is supposed, that the rotation is roughly about the y-axis
        # TODO: fix this for any axis
        self.quat.make_rotate(last_vec, new_vec)
        angle = math.degrees(self.quat.get_angle())
        if self.quat.get_axis().y>0:
            angle = -angle

        # finally, clamp new slider value to Min/Max and set new value if changed from old one
        # The slider geometry will change automatically via field connections
        old_value = self.Layouter.value.Widget.value.Value.value
        new_value = max(self.Layouter.value.slider.Min.value,
                        min(self.Layouter.value.slider.Max.value,
                            old_value + angle * self.value_range / self.FullRangeAngle.value))
        if old_value!=new_value:
            self.Layouter.value.Widget.value.Value.value = new_value

        self.last_point = new_point

    def cleanup(self):
        self.disconnect_all_fields()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "SliderStylusInteractor deleted"
