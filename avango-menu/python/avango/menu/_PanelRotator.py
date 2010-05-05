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

import avango.osg
import avango.script
import Preferences

class PanelRotator(avango.script.Script):
    TimeIn = avango.SFFloat()
    Duration = avango.SFFloat()
    Axis = avango.osg.Vec3()
    StartAngle = avango.SFFloat()
    EndAngle = avango.SFFloat()
    Panel = avango.script.SFObject()
    EndVisible = avango.SFBool()
    Caller = avango.script.SFObject()

    MatrixOut = avango.osg.SFMatrix()
    CullFaceModeOut = avango.SFInt()
    CullFaceOut = avango.osg.SFCullFace()
    VisibleOut = avango.SFBool()
    TransitionOut = avango.SFBool()

    def __init__(self):
        self.super(PanelRotator).__init__()

        self.time_sensor = avango.nodes.TimeSensor()
        self.TimeIn.connect_from(self.time_sensor.Time)
        self.start_time = None
        self.Duration.value = 1.0 # one second
        self.old_cull_face_mode = 0
        self.old_cull_face = None
        self.aborted = False
        self.lastframe = False

    def evaluate(self):
        if self.aborted:
            return

        # first frame
        if self.start_time is None:
            self.start_time = self.TimeIn.value
            if self.Panel.value.root.StateSet.value is not None:
                self.old_cull_face_mode = self.Panel.value.root.StateSet.value.CullFaceMode.value
                self.old_cull_face = self.Panel.value.root.StateSet.value.CullFace.value
            else:
                self.Panel.value.root.StateSet.value = avango.osg.nodes.StateSet()
            self.CullFaceModeOut.value = 1
            self.CullFaceOut.value = avango.osg.nodes.CullFace(Mode=1029) # GL_BACK 1029
            self.TransitionOut.value = True
            self.VisibleOut.value = True
            # update via field connections to have a defined evaluation order!
            self.Panel.value.Visible.connect_from(self.VisibleOut)
            self.Panel.value.Transition.connect_from(self.TransitionOut)
            self.Panel.value.root.Matrix.connect_from(self.MatrixOut)
            self.Panel.value.root.StateSet.value.CullFaceMode.connect_from(self.CullFaceModeOut)
            self.Panel.value.root.StateSet.value.CullFace.connect_from(self.CullFaceOut)

        fraction = (self.TimeIn.value - self.start_time) / self.Duration.value

        # last frame
        if fraction>=1 and not self.lastframe:
            self.MatrixOut.value = avango.osg.make_rot_mat(avango.osg.Quat(self.EndAngle.value, self.Axis.value))
            self.VisibleOut.value = self.EndVisible.value
            self.TransitionOut.value = False
            self.CullFaceModeOut.value = self.old_cull_face_mode
            if self.old_cull_face is not None:
                self.CullFaceOut.value = self.old_cull_face
            self.Caller.value.transition_finished(self)
            self.cleanup()
            return

        # every frame from first to last-1
        startquat = avango.osg.Quat(self.StartAngle.value, self.Axis.value)
        endquat = avango.osg.Quat(self.EndAngle.value, self.Axis.value)
        quat = avango.osg.Quat()
        quat.slerp(fraction, startquat, endquat)
        self.MatrixOut.value = avango.osg.make_rot_mat(quat)

    def abort(self):
        self.aborted = True
        self.Panel.value.Transition.value = False
        self.cleanup()

    def cleanup(self):
        self.Panel.value.Visible.disconnect_from(self.VisibleOut)
        self.Panel.value.Transition.disconnect_from(self.TransitionOut)
        self.Panel.value.root.Matrix.disconnect_from(self.MatrixOut)
        self.Panel.value.root.StateSet.value.CullFaceMode.disconnect_from(self.CullFaceModeOut)
        self.Panel.value.root.StateSet.value.CullFace.disconnect_from(self.CullFaceOut)
        self.disconnect_all_fields()
        self.Panel.value = None
        self.time_sensor.always_evaluate(False)
        self.time_sensor = None

    def __del__(self):
        if Preferences.print_destruction_of_menu_objects:
            print "Rotator deleted"
