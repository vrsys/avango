# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# AVANGO is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# AVANGO is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
##########################################################################

import avango
import avango.osg
import avango.utils
import unittest

class AddSubtractImmediateDeltaTrigger(avango.utils.ImmediateEdgeTrigger):
    Delta = avango.SFInt()
    Value = avango.SFInt()
    
    def __init__(self):
        self.super(AddSubtractImmediateDeltaTrigger).__init__()
        self.Delta.value = 1
        self.Value.value = 0
    
    def on_up_transition(self):
        self.Value.value += self.Delta.value
        
    def on_down_transition(self):
        self.Value.value -= self.Delta.value
      
class AddSubtractDeltaTrigger(avango.utils.EdgeTrigger):
    Delta = avango.SFInt()
    Value = avango.SFInt()
    
    def __init__(self):
        self.super(AddSubtractDeltaTrigger).__init__()
        self.Delta.value = 1
        self.Value.value = 0
    
    def on_up_transition(self):
        self.Value.value += self.Delta.value
        
    def on_down_transition(self):
        self.Value.value -= self.Delta.value
          
        
class TriggersTestCase(unittest.TestCase):
    def testMakeInstance(self):
        self.assert_(avango.utils.nodes.ImmediateEdgeTrigger())
        self.assert_(avango.utils.nodes.EdgeTrigger())
        
    def testImmediateEdgeTrigger(self):
        field_container = AddSubtractImmediateDeltaTrigger()
        self.assertEqual(field_container.Value.value, 0)
        field_container.Trigger.value = True
        self.assertEqual(field_container.Value.value, 1)
        field_container.Trigger.value = True
        self.assertEqual(field_container.Value.value, 1)
        field_container.Trigger.value = False
        self.assertEqual(field_container.Value.value, 0)
        field_container.Trigger.value = False
        self.assertEqual(field_container.Value.value, 0)
  

    def testEdgeTrigger(self):
        field_container = AddSubtractDeltaTrigger()
        self.assertEqual(field_container.Value.value, 0)
        field_container.Trigger.value = True
        self.assertEqual(field_container.Value.value, 0)
        avango.evaluate()
        self.assertEqual(field_container.Value.value, 1)
        
        
        field_container.Trigger.value = False
        self.assertEqual(field_container.Value.value, 1)
        avango.evaluate()
        self.assertEqual(field_container.Value.value, 0)
        field_container.Trigger.value = False
        self.assertEqual(field_container.Value.value, 0)
    

def Suite():
   suite = unittest.TestLoader().loadTestsFromTestCase(TriggersTestCase)
   return suite
