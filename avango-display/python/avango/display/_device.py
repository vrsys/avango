import avango.osg.viewer
from math import *

class User(avango.script.Script):
    Active = avango.SFBool()
    Matrix = avango.osg.SFMatrix()

    def __init__(self):
        self.Active.value = True

class Device(avango.script.Script):
    Active = avango.SFBool()
    Matrix = avango.osg.SFMatrix()
    Button1 = avango.SFBool()
    Button2 = avango.SFBool()
    Button3 = avango.SFBool()
    Button4 = avango.SFBool()
    Button5 = avango.SFBool()

    def __init__(self):
        self.Active.value = True
        
        
class KeyboardDevice(avango.script.Script):
    
    KeysPressed = avango.MFInt()

    KeyRight = avango.SFBool()
    KeyLeft = avango.SFBool()
    KeyUp = avango.SFBool()
    KeyDown = avango.SFBool()
    PageDown = avango.SFBool()
    PageUp = avango.SFBool()
    Shift =  avango.SFBool()
    Ctrl =  avango.SFBool()
    KeyAlt =  avango.SFBool()
    Insert =  avango.SFBool()
    Delete = avango.SFBool()
    Home = avango.SFBool()
    End = avango.SFBool()
    KeyEsc = avango.SFBool()
    KeySpace = avango.SFBool()
    KeyEnter = avango.SFBool()
    AltReturn = avango.SFBool()

    KeyF1 = avango.SFBool()
    KeyF2 = avango.SFBool()
    KeyF3 = avango.SFBool()
    KeyF4 = avango.SFBool()
    KeyF5 = avango.SFBool()
    KeyF6 = avango.SFBool()
    KeyF7 = avango.SFBool()
    KeyF8 = avango.SFBool()
    KeyF9 = avango.SFBool()
    KeyF10 = avango.SFBool()
    KeyF11 = avango.SFBool()
    KeyF12 = avango.SFBool()
 
    def __init__(self):
        self.super(KeyboardDevice).__init__()
        
    def connect(self, eventfields):
        
        self.KeysPressed.connect_from(eventfields.KeysPressed)
    
        self.KeyRight.connect_from(eventfields.KeyRight)
        self.KeyLeft.connect_from(eventfields.KeyLeft)
        self.KeyUp.connect_from(eventfields.KeyUp)
        self.KeyDown.connect_from(eventfields.KeyDown)
        self.PageUp.connect_from(eventfields.KeyPageUp)
        self.PageDown.connect_from(eventfields.KeyPageDown)
        self.Shift.connect_from(eventfields.KeyShift)
        self.Ctrl.connect_from(eventfields.KeyCtrl) 
        self.KeyAlt.connect_from(eventfields.KeyAlt)
        self.Insert.connect_from(eventfields.KeyInsert)
        self.Delete.connect_from(eventfields.KeyDelete)
        self.Home.connect_from(eventfields.KeyHome)
        self.End.connect_from(eventfields.KeyEnd)
        self.KeyEsc.connect_from(eventfields.KeyEsc)
        self.KeySpace.connect_from(eventfields.KeySpace)
        self.KeyEnter.connect_from(eventfields.KeyEnter)
        self.AltReturn.connect_from(eventfields.KeyAltReturn)
        self.KeyF1.connect_from(eventfields.KeyF1)
        self.KeyF2.connect_from(eventfields.KeyF2)
        self.KeyF3.connect_from(eventfields.KeyF3)
        self.KeyF4.connect_from(eventfields.KeyF4)
        self.KeyF5.connect_from(eventfields.KeyF5)
        self.KeyF6.connect_from(eventfields.KeyF6)
        self.KeyF7.connect_from(eventfields.KeyF7)
        self.KeyF8.connect_from(eventfields.KeyF8)
        self.KeyF9.connect_from(eventfields.KeyF9)
        self.KeyF10.connect_from(eventfields.KeyF10)
        self.KeyF11.connect_from(eventfields.KeyF11)
        self.KeyF12.connect_from(eventfields.KeyF12)
        

class MouseDevice(avango.script.Script):
    Matrix = avango.osg.SFMatrix()
    MousePosNorm = avango.osg.SFVec2()
    MousePos = avango.osg.SFVec2()
    MouseMovementNorm = avango.osg.SFVec2()
    MouseButtonLeft = avango.SFBool()
    MouseButtonRight = avango.SFBool()
    MouseButtonMiddle = avango.SFBool()
    MouseScrollUp = avango.SFBool()
    MouseScrollDown = avango.SFBool()   
    MouseLeftDoubleClick = avango.SFBool()
    MouseLeftAndRight = avango.SFBool()
    
    def __init__(self):
        self.super(MouseDevice).__init__()
        
    def connect(self, eventfields, camera, window):
        self.Matrix.connect_from(camera.MouseNearTransform)
    
        self.MousePosNorm.connect_from(window.MousePositionNorm)
        self.MousePos.connect_from(window.MousePosition)
        self.MouseMovementNorm.connect_from(window.MouseMovementNorm)
        
        self.MouseButtonLeft.connect_from(eventfields.MouseButtonLeft)
        self.MouseButtonRight.connect_from(eventfields.MouseButtonRight)
        self.MouseScrollUp.connect_from(eventfields.MouseScrollUp)
        self.MouseScrollDown.connect_from(eventfields.MouseScrollDown)
        self.MouseButtonMiddle.connect_from(eventfields.MouseButtonMiddle)
        self.MouseLeftDoubleClick.connect_from(eventfields.MouseButtonLeftDoubleClick)
        self.MouseLeftAndRight.connect_from(eventfields.MouseButtons_LeftAndRight)
