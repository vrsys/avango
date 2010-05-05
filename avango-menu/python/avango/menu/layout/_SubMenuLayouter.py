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
import avango.osg
import avango.script
from avango.script import field_has_changed
from _PushButtonLayouter import PushButtonLayouter

class SubMenuLayouter(PushButtonLayouter):
    ArrowSize = avango.SFFloat()
    ArrowPadding = avango.SFFloat()

    def __init__(self):
        self.super(SubMenuLayouter).__init__()
        self.arrow = _ContainerAlignedSubMenuLayouter()
        self.arrow.ArrowSize.connect_from(self.ArrowSize)
        self.root.Children.value.append(self.arrow.root)

    def init_widget(self):
        self.super(SubMenuLayouter).init_widget()
        widget = self.Widget.value
        self.ArrowSize.connect_from(widget.ArrowSize)
        self.ArrowPadding.connect_from(widget.ArrowPadding)
        self.arrow.ArrowFilenames.connect_from(widget.ArrowFilenames)
        self.arrow.IconColor.connect_from(self.icon.Color)

    def update_container(self):
        self.super(SubMenuLayouter).update_container()
        self.arrow.ContainerWidth.connect_from(self.Container.value.Width)

    def update_dimensions(self):
        self.super(SubMenuLayouter).update_dimensions()
        self.Width.value += self.ArrowPadding.value + self.ArrowSize.value
        self.Height.value = max(self.Height.value, self.ArrowSize.value)

    def cleanup(self):
        self.disconnect_all_fields()
        self.arrow.cleanup()
        self.super(SubMenuLayouter).pushbuttonlayouter_cleanup()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "SubMenuLayouter deleted"

class _ContainerAlignedSubMenuLayouter(avango.script.Script):
    ContainerWidth = avango.SFFloat()
    ArrowSize = avango.SFFloat()
    ArrowFilenames = avango.MFString()
    IconColor = avango.osg.SFVec4()

    def __init__(self):
        self.super(_ContainerAlignedSubMenuLayouter).__init__()

        self.dimensions_dirty = False
        self.icon = self.create_icon()
        self.root = self.icon.root

    def create_icon(self):
        icon = avango.menu._Icon.Icon()
        icon.Width.connect_from(self.ArrowSize)
        icon.Height.connect_from(self.ArrowSize)
        icon.Filenames.connect_from(self.ArrowFilenames)
        icon.Color.connect_from(self.IconColor)
        return icon

    @field_has_changed(ContainerWidth)
    def container_width_changed(self):
        self.dimensions_dirty = True

    def evaluate(self):
        if self.dimensions_dirty:
            self.update_dimensions()
            self.dimensions_dirty = False

    def update_dimensions(self):
        self.icon.Position.value = avango.osg.Vec3(self.ContainerWidth.value - self.ArrowSize.value,0,0)

    def cleanup(self):
        self.disconnect_all_fields()
        self.icon.cleanup()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "_ContainerAlignedSubMenuLayouter deleted"
