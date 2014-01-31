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
        print "###"
        for tInfo in self.TrackerInfo.value:
            print str(tInfo.Number.value)
            print str(tInfo.Matrix.value)
            
                    
#init avango.display
argv = avango.display.init(sys.argv)
view = avango.display.make_view()
view.EnableTrackball.value = False

dev = avango.vrpn.nodes.DTrackDevice()

#set up a simple scenegraph
rootNode = avango.osg.nodes.MatrixTransform();
rootNode.Matrix.value = avango.osg.make_trans_mat(0,1.7,-20)
view.Root.value = rootNode

#add a sphere
sphere = avango.osg.nodes.Sphere()
rootNode.Children.value.append(sphere)


gamepad = avango.vrpn.nodes.Device()
#wiimote.VRPNID.value="WiiMote0@129.26.74.41"
gamepad.VRPNID.value="XInput0@localhost"

printer = avango.utils.ScriptFieldPrinter()
printer.Script.value = gamepad

#create a service and register the wiimote device
service = avango.vrpn.nodes.Service()
service.always_evaluate(True)
service.Devices.value.append(gamepad)

##create a simple moving device
#mover = Mover()
#mover.UpTrigger.connect_from(wiimote.Button10)
#mover.DownTrigger.connect_from(wiimote.Button9)
#mover.LeftTrigger.connect_from(wiimote.Button7)
#mover.RightTrigger.connect_from(wiimote.Button8)
#sphere.Matrix.connect_from(mover.Matrix)

#wiimote.LED3.value = True

#rumble toggle
#wiimote.Rumble.connect_from(wiimote.Button3)

#radius changer
#radiusRumbler = RadiusRumbler(Radius = 1.0)
#sphere.Radius.connect_from(radiusRumbler.Radius)
#radiusRumbler.Trigger.connect_from(wiimote.Button3)

#run the avango mainloop
avango.display.run()

