import avango
import avango.osg
import avango.display
import avango.script
from avango.script import field_has_changed
import avango.vrpn

import sys

def get_two_view_dtrack_mapping():
   return [[]] 
                    
#init avango.display
argv = avango.display.init(sys.argv)
view = avango.display.make_view()
view.EnableTrackball.value = False


#set up a simple scenegraph
rootNode = avango.osg.nodes.MatrixTransform();
rootNode.Matrix.value = avango.osg.make_trans_mat(0,1.7,-20)
view.Root.value = rootNode

#add a sphere
sphere = avango.osg.nodes.Sphere()
rootNode.Children.value.append(sphere)

#create a dTrack device
generic_dtrack_device = avango.vrpn.nodes.Device()
generic_dtrack_device.VRPNID.value="DTrack@localhost"
dtrack_device = avango.vrpn.nodes.DTrackDevice()
#connect the tracker output from the generic device
dtrack_device.TrackerInfo.connect_from(generic_dtrack_device.TrackerInfo)
#register some interested tracker ids and names
dtrack_device.populate_interested_target_ids([[0,"dtrack1"]], False)

#create a service and register the dtrack device
service = avango.vrpn.nodes.Service()
service.always_evaluate(True)
service.Devices.value.append(generic_dtrack_device)


#run the avango mainloop
avango.display.run()

