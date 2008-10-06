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
    BoolOption('OPENAL_SUPPORT',
               'Enable building OpenAL sound module',
               False)
    )

avango.build.setup()

avango.build.set_config('avango-unittest', avango.build.PlainConfig(libraries = ['avangoUnitTest++']))
avango.build.set_config('avango-core', avango.build.PlainConfig(libraries = ['avango']))

# avango.build.set_config('avango-osg', avango.build.PlainConfig(libraries = ['avangoOsg', 'avangoOsgParticle']))
avango.build.set_config('avango-osg', avango.build.PlainConfig(libraries = ['avangoOsg']))
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
    uuid
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

avango.build.SConscript('avango-build/SConscript')
avango.build.SConscript('avango-core/SConscript')
avango.build.SConscript('avango-python/SConscript')
if env['OPENSCENEGRAPH_SUPPORT']:
    avango.build.SConscript('avango-osg/SConscript')
    avango.build.SConscript('avango-moving/SConscript')
    avango.build.SConscript('avango-menu/SConscript')
    if not avango.build.oshelper.os_is_windows(): #FIXME get module running under Windows
        avango.build.SConscript('avango-daemon/SConscript')
if not avango.build.oshelper.os_is_windows(): #FIXME get module running under Windows
    avango.build.SConscript('avango-tools/SConscript')
    avango.build.SConscript('avango-sound/SConscript')
if env['OPENAL_SUPPORT']:
    avango.build.SConscript('avango-openal/SConscript')
avango.build.SConscript('avango-inspector/SConscript')
avango.build.SConscript('avango-doc/SConscript')
