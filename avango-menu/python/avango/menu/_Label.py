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
import avango.script
from avango.script import field_has_changed
import Preferences

class Label(avango.script.Script):
    '''
    Text Label for Widget Layouters
    '''
    String = avango.SFString()
    Size = avango.SFFloat()
    Color = avango.osg.SFVec4()
    OffsetX = avango.SFFloat()
    OffsetY = avango.SFFloat()
    Fontname = avango.SFString()
    WidgetHeight = avango.SFFloat()
    Alignment = avango.SFInt()
    Position = avango.osg.SFVec3()

    def __init__(self):
        self.super(Label).__init__()

        self.Alignment.value = 0
        self.OffsetX.value = 0
        self.OffsetY.value = 0
        self.text = avango.osg.nodes.Text()
        self.text.Fontname.connect_from(self.Fontname)
        self.text.String.connect_from(self.String)
        self.text.Size.connect_from(self.Size)
        self.text.Color.connect_from(self.Color)
        self.text.Alignment.connect_from(self.Alignment)
        self.text.Position.connect_from(self.Position)

        self.geode = avango.osg.nodes.Geode()
        self.geode.Drawables.value.append(self.text)

        self.root = avango.osg.nodes.MatrixTransform()
        self.root.Children.value.append(self.geode)

        self.dimensions_dirty = False

    @field_has_changed(String)
    def string_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(Size)
    def size_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(Fontname)
    def fontname_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(OffsetX)
    def offset_x_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(WidgetHeight)
    def widget_height_changed(self):
        self.dimensions_dirty = True

    def evaluate(self):
        if self.dimensions_dirty:
            self.update_dimensions()
            self.dimensions_dirty = False

    def update_dimensions(self):
        offset_y = max(0, (self.WidgetHeight.value - self.get_height()) / 2)
        self.Position.value = avango.osg.Vec3(self.OffsetX.value, self.OffsetY.value - offset_y, 0)

    def get_width(self):
        return self.geode.get_bounding_box().x_max() - self.geode.get_bounding_box().x_min()

    def get_height(self):
        if '\n' in self.String.value:
            return self.geode.get_bounding_box().y_max() - self.geode.get_bounding_box().y_min()
        else:
            return self.Size.value

    def cleanup(self):
        self.String.disconnect()
        self.Size.disconnect()
        self.Color.disconnect()
        self.OffsetX.disconnect()
        self.Fontname.disconnect()
        self.WidgetHeight.disconnect()
        if self.text is not None:
            self.text.Fontname.disconnect()
            self.text.String.disconnect()
            self.text.Size.disconnect()
            self.text.Color.disconnect()
            self.text.Position.disconnect()
            self.text = None

    def __del__(self):
        if Preferences.print_destruction_of_menu_objects:
            print "Label deleted"
