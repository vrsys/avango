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

from config import *
from configstore import _config_store
import oshelper

def _setup_default():
    _plain_libs = """
        dl
        osg
        osgDB
        osgViewer
        osgParticle
        osgShadow
        osgText
        osgUtil
        osgGA
        osgFX
        alut
        openal
        vrpn
        CGAL
        gml
        """

    for lib in _plain_libs.split():
        _config_store.set(lib, PlainConfig(libraries = [lib]))

    _config_store.set('boost_thread', BoostConfig('boost_thread'))
    _config_store.set('boost_signals', BoostConfig('boost_signals'))
    _config_store.set('boost_filesystem', BoostConfig('boost_filesystem'))
    _config_store.set('boost_regex', BoostConfig('boost_regex'))
    _config_store.set('boost_date_time', BoostConfig('boost_date_time'))
    _config_store.set('boost_python', BoostConfig('boost_python', dependencies = ['python']))

    _config_store.set('avango-core', PKGConfig('avango-core'))
    _config_store.set('avango-daemon', PKGConfig('avango-daemon'))
    _config_store.set('avango-python', PlainConfig(libraries = ["avangoScript"]))
    _config_store.set('avango-osg', PKGConfig('avango-osg'))
    _config_store.set('avango-osgviewer', PKGConfig('avango-osgviewer'))
    _config_store.set('avango-moving', PKGConfig('avango-moving'))
    _config_store.set('avango-tools', PKGConfig('avango-tools'))
    _config_store.set('avango-ensemble', PKGConfig('avango-ensemble'))
    _config_store.set('avango-unittest', PKGConfig('avango-unittest'))
    _config_store.set('avango-sound', PlainConfig(libraries = ["avangoSound"]))
    _config_store.set('avango-shade', PlainConfig(libraries = ["avangoShade", "avangoShadeCore"]))
    _config_store.set('avango-utils', PlainConfig(libraries = ["avangoUtils"]))
    _config_store.set('vorbisfile', PKGConfig('vorbisfile'))
    _config_store.set('xerces', PlainConfig(libraries = ['xerces-c']))
    _config_store.set('graphicsMagick', PlainConfig(libraries = ['GraphicsMagick++']))

    _config_store.set('python', PythonConfig())
    
    _config_store.set('osgEarth', PlainConfig(libraries = ["osgEarth", "osgEarthUtil", "osgEarthFeatures"]))

    # Dummy packages
    _config_store.set('osgUtil', PlainConfig(libraries = ['']))
    _config_store.set('GL', PlainConfig(libraries = ['']))
    _config_store.set('GLU', PlainConfig(libraries = ['']))

    if oshelper.os_is_mac():
      _config_store.set('osg', PlainConfig(libraries = ['osg', 'OpenThreads']))
      _config_store.set('osgUtil', PlainConfig(libraries = ['osgUtil']))
      _config_store.set('GL', PlainConfig(libraries = ['GL']))
      _config_store.set('GLU', PlainConfig(libraries = ['GLU']))

    # Override some libraries for Windows
    if oshelper.os_is_windows():
        # Boost libraries use auto-link
        _config_store.set('boost_thread', PlainConfig(libraries = ['']))
        _config_store.set('boost_signals', PlainConfig(libraries = ['']))
        _config_store.set('boost_regex', PlainConfig(libraries = ['']))
        _config_store.set('boost_python', PlainConfig(libraries = [''], dependencies = ["python"]))
        # dl is in standard system libraries
        # TODO check that dl functionality is included in system library
        _config_store.set('dl', PlainConfig(libraries = ['']))
        # Windows-only(?) OSG library
        _config_store.set('osg', PlainConfig(libraries = ['osg', 'OpenThreads']))
        _config_store.set('osgUtil', PlainConfig(libraries = ['osgUtil']))
        # GL libraries are required under Windows
        _config_store.set('GL', PlainConfig(libraries = ['OPENGL32']))
        _config_store.set('GLU', PlainConfig(libraries = ['GLU32']))
        # for avango extensions, there is no pkg-config
        _config_store.set('avango-core', PlainConfig(libraries = ['avango'], dependencies = ["boost_signals", "boost_thread", "boost_regex"]))
        _config_store.set('avango-osg', PlainConfig(libraries = ['avangoOsg'], dependencies = ['osg', 'osgDB', 'osgViewer', 'osgParticle', 'osgText', 'osgUtil']))
        _config_store.set('avango-utils', PlainConfig(libraries = ['avangoUtils']))
        _config_store.set('avango-tools', PlainConfig(libraries = ['avangoTools']))
        _config_store.set('avango-osgviewer', PlainConfig(libraries = ['avangoOsgViewer']))
        # xerces
        _config_store.set('xerces', PlainConfig(libraries = ['xerces-c_2']))

def set_config_flag(key, value):
    if key == "OPENSCENEGRAPH_DEBUG" and value:
        _config_store.set('osg', PlainConfig(libraries = ['osgd', 'OpenThreadsd']))
        _config_store.set('osgDB', PlainConfig(libraries = ['osgDBd']))
        _config_store.set('osgViewer', PlainConfig(libraries = ['osgViewerd']))
        _config_store.set('osgParticle', PlainConfig(libraries = ['osgParticled']))
        _config_store.set('osgText', PlainConfig(libraries = ['osgTextd']))
        _config_store.set('osgGA', PlainConfig(libraries = ['osgGAd']))
        _config_store.set('osgUtil', PlainConfig(libraries = ['osgUtild']))
        _config_store.set('osgShadow', PlainConfig(libraries = ['osgShadowd']))


_setup_default()
