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
        
        
        
class ScreenSpaceToPixelTransform(avango.script.Script):

    ScreenTransform = avango.osg.SFMatrix()
    ViewerTransform = avango.osg.SFMatrix()
    RealSize = avango.osg.SFVec2()
    PixelSize = avango.osg.SFVec2()
    Transform = avango.osg.SFMatrix()
    
    def __init__(self):
        self.super(ScreenSpaceToPixelTransform).__init__()
        self.RealSize.value = avango.osg.Vec2(1,1)
        self.PixelSize.value = avango.osg.Vec2(1600,1200)

    def evaluate(self):
        x_trans = -(self.PixelSize.value.x*0.5)
        y_trans = -(self.PixelSize.value.y*0.5)
        z_trans = 0
        trans = avango.osg.Vec3(x_trans,y_trans,z_trans)
        scale = self.RealSize.value.x / self.PixelSize.value.x
        scale_mat = avango.osg.make_scale_mat(scale, scale, scale)
        trans_mat = avango.osg.make_trans_mat(trans)

        self.Transform.value =  trans_mat * scale_mat * self.ScreenTransform.value * self.ViewerTransform.value
