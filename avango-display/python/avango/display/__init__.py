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

import avango # Trigger type registration
import avango.nodefactory
nodes = avango.nodefactory.NodeFactory(module=__name__)

from _view import *
from _screen_descriptor import *
from _device import *
from _display import Display
from _display import ViewUserSelector

import getopt
import os.path
import itertools

def init(argv,options="", long_options=[]):
    "Initialize display setup"

    try:
        opts, args = getopt.getopt(argv[1:], "hn:l:d:io:" + options,
                                   ["help", "notify=", "log-file=",
                                    "display=", "inspector", "option="] + long_options)
    except getopt.GetoptError, err:
        opts = []
        args = list(argv)

    display_type='Monitor'
    notify_level = -1
    notify_logfile = ''
    inspector = None
    options = {}
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            print "Usage: python %s [OPTIONS ...]" % argv[0]
            print "Allowed options:"
            print "--display <Display Name> : Selects Display setup"
            print "--notify <Level>         : Selects notify level"
            print "--log-file <Log File>    : Selects log messages output file"
            print "--inspector              : Inspect scene graph with AVANGO inspector"
            print "--option <Key>:<Value>   : Sets specific option"
        elif opt in ("-n", "--notify"):
            notify_level = int(arg)
        elif opt in ("-l", "--log-file"):
            notify_logfile = arg
        elif opt in ("-d", "--display"):
            display_type = arg
        elif opt in ("-i", "--inspector"):
            # Only import if request! GTK fails without a proper display.
            # 'global' statement is used to avoid making avango a local variable and
            # thus inaccessible for the rest of the function!
            global avango
            import avango.inspector
            inspector = avango.inspector.nodes.Inspector()
        elif opt in ("-o", "--option"):
            try:
                key, value = arg.split(":")
                options[key] = value
            except:
                print "WARNING: Ignoring ill-formated option: '%s'" % arg
        else:
            if arg:
                args.insert(0, arg)
            args.insert(0, opt)


    if notify_level > -1:
        if notify_logfile == '':
            avango.enable_logging(notify_level)
        else:
            avango.enable_logging(notify_level, notify_logfile)

    global _selected_display
    _selected_display = _make_display(display_type, inspector, options)

    return args

def get_display_type():
    "Returns a string identifying the used display"

    return _selected_display.get_display_type()

def get_num_users(subdisplay=""):
    "Returns the number of users"

    return _selected_display.get_num_users(subdisplay)

def get_camera(subdisplay):
    """
    Get the osg.Camera object of the selected subdisplay. If no camera for the
    given subdisplay is found, None is returned
    """
    return _selected_display.get_camera(subdisplay)

def get_viewer():
    """
    Get the av::osg::viewer of the used display. Currently, all displays use a
    CompositeViewer
    """
    return _selected_display.get_viewer()

def get_view(subdisplay):
    """
    Get the osg.viewer.View object of the selected subdisplay. If no view for the
    given subdisplay is found, None is returned
    """
    return _selected_display.get_view(subdisplay)

def get_window(subdisplay):
    """
    Get the osg.viewer.GraphicsWindow object of the selected subdisplay. If no window for the
    given subdisplay is found, None is returned
    """
    return _selected_display.get_window(subdisplay)

def make_user(user=0, interface="", subdisplay=""):
    """Create a field container that represents the user position.

      - *user* selects the n-th *active* user of the display.

      - *interface* is used to select a different field container than the
        default one, i.e. different display may provide more specific or
        incompatible information about the active user. The resulting interface
        will still always contain the *Active* boolean field (see below).

      - *subdisplay* is used, if the display has multiple disjoint subdisplays
        and the user of a specific display has to be selected.

    The resulting (default) field container will have the following fields:

      - *Position* is a matrix representing the position of the user in display
        local coordinates.

      - *Active* is a boolean that is true if the user is recognized by the
        system (e.g. because the tracking cameras see the corresponding target)

      - *Identifier* is a unique name that indicate which user is the active
        user. Some tracking systems can recognize different target and this
        identifiers contains the informations which target was identified and
        chosen as the active user.
    """

    return _selected_display.make_user(user, interface, subdisplay)

