import avango
import avango.script
import avango.osg

class FloatXAdd(avango.script.Script):
    "Add x float values"
    
    BaseFieldName = avango.SFString()
    NumFieldsOut = avango.SFInt() 
    
    Output = avango.SFFloat()
    
    def __init__(self):
        self.super(FloatXAdd).__init__()  
        
        self.Output.value = 0      
        self.BaseFieldName.value = "Input"
        self.NumFields.value = 0
        
        self.__actual_id=0
    
    def add_and_connect_float_field(self,field):
        
        field_name = self.BaseFieldName.value + str(self.__actual_id)
        if self.has_field(field_name):
            return
        #create and add the new field
        self.add_and_init_field(avango.SFFloat(), field_name, 0)
        
        #connect the field with the given field
        getattr(self, field_name).connect_from(field)
        
        self.__actual_id += 1
        self.NumFieldsOut.value = self.__actual_id
       
    def evaluate(self):
        sum = 0
        for field_id in range(0,self.__actual_id):
            field_name = self.BaseFieldName.value + str(field_id)
            field = self.get_field(field_name)
            if not field:
                continue
            sum += field.value
         
        self.Output.value = sum    
    

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

class SFNode2MFContainerConverter(avango.script.Script):
    "Converts a SFNode to a MFNode"

    Input = avango.osg.SFNode()
    Output = avango.MFContainer()

    def evaluate(self):
        self.Output.value = [ self.Input.value ]

class Float2Vec2Converter(avango.script.Script):
    "Converts two Floats into on Vec2"

    Value0 = avango.SFFloat()
    Value1 = avango.SFFloat()
    Output = avango.osg.SFVec2()

    def evaluate(self):
        self.Output.value = avango.osg.Vec2(self.Value0.value, self.Value1.value)
        