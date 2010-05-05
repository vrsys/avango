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
import avango.menu.Preferences

class CheckBoxLayouter(PushButtonLayouter):
    '''
    Layouter for CheckBox and RadioButton
    '''
    CheckState = avango.SFBool()

    def init_widget(self):
        self.super(CheckBoxLayouter).init_widget()
        self.CheckState.connect_from(self.Widget.value.CheckState)

    @field_has_changed(CheckState)
    def check_state_changed(self):
        if self.CheckState.value:
            self.icon.Switch.value = 2
        else:
            self.icon.Switch.value = 1

    def cleanup(self):
        self.disconnect_all_fields()
        self.super(CheckBoxLayouter).pushbuttonlayouter_cleanup()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "CheckBoxLayouter deleted"
