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
from configstore import _config_store
import distutils.sysconfig
import SCons.Script as scons

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

    def __init__(self, library_path = [], include_path = [], python_path = [], libraries = [], dependencies = []):
        self._library_path = library_path
        self._include_path = include_path
        self._python_path = python_path
        self._libraries = libraries
        self._dependencies = dependencies

    def _join_dependencies(self, retriever):
        result = []
        for d in self._dependencies:
            result += retriever(d)
        return result

    def get_library_path(self):
        return self._library_path+self._join_dependencies(_config_store.get_library_path)

    def get_include_path(self):
        return self._include_path+self._join_dependencies(_config_store.get_include_path)

    def get_python_path(self):
        return self._python_path+self._join_dependencies(_config_store.get_python_path)

    def get_libraries(self):
        return self._libraries+self._join_dependencies(_config_store.get_libraries)

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

class PythonConfig(Config):

    def get_library_path(self):
        result = distutils.sysconfig.get_config_var('LIBDIR')
        if not result:
            return []
        return [result]

    def get_include_path(self):
        result = distutils.sysconfig.get_python_inc()
        if not result:
            return []
        return [result]

    def get_libraries(self):
        env = scons.Environment()
        flags = env.ParseFlags(distutils.sysconfig.get_config_var('BLDLIBRARY'))
        return flags['LIBS']

