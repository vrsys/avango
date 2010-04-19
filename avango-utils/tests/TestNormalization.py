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
import time


class Widget(avango.script.Script):
    
    Value = avango.SFFloat()
    
    def __init__(self):
        self.super(Widget).__init__()

                  
        
class NormalizationTestCase(unittest.TestCase):
    def testMakeInstance(self):
        self.assert_(avango.utils.nodes.FloatNormalization())
        
    def testPositiveNormalization(self):
        normalization = avango.utils.nodes.FloatNormalization()
        normalization.MinIn.value = 5.0
        normalization.MaxIn.value = 10.0
        normalization.MinOut.value = 0.0
        normalization.MaxOut.value = 1.0
        normalization.ValueIn.value = 7.5
        avango.evaluate()
        self.assertEqual(normalization.ValueOut.value, 0.5)

        normalization.MinOut.value = 11.0
        normalization.MaxOut.value = 12.0
        avango.evaluate()
        self.assertEqual(normalization.ValueOut.value, 11.5)
        
        normalization.MinIn.value = 0.0
        normalization.MaxIn.value = 360.0
        normalization.MinOut.value = 0.0
        normalization.MaxOut.value = 1.0
        normalization.ValueIn.value = 90.0
        avango.evaluate()
        self.assertEqual(normalization.ValueOut.value, 0.25)
        
    def testNegativeNormalization(self):
        normalization = avango.utils.nodes.FloatNormalization()
        normalization.MinIn.value = -10.0
        normalization.MaxIn.value = -5.0
        normalization.MinOut.value = 0.0
        normalization.MaxOut.value = 1.0
        normalization.ValueIn.value = -7.5
        avango.evaluate()
        self.assertEqual(normalization.ValueOut.value, 0.5)

        normalization.MinIn.value = -20.0
        normalization.MaxIn.value = -10.0
        normalization.MinOut.value = -1.0
        normalization.MaxOut.value = 0.0
        normalization.ValueIn.value = -15.0
        avango.evaluate()
        self.assertEqual(normalization.ValueOut.value, -0.5)
        
    def testZeroCrossNormalization(self):
        normalization = avango.utils.nodes.FloatNormalization()
        normalization.MinIn.value = -10.0
        normalization.MaxIn.value = 10.0
        normalization.MinOut.value = 0.0
        normalization.MaxOut.value = 1.0
        normalization.ValueIn.value = 0.0
        avango.evaluate()
        self.assertEqual(normalization.ValueOut.value, 0.5)

        normalization.MinIn.value = -10.0
        normalization.MaxIn.value = 90.0
        normalization.MinOut.value = -1.0
        normalization.MaxOut.value = 0.0
        normalization.ValueIn.value = 40.0
        avango.evaluate()
        self.assertEqual(normalization.ValueOut.value, -0.5)
        
    def testCreatorFunction(self):
        w1 = Widget()
        w2 = Widget()
        w1.Value.value = 0.5
        
        w2.Value.connect_from( avango.utils.make_float_normalization(-1,1,0,1,w1.Value) )
        avango.evaluate()
        self.assertEqual(w2.Value.value, 0.75)
        
        w1.Value.value = 1.0
        avango.evaluate()
        self.assertEqual(w2.Value.value, 1.0)
        
def Suite():
   suite = unittest.TestLoader().loadTestsFromTestCase(NormalizationTestCase)
   return suite
