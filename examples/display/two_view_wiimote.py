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

from avango import display
import avango
import avango.utils
import avango.display
import avango.osg
import avango.tools
import sys


class ExitTrigger(avango.utils.EdgeTrigger):

    def on_up_transition(self):
        sys.exit()


class StatusToggleTrigger(avango.utils.EdgeTrigger):
    StatsNumOut = avango.SFInt()
    MaxStatsNum = avango.SFInt()

    def __init__(self):
        self.super(StatusToggleTrigger).__init__()

        self.StatsNumOut.value = 0
        self.MaxStatsNum.value = 3

    def on_up_transition(self):
        self.StatsNumOut.value =  (self.StatsNumOut.value+1)%self.MaxStatsNum.value


#init avango.display
argv = avango.display.init(sys.argv)
view1 = avango.display.make_view()

#create a wiimote device
wiimote = avango.display.make_device("wiimote3")

#create a stats viewer device
stats_viewer = avango.display.make_device("StatsViewer")

#continue only if a wiimote and the statsviewer device were successfully created (The TwoView display supports this)
if not wiimote or not stats_viewer:
    sys.exit()

stats_toggle_tigger = StatusToggleTrigger()
stats_toggle_tigger.Trigger.connect_from(wiimote.Button9) #- button
stats_viewer.StatsNum.connect_from(stats_toggle_tigger.StatsNumOut)

#create a trigger for closing the application
exit_trigger = ExitTrigger()
exit_trigger.Trigger.connect_from(wiimote.Button0) #home button

#add a visual pickeray representation
pickray_transform = avango.osg.nodes.MatrixTransform()
pickray_transform.Matrix.connect_from(wiimote.Matrix)
pickray_object = avango.osg.nodes.LoadFile(Filename="data/models/ray-gray.iv")
pickray_object.add_and_init_field(avango.SFUInt(), "PickMask", 0)
pickray_transform.Children.value.append(pickray_object)

#geometry which can be picked
sphere = avango.osg.nodes.Sphere(Radius=0.3, Matrix=avango.osg.make_trans_mat(0, 1.2, -3), Color=avango.osg.Vec4(1,1,0,1))

#setup the scenegraph
scene_root = avango.osg.nodes.Group()
scene_root.Children.value = [pickray_transform, sphere]
view1.Root.value = scene_root

#drag interaction
pick_selector = avango.tools.nodes.PickSelector()
pick_selector.PickTrigger.connect_from(wiimote.Button5) #A Button
pick_selector.PickRayTransform.connect_from(wiimote.Matrix)
pick_selector.RootNode.value = scene_root
drag_tool = avango.tools.nodes.DragTool()
drag_tool.DragTransform.connect_from(wiimote.Matrix)
drag_tool.Targets.connect_from(pick_selector.SelectedTargets)

display.run()