def make_dominant_user_device(user=0, interface="", subdisplay=""):
    """Create a field container that represents the device held in the dominant
    hand of the active user

      - *user* selects the n-th *active* user of the display.

      - *interface* is used to select a different field container than the
        default one, i.e. different display may provide more specific or
        incompatible information about the active device. The resulting interface
        will still always contain the *Active* boolean field (see below).

      - *subdisplay* is used, if the display has multiple disjoint subdisplays
        and the user (and device) of a specific display has to be selected.

    The resulting (default) field container will have the following fields:

      - *Position* is a matrix representing the position of the device in display
        local coordinates.

      - *Button[0-4]* are bool field representing buttons on a device

      - *Active* is a boolean that is true if the device is recognized by the
        system (e.g. because the tracking cameras see the corresponding target)

      - *Identifier* is a unique name that indicate which device is the active
        device. Some tracking systems can recognize different target and this
        identifiers contains the informations which target was identified and
        chosen as the active device.
    """

    return _selected_display.make_dominant_user_device(user, interface, subdisplay)

def make_non_dominant_user_device(user=0, interface="", subdisplay=""):
    """Create a field container that represents the device held in the
    non-dominant hand of the active user. See make_dominant_user_device() for
    more information.
    """

    return _selected_display.make_non_dominant_user_device(user, interface, subdisplay)

def make_view(subdisplay=""):
    """Create a field container that represents a viewport into a scene.

      - *subdisplay* is used, if the display has multiple disjoint subdisplays
        and the viewport on a specific display has to be selected.

    The resulting (default) field container will have the following fields:

      - *Root* is the root node of the scene.

      - *Camera* is a matrix describing the translation of the display local
        coordinates into the scene.

      - *Viewport* is a Vec4 describing the relative position on the screen
        (link to diagram).

      - *Depth* is a float value indicating the stacking order of multiple
        views.

      - *UserSelector* contains the *active* user that should see this viewport.

      - *Near* is the near clipping plane.

      - *Far* is the near clipping plane.

      - *BackgroundColor* is the clearing color
    """

    return _selected_display.make_view(subdisplay)

def make_screen_descriptor(subdisplay=""):
    """Create a field container that represents the virtual screen defined by
       the display

      - *subdisplay* is used, if the display has multiple disjoint subdisplays
        and the viewport on a specific display has to be selected.

    The resulting (default) field container will have the following fields:

      - *Transformation* is a matrix describing the transformation of the
        virtual screen in display local coordinates.

      - *RealSize* is a Vec2 describing the dimension of the screen in display
        local coordinates.

      - *PixelSize* is a Vec2 describing the dimension of the screen in
        pixels. Note that this is only an approximation and the real pixel
        counts may be different.
    """

    return _selected_display.make_screen_descriptor(subdisplay)

def make_device(device, interface=""):
    """Create a field container that represents a generic device.

      - *device* is the name of the device. A display maps this name to a
        specific device and returns a field container representing it.

      - *interface* is used to select a different field container than the
        default one, i.e. different display may provide more specific or
        incompatible information about the device. The resulting interface
        will still always contain the *Active* boolean field (see below).

    The resulting (default) field container will have the following fields. As
    this interface is rather limited, one typically provides a specific
    interface:

      - *Active* is a boolean that is true if the device is recognized by the
        system (e.g. because the tracking cameras see the corresponding target)

      - *Identifier* is a unique name that indicate which device is the active
        device. Some tracking systems can recognize different target and this
        identifiers contains the informations which target was identified and
        chosen as the active device.
    """

    return _selected_display.make_device(device, interface)

def run():
    "Run the AVANGO evaluation loop."
    _selected_display.run()

def done():
    "Stop the AVANGO evaluation loop."
    _selected_display.done()

_search_path = []

def add_search_path(path):
    "Add path to search paths for display setups"
    global _search_path
    _search_path.append(path)

class DisplayNotFound(IOError):
    'The given Display setup file was not found'
    def __init__(self, display_type):
        self._display_type = display_type
    def __str__(self):
        return "Display '%s' was not found" % self._display_type

def _make_display(display_type, inspector, options):
    module_path = [os.path.join(os.path.split(__file__)[0], 'setups')]
    env_path = os.environ.get('AVANGO_DISPLAY_PATH', "").split(':')

    display_filename = display_type+'.py'
    for path in itertools.chain(env_path, _search_path, module_path):
        if not path:
            continue
        display_path = os.path.join(path, display_filename)
        if os.path.exists(display_path):
            break
    else:
        raise DisplayNotFound(display_type)

    source_file = open(display_path, 'r')
    source = source_file.read()
    code = compile(source, display_path, 'exec')

    scope = {}
    exec code in scope

    return scope[display_type](inspector, options)
