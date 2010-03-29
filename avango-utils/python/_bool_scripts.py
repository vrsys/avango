import avango
import avango.script

class Bool2And(avango.script.Script):
    
    Input1 = avango.SFBool()
    Input2 = avango.SFBool()
    
    Output = avango.SFBool()
    
    def evaluate(self):
        if self.Input1.value and self.Input2.value:
            self.Output.value = True
        else: 
            self.Output.value = False
            
class Bool2Or(avango.script.Script):
    
    Input1 = avango.SFBool()
    Input2 = avango.SFBool()
    
    Output = avango.SFBool()
    
    def evaluate(self):
        if self.Input1.value or self.Input2.value:
            self.Output.value = True
        else: 
            self.Output.value = False

class Bool3And(avango.script.Script):
    
    Input1 = avango.SFBool()
    Input2 = avango.SFBool()
    Input3 = avango.SFBool()
    
    Output = avango.SFBool()
    
    def evaluate(self):
        if self.Input1.value and self.Input2.value and self.Input3.value:
            self.Output.value = True
        else: 
            self.Output.value = False
            
class Bool3Or(avango.script.Script):
    
    Input1 = avango.SFBool()
    Input2 = avango.SFBool()
    Input3 = avango.SFBool()
    
    Output = avango.SFBool()
    
    def evaluate(self):
        if self.Input1.value or self.Input2.value or self.Input3.value:
            self.Output.value = True
        else: 
            self.Output.value = False
            
def make_bool2_or(input1,input2):
    bool2_or = Bool2Or()
    bool2_or.Input1.connect_from(input1)
    bool2_or.Input2.connect_from(input2)
    return bool2_or.Output
            
def make_bool2_and(input1,input2):
    bool2_and = Bool2And()
    bool2_and.Input1.connect_from(input1)
    bool2_and.Input2.connect_from(input2)
    return bool2_and.Output

def make_bool3_and(input1,input2,input3):
    bool3_and = Bool3And()
    bool3_and.Input1.connect_from(input1)
    bool3_and.Input2.connect_from(input2)
    bool3_and.Input3.connect_from(input3)
    return bool3_and.Output

def make_bool3_or(input1,input2,input3):
    bool3_and = Bool3Or()
    bool3_and.Input1.connect_from(input1)
    bool3_and.Input2.connect_from(input2)
    bool3_and.Input3.connect_from(input3)
    return bool3_and.Output