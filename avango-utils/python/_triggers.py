import avango
import avango.script
from avango.script import field_has_changed

class ImmediateTriggerCallback(avango.script.Script):
    Trigger = avango.SFBool()
    EnableCallback = avango.script.SFObject()
    DisableCallback = avango.script.SFObject()

    def __init__(self):
        self._last_trigger = False

    def cleanup(self):
        self.disconnect_and_clear_all_fields()
        self._last_trigger = False

    @field_has_changed(Trigger)
    def trigger_changed(self):
        callback = None
        if self.Trigger.value:
            if not self._last_trigger:
                self._last_trigger = True
                callback = self.EnableCallback.value
        else:
            if self._last_trigger:
                self._last_trigger = False
                callback = self.DisableCallback.value
        if callback is not None:
            callback()


class TriggerCallback(avango.script.Script):
    Trigger = avango.SFBool()
    EnableCallback = avango.script.SFObject()
    DisableCallback = avango.script.SFObject()

    def __init__(self):
        self._last_trigger = False

    def cleanup(self):
        self.disconnect_and_clear_all_fields()
        self._last_trigger = False

    def evaluate(self):
        callback = None
        if self.Trigger.value:
            if not self._last_trigger:
                self._last_trigger = True
                callback = self.EnableCallback.value
        else:
            if self._last_trigger:
                self._last_trigger = False
                callback = self.DisableCallback.value
        if callback is not None:
            callback()