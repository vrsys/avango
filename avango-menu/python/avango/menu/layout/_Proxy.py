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
import avango.tools
import avango.menu.Preferences
import avango.script
from avango.script import field_has_changed

class Proxy(avango.script.Script):
    '''
    Proxy creates an invisible Quad Geometry in front of a menu widget.
    Every layouter creates a proxy for handling ray-over and picking
    of the widget.
    '''
    Layouter = avango.script.SFObject()
    Width = avango.SFFloat()
    Height = avango.SFFloat()
    Displacement = avango.SFFloat()
    VerticalPadding = avango.SFFloat()
    HorizontalPadding = avango.SFFloat()
    Color = avango.osg.SFVec4()

    def __init__(self):
        self.super(Proxy).__init__()

        self.root = avango.osg.nodes.Group()
        self.geom = avango.osg.nodes.Quad()
        self.geode = avango.osg.nodes.Geode()
        self.geode.Drawables.value = [self.geom]
        self.root.Children.value = [self.geode]
        self.Displacement.connect_from(avango.menu.Preferences.layouter.ProxyDisplacement)
        self.VerticalPadding.connect_from(avango.menu.Preferences.layouter.ProxyVerticalPadding)
        self.HorizontalPadding.connect_from(avango.menu.Preferences.layouter.ProxyHorizontalPadding)
        self.Color.connect_from(avango.menu.Preferences.layouter.ProxyColor)
        self.geode.StateSet.value = avango.osg.nodes.StateSet(CullFaceMode=1)
        self.geode.StateSet.value.CullFace.connect_from(avango.menu.Preferences.layouter.ProxyCullFace)
        self.geom.Color.connect_from(avango.menu.Preferences.layouter.ProxyColor)
        self.add_behavior()
        self.dimensions_dirty = False

    def add_behavior(self):
        self.click_behavior = ProxyBehavior(Proxy=self)
        self.geode.add_field(avango.script.SFObject(), "AvangoMenuBehavior")
        self.geode.AvangoMenuBehavior.value = self.click_behavior

    def evaluate(self):
        if self.dimensions_dirty and self.Layouter.value:
            self.update_dimensions()

    @field_has_changed(Width)
    def width_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(Height)
    def height_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(Displacement)
    def displacement_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(Layouter)
    def layouter_changed(self):
        if not self.Layouter.value:
            return
        self.Width.disconnect()
        self.Height.disconnect()
        if self.Layouter.value.Container.value.Orientation.value == 1: # Vertical
            self.Width.connect_from(self.Layouter.value.Container.value.Width)
            self.Height.connect_from(self.Layouter.value.Height)
        elif self.Layouter.value.Container.value.Orientation.value == 2: # Horizontal
            self.Width.connect_from(self.Layouter.value.Width)
            self.Height.connect_from(self.Layouter.value.Container.value.Height)

        self.dimensions_dirty = True

    def update_dimensions(self):
        self.geom.Width.value = self.Width.value + 2 * self.HorizontalPadding.value
        self.geom.Height.value = self.Height.value + 2 * self.VerticalPadding.value
        self.geom.Position.value = avango.osg.Vec3(-self.HorizontalPadding.value,
                                                   self.VerticalPadding.value,
                                                   self.Displacement.value)
        self.dimensions_dirty = False

    def cleanup(self):
        self.disconnect_all_fields()
        if self.geode is not None:
            self.geode.StateSet.value.disconnect_all_fields()
        self.Layouter.value = None
        self.click_behavior.cleanup()
        self.geode.AvangoMenuBehavior.value = None
        if self.geom is not None:
            self.geom.disconnect_all_fields()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "Proxy deleted"

class ProxyBehavior(avango.script.Script):
    '''
    The methods of this class are called by avango.menu.Tools._SelectTool and avango.menu.Tools._HighlightTool
    '''
    Proxy = avango.script.SFObject()
    TargetHolder = avango.tools.SFTargetHolder()
    Caller = avango.script.SFObject()

    def select(self):
        if self.Proxy.value==None:
            return
        layouter = self.Proxy.value.Layouter.value
        if layouter.Panel.value.Lock.value:
            return
        if layouter.Panel.value.HighlightItem.value != layouter.entry_number:
            layouter.Highlight.value = True
        layouter.Select.value = True

    def deselect(self):
        if self.Proxy.value is not None:
            self.Proxy.value.Layouter.value.Select.value = False

    def highlight(self):
        if self.Proxy.value==None or self.Proxy.value.Layouter.value.Panel.value.Lock.value:
            return
        self.Proxy.value.Layouter.value.Highlight.value = True

    def unhighlight(self):
        if self.Proxy.value==None or self.Proxy.value.Layouter.value.Panel.value.Lock.value:
            return
        self.Proxy.value.Layouter.value.Highlight.value = False

    def cleanup(self):
        self.disconnect_all_fields()
        self.Proxy.value = None

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "ProxyBehavior deleted"
