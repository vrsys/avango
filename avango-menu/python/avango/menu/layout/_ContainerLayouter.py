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

from avango.menu.layout import LayoutBase
from avango.menu.widget import WidgetBase
import avango.script
from avango.script import field_has_changed
import avango.menu.Preferences

class ContainerLayouter(LayoutBase):
    Orientation = avango.SFInt()
    Widgets = avango.script.MFObject()
    Layouters = avango.script.MFObject()
    UpdateTrigger = avango.SFBool()
    Panel = avango.script.SFObject()

    # if layouter is used as internal panel container, the container should
    # at least be as wide as the panel content area
    PanelBaseWidth = avango.SFFloat()

    # Preferences Fields
    WidgetVerticalPadding = avango.SFFloat()
    WidgetHorizontalPadding = avango.SFFloat()

    def __init__(self):
        self.init_super(LayoutBase)
        self.WidgetVerticalPadding.connect_from(avango.menu.Preferences.container.WidgetVerticalPadding)
        self.WidgetHorizontalPadding.connect_from(avango.menu.Preferences.container.WidgetHorizontalPadding)
        self.Orientation.value = 1
        self.entries_dirty = False
        self.orientation_dirty = False
        self.UpdateTrigger.value = False
        self.PanelBaseWidth.value = 0

    def init_widget(self):
        widget = self.Widget.value
        self.Orientation.connect_from(widget.Orientation)
        self.Widget.value.container_layouters.append(self)
        self.WidgetEnable.connect_from(widget.Enable)
        for widget in widget.widgets:
            self.add_widget(widget)

    @field_has_changed(Panel)
    def panel_touched(self):
        for layouter in self.Layouters.value:
            layouter.Panel.value = self.Panel.value

    @field_has_changed(UpdateTrigger)
    def update_trigger_touched(self):
        self.dimensions_dirty = True

    @field_has_changed(Orientation)
    def orientation_touched(self):
        self.orientation_dirty = True

    @field_has_changed(PanelBaseWidth)
    def panel_base_width_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(LayoutBase.Enable)
    def enable_changed(self):
        for layouter in self.Layouters.value:
            layouter.ContainerEnable.value = self.Enable.value

    # do not create proxy as in LayoutBase class
    def update_container(self): pass

    def evaluate(self):
        if self.entries_dirty:
            self.update_entries()
            self.entries_dirty = False
        if self.orientation_dirty:
            self.update_orientation()
            self.orientation_dirty = False
        self.super().evaluate()

    def update_orientation(self):
        # clamp to allowed values
        if self.Orientation.value not in [1,2]:
            self.Orientation.value = 1
        self.dimensions_dirty = True

    def update_dimensions(self):
        width = 0
        height = 0
        if self.Orientation.value == 1:
            for layouter in self.Layouters.value:
                layouter.root.Matrix.value = avango.osg.make_trans_mat(0,-height,0)
                width = max(width, layouter.Width.value)
                height += layouter.Height.value + self.WidgetVerticalPadding.value
            if len(self.Layouters.value) > 0:
                height -= self.WidgetVerticalPadding.value
        elif self.Orientation.value == 2:
            for layouter in self.Layouters.value:
                layouter.root.Matrix.value = avango.osg.make_trans_mat(width,0,0)
                height = max(height, layouter.Height.value)
                width += layouter.Width.value + self.WidgetHorizontalPadding.value
            if len(self.Layouters.value) > 0:
                width -= self.WidgetHorizontalPadding.value
        self.Width.value = max(width, self.PanelBaseWidth.value)
        self.Height.value = height

    def add_widget(self, widget):
        self.Widgets.value.append(widget)
        layouter = widget.create_layouter()
        layouter.Panel.value = self.Panel.value
        layouter.Container.value = self
        layouter.ContainerEnable.value = self.Enable.value
        self.Layouters.value.append(layouter)
        self.UpdateTrigger.connect_from(layouter.Width)
        self.UpdateTrigger.connect_from(layouter.Height)
        self.root.Children.value.append(layouter.root)

    def add_widgets(self, widgetlist):
        for widget in widgetlist:
            self.add_widget(widget)

    def remove_widget(self, widget):
        while widget in self.Widgets.value:
            self.Widgets.value.remove(widget)
        layouter_to_be_romoved = []
        for layouter in self.Layouters.value:
            if layouter.Widget.value == widget:
                layouter_to_be_romoved.append(layouter)
        for layouter in layouter_to_be_romoved:
            self.Layouters.value.remove(layouter)
            self.root.Children.value.remove(layouter.root)
            self.UpdateTrigger.disconnect_from(layouter.Width)
            self.UpdateTrigger.disconnect_from(layouter.Height)
            layouter.cleanup()
        self.UpdateTrigger.touch()

    def remove_widgets(self, widgetlist):
        for widget in widgetlist:
            self.remove_widget(widget)

    def remove_all_widgets(self):
        while len(self.Widgets.value) != 0:
            self.remove_widget(self.Widgets.value[0])

    def get_widget_layouters(self):
        list = []
        for layouter in self.Layouters.value:
            layouter.position = self.position + layouter.root.Matrix.value.get_translate()
            list += layouter.get_widget_layouters()
        return list

    def create_layouter(self):
        return self

    def cleanup(self):
        self.disconnect_all_fields()
        if self.Widget.value is not None and self in self.Widget.value.container_layouters:
            self.Widget.value.container_layouters.remove(self)
        for widget in self.Widgets.value:
            self.remove_widget(widget)
        self.Widgets.value = []
        self.Widget.value = None

        for layouter in self.Layouters.value:
            layouter.cleanup()
        self.Layouters.value = []

        self.Panel.value = None
        self.super().layoutbase_cleanup()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "ContainerLayouter deleted"
