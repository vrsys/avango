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
import avango.menu._Label
import avango.osg
import avango.script
from avango.script import field_has_changed
from _LayoutBase import LayoutBase

class ImageLayouter(LayoutBase):
    '''
    Layouter for Image widget.
    '''
    ImageWidth = avango.SFFloat()
    ImageAspectRatio = avango.SFFloat()

    def __init__(self):
        self.super(ImageLayouter).__init__()
        self.image = _ContainerAlignedImageLayouter()
        self.root.Children.value.append(self.image.geode)

    def init_widget(self):
        widget = self.Widget.value
        self.image.ImageFilename.connect_from(widget.ImageFilename)
        self.image.ImageWidth.connect_from(widget.ImageWidth)
        self.image.ImageAspectRatio.connect_from(widget.ImageAspectRatio)
        self.image.ImageCentered.connect_from(widget.ImageCentered)

        self.ImageWidth.connect_from(widget.ImageWidth)
        self.ImageAspectRatio.connect_from(widget.ImageAspectRatio)

    def update_container(self):
        container = self.Container.value
        self.image.Orientation.connect_from(container.Orientation)
        self.image.ContainerWidth.connect_from(container.Width)
        self.image.ContainerHeight.connect_from(container.Height)
        # self.super(ImageLayouter).update_container() # do not call to prevent creation of proxy geometry.
        # without proxy, a menu entry is not highlightable and not clickable

    @field_has_changed(ImageAspectRatio)
    def image_aspect_ratio_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(ImageWidth)
    def image_width_changed(self):
        self.dimensions_dirty = True

    def update_dimensions(self):
        aspect_ratio = self.ImageAspectRatio.value
        self.Width.value = self.ImageWidth.value
        self.Height.value = self.ImageWidth.value / self.ImageAspectRatio.value

    def cleanup(self):
        self.disconnect_all_fields()
        self.image.cleanup()
        self.super(ImageLayouter).layoutbase_cleanup()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "ImageLayouter Deleted"

class _ContainerAlignedImageLayouter(avango.script.Script):
    Centered = avango.SFFloat()
    Orientation = avango.SFInt()
    ContainerWidth = avango.SFFloat()
    ContainerHeight = avango.SFFloat()
    ImageFilename = avango.SFString()
    ImageWidth = avango.SFFloat()
    ImageAspectRatio = avango.SFFloat()
    ImageCentered = avango.SFBool()

    def __init__(self):
        self.super(_ContainerAlignedImageLayouter).__init__()

        self.dimensions_dirty = False
        self.image = avango.osg.nodes.TexturedQuad()
        self.image.Filename.connect_from(self.ImageFilename)
        self.image.Width.connect_from(self.ImageWidth)
        self.geode = avango.osg.nodes.Geode(Drawables=[self.image])

    @field_has_changed(Orientation)
    def orientation_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(ContainerWidth)
    def container_width_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(ContainerHeight)
    def container_height_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(ImageWidth)
    def image_width_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(ImageAspectRatio)
    def image_aspect_ratio_changed(self):
        self.dimensions_dirty = True

    @field_has_changed(ImageCentered)
    def centered_changed(self):
        self.dimensions_dirty = True

    def evaluate(self):
        if self.dimensions_dirty:
            self.update_dimensions()
            self.dimensions_dirty = False

    def update_dimensions(self):
        self.image.Height.value = self.ImageWidth.value / self.ImageAspectRatio.value
        if self.ImageCentered.value:
            if self.Orientation.value==1: # Vertical
                offset_x = (self.ContainerWidth.value - self.image.Width.value) * 0.5
                self.image.Position.value = avango.osg.Vec3(offset_x, 0, 0)
            elif self.Orientation.value==2: # Horizontal
                offset_y = (self.ContainerHeight.value - self.image.Height.value) * -0.5
                self.image.Position.value = avango.osg.Vec3(0, offset_y, 0)

    def cleanup(self):
        self.disconnect_all_fields()
        self.image.disconnect_all_fields()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "_ContainerAlignedImageLayouter deleted"
