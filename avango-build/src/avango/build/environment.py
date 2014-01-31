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

from configstore import _config_store
import oshelper


class PathManager(object):
    def __init__(self):
        self._local_library_path = []
        self._global_library_path = []
        self._local_include_path = []
        self._global_include_path = []
        self._local_python_path = []
        self._global_python_path = []
        self._local_libraries = []
        self._global_libraries = []

    def _prepend_path(self, env, path):
        if isinstance(path, str):
            path = [ path ]
        env[:0] = path

    def prepend_local_library_path(self, path):
        self._prepend_path(self._local_library_path, path)

    def prepend_global_library_path(self, path):
        self._prepend_path(self._global_library_path, path)

    def prepend_local_include_path(self, path):
        self._prepend_path(self._local_include_path, path)

    def prepend_global_include_path(self, path):
        self._prepend_path(self._global_include_path, path)

    def prepend_local_python_path(self, path):
        self._prepend_path(self._local_python_path, path)

    def prepend_global_python_path(self, path):
        self._prepend_path(self._global_python_path, path)

    def prepend_local_libraries(self, path):
        self._prepend_path(self._local_libraries, path)

    def prepend_global_libraries(self, path):
        self._prepend_path(self._global_libraries, path)

    def get_library_path(self):
        return self._local_library_path + self._global_library_path

    def get_include_path(self):
        return self._local_include_path + self._global_include_path

    def get_python_path(self):
        return self._local_python_path + self._global_python_path

    def get_libraries(self):
        return self._local_libraries + self._global_libraries

    def merge(self, other):
        result = PathManager()
        result._local_library_path = self._local_library_path + other._local_library_path
        result._local_include_path = self._local_include_path + other._local_include_path
        result._local_python_path = self._local_python_path + other._local_python_path
        result._local_libraries = self._local_libraries + other._local_libraries
        result._global_library_path = self._global_library_path + other._global_library_path
        result._global_include_path = self._global_include_path + other._global_include_path
        result._global_python_path = self._global_python_path + other._global_python_path
        result._global_libraries = self._global_libraries + other._global_libraries
        return result


class Environment(object):
    """A singleton class to hold the global environment."""

    _path_manager = PathManager()

    @classmethod
    def collect_path(cls):
        return cls._path_manager

    @classmethod
    def prepend_library_path(cls, path):
        cls._path_manager.prepend_local_library_path(path)

    @classmethod
    def prepend_include_path(cls, path):
        cls._path_manager.prepend_local_include_path(path)

    @classmethod
    def prepend_python_path(cls, path):
        cls._path_manager.prepend_local_python_path(path)

    @classmethod
    def add_search_path(cls, library):
        """Prepends the search paths of the given dependency to the global
        state. *Note* that this function should only be invoked from the
        top-level "SConstruct" file. **Also note** that this function should
        not be called with its own name. A module always knows its own
        dependencies and the repository only knows the configuration of the
        installed module anyway."""

        cls._path_manager.prepend_global_library_path(_config_store.get_library_path(library))
        cls._path_manager.prepend_global_include_path(_config_store.get_include_path(library))
        cls._path_manager.prepend_global_python_path(_config_store.get_python_path(library))

    @classmethod
    def add_library(cls, library):
        """Prepends to libraries of the given dependency to the globale state.
        *Note* that this method should be used very sparingly. The only real
        use is to configure the TestEnvironment in the top-level "SConstruct"
        file."""

        cls._path_manager.prepend_global_libraries(_config_store.get_libraries(library))

    def __new__(cls):
        result = cls.env.Clone()
        path_manager = cls.collect_path()
        result.PrependUnique(LIBPATH=path_manager.get_library_path(),
                             CPPPATH=path_manager.get_include_path(),
                             LIBS=path_manager.get_libraries(),
                            )
        result.PrependENVPath(oshelper.get_library_search_path_env(), path_manager.get_library_path())
        result.PrependENVPath('PYTHONPATH', path_manager.get_python_path())
        return result


class TestEnvironment(Environment):
    """A singleton class to hold the global environment used for building and
    executing unit-tests. It tracks the state of the Environment class, so
    that only dependencies required for the unit-tests have to be added
    here."""

    _path_manager = PathManager()

    @classmethod
    def collect_path(cls):
        return cls._path_manager.merge(Environment._path_manager)


class PythonEnvironment(Environment):
    """A singleton class to hold the global environment used for building and
    executing unit-tests. It tracks the state of the Environment class, so
    that only dependencies required for the unit-tests have to be added
    here."""

    _path_manager = PathManager()

    @classmethod
    def collect_path(cls):
        return cls._path_manager.merge(Environment._path_manager)
