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

    Value = avango.SFFloat()

    def __init__(self):
        self.super(Widget).__init__()


class ConvertersTestCase(unittest.TestCase):
    def testMakeInstance(self):
        self.assert_(avango.utils.nodes.FloatXSum())

    def testFloatXSum(self):

        sum = avango.utils.nodes.FloatXSum()
        sum.BaseFieldName.value = "InputField"

        w1 = Widget(Value=1)
        sum.add_and_connect_float_field(w1.Value)
        w2 = Widget(Value=2)
        sum.add_and_connect_float_field(w2.Value)
        avango.evaluate()
        self.assertEqual(sum.Output.value, 3)

        w3 = Widget(Value=-3)
        sum.add_and_connect_float_field(w3.Value)
        self.assertEqual(sum.InputField2.value, -3)
        avango.evaluate()
        self.assertEqual(sum.Output.value, 0)

        w3.Value.value = 39
        avango.evaluate()
        self.assertEqual(sum.Output.value, 42)


    def testFloatXMin(self):

        min = avango.utils.nodes.FloatXMin()
        min.BaseFieldName.value = "InputField"

        w1 = Widget(Value=1)
        min.add_and_connect_float_field(w1.Value)
        w2 = Widget(Value=223)
        min.add_and_connect_float_field(w2.Value)
        avango.evaluate()
        self.assertEqual(min.Output.value, 1)

        w2.Value.value = -42
        avango.evaluate()
        self.assertEqual(min.Output.value, -42)

def Suite():
   suite = unittest.TestLoader().loadTestsFromTestCase(ConvertersTestCase)
   return suite
