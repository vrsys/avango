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

from environment import *

_do_parse_config = True
_do_recurse = True

class Config(object):
    def get_library_path(self):
        return []

    def get_include_path(self):
        return []

    def get_python_path(self):
        return []

    def get_libraries(self):
        return []

class PlainConfig(Config):

    def __init__(self, library_path = [], include_path = [], python_path = [], libraries = []):
        self._library_path = library_path
        self._include_path = include_path
        self._python_path = python_path
        self._libraries = libraries

    def get_library_path(self):
        return self._library_path

    def get_include_path(self):
        return self._include_path

    def get_python_path(self):
        return self._python_path

    def get_libraries(self):
        return self._libraries

class PKGConfig(Config):

    def __init__(self, name):
        self._name = name

    def get_library_path(self):
        if not _do_parse_config:
            return []
        return Environment.env.ParseFlags('!pkg-config --libs-only-L %s' % self._name)['LIBPATH']

    def get_include_path(self):
        if not _do_parse_config:
            return []
        return Environment.env.ParseFlags('!pkg-config --cflags-only-I %s' % self._name)['CPPPATH']

    def get_libraries(self):
        if not _do_parse_config:
            return []
        return Environment.env.ParseFlags('!pkg-config --libs-only-l %s' % self._name)['LIBS']

