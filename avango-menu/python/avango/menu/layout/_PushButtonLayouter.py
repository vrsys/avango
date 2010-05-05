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

import avango.menu._Icon
import avango.menu._Label
import avango.osg
import avango.script
from avango.script import field_has_changed
from _LayoutBase import LayoutBase

class PushButtonLayouter(LayoutBase):
    '''
    Layouter for simple PushButton widget with icon and text label.
    '''
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
    CloseOnRelease = avango.SFBool()
    TextOnly = avango.SFBool()

    def __init__(self):
        self.super(PushButtonLayouter).__init__()
        self.icon = self.create_icon()
        self.label = self.create_label()
        self._last_select = False
        self.CloseOnRelease.value = False

    def init_widget(self):
        widget = self.Widget.value
        self.WidgetEnable.connect_from(widget.Enable)
        self.Title.connect_from(widget.Title)
        self.IconFilenames.connect_from(widget.IconFilenames)
        self.IconSize.connect_from(widget.IconSize)
        self.IconPadding.connect_from(widget.IconPadding)
        self.IconColor.connect_from(widget.IconColor)
        self.IconDisabledColor.connect_from(widget.IconDisabledColor)
        self.TextSize.connect_from(widget.TextSize)
        self.TextColor.connect_from(widget.TextColor)
        self.TextDisabledColor.connect_from(widget.TextDisabledColor)
        self.TextFontname.connect_from(widget.TextFontname)
        self.TextOnly.connect_from(widget.TextOnly)

        # SubMenuLayouter also uses the PushButtonLayouter base class, but does not have a CloseOnRelease field
        if hasattr(widget, "CloseOnRelease"):
            self.CloseOnRelease.connect_from(widget.CloseOnRelease)

    @field_has_changed(Title)
    def title_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(IconSize)
    def icon_size_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(TextSize)
    def text_size_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(LayoutBase.Enable)
    def enable_changed(self):
        self.push_button_enable_changed()

    @field_has_changed(LayoutBase.Select)
    def select_changed(self):
        if self.Panel.value is not None and self.Enable.value:
            self.Panel.value.EntrySelect.value = self.Select.value
            if self.CloseOnRelease.value and self._last_select and not self.Select.value:
                self.Panel.value.close()
        self._last_select = self.Select.value

    def push_button_enable_changed(self):
        self.super(PushButtonLayouter).layout_base_enable_changed()
        self.label.Color.disconnect()
        self.icon.Color.disconnect()
        if self.Enable.value:
            self.label.Color.connect_from(self.TextColor)
            self.icon.Color.connect_from(self.IconColor)
        else:
            self.label.Color.connect_from(self.TextDisabledColor)
            self.icon.Color.connect_from(self.IconDisabledColor)

    def create_icon(self):
        icon = avango.menu._Icon.Icon()
        icon.Width.connect_from(self.IconSize)
        icon.Height.connect_from(self.IconSize)
        icon.Filenames.connect_from(self.IconFilenames)
        self.root.Children.value.append(icon.root)
        return icon

    def create_label(self):
        label = avango.menu._Label.Label()
        label.String.connect_from(self.Title)
        label.Size.connect_from(self.TextSize)
        label.Fontname.connect_from(self.TextFontname)
        self.root.Children.value.append(label.root)
        return label

    def update_dimensions(self):
        if not self.TextOnly.value:
            self.label.OffsetX.value = self.icon.Width.value + self.IconPadding.value
            self.Width.value = self.label.OffsetX.value + self.label.get_width()
            self.Height.value = max(self.icon.Height.value, self.label.get_height())
        else:
            self.label.OffsetX.value = 0
            self.Width.value = self.label.get_width()
            self.Height.value = self.label.get_height()
        self.label.WidgetHeight.value = self.Height.value

    def cleanup(self):
        self.pushbuttonlayouter_cleanup()

    def pushbuttonlayouter_cleanup(self):
        self.disconnect_all_fields()
        if self.icon is not None:
            self.icon.cleanup()
        if self.label is not None:
            self.label.cleanup()
        self.super(PushButtonLayouter).layoutbase_cleanup()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "PushButtonLayouter Deleted"
