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

import sys
import os, os.path
sys.path.insert(0, os.path.join(os.getcwd(), 'avango-build', 'src'))

import avango.build
import avango.build.oshelper

opts = avango.build.options
opts.AddOptions(
    BoolOption('DISTRIBUTION_SUPPORT',
               'Switch to enable distribution support (requires ensemble module)',
               False),
    BoolOption('OPENSCENEGRAPH_SUPPORT',
               'Enable building OpenSceneGraph module',
               True),
    BoolOption('OPENSCENEGRAPH_DEBUG',
               'Use debug version of OpenSceneGraph library',
               False),
    BoolOption('SHADE_SUPPORT',
               'Enable building shade module',
               False),
    BoolOption('OPENAL_SUPPORT',
               'Enable building OpenAL sound module',
               False),
    BoolOption('DISPLAY_SUPPORT',
               'Enable building display setup module (EXPERIMENTAL)',
               False),
    BoolOption('CONNECT_SUPPORT',
               'Enable building module to connect mobile devices to AVANGO',
               False),
    BoolOption('CONNECT_CSHARP_SUPPORT',
               'Enable C# support for the connect module',
               False),
    BoolOption('VRPN_SUPPORT',
               'Enable VRPN support for the AvangoDaemon module',
               False),
    BoolOption('UTILS_SUPPORT',
               'Enable building utility module (EXPERIMENTAL)',
               False),
    )

avango.build.setup()

avango.build.set_config('avango-unittest', avango.build.PlainConfig(libraries = ['avangoUnitTest++']))
avango.build.set_config('avango-core', avango.build.PlainConfig(libraries = ['avango'], dependencies = ["boost_signals", "boost_thread"]))

avango.build.set_config('avango-python', avango.build.PlainConfig(libraries = ['avangoScript']))

# avango.build.set_config('avango-osg', avango.build.PlainConfig(libraries = ['avangoOsg', 'avangoOsgParticle']))
avango.build.set_config('avango-osg', avango.build.PlainConfig(libraries = ['avangoOsg'], dependencies = ['osg', 'osgDB', 'osgViewer', 'osgParticle', 'osgText', 'osgUtil']))
avango.build.set_config('avango-shade', avango.build.PlainConfig(libraries = ['avangoSloow', 'shade']))
avango.build.set_config('avango-osgparticle', avango.build.PlainConfig(libraries = ['avangoOsgParticle']))

avango.build.set_config('avango-osgviewer', avango.build.PlainConfig(libraries = ['avangoOsgViewer']))
avango.build.set_config('avango-daemon', avango.build.PlainConfig(libraries = ['avangoDaemon']))

env = avango.build.Environment()

if env['DISTRIBUTION_SUPPORT']:
    avango.build.Environment.add_search_path('avango-ensemble')
if env['OPENSCENEGRAPH_DEBUG']:
    avango.build.set_config_flag("OPENSCENEGRAPH_DEBUG", True)

used_libs = Split("""
    dl
    boost_thread
    boost_signals
    boost_python
    osg
    osgDB
    osgViewer
    osgParticle
    osgText
    """)
for lib in used_libs:
    avango.build.Environment.add_search_path(lib)


avango.build.SConscript('avango-unittest/SConscript')
avango.build.TestEnvironment.add_library('avango-unittest')

# List is ordered by dependency of modules
avango.build.SConscript('avango-build/SConscript')
avango.build.SConscript('avango-core/SConscript')
avango.build.SConscript('avango-python/SConscript')
if env['OPENSCENEGRAPH_SUPPORT']:
    avango.build.SConscript('avango-osg/SConscript')
    if env['SHADE_SUPPORT']:
        avango.build.SConscript('avango-shade/SConscript')
    avango.build.SConscript('avango-moving/SConscript')
    avango.build.SConscript('avango-menu/SConscript')
    if not avango.build.oshelper.os_is_windows() and \
         not avango.build.oshelper.os_is_mac(): #FIXME get module running under Windows/MAC
        avango.build.SConscript('avango-daemon/SConscript')
if not avango.build.oshelper.os_is_windows() and \
   not avango.build.oshelper.os_is_mac(): #FIXME get module running under Windows/MAC
    avango.build.SConscript('avango-sound/SConscript')
if env['OPENAL_SUPPORT']:
    avango.build.SConscript('avango-openal/SConscript')
avango.build.SConscript('avango-tools/SConscript')
if env['DISPLAY_SUPPORT']:
    avango.build.SConscript('avango-display/SConscript')
if env['CONNECT_SUPPORT']:
    avango.build.SConscript('avango-connect/SConscript')
if env['UTILS_SUPPORT']:
    avango.build.SConscript('avango-utils/SConscript')
avango.build.SConscript('avango-inspector/SConscript')
avango.build.SConscript('avango-doc/SConscript')
avango.build.SConscript('examples/SConscript')
