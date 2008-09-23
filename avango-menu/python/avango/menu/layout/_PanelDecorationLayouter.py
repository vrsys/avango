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

import avango.menu._Icon
import avango
import avango.script
from avango.script import field_has_changed
from _LayoutBase import LayoutBase

class PanelDecorationLayouter(LayoutBase):
    '''
    Layouter for panel decorations (back button, close button)
    '''
    IconFilenames = avango.MFString()
    IconSize = avango.SFFloat()
    Layouter = avango.script.SFObject()

    def __init__(self):
        self.callback = None
        self.init_super(LayoutBase)
        self.icon = self.create_icon()
        self.proxy = PanelDecorationProxy(Layouter=self)
        self.proxy.Width.connect_from(self.IconSize)
        self.proxy.Height.connect_from(self.IconSize)
        self.root.Children.value.append(self.proxy.root)
        self.Select.value = False

    def init_widget(self):
        pass

    def create_icon(self):
        icon = avango.menu._Icon.Icon()
        icon.Width.connect_from(self.IconSize)
        icon.Height.connect_from(self.IconSize)
        icon.Filenames.connect_from(self.IconFilenames)
        self.root.Children.value.append(icon.root)
        return icon

    def update_dimensions(self):
        self.Width.value = self.IconSize.value
        self.Height.value = self.IconSize.value

    @field_has_changed(LayoutBase.Highlight)
    def highlight_changed(self):
        if self.Highlight.value:
            self.icon.Switch.value = 2
        else:
            self.icon.Switch.value = 1

    @field_has_changed(LayoutBase.Select)
    def select_changed(self):
        if self.callback is not None and self.Select.value:
            self.callback()
        self.super().select_changed()

    def create_proxy(self):
        pass

    def cleanup(self):
        self.paneldecorationlayouter_cleanup()

    def paneldecorationlayouter_cleanup(self):
        self.disconnect_all_fields()
        if self.icon is not None:
            self.icon.cleanup()
        if self.proxy is not None:
            self.proxy.cleanup()
        self.super().layoutbase_cleanup()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "PanelDecorationLayouter Deleted"

class PanelDecorationProxy(avango.script.Script):
    Layouter = avango.script.SFObject()
    Width = avango.SFFloat()
    Height = avango.SFFloat()
    Displacement = avango.SFFloat()
    Color = avango.osg.SFVec4()

    def __init__(self):
        self.root = avango.osg.nodes.Group()
        self.geom = avango.osg.nodes.Quad()
        self.geode = avango.osg.nodes.Geode()
        self.geode.Drawables.value = [self.geom]
        self.root.Children.value = [self.geode]
        self.Displacement.connect_from(avango.menu.Preferences.layouter.ProxyDisplacement)
        self.Color.connect_from(avango.menu.Preferences.layouter.ProxyColor)
        self.geode.StateSet.value = avango.osg.nodes.StateSet(CullFaceMode=1)
        self.geode.StateSet.value.CullFace.connect_from(avango.menu.Preferences.layouter.ProxyCullFace)
        self.geom.Color.connect_from(avango.menu.Preferences.layouter.ProxyColor)
        self.click_behavior = PanelDecorationProxyBehavior(Proxy=self)
        self.geode.add_field(avango.script.SFObject(), "AvangoMenuBehavior")
        self.geode.AvangoMenuBehavior.value = self.click_behavior
        self.dimensions_dirty = False

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
        self.Width.connect_from(self.Layouter.value.Width)
        self.Height.connect_from(self.Layouter.value.Height)
        self.dimensions_dirty = True

    def update_dimensions(self):
        self.geom.Width.value = self.Width.value
        self.geom.Height.value = self.Height.value
        self.geom.Position.value = avango.osg.Vec3(0, 0, self.Displacement.value)
        self.dimensions_dirty = False

    def cleanup(self):
        self.disconnect_all_fields()
        if self.geode is not None:
            self.geode.StateSet.value.disconnect_all_fields()
        self.Layouter.value = None
        self.click_behavior.cleanup()
        self.geode.AvangoMenuBehavior.value = None

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "PanelDecorationProxy deleted"


class PanelDecorationProxyBehavior(avango.script.Script):
    Proxy = avango.script.SFObject()
    TargetHolder = avango.tools.SFTargetHolder()
    Caller = avango.script.SFObject()

    def select(self):
        layouter = self.Proxy.value.Layouter.value
        if layouter.Panel.value.Lock.value:
            return
        layouter.Panel.value.HighlightItem.value = 0
        layouter.Panel.value.HighlightWidget.value = None
        layouter.Select.value = True

    def deselect(self):
        self.Proxy.value.Layouter.value.Select.value = False
        self.Proxy.value.Layouter.value.Panel.value.HighlightWidget.value = None

    def highlight(self):
        layouter = self.Proxy.value.Layouter.value
        if layouter.Panel.value.Lock.value:
            return
        if layouter.Panel.value.HighlightItem.value!=0:
            layouter.Panel.value.HighlightItem.value = 0
        layouter.Highlight.value = True

    def unhighlight(self):
        if self.Proxy.value.Layouter.value.Panel.value.Lock.value:
            return
        self.Proxy.value.Layouter.value.Highlight.value = False

    def cleanup(self):
        self.disconnect_all_fields()
        self.Proxy.value = None

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "PanelDecorationProxyBehavior deleted"
