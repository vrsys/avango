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

import avango.connect as connect
import unittest
import avango
import StringIO

class SimpleStream(object):
    def __init__(self):
        self.input = StringIO.StringIO()
        self.output = StringIO.StringIO()
        self.flushed = False

    def flush(self):
        self.flushed = True

class AOutputNode(connect.OutputNode):
    anint = avango.SFInt()
    astring = avango.SFString()
    morestrings = avango.MFString()

    def __init__(self):
        self.init_super(connect.OutputNode)
        self.anint.value = 0

class AInputNode(connect.InputNode):
    anint = avango.SFInt()
    astring = avango.SFString()
    morestrings = avango.MFString()

    def __init__(self):
        self.init_super(connect.InputNode)
        self.anint.value = 0

class NodesTestCase(unittest.TestCase):

    def testInitalOutputOfNode(self):
        stream = SimpleStream()
        node = AOutputNode()
        node.set_stream(stream)
        avango.evaluate()

        result = stream.output.getvalue().split()
        self.assert_("anint\x00SFInt\x000"  in result)
        self.assert_("astring\x00SFString\x00" in result)
        self.assert_("morestrings\x00MFString" in result)

    def testSilentOutputOfNode(self):
        stream = SimpleStream()
        node = AOutputNode()
        node.set_stream(stream)
        avango.evaluate()
        offset = len(stream.output.getvalue())
        avango.evaluate()

        self.assertEqual("", stream.output.getvalue()[offset:])

    def testDeltaOutputOfNode(self):
        stream = SimpleStream()
        node = AOutputNode()
        node.set_stream(stream)
        avango.evaluate()
        offset = len(stream.output.getvalue())

        node.anint.value = 42
        avango.evaluate()

        self.assertEqual("anint\x00SFInt\x0042\n", stream.output.getvalue()[offset:])

    def testSilentInputOfNode(self):
        stream = SimpleStream()
        node = AInputNode()
        node.set_stream(stream)

        node.anint.value = 42
        node.astring.value = "Bingo"
        node.morestrings.value = ["A", "B"]

        avango.evaluate()

        self.assertEqual(42, node.anint.value)
        self.assertEqual("Bingo", node.astring.value)
        self.assertEqual("A", node.morestrings.value[0])
        self.assertEqual("B", node.morestrings.value[1])

    def testModifiedInputOfNode(self):
        stream = SimpleStream()
        node = AInputNode()
        stream.input = StringIO.StringIO(
            "anint\x00SFInt\x000\n"
            "astring\x00SFString\x00\n"
            "morestrings\x00MFString\n")
        node.set_stream(stream)

        node.anint.value = 42
        node.astring.value = "Bingo"
        node.morestrings.value = ["A", "B"]

        avango.evaluate()

        self.assertEqual(0, node.anint.value)
        self.assertEqual("", node.astring.value)
        self.assertEqual(0, len(node.morestrings.value))

    def testFlushStreamInOutput(self):
        stream = SimpleStream()
        node = AOutputNode()
        node.set_stream(stream)
        avango.evaluate()

        self.assert_(stream.flushed)

    def testFlushStreamInInput(self):
        stream = SimpleStream()
        node = AInputNode()
        node.set_stream(stream)
        avango.evaluate()

        self.assert_(stream.flushed)


def Suite():
    suite = unittest.TestSuite()

    NodesTests= [
        'testInitalOutputOfNode',
        'testSilentOutputOfNode',
        'testDeltaOutputOfNode',
        'testSilentInputOfNode',
        'testModifiedInputOfNode',
        'testFlushStreamInOutput',
        'testFlushStreamInInput',
    ]
    suite.addTests(map(NodesTestCase, NodesTests))

    return suite
