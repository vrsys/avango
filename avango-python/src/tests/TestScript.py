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
import apackage.empty

import avango.nodefactory
nodes = avango.nodefactory.NodeFactory(module=__name__)

class Empty(avango.script.Script):
    pass

class Local(avango.script.Script):
    value = 42

    def __init__(self):
        self.other_value = 10

class DerivedLocal(Local): pass

class HasFields(avango.script.Script):
    field = avango.SFInt()
    anobject = avango.script.SFObject()

class DerivedHasFields(HasFields): pass

class DerivedHasFieldsWithOwnCallback(HasFields):
    def __init__(self):
        self.value = 0

    @avango.script.field_has_changed(HasFields.field)
    def callback(self):
        self.value = 1

class DoubleDerivedHasFields(DerivedHasFields): pass

class IncValue(avango.script.Script):
    field = avango.SFInt()

    def evaluate(self):
        self.field.value += 1

class IncValueWithValues(avango.script.Script):
    field = avango.SFInt()

    def evaluate(self):
        values = self.get_values()
        values.field += 1

class HasFieldCallbacks(avango.script.Script):
    value1 = avango.SFInt()
    value2 = avango.SFInt()

    def __init__(self):
        self.set_value1 = 0
        self.set_value2 = 0

    @avango.script.field_has_changed(value1)
    def callback1(self):
        self.set_value1 = 2

    @avango.script.field_has_changed(value2)
    def callback2(self):
        self.set_value2 = 3

class DerivedHasFieldCallbacks(HasFieldCallbacks): pass

class DoubleDerivedHasFieldCallbacks(DerivedHasFieldCallbacks): pass

class SelfReturn(avango.script.Script):
    def self(self):
        return self

class TouchMyself(avango.script.Script):
    def __init__(self):
        self.value = 1
        self.touch()
    def evaluate(self):
        self.value += 1

class ExposedTouch(avango.script.Script):
    def __init__(self):
        self.value = 1
    def exposed_touch(self):
        self.touch()
    def evaluate(self):
        self.value += 1

class DerivedExposedTouch(ExposedTouch):
    def derived_touch(self):
        self.exposed_touch()

class IndirectField(avango.script.Script):
    field = avango.SFInt()
    def get(self):
        return self.field.value

class GenericFieldHasChanged(avango.script.Script):
    field = avango.SFInt()

    def __init__(self):
        self.name = ''
        self.value = 0

    @avango.script.field_has_changed
    def callback(self, name, field):
        self.name = name
        self.value = field.value

class DerivedLocalWithInit(Local):
    def __init__(self):
        self.init_super(Local)

class DerivedUpcallingIncValue(IncValue):
    def __init__(self):
        self.init_super(IncValue)
        self.value = 0

    def evaluate(self):
        self.super().evaluate()
        self.value = 1


