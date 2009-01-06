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
import time

class ServerTestCase(unittest.TestCase):

    def setUp(self):
        self.server = connect.Server()
        self.client = connect.Client('localhost', self.server.port)

    def assertReadLineEqual(self, content, stream):
        starttime = time.clock()
        while True:
            stream.flush()
            line = stream.input.readline()
            if line: break
            if time.clock()-starttime > 2:
                self.fail('Timout while reading.')
        self.assertEqual(content, line)

    def testServerWrite(self):
        content = 'One\n'
        self.server.output.write(content)
        self.server.flush()
        self.assertReadLineEqual(content, self.client)

    def testClientWrite(self):
        content = 'One\n'
        self.client.output.write(content)
        self.client.flush()
        self.assertReadLineEqual(content, self.server)


class ConnectionTestCase(unittest.TestCase):

    def testChangingOutput(self):
        server = connect.Server()
        output = server.output
        client = connect.Client('localhost', server.port)
        self.assert_(output != server.output)

    def testReconnectionChangingOutput(self):
        server = connect.Server()
        client = connect.Client('localhost', server.port)
        output = server.output
        client.close()
        server.flush()
        client = connect.Client('localhost', server.port)
        self.assert_(output != server.output)


def Suite():
    suite = unittest.TestSuite()

    ServerTests= [
        'testServerWrite',
        'testClientWrite',
    ]
    suite.addTests(map(ServerTestCase, ServerTests))

    ConnectionTests= [
        'testChangingOutput',
        'testReconnectionChangingOutput',
    ]
    suite.addTests(map(ConnectionTestCase, ConnectionTests))

    return suite
