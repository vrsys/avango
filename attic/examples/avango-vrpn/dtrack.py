import avango
import avango.osg
import avango.display
import avango.script
from avango.script import field_has_changed
import avango.vrpn
import avango.tools

import sys
from math import *

class ExitTrigger(avango.utils.EdgeTrigger):

    def on_up_transition(self):
        sys.exit()

def get_two_view_dtrack_mapping():
    return [
    [0,"ve-dtrack-head1"] ,
    [1, "ve-dtrack-head2"] ,
    [2, "ve-dtrack-head3"] ,
    [3, "ve-dtrack-pda1"] ,
    [4, "ve-dtrack-pda2"] ,
    [5, "ve-dtrack-head4"] ,
    [6, "ve-dtrack-head5"] ,
    [7, "ve-dtrack-head6"] ,
    [8, "ve-dtrack-head7"] ,
    [9, "ve-dtrack-atek"] ,
    [10, "ve-dtrack-reddot"] ,
    [11, "ve-dtrack-raytac"] ,
    [12, "ve-dtrack-spare"] ,
    [13, "ve-dtrack-head8"] ,
    [14, "ve-dtrack-logitech"] ,
    [15, "ve-dtrack-tablet2"] ,
    [16, "ve-dtrack-xpand2"] ,
    [17, "ve-dtrack-xpand1"] ] 
    
#init avango.display
argv = avango.display.init(sys.argv)
view = avango.display.make_view()
#view.EnableTrackball.value = False

#create a dtrack vrpn device
generic_vrpn_device, dtrack = avango.display.make_device("DTrackVRPN", ["DTrack@localhost:18081", get_two_view_dtrack_mapping()])

#get two user objects
user1 = avango.display.get_user(0)
user2 = avango.display.get_user(1)

#set up a simple scenegraph
rootNode = avango.osg.nodes.MatrixTransform();
view.Root.value = rootNode

#add a sphere
sphere = avango.osg.nodes.Sphere(Radius=0.5, Matrix=avango.osg.make_trans_mat(0, 1.2, -5), Color=avango.osg.Vec4(1,1,0,1))
rootNode.Children.value.append(sphere)

printer = avango.utils.ScriptFieldPrinter()

#create a service and register the dtrack device
service = avango.vrpn.nodes.Service()
service.always_evaluate(True)
service.Devices.value.append(generic_vrpn_device)

#create the head tracked targets
perf2osg = avango.osg.make_rot_mat(radians(-90), 1, 0, 0)
user1Tracker = avango.vrpn.create_dtrack_target("ve-dtrack-xpand1", avango.osg.make_trans_mat(avango.osg.Vec3(-0.0825, 0.0, -0.045)), perf2osg)
user2Tracker = avango.vrpn.create_dtrack_target("ve-dtrack-xpand2", avango.osg.make_trans_mat(avango.osg.Vec3(0.12, 0.043, 0.0 )), perf2osg)


osg2perf = avango.osg.make_rot_mat(radians(90), 1, 0, 0)
perf2osg = avango.osg.make_rot_mat(radians(-90), 1, 0, 0)
wiimote_config = {}
wiimote_config["wiimote1"] = ["ve-dtrack-atek",     avango.osg.Vec3( 0.045, 0.0,  0.022)]
wiimote_config["wiimote2"] = ["ve-dtrack-reddot",   avango.osg.Vec3(-0.05 , 0.0,  0.031)]
wiimote_config["wiimote3"] = ["ve-dtrack-logitech", avango.osg.Vec3(-0.04 , 0.0, -0.025)]
wiimote_config["wiimote4"] = ["ve-dtrack-raytac",   avango.osg.Vec3( 0.068, 0.02, 0.035)]
wiimote_offset = wiimote_config["wiimote4"][1]
wiimote_offset.y -= 0.1
wiimote4Tracker = avango.vrpn.create_dtrack_target("ve-dtrack-raytac", osg2perf * avango.osg.make_trans_mat(wiimote_offset), perf2osg)

printer.Script.value = wiimote4Tracker


#add the tracker targets to the dtrack object
dtrack.Targets.value = [user1Tracker, user2Tracker, wiimote4Tracker]

#connect the matrices to the users
user1.Matrix.connect_from(user1Tracker.Matrix)
user2.Matrix.connect_from(user2Tracker.Matrix)


#create a wiimote sensor
wiimote = avango.display.make_device("WiimoteVRPN","WiiMote0@localhost")
#printer.Script.value = wiimote
service.Devices.value.append(wiimote)

exit_trigger = ExitTrigger()
exit_trigger.Trigger.connect_from(wiimote.Button0)

#add a visual pickeray representation
pickray_transform = avango.osg.nodes.MatrixTransform()
pickray_transform.Matrix.connect_from(wiimote4Tracker.Matrix)
pickray_object = avango.osg.nodes.LoadFile(Filename="ray-gray.iv")
pickray_object.add_and_init_field(avango.SFUInt(), "PickMask", 0)
pickray_transform.Children.value.append(pickray_object)
view.Root.value.Children.value.append(pickray_transform)

#drag interaction
pick_selector = avango.tools.nodes.PickSelector()
pick_selector.PickTrigger.connect_from(wiimote.Button3) #A Button
pick_selector.PickRayTransform.connect_from(wiimote4Tracker.Matrix)
pick_selector.RootNode.value = view.Root.value

drag_tool = avango.tools.nodes.DragTool()
drag_tool.DragTransform.connect_from(wiimote4Tracker.Matrix)
drag_tool.Targets.connect_from(pick_selector.SelectedTargets)


#run the avango mainloop
avango.display.run()

