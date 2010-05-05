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

import avango.script
from avango.script import field_has_changed
import Preferences

class RadioButtonGroup(avango.script.Script):
    CheckedRadioButton = avango.script.SFObject()
    RadioButtons = avango.script.MFObject()

    def __init__(self):
        self.super(RadioButtonGroup).__init__()
        self.checked_changed = False
        self.CheckedRadioButton.value = None

    @field_has_changed(CheckedRadioButton)
    def checked_radio_button_changed(self):
        self.checked_changed = True

    @field_has_changed(RadioButtons)
    def radio_buttons_changed(self):
        self.buttons_changed = True

    def evaluate(self):
        if self.checked_changed:
            self.update_checked()
            self.checked_changed = False

    def update_checked(self):
        for button in self.RadioButtons.value:
            if button == self.CheckedRadioButton.value:
                continue
            button.CheckState.value = False
        self.checked_changed = False

    def cleanup(self):
        self.disconnect_all_fields()
        self.CheckedRadioButton.value = None
        self.RadioButtons.value = []

    def __del__(self):
        if Preferences.print_destruction_of_menu_objects:
            print "RadioButtonGroup deleted"
