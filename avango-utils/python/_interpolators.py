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
        self.MaxVal.value = 1.0
        self.Duration.value = 1.0
        
        self.__start_time = 0.0
        
        self.__start_interpolation = False
        self.__running = False
        self.__shutdown = False
        
        self.__time_sensor = avango.nodes.TimeSensor()
        
        self.Name.value = "Interpolator"
        
#    def __del__(self):
#        print "~Interpolator"
    
    def do_cleanup(self):
#        print "do_cleanup"
        self.__time_sensor.disconnect_and_clear_all_fields()
        self.disconnect_and_clear_all_fields()
        self.__shutdown = True
        
    @field_has_changed(Start)
    def start_changed(self):
        if self.Start.value and not self.__running:
            self.__start_interpolation = True
            
    def stop_interpolation(self):
        self.TimeIn.disconnect()
        
    def evaluate(self):
        if self.__shutdown:
            return
        
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
         
def mix_color(col1, col2, val):
    return col1*val + col2*(1-val)
            
            
def make_color_fading(start_color, end_color, duration):
    color_fader = ColorFader()
    color_fader.StartColor.value = start_color
    color_fader.EndColor.value = end_color
    color_fader.Duration.value = duration
    
    return color_fader
    
    
class ColorFader(avango.script.Script):
    
    StartColor = avango.gua.SFVec4()
    EndColor = avango.gua.SFVec4()
    Duration = avango.SFFloat()
    
    StartFade = avango.SFBool()
    FadeFinished = avango.SFBool()
    
    RawInterpolatedValue = avango.SFFloat()
    Color = avango.gua.SFVec4()
    
    def __init__(self):
        self.super(ColorFader).__init__()
        
        self.StartFade.value = False
        self.FadeFinished.value = False
        self.Color.value = avango.gua.Vec4(1,1,1,1)
        self.__interpolator_running = True
        
        self.__interpolator = Interpolator()
        self.__interpolator.MinVal.value = 0.0
        self.__interpolator.MaxVal.value = 1.0
        
        self.__interpolator.Duration.connect_from(self.Duration)
        self.__interpolator.Start.connect_from(self.StartFade)

        self.FadeFinished.connect_from(self.__interpolator.Finished)
        self.RawInterpolatedValue.connect_weak_from(self.__interpolator.Value)
        
        self.Name.value = "ColorFader"
        
    @field_has_changed(StartFade)
    def start_changed(self):
        self.__interpolator_running = True
        
    @field_has_changed(FadeFinished)
    def fade_finished_changed(self):
        self.__interpolator_running = False
        
    def evaluate(self):
        if self.__interpolator_running:
            val = self.RawInterpolatedValue.value
            self.Color.value = mix_color(self.EndColor.value,self.StartColor.value,val)

        
