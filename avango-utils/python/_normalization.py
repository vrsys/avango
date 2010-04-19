import avango
import avango.script


class FloatNormalization(avango.script.Script):
    MinIn = avango.SFFloat()
    MaxIn = avango.SFFloat()
    
    MinOut = avango.SFFloat()
    MaxOut = avango.SFFloat()
    
    ValueIn = avango.SFFloat()
    ValueOut = avango.SFFloat()
    
    def __init__(self):
        self.super(FloatNormalization).__init__()
        
        self.MinIn.value = -1.0
        self.MaxIn.value = 1.0
        
        self.MinOut.value = 0.0
        self.MaxOut.value = 1.0
        
    def evaluate(self):
        val_in = self.ValueIn.value
        min_in = self.MinIn.value
        max_in = self.MaxIn.value
        min_out = self.MinOut.value
        max_out = self.MaxOut.value
        
        assert(min_in < max_in)
        assert(min_out < max_out)
        assert(val_in >= min_in)
        assert(val_in <= max_in)
        
        if min_in < 0:
            val_in += (-min_in)
            val_in /= max_in+(-min_in)
        else:
            val_in -= min_in
            val_in /= max_in-min_in
            
        self.ValueOut.value = min_out + val_in*(max_out-min_out)
        
         