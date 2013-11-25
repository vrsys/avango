# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       #
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
import mock
import unittest

class FieldContainerTestCase(unittest.TestCase):

    def testRawCreateInstance(self):
        self.failIfEqual(None, avango._make_instance_by_name("MockFieldContainer"))

    def testRawGetField(self):
        mcf = avango._make_instance_by_name("MockFieldContainer")
        self.failIfEqual(None, mcf)
        self.failIfEqual(None, mcf._get_field("AnIntField"))

    def testExplicitGetValue(self):
        mcf = avango._make_instance_by_name("MockFieldContainer")
        self.failIfEqual(None, mcf)
        field = mcf._get_field("AnIntField")
        self.failIfEqual(None, field)
        self.assertEqual(0, field.get_value())

    def testImplicitCreateInstance(self):
        node = mock.nodes.MockFieldContainer()
        self.failIfEqual(None, node)
        self.failIfEqual(None, node._get_field("AnIntField"))

    def testGetFieldAsAttribute(self):
        node = mock.nodes.MockFieldContainer()
        self.failIfEqual(None, node)
        field = node.AnIntField
        self.failIfEqual(None, field)
        self.assertEqual(0, field.get_value())

    def testCreateInstanceWithKeywordArguments(self):
        node = mock.nodes.MockFieldContainer(AnIntField = 42)
        self.failIfEqual(None, node)
        self.assertEqual(42, node.AnIntField.get_value())

    def testExplicitSetValue(self):
        mcf = mock.nodes.MockFieldContainer()
        self.failIfEqual(None, mcf)
        self.assertEqual(0, mcf.AnIntField.get_value())
        mcf.AnIntField.set_value(42)
        self.assertEqual(42, mcf.AnIntField.get_value())

    def testAccessFieldValueFromProperty(self):
        mcf = mock.nodes.MockFieldContainer()
        self.assertEqual(0, mcf.AnIntField.value)
        mcf.AnIntField.value = 42;
        self.assertEqual(42, mcf.AnIntField.value)

    def testGetUnknownAttribute(self):
        node = mock.nodes.MockFieldContainer()
        self.failIfEqual(None, node)
        self.assertRaises(AttributeError, lambda:node.NotAFieldName)

    def testSetUnknownFieldAsAttribute(self):
        node = mock.nodes.MockFieldContainer()
        self.failIfEqual(None, node)
        node.NotAFieldName = 42
        self.assert_(42, node.NotAFieldName)

    def testImplicitCreateUnknownInstance(self):
        def create():
            node = avango.nodes.NameANodeShouldNeverHave()
        self.assertRaises(ImportError, create)

    def testGetValues(self):
        mcf = mock.nodes.MockFieldContainer()
        values = mcf.get_values()
        self.assertEqual(0, values.AnIntField)
        self.assertEqual(0, mcf.AnIntField.value)
        values.AnIntField = 42;
        self.assertEqual(42, values.AnIntField)
        self.assertEqual(42, mcf.AnIntField.value)

    def testCompare(self):
        mcf = mock.nodes.MockFieldContainer()
        mcf.AFieldContainer.value = mcf
        self.assertEqual(mcf, mcf.AFieldContainer.value)

    def testGetInstanceByName(self):
        node = mock.nodes.MockFieldContainer()
        self.failIfEqual(None, node)
        name = "MyNode"
        node.Name.value = name
        self.assertEqual(node, avango.get_instance_by_name(name))

def Suite():
    suite = unittest.TestSuite()

    FieldContainerTests= [
        'testRawCreateInstance',
        'testRawGetField',
        'testExplicitGetValue',
        'testImplicitCreateInstance',
        'testGetFieldAsAttribute',
        'testCreateInstanceWithKeywordArguments',
        'testExplicitSetValue',
        'testAccessFieldValueFromProperty',
        'testGetUnknownAttribute',
        'testSetUnknownFieldAsAttribute',
        'testImplicitCreateUnknownInstance',
        'testGetValues',
        'testCompare',
        'testGetInstanceByName',
    ]
    suite.addTests(map(FieldContainerTestCase, FieldContainerTests))

    return suite
