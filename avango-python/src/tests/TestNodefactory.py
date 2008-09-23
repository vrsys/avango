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

class NodefactoryTestCase(unittest.TestCase):

    def testMangleNamespace(self):
        result = avango.nodefactory.mangle_class_name("A.B.C")
        self.assertEqual("A::B::C::", result)

    def testMangleClass(self):
        result = avango.nodefactory.mangle_class_name("A.B.C", "D")
        self.assertEqual("A::B::C::D", result)

    def testMangleAvangoClass(self):
        result = avango.nodefactory.mangle_class_name("avango.B.C", "D")
        self.assertEqual("av::B::C::D", result)

    def testMangleHiddenModules(self):
        result = avango.nodefactory.mangle_class_name("A._B.C._D", "E")
        self.assertEqual("A::C::E", result)


def Suite():
    suite = unittest.TestSuite()

    NodefactoryTests= [
        'testMangleNamespace',
        'testMangleClass',
        'testMangleAvangoClass',
        'testMangleHiddenModules',
    ]
    suite.addTests(map(NodefactoryTestCase, NodefactoryTests))

    return suite
