# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of Avango.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# Avango is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# Avango is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with Avango. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
# Avango is a trademark owned by FhG.                                    #
#                                                                        #
##########################################################################

import avango
import avango.script
import unittest

class A(avango.script.Script):
    Field1 = avango.SFInt()
    Field2 = avango.SFInt()
    Field3 = avango.SFInt()

    def __init__(self):
        self.super(A).__init__()
        self.Field1.value = 0
        self.Field2.value = 0
        self.Field3.value = 0


class UtilityTestCase(unittest.TestCase):

    def setUp(self):
        self.a = A()
        self.b = A()

    def testExplicitOneToOneConnection(self):
        avango.connect_fields(self.b, (self.a, [ ("Field2", "Field1") ]))
        self.a.Field1.value = 42
        self.assertEqual(42, self.b.Field2.value)

    def testImplicitOneToOneConnection(self):
        avango.connect_fields(self.b, (self.a, [ "Field1" ]))
        self.a.Field1.value = 42
        self.assertEqual(42, self.b.Field1.value)

    def testManyToManyConnection(self):
        avango.connect_fields(self.b, (self.a, [ "Field1", ("Field2", "Field3"), ("Field3", "Field2") ]))
        self.a.Field1.value = 1
        self.a.Field2.value = 2
        self.a.Field3.value = 3
        self.assertEqual(1, self.b.Field1.value)
        self.assertEqual(3, self.b.Field2.value)
        self.assertEqual(2, self.b.Field3.value)


def Suite():
    suite = unittest.TestSuite()

    UtilityTests= [
        'testExplicitOneToOneConnection',
        'testImplicitOneToOneConnection',
        'testManyToManyConnection',
    ]
    suite.addTests(map(UtilityTestCase, UtilityTests))

    return suite
