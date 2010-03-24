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
        
def create_and_connect_keyboard(eventfields):
    keyboard = KeyboardDevice()
    
    keyboard.KeysPressed.connect_from(eventfields.KeysPressed)
    
    keyboard.KeyRight.connect_from(eventfields.KeyRight)
    keyboard.KeyLeft.connect_from(eventfields.KeyLeft)
    keyboard.KeyUp.connect_from(eventfields.KeyUp)
    keyboard.KeyDown.connect_from(eventfields.KeyDown)
    keyboard.PageUp.connect_from(eventfields.KeyPageUp)
    keyboard.PageDown.connect_from(eventfields.KeyPageDown)
    keyboard.Shift.connect_from(eventfields.KeyShift)
    keyboard.Ctrl.connect_from(eventfields.KeyCtrl) 
    keyboard.KeyAlt.connect_from(eventfields.KeyAlt)
    keyboard.Insert.connect_from(eventfields.KeyInsert)
    keyboard.Delete.connect_from(eventfields.KeyDelete)
    keyboard.Home.connect_from(eventfields.KeyHome)
    keyboard.End.connect_from(eventfields.KeyEnd)
    keyboard.KeyEsc.connect_from(eventfields.KeyEsc)
    keyboard.KeySpace.connect_from(eventfields.KeySpace)
    keyboard.KeyEnter.connect_from(eventfields.KeyEnter)
    keyboard.AltReturn.connect_from(eventfields.KeyAltReturn)
    keyboard.KeyF1.connect_from(eventfields.KeyF1)
    keyboard.KeyF2.connect_from(eventfields.KeyF2)
    keyboard.KeyF3.connect_from(eventfields.KeyF3)
    keyboard.KeyF4.connect_from(eventfields.KeyF4)
    keyboard.KeyF5.connect_from(eventfields.KeyF5)
    keyboard.KeyF6.connect_from(eventfields.KeyF6)
    keyboard.KeyF7.connect_from(eventfields.KeyF7)
    keyboard.KeyF8.connect_from(eventfields.KeyF8)
    keyboard.KeyF9.connect_from(eventfields.KeyF9)
    keyboard.KeyF10.connect_from(eventfields.KeyF10)
    keyboard.KeyF11.connect_from(eventfields.KeyF11)
    keyboard.KeyF12.connect_from(eventfields.KeyF12)
    
    return keyboard
