import avango
import avango.gua
import avango.script
from avango.script import field_has_changed


class %CLASSNAME%(avango.script.Script):

%FIELD_DECLARATIONS%

    def __init__(self):
        self.super(%CLASSNAME%).__init__()

    def evaluate(self):
        #code here
        pass
