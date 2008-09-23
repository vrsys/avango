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

from avango.menu.widget import PushButton
from avango.script import field_has_changed
import avango.menu.Preferences
import avango.osg

class RadioButton(PushButton):
    CheckState = avango.SFBool()
    Group = avango.script.SFObject()

    def __init__(self):
        self.last_select = False
        self.CheckState.value = False
        self.init_super(PushButton)

    def init_defaults(self):
        self.Title.value = avango.menu.Preferences.radiobutton_text
        self.IconFilenames.value = avango.menu.Preferences.radiobutton_icons
        self.CheckState.value = False
        self.Group.value = None

    @field_has_changed(PushButton.Select)
    def select_changed(self):
        if not self.last_select and self.Select.value and not self.CheckState.value:
            self.CheckState.value = True
        self.last_select = self.Select.value

    @field_has_changed(CheckState)
    def state_changed(self):
        if self.Group.value is not None and self.CheckState.value:
            self.Group.value.CheckedRadioButton.value = self

    @field_has_changed(Group)
    def group_changed(self):
        if self.Group.value is not None:
            self.Group.value.RadioButtons.value.append(self)

    def create_layouter(self):
        # CheckBoxLayouter is used, as there is no difference in appearance
        # to a Radiobutton except other icons
        layouter = avango.menu.layout.CheckBoxLayouter(Widget=self)
        return layouter

    def cleanup(self):
        self.disconnect_all_fields()
        #self.super().cleanup()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "RadioButton deleted"
