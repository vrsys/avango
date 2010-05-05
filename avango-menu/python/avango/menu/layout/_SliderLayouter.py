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
from _PushButtonLayouter import PushButtonLayouter
from _Proxy import *
import avango.menu.Preferences

class SliderLayouter(PushButtonLayouter):
    '''
    Layouter for Slider
    '''
    SliderBarMinLength = avango.SFFloat()
    SliderBarMaxLength = avango.SFFloat()

    def __init__(self):
        self.super(SliderLayouter).__init__()
        self.slider = _ContainerAlignedSliderLayouter()

    def init_widget(self):
        self.super(SliderLayouter).init_widget()
        widget = self.Widget.value

        self.SliderBarMinLength.connect_from(widget.SliderBarMinLength)
        self.SliderBarMaxLength.connect_from(widget.SliderBarMaxLength)

        self.slider = _ContainerAlignedSliderLayouter()
        self.slider.Min.connect_from(widget.Min)
        self.slider.Max.connect_from(widget.Max)
        self.slider.Value.connect_from(widget.Value)
        self.slider.SliderBarMinLength.connect_from(self.SliderBarMinLength)
        self.slider.SliderBarMaxLength.connect_from(self.SliderBarMaxLength)
        self.slider.SliderBarBorderWidth.connect_from(widget.SliderBarBorderWidth)
        self.slider.SliderBarBorderColor.connect_from(widget.SliderBarBorderColor)
        self.slider.SliderBarFilenames.connect_from(widget.SliderBarFilenames)
        self.slider.SliderBarHeight.connect_from(widget.SliderBarHeight)
        self.root.Children.value.append(self.slider.root)

    @field_has_changed(PushButtonLayouter.Enable)
    def enable_changed(self):
        self.push_button_enable_changed()
        self.slider.foreground.Color.disconnect()
        self.slider.background.Color.disconnect()
        if self.Enable.value:
            self.slider.foreground.Color.connect_from(self.IconColor)
            self.slider.background.Color.connect_from(self.IconColor)
        else:
            self.slider.foreground.Color.connect_from(self.IconDisabledColor)
            self.slider.background.Color.connect_from(self.IconDisabledColor)

    def create_proxy(self):
        self.proxy = SliderProxy(Layouter=self)
        self.root.Children.value.append(self.proxy.root)

    def update_container(self):
        self.super(SliderLayouter).update_container()
        self.slider.ContainerWidth.connect_from(self.Container.value.Width)

    def update_dimensions(self):
        self.label.OffsetX.value = self.icon.Width.value + self.IconPadding.value
        if len(self.Title.value)>0:
            self.slider.Position.value = avango.osg.Vec3(self.label.OffsetX.value, -self.label.get_height(), 0)
            self.Height.value = max(self.icon.Height.value, self.label.get_height() + self.slider.SliderBarHeight.value)
        else:
            self.slider.Position.value = avango.osg.Vec3(self.label.OffsetX.value,
                                                         -max((self.IconSize.value - self.slider.SliderBarHeight.value)/2,0),
                                                          0)
            self.Height.value = max(self.icon.Height.value, self.slider.SliderBarHeight.value)

        self.Width.value = max(self.label.OffsetX.value + self.label.get_width(),
                               self.label.OffsetX.value + self.SliderBarMinLength.value)

    def cleanup(self):
        self.disconnect_all_fields()
        self.slider.cleanup()
        self.super(SliderLayouter).pushbuttonlayouter_cleanup()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "SliderLayouter deleted"