class ScriptTestCase(unittest.TestCase):

    def testCreateInstance(self):
        self.assert_(nodes.Empty())

    def testGetClassAttribute(self):
        node = nodes.Local()
        self.assert_(node)
        self.assertEqual(node.value, 42)

    def testSetClassAttribute(self):
        node1 = nodes.Local()
        node2 = nodes.Local()
        self.assert_(node1)
        self.assertEqual(node1.value, 42)
        self.assertEqual(node2.value, 42)
        node1.value = 12
        self.assertEqual(node1.value, 12)
        self.assertEqual(node2.value, 42)

    def testGetAttribute(self):
        node = nodes.Local()
        self.assert_(node)
        self.assertEqual(node.other_value, 10)

    def testSetAttribute(self):
        node1 = nodes.Local()
        node2 = nodes.Local()
        self.assert_(node1)
        self.assertEqual(node1.other_value, 10)
        self.assertEqual(node2.other_value, 10)
        node1.other_value = 12
        self.assertEqual(node1.other_value, 12)
        self.assertEqual(node2.other_value, 10)

    def testGetField(self):
        node = nodes.HasFields(field=42)
        self.assert_(node)
        self.assertEqual(node.field.value, 42)

    def testSetField(self):
        node = nodes.HasFields(field=42)
        self.assert_(node)
        self.assertEqual(node.field.value, 42)
        node.field.value = 10
        self.assertEqual(node.field.value, 10)

    def testGetValues(self):
        node = nodes.HasFields(field=42)
        values = node.get_values()
        self.assertEqual(values.field, 42)
        self.assertEqual(node.field.value, 42)
        values.field = 10
        self.assertEqual(values.field, 10)
        self.assertEqual(node.field.value, 10)

    def testSetFieldInMultipleInstances(self):
        node1 = nodes.HasFields(field=42)
        node2 = nodes.HasFields(field=10)
        self.assert_(node1)
        self.assert_(node2)
        self.assertEqual(node1.field.value, 42)
        self.assertEqual(node2.field.value, 10)
        node1.field.value = 43
        node2.field.value = 11
        self.assertEqual(node1.field.value, 43)
        self.assertEqual(node2.field.value, 11)

    def testEvaluate(self):
        node = nodes.IncValue()
        self.assert_(node)
        node.field.value = 0
        avango.evaluate()
        self.assertEqual(node.field.value, 1)
        node.field.value = 2
        avango.evaluate()
        self.assertEqual(node.field.value, 3)

    def testEvaluateWithValues(self):
        node = nodes.IncValueWithValues()
        self.assert_(node)
        node.field.value = 0
        avango.evaluate()
        self.assertEqual(node.field.value, 1)
        node.field.value = 2
        avango.evaluate()
        self.assertEqual(node.field.value, 3)

    def testStandardConstructor(self):
        node = IncValue()
        self.assert_(node)
        node.value = 42
        self.assertEqual(node.value, 42)

    def testStandardConstructorWithInitialization(self):
        node = IncValue(field = 42)
        self.assert_(node)
        self.assertEqual(node.field.value, 42)

    def testFieldHasChanged(self):
        node = HasFieldCallbacks()
        self.assert_(node)

        node.value1.value = 1
        self.assertEqual(node.set_value1, 2)

        node.value2.value = 1
        self.assertEqual(node.set_value2, 3)

    def testFieldConnection(self):
        node1 = nodes.HasFields(field=42)
        node2 = nodes.HasFields(field=42)
        self.assert_(node1)
        self.assertEqual(node1.field.value, 42)
        self.assertEqual(node2.field.value, 42)
        node2.field.connect_from(node1.field)
        node1.field.value = 10
        self.assertEqual(node1.field.value, 10)
        self.assertEqual(node2.field.value, 10)

    def testSetObjectValue(self):
        value = lambda x: x+1
        node = nodes.HasFields(anobject = value)
        self.assertEqual(node.anobject.value, value)

    def testDerivedGetClassAttribute(self):
        node = nodes.DerivedLocal()
        self.assert_(node)
        self.assertEqual(node.value, 42)

    def testDerivedGetField(self):
        node = nodes.DerivedHasFields(field=42)
        self.assert_(node)
        self.assertEqual(node.field.value, 42)

    def testDoubleDerivedGetField(self):
        node = nodes.DoubleDerivedHasFields(field=42)
        self.assert_(node)
        self.assertEqual(node.field.value, 42)

    def testDerivedFieldHasChanged(self):
        node = DerivedHasFieldCallbacks()
        self.assert_(node)

        node.value1.value = 1
        self.assertEqual(node.set_value1, 2)

        node.value2.value = 1
        self.assertEqual(node.set_value2, 3)

    def testDerivedHasFieldsWithOwnCallback(self):
        node = DerivedHasFieldsWithOwnCallback()
        self.assert_(node)

        node.field.value = 1
        self.assertEqual(node.value, 1)

    def testDoubleDerivedFieldHasChanged(self):
        node = DoubleDerivedHasFieldCallbacks()
        self.assert_(node)

        node.value1.value = 1
        self.assertEqual(node.set_value1, 2)

        node.value2.value = 1
        self.assertEqual(node.set_value2, 3)

    def testCreateInstanceFromModule(self):
        self.assert_(apackage.empty.nodes.Empty())

    def testCreateInstanceFromModuleWithConstructor(self):
        self.assert_(apackage.empty.Empty())

    def testInstanceSelfCompare(self):
        node = SelfReturn()
        self.assertEqual(node, node)
        self.assertEqual(node.self(), node)
        self.assertEqual(node, node.self())
        self.assertEqual(node.self(), node.self())

        empty = Empty()
        self.assertNotEqual(node, empty)
        self.assertNotEqual(node.self(), empty)
        self.assertNotEqual(empty, node)
        self.assertNotEqual(empty, node.self())

        self.assertNotEqual(node, 5)
        self.assertNotEqual(5, node)
        self.assertNotEqual(node.self(), 5)
        self.assertNotEqual(5, node.self())

    def testTouchMyself(self):
        node = TouchMyself()
        avango.evaluate()
        self.assertEqual(2, node.value)

    def testDerivedExposedTouch(self):
        node = DerivedExposedTouch()
        node.derived_touch()
        avango.evaluate()
        self.assertEqual(2, node.value)

    def testIndirectField(self):
        node = IndirectField(field = 42)
        self.assertEqual(42, node.get())

    def testAddField(self):
        node = Empty()
        node.add_field(avango.SFInt(), "field")
        self.assert_(node._get_field('field'))

    def testGenericFieldHadChanged(self):
        node = GenericFieldHasChanged()
        node.field.value = 42
        self.assertEqual('field', node.name)
        self.assertEqual(42, node.value)

    def testInitCascade(self):
        node = DerivedLocalWithInit()
        self.assertEqual(10, node.other_value)

    def testDerivedUpcalling(self):
        node = DerivedUpcallingIncValue()
        node.field.value = 0
        avango.evaluate()
        self.assertEqual(node.field.value, 1)
        self.assertEqual(1, node.value)

    def testSetAttributeWithNameOfField(self):
        node = HasFields(field = 5)
        def assign():
            node.field = 10
        self.assertRaises(AttributeError, assign)
        self.assertEqual(5, node.field.value)


def Suite():
    suite = unittest.TestSuite()

    ScriptTests= [
        'testCreateInstance',
        'testGetClassAttribute',
        'testSetClassAttribute',
        'testGetAttribute',
        'testSetAttribute',
        'testGetField',
        'testSetField',
        'testGetValues',
        'testSetFieldInMultipleInstances',
        'testEvaluate',
        'testEvaluateWithValues',
        'testStandardConstructor',
        'testStandardConstructorWithInitialization',
        'testFieldHasChanged',
        'testFieldConnection',
        'testSetObjectValue',
        'testDerivedGetClassAttribute',
        'testDerivedGetField',
        'testDoubleDerivedGetField',
        'testDerivedFieldHasChanged',
        'testDerivedHasFieldsWithOwnCallback',
        'testDoubleDerivedFieldHasChanged',
        'testCreateInstanceFromModule',
        'testCreateInstanceFromModuleWithConstructor',
        'testInstanceSelfCompare',
        'testTouchMyself',
        'testDerivedExposedTouch',
        'testIndirectField',
        'testAddField',
        'testGenericFieldHadChanged',
        'testInitCascade',
        'testDerivedUpcalling',
        'testSetAttributeWithNameOfField',
    ]
    suite.addTests(map(ScriptTestCase, ScriptTests))

    return suite
