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

import avango.script
import unittest

class UpdateTestCase(unittest.TestCase):

    def testCreateInstance(self):
        self.assert_(avango.script.nodes.Update())

    def testCallback(self):
        update = avango.script.nodes.Update()
        class callback(object):
            def __init__(self):
                self.value = 0
            def inc(self):
                self.value += 1
        callback_inst = callback()
        update.Callback.value = callback_inst.inc
        update.Active.value = True
        avango.evaluate()
        avango.evaluate()
        self.assertEqual(2, callback_inst.value)

    def testDeactivatedCallback(self):
        update = avango.script.nodes.Update()
        class callback(object):
            def __init__(self):
                self.value = 0
            def inc(self):
                self.value += 1
        callback_inst = callback()
        update.Callback.value = callback_inst.inc
        update.Active.value = False
        avango.evaluate()
        avango.evaluate()
        self.assertEqual(0, callback_inst.value)


def Suite():
    suite = unittest.TestSuite()

    UpdateTests= [
        'testCreateInstance',
        'testCallback',
        'testDeactivatedCallback',
    ]
    suite.addTests(map(UpdateTestCase, UpdateTests))

    return suite
