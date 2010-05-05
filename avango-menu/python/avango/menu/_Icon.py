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

import avango.osg
import avango.script
from avango.script import field_has_changed
import Preferences

class Icon(avango.script.Script):
    '''
    Icon creates textured quads with textures, that
    can be positioned, resized and switched.
    '''
    Width = avango.SFFloat()
    Height = avango.SFFloat()
    Position = avango.osg.SFVec3()
    Filenames = avango.MFString()
    Color= avango.osg.SFVec4()
    Switch = avango.SFInt()

    def __init__(self):
        self.super(Icon).__init__()

        self.Width.value = 1
        self.Height.value = 1
        self.Filenames.value = []
        self.Color.value = avango.osg.Vec4(1,1,1,1)
        self.Position.value = avango.osg.Vec3(0,0,0)
        self.Switch.value = 1
        self.geodes = []
        self.quads = []
        self.root = avango.osg.nodes.Switch()
        self.root.VisibleChild.connect_from(self.Switch)
        self.filenames_dirty = False

    @field_has_changed(Filenames)
    def set_filenames(self):
        self.filenames_dirty = True

    def evaluate(self):
        if self.filenames_dirty:
            self.build_icons()
            self.filenames_dirty = False

    def delete_all_icons(self):
        for geode in self.geodes:
            self.root.Children.value.remove(geode)
            geode.Drawables.value = []
        for quad in self.quads:
            quad.disconnect_all_fields()
        self.geodes = []
        self.quads = []

    def build_icons(self):
        self.delete_all_icons()
        for filename in self.Filenames.value:
            geode = avango.osg.nodes.Geode()
            quad = avango.osg.nodes.TexturedQuad()
            quad.Filename.value = filename
            quad.Width.connect_from(self.Width)
            quad.Height.connect_from(self.Height)
            quad.Position.connect_from(self.Position)
            quad.Color.connect_from(self.Color)
            geode.Drawables.value = [quad]
            self.root.Children.value.append(geode)
            self.geodes.append(geode)
            self.quads.append(quad)

    def cleanup(self):
        self.delete_all_icons()
        self.disconnect_all_fields()

    def __del__(self):
        if Preferences.print_destruction_of_menu_objects:
            print "Icon deleted"
