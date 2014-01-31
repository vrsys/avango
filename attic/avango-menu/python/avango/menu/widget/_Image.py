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

from avango.menu.widget import WidgetBase
import avango.menu.Preferences
import avango.osg

class Image(WidgetBase):
    ImageFilename = avango.SFString()
    ImageWidth = avango.SFFloat()
    ImageAspectRatio = avango.SFFloat()
    ImageCentered = avango.SFBool()

    def __init__(self):
        self.super(Image).__init__()
        self.ImageWidth.value = 1.0
        self.ImageCentered.value = True
        self.ImageAspectRatio.value = 1.0

    def create_layouter(self):
        layouter = avango.menu.layout.ImageLayouter(Widget=self)
        return layouter

    def cleanup(self):
        self.disconnect_all_fields()
        self.super(Image).widgetbase_cleanup()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "Image deleted"
