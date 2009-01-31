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
