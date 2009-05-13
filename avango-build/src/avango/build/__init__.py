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

"""The Avango build system is a set of helper functions and classes designed to
make development of internal and external modules as easy as possible.


Build Environment
=================

When building a program, each step of the build process requires some external
data. The compiler e.g. will requires header files of the used libraries. There
exist standard search paths, so that libraries that are installed in these
locations are found automatically by the tools requiring them. Often some
external dependencies are not installed in these locations though, e.g. to
install different conflicting versions.

The build system therefore has to provide to user a way to provide additional
search paths. In the Avango build system these search paths are *global* to
all tools and build targets. This ensures that all tools make use of the same
installed files. *Note* that this is different to other build systems, most
notable the autotools system, which provide a way to specify external search
paths per external dependency. In this case it is not transparent to the user,
which build target has what build environment.

The following search paths are used:
    - INCLUDE_PATH
        Search path for C/C++ header files
    - LIBRARY_PATH
        Search path for libraries. This path will be used by the
        normal linker *and*  run-time linker, that to resolve dynamic
        libraries for executables to run. This is needed to run applications
        that are build targets and is also required to correctly handle
        transitive linking.
    - PYTHON_PATH
        Search path for python modules. *Note* that this only affects
        modules required by build targets that are run. The "SConscript" files
        do not use this search path
    - PKG_CONFIG_PATH
        Search path for "pkgconfig" files


Dependency Repository
=====================

The "pkgconfig" tool uses a repository to store the search path of
dependencies and other metadata, e.g. the name of the library to link to. The
Avango build system provides a wrapper around this to make similar
functionality also available on platforms, where "pkgconfig" is not available.
The abstractions also allow external and internal modules to be treated
identical (see below).

Each external dependency is referred to by a unique name. If this dependency
provides a "pkgconfig" file the unique name will be the same as used with
"pkgconfig". Using this name the module author can add the (automatically
detected) search path to environments, add libraries for linking, etc.


SConstruct and SConscript
=========================

To use the Avango build system one has to import this module::

    import avango.build

The "SConstruct" file should only specify any external dependency and add their
search path to the global environment. *Note* that only the search path should
be added, not libraries for linking. The following code adds the search path
for the Avango core and Avango python modules::

    avango.build.Environment.add_search_path("avango-core")
    avango.build.Environment.add_search_path("avango-python")

Any other task should be performed in the top-level "SConscript" files. This
file can of course defer this to other lower-level "SConscript" files.

External Modules
----------------

The user has to setup the "PYTHONPATH" environment variable properly, so that
the Avango build module can be found. The path should point to the *installed*
version of Avango. The user then does not have to add the installation
directory of Avango or the dependencies to the search paths as this happens
automatically.

Internal Modules
----------------

Internal packages of the Avango distribution behave exactly like external
modules. The module specific "SConstruct" file is not used anymore but a
dispatch from the top-level Avango "SConstruct" file has to be added.
External dependencies also have to be specified in this file.
"""

import SCons.Script as scons

import os
import os.path
import sys
import commands
import distutils.sysconfig

from environment import *
from config import *
from config import _do_parse_config
from config import _do_recurse
from configstore import _config_store
from utility import *
import recipes
import defaults
import oshelper

scons.EnsureSConsVersion(0,96,93)

if '-h' in sys.argv or '--help' in sys.argv:
    _do_parse_config = False
    _do_recurse = False
if '-c' in sys.argv or '--clean' in sys.argv or '--remove' in sys.argv:
    _do_parse_config = False

def build_default_options():
    result = scons.Options(['sitedefs.py', 'localdefs.py'])
    result.AddOptions(
        scons.PathOption('BUILD', 'Build path', '.', scons.PathOption.PathIsDirCreate),
        ('CXX','C++ compiler'),
        scons.BoolOption('DEBUG', 'Debug build option', False),
        scons.BoolOption('TRACE_LOGGING', 'Enable/Disable trace log level', False),
        scons.PathOption('BINARY_PATH',
                   'Additional binary search paths',
                   defaults.BINARY_PATH,
                   scons.PathOption.PathAccept),
        scons.PathOption('INCLUDE_PATH',
                   'Additional include search paths',
                   defaults.INCLUDE_PATH,
                   scons.PathOption.PathAccept),
        scons.PathOption('LIBRARY_PATH',
                   'Additional library search paths',
                   defaults.LIBRARY_PATH,
                   scons.PathOption.PathAccept),
        scons.PathOption('PKG_CONFIG_PATH',
                   'Additional pkg-config search paths',
                   defaults.PKG_CONFIG_PATH,
                   scons.PathOption.PathAccept),
        scons.PathOption('PREFIX',
                   'Installation prefix path',
                   '/usr/local',
                   scons.PathOption.PathAccept),
        scons.PathOption('PYTHON_PATH',
                   'Additional python search paths',
                   defaults.PYTHON_PATH,
                   scons.PathOption.PathAccept),
        )
    return result

def _build_environment():
    result = scons.Environment(options = options)
    result.Tool('subst', toolpath = [os.path.dirname(__file__)+'/tools'])

