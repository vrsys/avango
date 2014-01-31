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

#get matrix mul scripts
user1_mat_mul = avango.display.make_device("MatrixUserMul","user1")
user2_mat_mul = avango.display.make_device("MatrixUserMul","user2")
assert(user1_mat_mul!=None)
assert(user2_mat_mul!=None)

#continue only if a wiimote and the statsviewer device were successfully created (The TwoView display supports this)
if not wiimote or not stats_viewer:
    print "Wiimote or stats viewer is Null. Exit"
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
pickray_object = avango.osg.nodes.LoadFile(Filename="ray-gray.iv")
pickray_object.add_and_init_field(avango.SFUInt(), "PickMask", 0)
pickray_transform.Children.value.append(pickray_object)

#geometry which can be picked
sphere = avango.osg.nodes.Sphere(Radius=0.3, Matrix=avango.osg.make_trans_mat(0, 1.2, -3), Color=avango.osg.Vec4(1,1,0,1))

scale_node = avango.osg.nodes.MatrixTransform(Matrix= avango.osg.make_trans_mat(0,1.7,-10))# * avango.osg.make_scale_mat(10,10,10))

pcd_geode = avango.utils.load_pcd_file("/localhome/ddangelo/apps/avango/examples/avango-utils/table_scene_mug_stereo_textured.pcd")
pcd_center = pcd_geode.get_bounding_sphere().get_center()
pcd_radius = pcd_geode.get_bounding_sphere().radius()

bb = avango.osg.nodes.Box(Matrix = avango.osg.make_trans_mat(pcd_center), Width=pcd_radius*2, Height=pcd_radius*2,
                                                StateSet = avango.osg.nodes.StateSet(WireframeMode = 1, LineWidth=avango.osg.nodes.LineWidth(Width=30)),
                                                Color = avango.osg.Vec4(1.0,0.0,0.0,1.0))

bb.Children.value.append(pcd_geode)
scale_node.Children.value.append(bb)


print "Center: " + str(pcd_geode.get_bounding_sphere().get_center())
print "Radius: " + str(pcd_geode.get_bounding_sphere().radius())


#setup the scenegraph
scene_root = avango.osg.nodes.Group()
scene_root.Children.value = [pickray_transform, sphere, scale_node]
#scene_root.Children.value = [pickray_transform, sphere]
view1.Root.value = scene_root


scene_root.Children.value.append(bb)

#drag interaction
pick_selector = avango.tools.nodes.PickSelector()
pick_selector.PickTrigger.connect_from(wiimote.Button5) #A Button
pick_selector.PickRayTransform.connect_from(wiimote.Matrix)
pick_selector.RootNode.value = scene_root


drag_tool = avango.tools.nodes.ScaledGrabTool()
drag_tool.HeadTransform.connect_from(avango.display.get_user(0).Matrix)
drag_tool.DeviceTransform.connect_from(wiimote.Matrix)
drag_tool.Targets.connect_from(pick_selector.SelectedTargets)

display.run()
