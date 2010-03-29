import avango
import avango.script
import avango.osg

class Float2Add(avango.script.Script):
    "Adds two float values"
    
    Value0 = avango.SFFloat()
    Value1 = avango.SFFloat()
    
    Output = avango.SFFloat()
    
    def evaluate(self):
        self.Output.value = self.Value0.value + self.Value1.value

class Float4AddVec2Converter(avango.script.Script):
    "Converts four Floats into a Vec2, where vec.x = Value00 + Value01 and vec.y = Value10 + Value11"

    Value00 = avango.SFFloat()
    Value01 = avango.SFFloat()
    
    Value10 = avango.SFFloat()
    Value11 = avango.SFFloat()
    
    Output = avango.osg.SFVec2()

    def evaluate(self):
        self.Output.value = avango.osg.Vec2(self.Value00.value+self.Value01.value, self.Value10.value+self.Value11.value)
