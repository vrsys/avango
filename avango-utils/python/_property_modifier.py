import avango
import avango.script


class PropertyModifierInt(avango.script.Script):
    
    Enable = avango.SFBool()
    
    Property = avango.SFInt()
    PropertyMin = avango.SFInt()
    PropertyMax = avango.SFInt()
    
    TimeIn = avango.SFDouble()
    
    PropertyStepSize = avango.SFInt()
    PropertyInc = avango.SFBool()
    PropertyDec = avango.SFBool()
    TriggerTimeDelta = avango.SFFloat()
    
    Triggered = avango.SFBool()
    
    def __init__(self):
        self.always_evaluate(True)
        self.last_trigger_time = TimeIn.value
        
        self.PropertyInc.value = False
        self.PropertyDec.value = False
        
        self.Enable.value = False
        
        self.PropertyStepSize.value = 1
        self.PropertyMin.value = 0
        self.PropertyMax.value = 100000
        self.TriggerTimeDelta.value = 0.05
        
    def evaluate(self):
        
        if (not self.PropertyInc.value and not self.PropertyDec.value) or not self.Enable.value:
            return
        
        time = TimeIn.value
        time_delta = time - self.last_trigger_time
        if time_delta > self.TriggerTimeDelta.value:
                        
            if self.PropertyInc.value:
                new_val = self.Property.value+self.PropertyStepSize.value
                if new_val <= self.PropertyMax.value:
                    self.Property.value = new_val
                    self.Triggered.value = True
                self.last_trigger_time = time
                    
            elif self.PropertyDec.value:
                new_val = self.Property.value-self.PropertyStepSize.value
                if new_val >= self.PropertyMin.value:
                    self.Property.value = new_val
                    self.Triggered.value = True
                self.last_trigger_time = time