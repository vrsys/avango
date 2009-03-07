import avango
import avango.osg
import avango.script

class View(avango.script.Script):

    Root = avango.osg.SFNode()
    Camera = avango.osg.SFMatrix()
    Viewport = avango.osg.SFVec4()
    Depth = avango.SFFloat()
    UserSelector = avango.SFString()
    Near = avango.SFFloat()
    Far = avango.SFFloat()
    BackgroundColor = avango.osg.SFVec4()

    def __init__(self):
        self.Viewport.value = avango.osg.Vec4(0, 0, 1, 1)
        self.Depth.value = 0
        self.Near.value = 0.1
        self.Far.value = 500.
        self.BackgroundColor.value = avango.osg.Vec4(0., 0., 0., 1.)

    def evaluate(self):
        pass


