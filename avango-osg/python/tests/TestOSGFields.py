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

import avango.osg
import avango.script
import unittest


class Node(avango.script.Script):
    field = avango.osg.MFNode()


class FieldTestCase(unittest.TestCase):
    def setUp(self):
        self.container = Node()
        self.nodes = [ avango.osg.nodes.Sphere(Radius = i) for i in xrange(1,4) ]

    def assertEqualNode(self, a, b):
        self.assertEqual(a.Radius.value, b.Radius.value)

    def assertEqualNodesList(self, a, b):
        for i,j in zip(a, b):
            self.assertEqualNode(i, j)


class MultiFieldTestCase(FieldTestCase):

    def testMultiFieldAccess(self):
        self.assertEqual([], list(self.container.field.value))
        self.container.field.value = self.nodes
        set_nodes = list(self.container.field.value)
        self.assertEqualNodesList(self.nodes, set_nodes)

    def testIndexAssignment(self):
        self.container.field.value = self.nodes
        self.container.field.value[2] = self.nodes[1]

        expected_nodes = [self.nodes[0], self.nodes[1], self.nodes[1]]
        set_nodes = list(self.container.field.value)
        self.assertEqualNodesList(expected_nodes, set_nodes)

    def testAppend(self):
        self.container.field.value = self.nodes
        self.container.field.value.append(self.nodes[1])

        expected_nodes = [self.nodes[0], self.nodes[1], self.nodes[2], self.nodes[1]]
        set_nodes = list(self.container.field.value)
        self.assertEqualNodesList(expected_nodes, set_nodes)

    def testExtend(self):
        self.container.field.value = self.nodes
        self.container.field.value.extend([self.nodes[1], self.nodes[2]])

        expected_nodes = [self.nodes[0], self.nodes[1], self.nodes[2],
                          self.nodes[1], self.nodes[2] ]
        set_nodes = list(self.container.field.value)
        self.assertEqualNodesList(expected_nodes, set_nodes)

    def testInsert(self):
        self.container.field.value = self.nodes
        self.container.field.value.insert(1, self.nodes[2])

        expected_nodes = [self.nodes[0], self.nodes[2], self.nodes[1], self.nodes[2] ]
        set_nodes = list(self.container.field.value)
        self.assertEqualNodesList(expected_nodes, set_nodes)

    def testRemove(self):
        self.container.field.value = self.nodes
        self.container.field.value.remove(self.nodes[1])

        expected_nodes = [self.nodes[0], self.nodes[2] ]
        set_nodes = list(self.container.field.value)
        self.assertEqualNodesList(expected_nodes, set_nodes)

    def testGet(self):
        node = avango.osg.nodes.Node()
        self.container.field.value.append(node)
        self.assertEqual(node, self.container.field.value[0])


def Suite():
    suite = unittest.TestSuite()

    MultiFieldTests = [
        'testMultiFieldAccess',
        'testIndexAssignment',
        'testAppend',
        'testExtend',
        'testInsert',
        'testRemove',
        'testGet',
    ]
    suite.addTests(map(MultiFieldTestCase, MultiFieldTests))

    return suite
