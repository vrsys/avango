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

class TouchscreenEmulator(avango.display.Display):

    def __init__(self, inspector, options):
        super(TouchscreenEmulator, self).__init__("TouchscreenEmulator", inspector)

        twoview = False
        if 'twoview' in options:
            twoview = True

        self._touchscreen_camera = None
        self._touchscreen_window = None

        user = avango.display.nodes.User()
        user.Matrix.value = avango.osg.make_trans_mat(avango.osg.Vec3(0., 1.7, 0.))
        self.add_user(user)

        if not twoview:
            window = self.make_window(0, 0, 1280, 1024, 6, 2.4, False)
            window.Name.value = ""
            self.add_window(window, avango.osg.make_trans_mat(0., 1.2, -2.4), 0)

            self._touchscreen_window = self.make_window(0, 0, 1280, 1024, 6, 2.4, False)

        else:
            window = self.make_window(0, 0, 1280, 1024, 6, 2.4, True, ":0.0")
            window.Name.value = ""
            self.add_window(window, avango.osg.make_trans_mat(0., 1.2, -2.4), 0)

            self._touchscreen_window = self.make_window(1280, 0, 1280, 1024, 6., 2.4, False, ":0.0")
            self._touchscreen_window.Name.value = "Touchscreen"

    def make_dominant_user_device(self, user, interface, subdisplay):
        device = avango.display.nodes.Device()
        device.Matrix.connect_from(self._touchscreen_camera.MouseNearTransform)
        device.Button1.connect_from(self._touchscreen_event.MouseButtons_OnlyLeft)
        return device

    def make_view(self, subdisplay):
        if subdisplay == "":
            return super(TouchscreenEmulator, self).make_view(subdisplay)

        if subdisplay != "Touchscreen":
            return None

        display_view = avango.display.nodes.View()

        camera, view = self.make_camera(display_view, 0., self._touchscreen_window)
        camera.ScreenTransform.value = avango.osg.make_trans_mat(0, 1.2, -2.4)
        camera.Window.value.Decoration.value = True
        camera.Window.value.ShowCursor.value = True
        camera.Window.value.Title.value = "Touchscreen"

        self.add_view(view)

        self._touchscreen_camera = camera
        self._touchscreen_event = avango.osg.viewer.nodes.EventFields(View = view)
        self._touchscreen_window.DragEvent.connect_from(self._touchscreen_event.DragEvent)
        self._touchscreen_window.MoveEvent.connect_from(self._touchscreen_event.MoveEvent)

        return display_view
