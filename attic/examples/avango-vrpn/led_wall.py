import avango
import avango.osg
import avango.display
import avango.script
from avango.script import field_has_changed
import avango.vrpn

import sys

class Mover(avango.script.Script):
    LeftTrigger = avango.SFBool()
    RightTrigger = avango.SFBool()
    UpTrigger = avango.SFBool()
    DownTrigger = avango.SFBool()
    
    ResetTrigger = avango.SFBool()
    Matrix = avango.osg.SFMatrix()
    
    def __init__(self):
        self.super(Mover).__init__()
        self._dist = 0.1;
        
        self.Matrix.value = avango.osg.make_ident_mat()
        
    def evaluate(self):
        
        if self.UpTrigger.value:
            self.Matrix.value *= avango.osg.make_trans_mat(0,self._dist,0)
        if self.DownTrigger.value:
            self.Matrix.value *= avango.osg.make_trans_mat(0,-self._dist,0)
        if self.LeftTrigger.value:
            self.Matrix.value *= avango.osg.make_trans_mat(-self._dist,0,0)
        if self.RightTrigger.value:
            self.Matrix.value *= avango.osg.make_trans_mat(self._dist,0,0)
        if self.ResetTrigger.value:
            self.ResetTrigger.value = avango.osg.make_ident_mat()
            
class RadiusRumbler(avango.script.Script):
    
    Trigger = avango.SFBool()
    Radius = avango.SFFloat()
    
    def __init__(self):
        self.super(RadiusRumbler).__init__()
        self._rad_delta = 0.02
        self._enlarge = True
    
    @field_has_changed(Trigger)
    def trigger_changed(self):
        if self.Trigger.value:
            self.always_evaluate(True)
        else:
            self.always_evaluate(False)
    
    def evaluate(self):
        if self.Trigger.value:
            delta = self._rad_delta
            if not self._enlarge:
                delta = -delta
            
            self.Radius.value += delta
            self._enlarge = not self._enlarge
            
class TrackerInfoPrinter(avango.script.Script):
    
    TrackerInfo = avango.vrpn.MFTrackerInformation()
    
    def __init__(self):
        self.super(TrackerInfoPrinter).__init__()
        
    def evaluate(self):
        print "#########"
        for tInfo in self.TrackerInfo.value:
            print str(tInfo.Number.value)
            print str(tInfo.Matrix.value)
            


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
view.EnableTrackball.value = False

#set up a simple scenegraph
rootNode = avango.osg.nodes.MatrixTransform();
rootNode.Matrix.value = avango.osg.make_trans_mat(0,1.7,-20)
view.Root.value = rootNode

#add a sphere
sphere = avango.osg.nodes.Sphere()
rootNode.Children.value.append(sphere)


#create a dtrack vrpn device
generic_vrpn_device, dtrack = avango.display.make_device("DTrackVRPN", ["DTrack@localhost:5000", get_two_view_dtrack_mapping()])

#printer = avango.utils.ScriptFieldPrinter()
#printer.Script.value = dtrack

p = TrackerInfoPrinter()
p.TrackerInfo.connect_from(dtrack.TrackerInfo)

#create a wiimote
wiimote = avango.vrpn.nodes.Wiimote()
#wiimote.VRPNID.value="WiiMote0@129.26.74.41"
wiimote.VRPNID.value="WiiMote0@localhost"

#printer = avango.utils.ScriptFieldPrinter()
#printer.Script.value = wiimote

#create a service and register the wiimote device
service = avango.vrpn.nodes.Service()
service.always_evaluate(True)
service.Devices.value.append(wiimote)
service.Devices.value.append(generic_vrpn_device)

#create a simple moving device
mover = Mover()
mover.UpTrigger.connect_from(wiimote.Button10)
mover.DownTrigger.connect_from(wiimote.Button9)
mover.LeftTrigger.connect_from(wiimote.Button7)
mover.RightTrigger.connect_from(wiimote.Button8)
sphere.Matrix.connect_from(mover.Matrix)

wiimote.LED3.value = True

#rumble toggle
wiimote.Rumble.connect_from(wiimote.Button3)

#radius changer
radiusRumbler = RadiusRumbler(Radius = 1.0)
sphere.Radius.connect_from(radiusRumbler.Radius)
radiusRumbler.Trigger.connect_from(wiimote.Button3)


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

