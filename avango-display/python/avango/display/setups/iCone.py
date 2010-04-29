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

import avango.display
import avango.daemon
import avango.utils
import math
import re

class iCone(avango.display.Display):

    def __init__(self, inspector, options):
        super(iCone, self).__init__("iCone", inspector)
        
        stereo=True
        if options.has_key("stereo") and self._bool_dict.has_key(options["stereo"].lower()):
            stereo=self._bool_dict[options["stereo"].lower()]
        
        self._eye_vec = avango.osg.Vec3(0., 1.7, 0.)

        common_transform = avango.osg.make_rot_mat(math.radians(-4.43), 1, 0, 0) \
                         * avango.osg.make_trans_mat(0, 1.390, -2.818)

        transforms = [
            common_transform * avango.osg.make_rot_mat(math.radians(84.135), 0, 1, 0),
            common_transform * avango.osg.make_rot_mat(math.radians(28.045), 0, 1, 0),
            common_transform * avango.osg.make_rot_mat(math.radians(-28.045), 0, 1, 0),
            common_transform * avango.osg.make_rot_mat(math.radians(-84.135), 0, 1, 0),
        ]

        self._cone_windows = []
        for i in range(0,len(transforms)):
            window = self.make_window(0, 0, 1440, 1320, 3.540, 2.830, stereo, ":0.%i"%i)
            self._cone_windows.append((window, transforms[i]))

        #create a device service
        self._device_service = avango.daemon.DeviceService()
        
        #store the created views. Needed for the stats display
        self._views = []
        
        self._wiimote_config = {}
        self._wiimote_config["wiimote1"] = ["ve-dtrack-atek",     avango.osg.Vec3( 0.045, 0.0,  0.022)]
        self._wiimote_config["wiimote2"] = ["ve-dtrack-reddot",   avango.osg.Vec3(-0.05 , 0.0,  0.031)]
        self._wiimote_config["wiimote3"] = ["ve-dtrack-logitech", avango.osg.Vec3(-0.04 , 0.0, -0.025)]
        self._wiimote_config["wiimote4"] = ["ve-dtrack-raytac",   avango.osg.Vec3( 0.068, 0.02, 0.035)]
        
    def view_created(self, camera, view):
        self._views.append(view)
            
    def make_view(self, subdisplay):
        display_view = avango.display.nodes.View()

        for window, transform in self._cone_windows:
            camera, view = self.make_camera(display_view, 0.03, window)
            camera.EyeTransform.value = avango.osg.make_trans_mat(self._eye_vec)
            camera.ScreenTransform.value = transform
            self.add_view(view)

        return display_view
    
    def make_screen_descriptor(self, subdisplay):
        #assume horizontal ordering of the screens
        screen = avango.display.nodes.ScreenDescriptor()
        screen.Transform.value = avango.osg.make_ident_mat()
        
        real_size_sum_width = avango.utils.nodes.FloatXSum()
        real_size_min_height = avango.utils.nodes.FloatXMin()
        real_size = avango.utils.nodes.Float2Vec2Converter()
        real_size.Value0.connect_from(real_size_sum_width.Output)
        real_size.Value1.connect_from(real_size_min_height.Output)
        screen.RealSize.connect_from(real_size.Output)
        
        pixel_size_sum_width = avango.utils.nodes.FloatXSum()
        pixel_size_min_height = avango.utils.nodes.FloatXMin()
        pixel_size = avango.utils.nodes.Float2Vec2Converter()
        pixel_size.Value0.connect_from(pixel_size_sum_width.Output)
        pixel_size.Value1.connect_from(pixel_size_min_height.Output)
        screen.PixelSize.connect_from(pixel_size.Output)
        
        for window_config in self._windows:
            window = window_config[0]
            real_size_sum_width.add_and_connect_float_field(window.RealActualWidth)
            real_size_min_height.add_and_connect_float_field(window.RealActualHeight)
            
            pixel_size_sum_width.add_and_connect_float_field(window.ActualWidth)
            pixel_size_min_height.add_and_connect_float_field(window.ActualHeight)

        return screen
            
    
    def make_device(self, device, interface):
        #check for wiimote
        if "wiimote" in device:
            pattern = re.compile("wiimote(\d+)$")
            result = pattern.match(device)
            if not result:
                print "Wiimote device <" + device +"> does not exist."
                return None
            wiimote_num = int(result.group(1))
            if wiimote_num < 1 or wiimote_num > 4:
                print "Wiimote device <" + device +"> does not exist. Currently wiimote 1-4 exists."
                return None
            
            osg2perf = avango.osg.make_rot_mat(math.radians(90), 1, 0, 0)
            perf2osg = avango.osg.make_rot_mat(math.radians(-90), 1, 0, 0)
            
            wiimote_station_name = device
            wiimote_dtrack_station_name = self._wiimote_config[wiimote_station_name][0]
            wiimote_offset = self._wiimote_config[wiimote_station_name][1]
            
            wiimote_offset.y -= 0.1
            wiimote_device = avango.display.make_wiimote_device(wiimote_station_name, 
                                                                wiimote_dtrack_station_name,
                                                                self._device_service,
                                                                osg2perf * avango.osg.make_trans_mat(wiimote_offset),
                                                                perf2osg)
            
            
            if not wiimote_device:
                print "Could not create device <" + str(device) + ">"
                return None
            
            #activate the LED according to the num (-1, since the numbering of the leds starts with 0) of the wiimote
            led_field = getattr(wiimote_device,"LED"+str(wiimote_num-1))
            led_field.value = True
            
            return wiimote_device
        
        elif device == "StatsViewer":
            stats_viewer = StatsViewer()
            stats_viewer.Views.value = self._views
            return stats_viewer
