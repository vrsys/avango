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

from avango.menu.widget import WidgetBase
import avango.menu.Preferences
import avango.script
from avango.script import field_has_changed
import avango.osg

class Container(WidgetBase):
    '''
    Widget container.
    Orientation = 1: Vertical (default)
                  2: Horizontal
    '''
    Orientation = avango.SFInt()

    def __init__(self):
        self.super(Container).__init__()
        self.widgets = []
        self.container_layouters = []
        self.Orientation.value = 1

    def add_widget(self, widget):
        self.widgets.append(widget)
        for layouter in self.container_layouters:
            layouter.add_widget(widget)

    def add_widgets(self, widgetlist):
        for widget in widgetlist:
            self.add_widget(widget)

    def remove_widget(self, widget):
        while widget in self.widgets:
            self.widgets.remove(widget)
        for layouter in self.container_layouters:
            layouter.remove_widget(widget)

    def remove_widgets(self, widgetlist):
        for widget in widgetlist:
            self.remove_widget(widget)

    def remove_all_widgets(self):
        while len(self.widgets) != 0:
            self.remove_widget(self.widgets[0])

    def create_layouter(self):
        layouter = avango.menu.layout.ContainerLayouter(Widget=self, ContainerEnable=self.Enable.value)
        return layouter

    @field_has_changed(WidgetBase.Enable)
    def enable_changed(self):
        container_layouters = getattr(self, 'container_layouters', [])
        for layouter in container_layouters:
            layouter.Enable.value = self.Enable.value

    def cleanup(self):
        self.disconnect_all_fields()
        for layouter in self.container_layouters:
            layouter.cleanup()
        self.container_layouters = []
        self.widgets = []
        self.super(Container).widgetbase_cleanup()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "Container deleted"
