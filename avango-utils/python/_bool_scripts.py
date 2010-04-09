from _nodes import *

import avango
import avango.script
            
def make_bool2_or(input1,input2):
    bool2_or = nodes.Bool2Or()
    bool2_or.Input1.connect_from(input1)
    bool2_or.Input2.connect_from(input2)
    return bool2_or.Output
            
def make_bool2_and(input1,input2):
    bool2_and = nodes.Bool2And()
    bool2_and.Input1.connect_from(input1)
    bool2_and.Input2.connect_from(input2)
    return bool2_and.Output

def make_bool3_and(input1,input2,input3):
    bool3_and = nodes.Bool3And()
    bool3_and.Input1.connect_from(input1)
    bool3_and.Input2.connect_from(input2)
    bool3_and.Input3.connect_from(input3)
    return bool3_and.Output

def make_bool3_or(input1,input2,input3):
    bool3_and = nodes.Bool3Or()
    bool3_and.Input1.connect_from(input1)
    bool3_and.Input2.connect_from(input2)
    bool3_and.Input3.connect_from(input3)
    return bool3_and.Output