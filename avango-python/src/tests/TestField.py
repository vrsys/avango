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
import mock
import unittest
import math


class FieldTestCase(unittest.TestCase):
    def setUp(self):
        self.node = mock.nodes.MockFieldContainer(AnIntField=2)


class SingleFieldTestCase(FieldTestCase):

    def testConstantAddition(self):
        self.assertEqual(3, self.node.AnIntField.value + 1)

    def testInplaceAddition(self):
        self.node.AnIntField.value += 1
        self.assertEqual(3, self.node.AnIntField.value)

    def testArithmetics(self):
        self.node.AnIntField.value *= 3
        self.assertEqual(6, self.node.AnIntField.value)
        self.node.AnIntField.value = 18 / self.node.AnIntField.value
        self.assertEqual(3, self.node.AnIntField.value)
        self.node.AnIntField.value |= 4
        self.assertEqual(7, self.node.AnIntField.value)

    def testEqual(self):
        self.assertEqual(self.node.AnIntField, self.node.AnIntField)

    def testUnequal(self):
        self.failIfEqual(self.node.AnIntField, self.node.AnIntMultiField)

    def testHash(self):
        hash = {}
        hash[self.node.AnIntField] = 5
        self.assertEqual(5, hash[self.node.AnIntField])


class MultiFieldTestCase(FieldTestCase):

    def testMultiFieldInt(self):
        self.assertEqual([], list(self.node.AnIntMultiField.value))
        self.node.AnIntMultiField.value = [1, 2, 3]
        self.assertEqual([1, 2, 3], list(self.node.AnIntMultiField.value))

    def testMixedMultiFieldInt(self):
        self.assertEqual([], list(self.node.AnIntMultiField.value))
        def assign():
            self.node.AnIntMultiField.value = [1, "A", 3]
        self.assertRaises(TypeError, assign)

    def testIndexAccess(self):
        self.node.AnIntMultiField.value = [1, 2, 3]
        self.assertEqual(3, self.node.AnIntMultiField.value[2])

    def testIndexAssignment(self):
        self.node.AnIntMultiField.value = [1, 2, 3]
        self.node.AnIntMultiField.value[2] = 42
        self.assertEqual([1, 2, 42], list(self.node.AnIntMultiField.value))

    def testLen(self):
        self.node.AnIntMultiField.value = [1, 2, 3]
        self.assertEqual(3, len(self.node.AnIntMultiField.value))

    def testIteration(self):
        self.node.AnIntMultiField.value = [1, 2, 3]
        sum = 0
        for v in self.node.AnIntMultiField.value:
            sum += v
        self.assertEqual(6, sum)

    def testDeletion(self):
        self.node.AnIntMultiField.value = [1, 2, 3, 2]
        del self.node.AnIntMultiField.value[2]
        self.assertEqual([1, 2, 2], list(self.node.AnIntMultiField.value))

    def testAppend(self):
        self.node.AnIntMultiField.value = [1, 2, 3]
        self.node.AnIntMultiField.value.append(4)
        self.assertEqual([1, 2, 3, 4], list(self.node.AnIntMultiField.value))

    def testExtend(self):
        self.node.AnIntMultiField.value = [1, 2, 3]
        self.node.AnIntMultiField.value.extend([4, 5])
        self.assertEqual([1, 2, 3, 4, 5], list(self.node.AnIntMultiField.value))

    def testInsert(self):
        self.node.AnIntMultiField.value = [1, 2, 3]
        self.node.AnIntMultiField.value.insert(1, 4)
        self.assertEqual([1, 4, 2, 3], list(self.node.AnIntMultiField.value))

    def testRemove(self):
        self.node.AnIntMultiField.value = [1, 2, 3]
        self.node.AnIntMultiField.value.remove(2)
        self.assertEqual([1, 3], list(self.node.AnIntMultiField.value))



def Suite():
    suite = unittest.TestSuite()

    SingleFieldTests= [
        'testConstantAddition',
        'testInplaceAddition',
        'testArithmetics',
        'testEqual',
        'testUnequal',
        'testHash',
    ]
    suite.addTests(map(SingleFieldTestCase, SingleFieldTests))

    MultiFieldTests = [
        'testMultiFieldInt',
        'testMixedMultiFieldInt',
        'testIndexAccess',
        'testIndexAssignment',
        'testLen',
        'testIteration',
        'testDeletion',
        'testAppend',
        'testExtend',
        'testInsert',
        'testRemove',
    ]
    suite.addTests(map(MultiFieldTestCase, MultiFieldTests))

    return suite
