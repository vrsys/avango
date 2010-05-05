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

from avango.menu.widget import WidgetBase
import avango.script
from avango.script import field_has_changed
import avango.menu.Preferences
import avango.osg

class Slider(WidgetBase):
    Min = avango.SFFloat()
    Max = avango.SFFloat()
    Value = avango.SFFloat()
    Step = avango.SFFloat()
    SliderBarFilenames = avango.MFString()
    SliderBarHeight = avango.SFFloat()
    SliderBarMinLength = avango.SFFloat()
    SliderBarMaxLength = avango.SFFloat()
    SliderBarBorderWidth = avango.SFFloat()
    SliderBarBorderColor = avango.osg.SFVec4()
    Title = avango.SFString()
    IconFilenames = avango.MFString()
    IconSize = avango.SFFloat()
    IconPadding = avango.SFFloat()
    IconColor = avango.osg.SFVec4()
    IconDisabledColor = avango.osg.SFVec4()
    TextSize = avango.SFFloat()
    TextColor = avango.osg.SFVec4()
    TextDisabledColor = avango.osg.SFVec4()
    TextFontname = avango.SFString()

    def __init__(self):
        self.super(Slider).__init__()

        # preferences field connections
        self.IconSize.connect_from(avango.menu.Preferences.widget.IconSize)
        self.IconPadding.connect_from(avango.menu.Preferences.widget.IconPadding)
        self.IconColor.connect_from(avango.menu.Preferences.widget.IconColor)
        self.IconDisabledColor.connect_from(avango.menu.Preferences.widget.IconDisabledColor)
        self.TextSize.connect_from(avango.menu.Preferences.widget.SliderTextSize)
        self.TextColor.connect_from(avango.menu.Preferences.widget.TextColor)
        self.TextDisabledColor.connect_from(avango.menu.Preferences.widget.TextDisabledColor)
        self.TextFontname.connect_from(avango.menu.Preferences.widget.TextFontname)
        self.SliderBarFilenames.connect_from(avango.menu.Preferences.widget.SliderBarFilenames)
        self.SliderBarHeight.connect_from(avango.menu.Preferences.widget.SliderBarHeight)
        self.SliderBarMinLength.connect_from(avango.menu.Preferences.widget.SliderBarMinLength)
        self.SliderBarMaxLength.connect_from(avango.menu.Preferences.widget.SliderBarMaxLength)
        self.SliderBarBorderWidth.connect_from(avango.menu.Preferences.widget.SliderBarBorderWidth)
        self.SliderBarBorderColor.connect_from(avango.menu.Preferences.widget.SliderBarBorderColor)
        self.Step.connect_from(avango.menu.Preferences.widget.SliderStep)

        self.init_defaults()

    def init_defaults(self):
        self.Title.value = avango.menu.Preferences.slider_text
        self.IconFilenames.value = avango.menu.Preferences.slider_icon
        self.Min.value = avango.menu.Preferences.slider_min
        self.Max.value = avango.menu.Preferences.slider_max
        self.Value.value = avango.menu.Preferences.slider_value

    @field_has_changed(WidgetBase.ValueUp)
    def value_up_changed(self):
        self.Select.value = self.ValueUp.value or self.ValueDown.value
        if not self.ValueUp.value:
            return
        new_value = self.Value.value + self.Step.value
        new_value = min(self.Max.value, new_value)
        if new_value != self.Value.value:
            self.Value.value = new_value

    @field_has_changed(WidgetBase.ValueDown)
    def value_down_changed(self):
        self.Select.value = self.ValueUp.value or self.ValueDown.value
        if not self.ValueDown.value:
            return
        new_value = self.Value.value - self.Step.value
        new_value = max(self.Min.value, new_value)
        if new_value != self.Value.value:
            self.Value.value = new_value

    def create_layouter(self):
        layouter = avango.menu.layout.SliderLayouter(Widget=self)
        return layouter

    def cleanup(self):
        self.disconnect_all_fields()
        self.super(Slider).widgetbase_cleanup()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "Slider Deleted"
