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
import avango.menu.Preferences
import avango.osg

class PushButton(WidgetBase):
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

    def __init__(self):
        self.super(PushButton).__init__()

        # preferences field connections
        self.IconSize.connect_from(avango.menu.Preferences.widget.IconSize)
        self.IconPadding.connect_from(avango.menu.Preferences.widget.IconPadding)
        self.IconColor.connect_from(avango.menu.Preferences.widget.IconColor)
        self.IconDisabledColor.connect_from(avango.menu.Preferences.widget.IconDisabledColor)
        self.TextSize.connect_from(avango.menu.Preferences.widget.TextSize)
        self.TextColor.connect_from(avango.menu.Preferences.widget.TextColor)
        self.TextDisabledColor.connect_from(avango.menu.Preferences.widget.TextDisabledColor)
        self.TextFontname.connect_from(avango.menu.Preferences.widget.TextFontname)
        self.CloseOnRelease.value = avango.menu.Preferences.pushbutton_close_on_release
        self.TextOnly.connect_from(avango.menu.Preferences.widget.TextOnly)

        self.init_defaults()

    def init_defaults(self):
        self.Title.value = avango.menu.Preferences.pushbutton_text
        self.IconFilenames.value = avango.menu.Preferences.pushbutton_icon

    def create_layouter(self):
        layouter = avango.menu.layout.PushButtonLayouter(Widget=self)
        return layouter

    def cleanup(self):
        self.pushbutton_cleanup()

    def pushbutton_cleanup(self):
        self.disconnect_all_fields()
        self.super(PushButton).widgetbase_cleanup()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "PushButton Deleted"
