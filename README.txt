# -*- Mode:Outline -*-

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

AVANGO uses SCons (www.scons.org) as build system. Detailed information 
about the build process of AVANGO for different platforms can be found under:
http://www.avango.org/wiki/Installation

Here are some basic infos how to build AVANGO under Linux.
Use:

  scons --help

to see all configurations options. These options can also be set in a file
called 'localdefs.py' in the top-level directory. Note that this is a python
script with full support for all advanced features of python.
Here is a sample localdefs.py file:
--------------------------------------------------------------------
#adjust these according to your system
avango_src_dir     ='/home/YOUR_USER/apps/avango'
avango_build_dir   ='/home/YOUR_USER/apps/avango_build'
avango_install_dir ='/home/YOUR_USER/apps/avango_install'
osg_install_dir    ='/home/YOUR_USER/apps/OpenSceneGraph-2.9.5_install'

#This is the directory, where AVANGO will put its object files during compilation
BUILD=avango_build_dir
#This is the installation directory of AVANGO
PREFIX=avango_install_dir

PKG_CONFIG_PATH = avango_install_dir + '/lib/pkgconfig'
INCLUDE_PATH    = osg_install_dir+'/include'
LIBRARY_PATH    = osg_install_dir+'/lib'

#Build in debug mode
DEBUG=True
#Build with OpenSceneGraph support
OPENSCENEGRAPH=True
#Set the log level
LOG_LEVEL="WARN"
--------------------------------------------------------------------
Note, that this file assumes that python and boost are in the system paths. If
this is not the case, you also have to add them to the INCLUDE_PATH respectively 
LIBRARY_PATH variables in your localdefs.py.


By default the SCons build uses the 'all' build target which will recursively
build all modules with respect to your 'localdefs.py' environment.  For each
AVANGO module different build targets exist. For example to just build the
'avango-core' module use:

  scons core

To execute the unit tests for 'avango-core' use:

  scons check-core

And finally to install the module use:

  scons install-core

Replace 'core' with any other module name (including 'all' for all configured
modules) for other build targets.

Further information about the build system can be found in the python (source)
documentation of avango-build found here:

  avango-build/src/avango/build/__init__.py
