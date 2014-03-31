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

from avango.menu.widget import PushButton
from avango.script import field_has_changed
import avango.menu.Preferences
import avango.osg

class CheckBox(PushButton):
    CheckState = avango.SFBool()

    def __init__(self):
        self.super(CheckBox).__init__()

        self.last_select = getattr(self, 'last_select', False)
        self.CheckState.value = False

    def init_defaults(self):
        self.Title.value = avango.menu.Preferences.checkbox_text
        self.IconFilenames.value = avango.menu.Preferences.checkbox_icons
        self.CheckState.value = False

    @field_has_changed(PushButton.Select)
    def select_changed(self):
        last_select = getattr(self, 'last_select', False)
        if not last_select and self.Select.value:
            self.CheckState.value = not self.CheckState.value
        self.last_select = self.Select.value

    def create_layouter(self):
        layouter = avango.menu.layout.CheckBoxLayouter(Widget=self)
        return layouter

    def cleanup(self):
        self.disconnect_all_fields()
        self.super(CheckBox).pushbutton_cleanup()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "CheckBox deleted"
