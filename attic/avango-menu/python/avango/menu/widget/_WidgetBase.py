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

import avango.script

class WidgetBase(avango.script.Script):

    Highlighted = avango.SFBool()
    Select = avango.SFBool()
    Enable = avango.SFBool()
    ValueUp = avango.SFBool()
    ValueDown = avango.SFBool()
    TextOnly = avango.SFBool() # only supported by PushButton

    def __init__(self):
        self.super(WidgetBase).__init__()

        self.Name.value = ""
        self.Highlighted.value = False
        self.Select.value = False
        self.Enable.value = True

    def create_layouter(self):
        assert 0, "avango.menu.layout.WidgetBase.create_layouter() must be implemented in Subclass"

    def cleanup(self):
        self.widgetbase_cleanup()

    def widgetbase_cleanup(self):
        self.disconnect_all_fields()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "WidgetBase deleted"
