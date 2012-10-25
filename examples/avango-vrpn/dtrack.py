import avango
import avango.osg
import avango.display
import avango.script
from avango.script import field_has_changed
import avango.vrpn

import sys
from math import *

def get_two_view_dtrack_mapping():
    return [
    [1,"ve-dtrack-head1"] ,
    [2, "ve-dtrack-head2"] ,
    [3, "ve-dtrack-head3"] ,
    [4, "ve-dtrack-pda1"] ,
    [5, "ve-dtrack-pda2"] ,
    [6, "ve-dtrack-head4"] ,
    [7, "ve-dtrack-head5"] ,
    [8, "ve-dtrack-head6"] ,
    [9, "ve-dtrack-head7"] ,
    [10, "ve-dtrack-atek"] ,
    [11, "ve-dtrack-reddot"] ,
    [12, "ve-dtrack-raytac"] ,
    [13, "ve-dtrack-spare"] ,
    [14, "ve-dtrack-head8"] ,
    [15, "ve-dtrack-logitech"] ,
    [16, "ve-dtrack-tablet2"] ,
    [17, "ve-dtrack-xpand2"] ,
    [18, "ve-dtrack-xpand1"] ] 
                    
#init avango.display
argv = avango.display.init(sys.argv)
view = avango.display.make_view()
view.EnableTrackball.value = False

#create a dtrack vrpn device
generic_vrpn_device, dtrack = avango.display.make_device("DTrackVRPN", ["DTrack@localhost", get_two_view_dtrack_mapping()])

#get two user objects
user1 = avango.display.get_user(0)
user2 = avango.display.get_user(1)

#set up a simple scenegraph
rootNode = avango.osg.nodes.MatrixTransform();
rootNode.Matrix.value = avango.osg.make_trans_mat(0,1.7,-20)
view.Root.value = rootNode

#add a sphere
sphere = avango.osg.nodes.Sphere()
rootNode.Children.value.append(sphere)

#create a dTrack device
#generic_dtrack_device = avango.vrpn.nodes.Device()
#generic_dtrack_device.VRPNID.value="DTrack@localhost"
#dtrack_device = avango.vrpn.nodes.DTrackDevice()
##connect the tracker output from the generic device
#dtrack_device.TrackerInfo.connect_from(generic_dtrack_device.TrackerInfo)
##register some interested tracker ids and names
#dtrack_device.populate_interested_target_ids(get_two_view_dtrack_mapping(), False)

#create a service and register the dtrack device
service = avango.vrpn.nodes.Service()
service.always_evaluate(True)
service.Devices.value.append(generic_vrpn_device)

#create the head tracked targets
perf2osg = avango.osg.make_rot_mat(radians(-90), 1, 0, 0)
user1Tracker = avango.vrpn.create_dtrack_target("ve-dtrack-xpand1", avango.osg.make_trans_mat(avango.osg.Vec3(-0.0825, 0.0, -0.045)), perf2osg)
user2Tracker = avango.vrpn.create_dtrack_target("ve-dtrack-xpand2", avango.osg.make_trans_mat(avango.osg.Vec3(0.12, 0.043, 0.0 )), perf2osg)

user1.Matrix.connect_from(user1Tracker.Matrix)
user2.Matrix.connect_from(user2Tracker.Matrix)

#run the avango mainloop
avango.display.run()

