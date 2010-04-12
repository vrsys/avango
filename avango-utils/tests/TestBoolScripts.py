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

class Widget(avango.script.Script):
    
    Trigger = avango.SFBool()
    
    def __init__(self):
        self.super(Widget).__init__()
        

class BoolScriptsTestCase(unittest.TestCase):
    def testMakeInstance(self):
        
        self.assert_(avango.utils.nodes.Bool2And())
        self.assert_(avango.utils.nodes.Bool2Or())
        self.assert_(avango.utils.nodes.Bool3And())
        self.assert_(avango.utils.nodes.Bool3Or())
        
    def testBoolMerge(self):
        
        w1 = Widget()
        w1.Trigger.value = False
        w2 = Widget()
        w2.Trigger.value = False
        
        combined_widget = avango.utils.merge_and_connect_bool_scripts(w1,w2)
        self.assertEqual(combined_widget.Trigger.value, False)
        
        w1.Trigger.value = True
        avango.evaluate()
        self.assertEqual(combined_widget.Trigger.value, True)
        
        w1.Trigger.value = False
        w2.Trigger.value = True
        avango.evaluate()
        self.assertEqual(combined_widget.Trigger.value, True)
        
        w1.Trigger.value = False
        w2.Trigger.value = False
        avango.evaluate()
        self.assertEqual(combined_widget.Trigger.value, False)
        

def Suite():
   suite = unittest.TestLoader().loadTestsFromTestCase(BoolScriptsTestCase)
   return suite
