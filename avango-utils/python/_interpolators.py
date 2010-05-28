import avango
import avango.script
from avango.script import field_has_changed


def normalize_float(min_in, max_in, min_out, max_out, val_in):
    if min_in < 0:
        val_in += (-min_in)
        val_in /= max_in+(-min_in)
    else:
        val_in -= min_in
        val_in /= max_in-min_in

    return min_out + val_in*(max_out-min_out)


class Interpolator(avango.script.Script):
    
    TimeIn = avango.SFDouble()
    
    MinVal = avango.SFFloat()
    MaxVal = avango.SFFloat()
    Duration = avango.SFFloat()
    
    Value = avango.SFFloat()
    
    Start = avango.SFBool()
    Started = avango.SFBool()
    Finished = avango.SFBool() 
    
    def __init__(self):
        self.super(Interpolator).__init__()
        
        self.MinVal.value = 0.0
        self.MaxVal.value = 10.0
        self.Duration.value = 1.0
        
        self.__start_time = 0.0
        
        self.__start_interpolation = False
        self.__running = False
        
        
        self.__time_sensor = avango.nodes.TimeSensor()
        
    @field_has_changed(Start)
    def start_changed(self):
        if self.Start.value and not self.__running:
            self.__start_interpolation = True
            
    def stop_interpolation(self):
        self.TimeIn.disconnect()
        
    def evaluate(self):
        #start received
        if not self.__running and self.__start_interpolation: 
            self.TimeIn.connect_from(self.__time_sensor.Time)
            self.__start_time = self.TimeIn.value
            self.Started.value = True
            self.__running = True
            
        #actual time
        actual_time = self.TimeIn.value
        #get elapsed time
        elapsed = actual_time - self.__start_time
        #get percentage of the complete duration
        process = elapsed / self.Duration.value
        
        value = normalize_float(0,1,self.MinVal.value,self.MaxVal.value,process)
        
        if process >= 1.0:
            value = self.MaxVal.value
                
        self.Value.value = value
        
        #check for modes
        if process >= 1.0:
            self.Started.value = False
            self.Finished.value = True
            
            self.__running = False
            self.__start_interpolation = False
            
            self.stop_interpolation()