# windows
    if 'icpc' in result['CXX']:
        result.Append(CCFLAGS='-wd177,383,424,810,981,1418,1419,1572')
        result.Append(LINKFLAGS='-static-intel')
    if oshelper.os_is_windows():
        if result.get('OPENSCENEGRAPH_DEBUG', None):  # FIXME move this elswhere
            result.Append(CXXFLAGS='/EHsc /W3 /MDd /wd4099 /wd4244 /wd4800 /wd4996 /wd4251 /wd4661') # FIXME remove deactivation of warnings
        else:
            result.Append(CXXFLAGS='/EHsc /W3 /MD /wd4099 /wd4244 /wd4800 /wd4996 /wd4251 /wd4661') # FIXME remove deactivation of warnings
        result.Append(CPPDEFINES=['WIN32', 'AV_INSTANTIATE_FIELD_TEMPLATES'])
        result['SHLINKCOM'] = [result['SHLINKCOM'], 'mt.exe -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;2']
    else:
        result.Append(CCFLAGS='-ansi -Wall')
    if result['DEBUG']:
        if oshelper.os_is_windows():
            result.Append(CCFLAGS='/DEBUG /Zi /D_DEBUG /Ob0 /Od /RTC1')
        else:
            result.Append(CCFLAGS='-g')
    else:
        result.Append(CCFLAGS='-O2 -DNDEBUG')
        if result['CXX'] == 'g++':
            result.Append(CCFLAGS='-fno-strict-aliasing')
    if result['TRACE_LOGGING']:
        result.Append(CPPDEFINES='TRACE_LOGGING')
    result.PrependENVPath('PATH', result['BINARY_PATH'])
    result.Prepend(CPPPATH=result['INCLUDE_PATH'].split(os.pathsep))
    result.Prepend(LIBPATH=result['LIBRARY_PATH'].split(os.pathsep))
    result.PrependENVPath(oshelper.get_library_search_path_env(), result['LIBRARY_PATH'])
    result.PrependENVPath('PYTHONPATH', result['PYTHON_PATH'])
    result.PrependENVPath('PKG_CONFIG_PATH', result['PKG_CONFIG_PATH'])
    os.environ['PKG_CONFIG_PATH'] = result['PKG_CONFIG_PATH']
    return result

def _build_python_environment(env):
    result = env.Clone()
    result.PrependUnique(CPPPATH=[distutils.sysconfig.get_python_inc()],
                             LIBPATH=[distutils.sysconfig.get_config_var('LIBDIR')])
    result.MergeFlags(distutils.sysconfig.get_config_var('BLDLIBRARY'))
    result['SHLIBPREFIX'] = ''
    result['SHLIBSUFFIX'] = distutils.sysconfig.get_config_var('SO')
    return result

def setup():
    scons.SetOption('duplicate', 'soft-hard-copy')

    env = _build_environment()
    Environment.env = env
    TestEnvironment.env = env.Clone()
    if 'AV_DEBUGGER' in os.environ:
        TestEnvironment.env['ENV']['AV_DEBUGGER'] = os.environ['AV_DEBUGGER']
    PythonEnvironment.env = _build_python_environment(env)

    scons.Help(options.GenerateHelpText(env))
    if os.path.abspath(env['BUILD']) != os.path.abspath('.'):
        scons.BuildDir(env['BUILD'], '.', not oshelper.os_is_windows())
        env.SConsignFile("${BUILD}/sconsign")

def get_prefix(path = ''):
    "Returns the generic installation prefix."
    return scons.Dir(Environment.env['PREFIX']+'/'+path)

def get_binary_path(path = ''):
    "Returns the installation prefix for binaries."
    return scons.Dir(Environment.env['PREFIX']+'/bin/'+path)

def get_library_path(path = ''):
    "Returns the installation prefix for libraries."
    return scons.Dir(Environment.env['PREFIX']+'/lib/'+path)

def get_documentation_path(path = ''):
    "Returns the installation prefix for documentation."
    return scons.Dir(Environment.env['PREFIX']+'/doc/'+path)

def get_binary_path(path = ''):
    "Returns the installation prefix for binary files."
    return scons.Dir(Environment.env['PREFIX']+'/bin/'+path)

def get_include_path(path = ''):
    "Returns the installation prefix for header files."
    return scons.Dir(Environment.env['PREFIX']+'/include/'+path)

def get_pkgconfig_path(path = ''):
    "Returns the installation prefix for pkg-config files."
    return scons.Dir(Environment.env['PREFIX']+'/lib/pkgconfig/'+path)

def get_python_path(path = ''):
    "Returns the installation prefix for python modules."
    return scons.Dir(Environment.env['PREFIX']+'/lib/python'+get_python_version()+'/'+path)

def get_python_version():
    return distutils.sysconfig.get_python_version()

def SConscript(sconsfile):
    """A wrapper to recurse into other "Sconscript" files. The recursion is
    only performed if required by the build system, so e.g. if the available
    options are queried no recursion is performed."""
    if _do_recurse:
        scons.SConscript(Environment.env['BUILD']+'/'+sconsfile)

def add_search_path(env, library):
    """Add the named dependency to the search path of the given environment.
    *Note* that this function should not be called. Use the corresponding
    method from the Environment class instead."""

    libs = _config_store.get_library_path(library)
    include = _config_store.get_include_path(library)
    python = _config_store.get_python_path(library)
    env.Prepend(LIBPATH = libs, CPPPATH = include)
    env.PrependENVPath(oshelper.get_library_search_path_env(), libs)
    env.PrependENVPath('PYTHONPATH', python)

def add_library(env, library):
    """Add the libraries from the named dependency to the given enviroment."""
    libs = _config_store.get_libraries(library)
    env.Prepend(LIBS = libs)

def set_config(key, config):
    _config_store.set(key, config)

def set_config_flag(key, value):
    recipes.set_config_flag(key, value)


options = build_default_options()

scons.Default(scons.Alias('check'))
