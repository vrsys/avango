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
from avango.script import field_has_changed
import avango.osg
from _LayoutBase import LayoutBase

class DividerLayouter(LayoutBase):
    '''
    Layouter for simple Divider widget drawing a horizontal or vertical
    line, depending on container orientation.
    '''
    Orientation = avango.SFInt()
    Thickness = avango.SFFloat()
    Color = avango.osg.SFVec4()

    def __init__(self):
        self.super(DividerLayouter).__init__()
        self.divider = _ContainerAlignedDividerLayouter()
        self.divider.Thickness.connect_from(self.Thickness)
        self.divider.Color.connect_from(self.Color)
        self.root.Children.value.append(self.divider.root)

    def init_widget(self):
        widget = self.Widget.value
        self.Thickness.connect_from(widget.Thickness)
        self.Color.connect_from(widget.Color)

    def update_container(self):
        container = self.Container.value
        self.divider.Orientation.connect_from(container.Orientation)
        self.divider.ContainerWidth.connect_from(container.Width)
        self.divider.ContainerHeight.connect_from(container.Height)
        self.Orientation.disconnect()
        self.Orientation.enable_dependency(False)
        self.Orientation.connect_from(container.Orientation)
        # self.super(DividerLayouter).update_container() # do not call to prevent creation of proxy geometry.
        # without proxy, a menu entry is not highlightable and not clickable

    @field_has_changed(Thickness)
    def thickness_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(Orientation)
    def thickness_changed(self):
        self.dimensions_dirty = True

    def update_dimensions(self):
        if self.Container.value is None:
            return
        if self.Container.value.Orientation.value == 1: # Vertical
            self.Width.value = 0
            self.Height.value = self.Thickness.value
        elif self.Container.value.Orientation.value == 2: # Horizontal
            self.Width.value = self.Thickness.value
            self.Height.value = 0

    def cleanup(self):
        self.disconnect_all_fields()
        self.divider.cleanup()
        self.super(DividerLayouter).layoutbase_cleanup()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "DividerLayouter deleted"

class _ContainerAlignedDividerLayouter(avango.script.Script):
    Thickness = avango.SFFloat()
    Orientation = avango.SFInt()
    ContainerWidth = avango.SFFloat()
    ContainerHeight = avango.SFFloat()
    Color = avango.osg.SFVec4()

    def __init__(self):
        self.super(_ContainerAlignedDividerLayouter).__init__()

        self.orientation_dirty = False
        self.quad = avango.osg.nodes.Quad()
        self.root = avango.osg.nodes.Geode()
        self.root.Drawables.value.append(self.quad)
        self.quad.Color.connect_from(self.Color)

    @field_has_changed(Orientation)
    def orientation_changed(self):
        self.orientation_dirty = True

    def evaluate(self):
        if self.orientation_dirty:
            self.update_orientation()
        self.orientation_dirty = False

    def update_orientation(self):
        self.quad.Width.disconnect()
        self.quad.Height.disconnect()
        if self.Orientation.value == 1: # Vertical
            self.quad.Width.connect_from(self.ContainerWidth)
            self.quad.Height.connect_from(self.Thickness)
        elif self.Orientation.value == 2: # Horizontal
            self.quad.Width.connect_from(self.Thickness)
            self.quad.Height.connect_from(self.ContainerHeight)

    def cleanup(self):
        self.disconnect_all_fields()
        if self.quad is not None:
            self.quad.disconnect_all_fields()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "_ContainerAlignedDividerLayouter deleted"
