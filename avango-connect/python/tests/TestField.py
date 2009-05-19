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
import avango.osg
import StringIO
import os.path

class SingleFieldTestCase(unittest.TestCase):

    osg_float_places = 5

    def assertFloatListEqual(self, pre, body, post, input, places = osg_float_places):
        self.assertEqual(pre, input[:len(pre)])
        self.assertEqual(post, input[-len(post):])
        expected_floats = [ float(x) for x in body.split('\x00') ]
        input_floats = [ float(x) for x in input[len(pre):-len(post)].split('\x00') ]
        for e, i in zip(expected_floats, input_floats):
            self.assertAlmostEqual(e, i, places)

    def testWriteSFInt(self):
        sfint = avango.SFInt()
        sfint.value = 42
        hout = StringIO.StringIO()
        connect.write("A", sfint, hout)
        self.assertEqual("A\x00SFInt\x0042\n", hout.getvalue())

    def testReadSFInt(self):
        hin = StringIO.StringIO("A\x00SFInt\x0042\n")
        name, field = connect.read(hin)
        self.assertEqual("A", name)
        self.assertEqual(42, field.value)

    def testWriteSFString(self):
        sfstring = avango.SFString()
        sfstring.value = "Test Number One"
        hout = StringIO.StringIO()
        connect.write("B", sfstring, hout)
        self.assertEqual("B\x00SFString\x00Test Number One\n", hout.getvalue())

    def testReadSFString(self):
        hin = StringIO.StringIO("B\x00SFString\x00Another one.\n")
        name, field = connect.read(hin)
        self.assertEqual("B", name)
        self.assertEqual("Another one.", field.value)

    def testWriteMFString(self):
        mfstring = avango.MFString()
        mfstring.value = [ "Test Number One", " Number 2. " ]
        hout = StringIO.StringIO()
        connect.write("C", mfstring, hout)
        self.assertEqual("C\x00MFString\x00Test Number One\x00 Number 2. \n", hout.getvalue())

    def testReadMFString(self):
        hin = StringIO.StringIO("C\x00MFString\x00Another one.\x00Two\n")
        name, field = connect.read(hin)
        self.assertEqual("C", name)
        for a, b in zip(["Another one.", "Two"], field.value):
            self.assertEqual(a, b)

    def testEmptyReadRaises(self):
        def read_empty():
            hin = StringIO.StringIO("")
            name, field = connect.read(hin)
        self.assertRaises(connect.NoDataAvailable, read_empty)

    def testReadFieldsFromFile(self):
        hin = open(os.path.join(os.path.dirname(__file__), 'fielddata'), 'r')
        name, field = connect.read(hin)
        self.assertEqual("A", name)
        self.assertEqual(42, field.value)
        name, field = connect.read(hin)
        self.assertEqual("B", name)
        self.assertEqual("Another one.", field.value)
        name, field = connect.read(hin)
        self.assertEqual("C", name)
        for a, b in zip(["Another one.", "Two"], field.value):
            self.assertEqual(a, b)

    def testWriteSFBool(self):
        field = avango.SFBool()
        field.value = True
        hout = StringIO.StringIO()
        connect.write("A", field, hout)
        self.assertEqual("A\x00SFBool\x001\n", hout.getvalue())

    def testReadSFBool(self):
        hin = StringIO.StringIO("A\x00SFBool\x001\n")
        name, field = connect.read(hin)
        self.assertEqual("A", name)
        self.assertEqual(True, field.value)

    def testWriteSFDouble(self):
        field = avango.SFDouble()
        field.value = 1.2
        hout = StringIO.StringIO()
        connect.write("A", field, hout)
        self.assertFloatListEqual("A\x00SFDouble\x00", "1.2", "\n", hout.getvalue())

    def testReadSFDouble(self):
        hin = StringIO.StringIO("A\x00SFDouble\x001.2\n")
        name, field = connect.read(hin)
        self.assertEqual("A", name)
        self.assertAlmostEqual(1.2, field.value, self.osg_float_places)

    def testWriteMFVec2(self):
        field = avango.osg.MFVec2()
        field.value = [ avango.osg.Vec2(1.1, 1.2), avango.osg.Vec2(2.1, 2.2) ]
        hout = StringIO.StringIO()
        connect.write("A", field, hout)
        self.assertFloatListEqual("A\x00MFVec2\x00", "1.1\x001.2\x002.1\x002.2", "\n", hout.getvalue())

    def testReadMFVec2(self):
        hin = StringIO.StringIO("C\x00MFVec2\x001.1\x001.2\x002.1\x002.2\n")
        name, field = connect.read(hin)
        self.assertEqual("C", name)
        for a, b in zip([ avango.osg.Vec2(1.1, 1.2), avango.osg.Vec2(2.1, 2.2)], field.value):
            self.assertAlmostEqual(a.x, b.x, self.osg_float_places)
            self.assertAlmostEqual(a.y, b.y, self.osg_float_places)

    def testWriteMFVec3(self):
        field = avango.osg.MFVec3()
        field.value = [ avango.osg.Vec3(1.1, 1.2, 1.3), avango.osg.Vec3(2.1, 2.2, 2.3) ]
        hout = StringIO.StringIO()
        connect.write("A", field, hout)
        self.assertFloatListEqual("A\x00MFVec3\x00", "1.1\x001.2\x001.3\x002.1\x002.2\x002.3", "\n",
                                  hout.getvalue())

    def testReadMFVec3(self):
        hin = StringIO.StringIO("C\x00MFVec3\x001.1\x001.2\x001.3\x002.1\x002.2\x002.3\n")
        name, field = connect.read(hin)
        self.assertEqual("C", name)
        for a, b in zip([ avango.osg.Vec3(1.1, 1.2, 1.3), avango.osg.Vec3(2.1, 2.2, 2.3)], field.value):
            self.assertAlmostEqual(a.x, b.x, self.osg_float_places)
            self.assertAlmostEqual(a.y, b.y, self.osg_float_places)
            self.assertAlmostEqual(a.z, b.z, self.osg_float_places)

    def testWriteMFMatrix(self):
        field = avango.osg.MFMatrix()
        field.value = [ avango.osg.make_trans_mat(1, 2, 3), avango.osg.make_trans_mat(4, 5, 6) ]
        hout = StringIO.StringIO()
        connect.write("A", field, hout)
        self.assertFloatListEqual("A\x00MFMatrix\x00", """1.0\x000.0\x000.0\x000.0\x00
                                                          0.0\x001.0\x000.0\x000.0\x00
                                                          0.0\x000.0\x001.0\x000.0\x00
                                                          1.0\x002.0\x003.0\x001.0\x00
                                                          1.0\x000.0\x000.0\x000.0\x00
                                                          0.0\x001.0\x000.0\x000.0\x00
                                                          0.0\x000.0\x001.0\x000.0\x00
                                                          4.0\x005.0\x006.0\x001.0
                                                          """, "\n",
                                  hout.getvalue())

    def testReadMFMatrix(self):
        hin = StringIO.StringIO("""C\x00MFMatrix\x00 \
                                   1.0\x000.0\x000.0\x000.0\x00 \
                                   0.0\x001.0\x000.0\x000.0\x00 \
                                   0.0\x000.0\x001.0\x000.0\x00 \
                                   1.0\x002.0\x003.0\x001.0\x00 \
                                   1.0\x000.0\x000.0\x000.0\x00 \
                                   0.0\x001.0\x000.0\x000.0\x00 \
                                   0.0\x000.0\x001.0\x000.0\x00 \
                                   4.0\x005.0\x006.0\x001.0\n""")
        name, field = connect.read(hin)
        self.assertEqual("C", name)
        for a, b in zip([ avango.osg.make_trans_mat(1, 2, 3), avango.osg.make_trans_mat(4, 5, 6) ], field.value):
            self.assertAlmostEqual(a.get_translate().x, b.get_translate().x, self.osg_float_places)
            self.assertAlmostEqual(a.get_translate().y, b.get_translate().y, self.osg_float_places)
            self.assertAlmostEqual(a.get_translate().z, b.get_translate().z, self.osg_float_places)

    def testWriteMFInt(self):
        mfint = avango.MFInt()
        mfint.value = [ 0, 8, 15 ]
        hout = StringIO.StringIO()
        connect.write("C", mfint, hout)
        self.assertEqual("C\x00MFInt\x000\x008\x0015\n", hout.getvalue())

    def testReadMFInt(self):
        hin = StringIO.StringIO("C\x00MFInt\x000\x008\x0015\n")
        name, field = connect.read(hin)
        self.assertEqual("C", name)
        for a, b in zip([0, 8, 15], field.value):
            self.assertEqual(a, b)

def Suite():
    suite = unittest.TestSuite()

    SingleFieldTests= [
        'testWriteSFInt',
        'testReadSFInt',
        'testWriteSFString',
        'testReadSFString',
        'testWriteMFString',
        'testReadMFString',
        'testEmptyReadRaises',
        'testReadFieldsFromFile',
        'testWriteSFBool',
        'testReadSFBool',
        'testWriteSFDouble',
        'testReadSFDouble',
        'testWriteMFVec2',
        'testReadMFVec2',
        'testWriteMFVec3',
        'testReadMFVec3',
        'testWriteMFMatrix',
        'testReadMFMatrix',
        'testWriteMFInt',
        'testReadMFInt',
    ]
    suite.addTests(map(SingleFieldTestCase, SingleFieldTests))

    return suite
