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

class PipeTestCase(unittest.TestCase):

    def testReturnEmpty(self):
        pipe = connect.Pipe()
        self.assertEqual('', pipe.readline())

    def testSimpleReadWrite(self):
        pipe = connect.Pipe()
        row = 'One row\n'
        pipe.write(row)
        self.assertEqual(row, pipe.readline())

    def testComplexReadWrite(self):
        pipe = connect.Pipe()
        rows = ['One\n', 'Two\n', 'Three\n']
        pipe.write(''.join(rows))
        for i in xrange(len(rows)):
            self.assertEqual(rows[i], pipe.readline())

    def testSequentialReadWrite(self):
        pipe = connect.Pipe()
        parts = ['One', 'Two\n']
        pipe.write(parts[0])
        self.assertEqual('', pipe.readline())
        pipe.write(parts[1])
        self.assertEqual(''.join(parts), pipe.readline())


def Suite():
    suite = unittest.TestSuite()

    PipeTests= [
        'testReturnEmpty',
        'testSimpleReadWrite',
        'testComplexReadWrite',
        'testSequentialReadWrite',
    ]
    suite.addTests(map(PipeTestCase, PipeTests))

    return suite
