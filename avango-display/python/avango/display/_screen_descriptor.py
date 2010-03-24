import avango
import avango.osg
import avango.script

class ScreenDescriptor(avango.script.Script):

    Transform = avango.osg.SFMatrix()
    RealSize = avango.osg.SFVec2()
    PixelSize = avango.osg.SFVec2()

    def __init__(self):
        self.super(ScreenDescriptor).__init__()
        self.RealSize.value = avango.osg.Vec2(1., 1.)
        self.PixelSize.value = avango.osg.Vec2(1., 1.)
    

class ScreenSpaceTransform(avango.script.Script):

    ScreenTransform = avango.osg.SFMatrix()
    ViewerTransform = avango.osg.SFMatrix()
    RealSize = avango.osg.SFVec2()
    Transform = avango.osg.SFMatrix()

    def evaluate(self):
        scale = 0.5 * min(self.RealSize.value.x, self.RealSize.value.y)
        scale_mat = avango.osg.make_scale_mat(scale, scale, scale)
        self.Transform.value = scale_mat * self.ScreenTransform.value * self.ViewerTransform.value