class _ContainerAlignedSliderLayouter(avango.script.Script):
    Min = avango.SFFloat()
    Max = avango.SFFloat()
    Value = avango.SFFloat()
    ValueFraction = avango.SFFloat()
    ContainerWidth = avango.SFFloat()
    SliderBarFilenames = avango.MFString()
    SliderBarHeight = avango.SFFloat()
    SliderBarMinLength = avango.SFFloat()
    SliderBarMaxLength = avango.SFFloat()
    SliderBarBorderWidth = avango.SFFloat()
    SliderBarBorderColor = avango.osg.SFVec4()
    Position = avango.osg.SFVec3()

    def __init__(self):
        self.super(_ContainerAlignedSliderLayouter).__init__()

        self.background_border = avango.osg.nodes.Quad()
        self.background_border.Color.connect_from(self.SliderBarBorderColor)
        self.background = avango.osg.nodes.TexturedQuad()
        self.foreground = avango.osg.nodes.TexturedQuad()
        self.geode = avango.osg.nodes.LayerGeode(Drawables=[self.background_border,
                                                            self.background,
                                                            self.foreground
                                                            ])
        self.root = avango.osg.nodes.MatrixTransform(Children=[self.geode])
        self.slider_length = 0
        self.dimensions_dirty = False
        self.slider_bar_filenames_dirty = False
        self.slider_bar_value_dirty = False

        # this is a workaround of a GL state bug in LayerGeode.
        self.background_border.StateSet.value = avango.osg.nodes.StateSet(PolygonOffsetMode=1)

    @field_has_changed(ContainerWidth)
    def container_width_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(SliderBarHeight)
    def slider_bar_height_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(SliderBarMinLength)
    def slider_bar_min_length_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(SliderBarMaxLength)
    def slider_bar_max_length_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(Position)
    def position_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(SliderBarFilenames)
    def slider_bar_filenames_changed(self):
        self.slider_bar_filenames_dirty = True

    @field_has_changed(Min)
    def min_changed(self):
        self.slider_bar_value_dirty = True

    @field_has_changed(Max)
    def max_changed(self):
        self.slider_bar_value_dirty = True

    @field_has_changed(Value)
    def value_changed(self):
        self.slider_bar_value_dirty = True

    def evaluate(self):
        if self.slider_bar_filenames_dirty:
            self.update_slider_textures()
            self.slider_bar_filenames_dirty = False
        if self.dimensions_dirty:
            self.update_dimensions()
            self.dimensions_dirty = False
        if self.slider_bar_value_dirty:
            self.update_slider_value()
            self.slider_bar_value_dirty = False

    def update_slider_textures(self):
        self.background.Filename.value = self.SliderBarFilenames.value[0]
        self.foreground.Filename.value = self.SliderBarFilenames.value[1]

    def update_dimensions(self):
        # Position and Size of Quad, that draws the background border
        self.background_border_width = max(min(self.ContainerWidth.value - self.Position.value.x, self.SliderBarMaxLength.value),
                                           self.SliderBarMinLength.value)
        self.background_border.Width.value = self.background_border_width
        self.background_border.Height.value = self.SliderBarHeight.value
        self.background_border.Position.value = self.Position.value

        # Position and Size of slider background
        self.background_width = self.background_border_width - 2 * self.SliderBarBorderWidth.value
        self.background_height = self.SliderBarHeight.value - 2 * self.SliderBarBorderWidth.value
        self.background_position = self.Position.value + avango.osg.Vec3(self.SliderBarBorderWidth.value,
                                                                         -self.SliderBarBorderWidth.value,
                                                                         0)
        self.background.Width.value = self.background_width
        self.background.Height.value = self.background_height
        self.background.Position.value = self.background_position

        # Position and Size of slider foreground (= the slider value bar)
        self.foreground.Height.value = self.background_height
        self.foreground.Position.value = self.background_position
        self.slider_bar_value_dirty = True

    def update_slider_value(self):
        # clamp value to allowed range
        if self.Value.value < self.Min.value or self.Value.value > self.Max.value:
            self.Value.value = min(max(self.Min.value, self.Value.value), self.Max.value)

        # Length of slider value bar
        value_fraction= (self.Value.value - self.Min.value) / (self.Max.value - self.Min.value)
        self.foreground.Width.value = value_fraction * self.background_width

    def cleanup(self):
        self.disconnect_all_fields()
        self.background_border.disconnect_all_fields()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "_ContainerAlignedSliderLayouter deleted"


class SliderProxy(Proxy):
    def add_behavior(self):
        self.click_behavior = SliderProxyBehavior(Proxy=self)
        self.geode.add_and_init_field(avango.script.SFObject(), "AvangoMenuBehavior", self.click_behavior)


class SliderProxyBehavior(ProxyBehavior):
    @field_has_changed(ProxyBehavior.TargetHolder)
    def target_holder_changed(self):
        self.Caller.value.SliderInteractor.value.Layouter.value = self.Proxy.value.Layouter.value
        self.Caller.value.SliderInteractor.value.TargetHolder.value = self.TargetHolder.value

    def cleanup(self):
        self.super(SliderProxyBehavior).cleanup()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "SliderProxyBehavior deleted"
