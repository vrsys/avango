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

import avango.osg
import avango.menu.Preferences
import avango.script
from avango.script import field_has_changed

class LayoutBase(avango.script.Script):
    '''
    Base class for all widget Layouters. A Layouter creates and manages all
    visible elements of a widget.
    '''
    Width = avango.SFFloat()
    Height = avango.SFFloat()
    Widget = avango.script.SFObject()
    Panel = avango.script.SFObject()
    Container = avango.script.SFObject()
    Select = avango.SFBool()
    Highlight = avango.SFBool()
    WidgetEnable = avango.SFBool()
    ContainerEnable = avango.SFBool()
    Enable = avango.SFBool()

    def __init__(self):
        self.super(LayoutBase).__init__()

        self.dimensions_dirty = False
        self.container_dirty = False
        self.widget_dirty = False
        self.root = avango.osg.nodes.MatrixTransform()
        self.Container.value = None
        self.proxy = None

        # For positioning the menu panel highlight, each layouter can store
        # it's position and number within the menu panel. These values are
        # updated during layouter traversal.
        self.position = avango.osg.Vec3(0,0,0)
        self.entry_number = 0

    def evaluate(self):
        if self.widget_dirty:
            self.update_widget()
            self.widget_dirty = False
        if self.container_dirty:
            if self.Container.value is not None:
                self.update_container()
            self.container_dirty = False
        if self.dimensions_dirty:
            self.update_dimensions()
            self.dimensions_dirty = False

    @field_has_changed(Widget)
    def widget_changed(self):
        self.widget_dirty = True

    @field_has_changed(Container)
    def container_changed(self):
        self.container_dirty = True

    @field_has_changed(Select)
    def select_changed(self):
        if self.Panel.value is not None and self.Enable.value:
            self.Panel.value.EntrySelect.value = self.Select.value

    @field_has_changed(Highlight)
    def highlight_changed(self):
        if self.Panel.value is not None:
            if self.Highlight.value and self.Enable.value:
                if self.Panel.value.HighlightItem.value != self.entry_number:
                    self.Panel.value.HighlightItem.value = self.entry_number
            else:
                self.Panel.value.HighlightItem.value = 0

    @field_has_changed(WidgetEnable)
    def widget_enable_changed(self):
        self.Enable.value = self.WidgetEnable.value and self.ContainerEnable.value

    @field_has_changed(ContainerEnable)
    def container_enable_changed(self):
        self.Enable.value = self.WidgetEnable.value and self.ContainerEnable.value

    @field_has_changed(Enable)
    def enable_changed(self):
        self.layout_base_enable_changed()

    def layout_base_enable_changed(self):
        if not self.Enable.value and self.Select.value:
            self.Select.value = False
        if self.Panel.value is not None:
            self.Panel.value.force_update_layouters()
            self.Panel.value.HighlightWidget.touch()

    def create_proxy(self):
        # Every layouter has an invisible proxy geometry, that is used for
        # highlighting and selecting the widgets with tools.
        self.proxy = avango.menu.layout.Proxy(Layouter=self)
        self.root.Children.value.append(self.proxy.root)

    def update_container(self):
        if self.Container.value:
            self.create_proxy()

    def update_widget(self):
        if self.Widget.value:
            self.init_widget()

    def update_dimensions(self):
        assert 0, "avango.menu.layout.LayoutBase.update_dimensions must be implemented in subclass"

    def init_widget(self):
        assert 0, "avango.menu.layout.LayoutBase.init_widget must be implemented in subclass"

    def cleanup(self):
        self.layoutbase_cleanup()

    def layoutbase_cleanup(self):
        self.disconnect_all_fields()
        if self.proxy is not None:
            self.proxy.cleanup()
            self.proxy = None
        self.Panel.value = None
        self.Container.value = None
        self.Widget.value = None

    def get_widget_layouters(self):
        self.entry_number = 0
        if self.Enable.value:
            return [self]
        else:
            return []

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "LayoutBase deleted"
