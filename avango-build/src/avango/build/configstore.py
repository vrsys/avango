# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
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

class ConfigStore(object):

    def __init__(self):
        self._store = {}

    def set(self, key, config):
        self._store[key] = config

    def get_library_path(self, key):
        return self._store[key].get_library_path()

    def get_include_path(self, key):
        return self._store[key].get_include_path()

    def get_python_path(self, key):
        return self._store[key].get_python_path()

    def get_libraries(self, key):
        return self._store[key].get_libraries()

_config_store = ConfigStore()
