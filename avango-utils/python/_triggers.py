import avango
import avango.script
from avango.script import field_has_changed

class ImmediateEdgeTrigger(avango.script.Script):
    """
    Triggers immediately (as soon as the field has been changed) on 
    the edge transitions of the value of the Trigger field.
    """
    
    Trigger = avango.SFBool()

    def __init__(self):
        self._last_trigger = False

    @field_has_changed(Trigger)
    def trigger_changed(self):
        callback = None
        if self.Trigger.value:
            if not self._last_trigger:
                self._last_trigger = True
                self.on_up_transition()
        else:
            if self._last_trigger:
                self._last_trigger = False
                self.on_down_transition()

    def on_up_transition(self):
        pass
    def on_down_transition(self):
        pass



class EdgeTrigger(avango.script.Script):
    "Triggers on the edge transitions of the value of the Trigger field."
    Trigger = avango.SFBool()

    def __init__(self):
        self._last_trigger = False

    def evaluate(self):
        callback = None
        if self.Trigger.value:
            if not self._last_trigger:
                self._last_trigger = True
                self.on_up_transition()
        else:
            if self._last_trigger:
                self._last_trigger = False
                self.on_down_transition()
            
    def on_up_transition(self):
        pass
    def on_down_transition(self):
        pass