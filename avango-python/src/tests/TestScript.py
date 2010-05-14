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
        self.super(Local).__init__()
        self.other_value = 10

class DerivedLocal(Local): pass

class HasFields(avango.script.Script):
    field = avango.SFInt()
    anobject = avango.script.SFObject()

class DerivedHasFields(HasFields): pass

class DerivedHasFieldsWithOwnCallback(HasFields):
    def __init__(self):
        self.super(DerivedHasFieldsWithOwnCallback).__init__()
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
        self.super(HasFieldCallbacks).__init__()
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
        self.super(TouchMyself).__init__()
        self.value = 1
        self.touch()
    def evaluate(self):
        self.value += 1

class ExposedTouch(avango.script.Script):
    def __init__(self):
        self.super(ExposedTouch).__init__()
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
        self.super(GenericFieldHasChanged).__init__()
        self.name = ''
        self.value = 0

    @avango.script.field_has_changed
    def callback(self, name, field):
        self.name = name
        self.value = field.value

class DerivedLocalWithInit(Local):
    def __init__(self):
        self.super(DerivedLocalWithInit).__init__()

class DerivedUpcallingIncValue(IncValue):
    def __init__(self):
        self.super(DerivedUpcallingIncValue).__init__()
        self.value = 0

    def evaluate(self):
        self.super(DerivedUpcallingIncValue).evaluate()
        self.value = 1

class DoubleDerivedUpcallingIncValue(DerivedUpcallingIncValue):
    def __init__(self):
        self.super(DoubleDerivedUpcallingIncValue).__init__()

    def evaluate(self):
        self.super(DoubleDerivedUpcallingIncValue).evaluate()

class HasSingleFieldCallbackBase1(avango.script.Script):
    value1 = avango.SFInt()

    def __init__(self):
        self.super(HasSingleFieldCallbackBase1).__init__()
        self.set_value1 = 0

    @avango.script.field_has_changed(value1)
    def callback1(self):
        self.set_value1 = 1

class HasSingleFieldCallbackBase2(avango.script.Script):
    value2 = avango.SFInt()

    def __init__(self):
        self.super(HasSingleFieldCallbackBase2).__init__()
        self.set_value2 = 0

    @avango.script.field_has_changed(value2)
    def callback2(self):
        self.set_value2 = 2

class DerivedFromMultipleScript(HasSingleFieldCallbackBase1, HasSingleFieldCallbackBase2):
    value3 = avango.SFInt()

    def __init__(self):
        self.super(DerivedFromMultipleScript).__init__()
        self.set_value3 = 0

    @avango.script.field_has_changed(value3)
    def callback(self):
        self.set_value3 = 3

class DynamicFieldScript(avango.script.Script):

    def add_dynamic_int_fields(self,prefix,num):
        for i in range(0,num):
            name = str(prefix) + str(i)
            self.add_and_init_field(avango.SFInt(), name, 0)


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

    def testStandardConstructor(self):
        node = IncValue()
        self.assert_(node)
        node.value = 42
        self.assertEqual(node.value, 42)

    def testStandardConstructorWithInitialization(self):
        node = IncValue(field = 42)
        self.assert_(node)
        self.assertEqual(node.field.value, 42)

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

    def testFieldHasChanged(self):
        node = HasFieldCallbacks()
        self.assert_(node)

        node.value1.value = 1
        self.assertEqual(node.set_value1, 2)

        node.value2.value = 1
        self.assertEqual(node.set_value2, 3)

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
        self.assertEqual(1, node.field.value)
        self.assertEqual(1, node.value)

    def testDoubleDerivedUpcalling(self):
        node = DoubleDerivedUpcallingIncValue()
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

    def testDerivedFromMultipleScriptFieldHasChanged(self):
        node = DerivedFromMultipleScript()
        self.assert_(node)

        self.assert_(hasattr(node, "set_value1"))
        self.assert_(hasattr(node, "set_value2"))
        self.assert_(hasattr(node, "set_value3"))

        node.value1.value = 1
        self.assertEqual(node.set_value1, 1)

        node.value2.value = 1
        self.assertEqual(node.set_value2, 2)

        node.value3.value = 1
        self.assertEqual(node.set_value3, 3)

    def testDynamicFieldRemoval(self):
        node1 = DynamicFieldScript()
        #Every script node has a Name field by default
        self.assertEqual(1, node1._get_num_fields())
        node1.add_dynamic_int_fields("field_", 1)
        self.assertEqual(2, node1._get_num_fields())

        node2 = DynamicFieldScript()
        self.assertEqual(1, node2._get_num_fields())
        node2.add_dynamic_int_fields("field_", 1)
        self.assertEqual(2, node2._get_num_fields())

        self.assertEqual(0, node1._get_field("field_0").get_number_of_connected_fields())
        node1._get_field("field_0").connect_from(node2._get_field("field_0"))
        self.assertEqual(1, node1._get_field("field_0").get_number_of_connected_fields())

        node2._get_field("field_0").value = 42
        avango.evaluate()
        self.assertEqual(42, node1._get_field("field_0").value)

        node2.remove_field("field_0")
        self.assertEqual(0, node1._get_field("field_0").get_number_of_connected_fields())
        self.assertEqual(1, node2._get_num_fields())

        node2.add_dynamic_int_fields("field_", 1)
        self.assertEqual(2, node2._get_num_fields())

        node1._get_field("field_0").connect_from(node2._get_field("field_0"))
        self.assertEqual(1, node1._get_field("field_0").get_number_of_connected_fields())
        node2._get_field("field_0").value = 23
        avango.evaluate()
        self.assertEqual(23, node1._get_field("field_0").value)

def Suite():
    return unittest.TestLoader().loadTestsFromTestCase(ScriptTestCase)
