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

'''
AvangoDaemon NG
===============

The AvangoDaemon is an independent instance for communication with
a variety of devices that serve as input data for Avango NG applications.
The AvangoDaemon has its own main loop in which for each device a thread
is running. Therefore the devices are decoupled from the Avango NG
main loop. Via shared memory segment the Avango NG application
exchanges data with the device instances.

On Avango NG application side a DeviceSensor is used to read out data
coming from an appropriate input device. Whereas a DeviceActuator
can be used to send specific commands to the device, for example
see WiimoteActuator to set LEDs or Rumble modes on a connected
Nintendo Wiimote.

On AvangoDaemon side implementations for different devices exist.
Examples are:

HIDInput
    To communicate with USB devices via standardized input interface
    (such as keyboard, mouse and gamepads).

Wiimote
    A specific HIDInput to use a Nintendo Wiimote device.
    Note: This device has a dependency to a patched version of 'wminput'.
    (http://abstrakraft.org/cwiid with patches #34,#35 and #36)

DTrack
    For processing DTrack udp packets (ASCII protocol).

WacomTablet
    To communicate with a Wacom tablet.


Examples
========

There are some basic examples that show the configuration and usage of
these input devices in examples/daemon within your Avango NG installation.
'''

from _daemon import *
from _daemon import _Device
from _daemon import _HIDHelper
from _daemon import _WacomTabletHelper
from _daemon import _WiimoteHelper
from _daemon import _DTrackHelper

import avango.nodefactory
nodes = avango.nodefactory.NodeFactory('av::daemon::')

class Station(object):
    """Just a plain object to create an Avango station based on its (unique) name.
    (properties: name). Station objects with the same name will refer to the same
    Avango station."""
    def __init__(self, name):
        self.name = name

class _DeviceMixIn(object):
    """A helper mix-in class that provides some advanced properties that can be set via []
    operator. In conjunction with the appropriate _DeviceHelper class a concrete device
    class can be constructed."""
    def __init__(self):
        super(_DeviceMixIn, self).__init__()
        self._station = None
        self._values = {}
        self._buttons = {}
        self._leds = {}

    def get_station(self):
        return self._station

    def set_station(self, st):
        self._station = st
        self.add_station(0, st.name)

    class ValueProxy(object):
        """Proxy object to override the functions that are called on access of list
        values via [] operator."""
        def __init__(self, hid):
            self._hid = hid
        def __getitem__(self, key):
            if self._hid._values.has_key(key):
                return self._hid._values[key]
            else : return ''
        def __setitem__(self, key, value):
            self._hid._values[key] = value
            return self._hid.map_to_station_value(key, value)

    class ButtonProxy(object):
        """Proxy object to override the functions that are called on access of list
        values via [] operator."""
        def __init__(self, hid):
            self._hid = hid
        def __getitem__(self, key):
            if self._hid._buttons.has_key(key):
                return self._hid._buttons[key]
            else : return ''
        def __setitem__(self, key, value):
            self._hid._buttons[key] = value
            return self._hid.map_to_station_button(key, value)

    class LEDProxy(object):
        """Proxy object to override the functions that are called on access of list
        values via [] operator."""
        def __init__(self, hid):
            self._hid = hid
        def __getitem__(self, key):
            if self._hid._leds.has_key(key):
                return self._hid._leds[key]
            else : return ''
        def __setitem__(self, key, value):
            self._hid._leds[key] = value
            return self._hid.map_to_station_led(key, value)

    station = property(get_station, set_station)
    buttons = property(ButtonProxy)
    values = property(ValueProxy)
    leds = property(LEDProxy)

class HIDInput(_DeviceMixIn, _HIDHelper):
    """Communicates with a device which is registered via Linux event system
    such as a mouse, keyboard, joystick or a gamepad. Required properties: station, device,
    value, button, led. Map incoming events like this: mydev.value[0] = \'EV_REL::REL_X\'.
    Optional properties: norm_abs, accum_rel_events, reset_rel_values_cycle, timeout."""
    def __init__(self):
        super(HIDInput, self).__init__()

class WacomTablet(_DeviceMixIn, _WacomTabletHelper):
    """Communicates with a Wacom tablet device which is registered via Linux event
    system. Required properties: station, device. Map incoming events like this:
    mydev.value[0] = \'EV_REL::REL_X\'. All appearing value und button events of a tablet
    are already set up in the constructor of this class. Optional properties: value, button,
    led, norm_abs, accum_rel_events, reset_rel_values_cycle, timeout, toggle_reset."""
    def __init__(self):
        super(WacomTablet, self).__init__()

class Wiimote(_DeviceMixIn, _WiimoteHelper):
    """Communicates with a Nintendo Wiimote device which is registered via Linux event
    system. Required properties: station, device, button, value. Map incoming events like
    this: mydev.value[0] = \'EV_REL::REL_X\'. All appearing LED and Rumble events of a
    Wiimote are already set up in the constructor of this class. Optional properties:
    led, norm_abs, accum_rel_events, reset_rel_values_cycle, timeout."""
    def __init__(self):
        super(Wiimote, self).__init__()

class DTrack(_DTrackHelper):
    """Avango NG device for processing DTrack udp packets (ASCII protocol).
    Required properties: stations, port."""
    def __init__(self):
        super(DTrack, self).__init__()
        self._stations = {}

    class StationProxy(object):
        """Proxy object to override the functions that are called on access of list
        values via [] operator."""
        def __init__(self, dtrack):
            self._dtrack = dtrack
        def __getitem__(self, key):
            if self._dtrack._stations.has_key(key):
                return self._dtrack._stations[key]
            else: return ''
        def __setitem__(self, key, st):
            self._dtrack._stations[key] = st
            return self._dtrack.add_station(key, st.name)

    stations = property(StationProxy)
