import avango
import avango.script
import avango.osg
from avango.script import field_has_changed

class LoadFileScaler(avango.script.Script):
    LoadFile = avango.osg.SFLoadFile()
    LoadFileFinished = avango.SFString()
    Radius = avango.SFFloat()
    MatrixOut = avango.osg.SFMatrix()
    
    def __init__(self):
        self.super(LoadFileScaler).__init__()
        
        self.Radius.value = 1.0
        self.__file_loaded = False
        
    @field_has_changed(LoadFileFinished)
    def loading_finished(self):
        
        if self.LoadFile.value.Filename.value == self.LoadFileFinished.value:
            self.__file_loaded = True
        
    def evaluate(self):
        if self.__file_loaded:
            self.MatrixOut.value = self.calc_scale_matrix()
            self.__file_loaded = False

    def calc_scale_matrix(self):
        b = avango.osg.calc_bounding_box(self.LoadFile.value)
        width = b.x_max() - b.x_min()
        height = b.y_max() - b.y_min()
        depth = b.z_max() - b.z_min()
        print "LoadFileScaler::width: " + str(width)
        print "LoadFileScaler::height: " + str(height)
        print "LoadFileScaler::depth: " + str(depth)
        max_extend = max(max(width,height),depth)
        scaleFactor = 2.0 / (max_extend);
        scaleFactor *= self.Radius.value;
        print "LoadFileScaler::scaleFactor: " + str(scaleFactor)
        return avango.osg.make_scale_mat(scaleFactor,scaleFactor,scaleFactor)