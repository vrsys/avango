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
import avango.utils
from avango.script import field_has_changed
import unittest

class MVFNode(avango.script.Script):

    FloatVec = avango.utils.MVFFloat()

    def __init__(self):
        self.super(MVFNode).__init__()


class MVFNodeIn(avango.script.Script):

    FloatVecIn = avango.utils.MVFFloat()

    def __init__(self):
        self.super(MVFNodeIn).__init__()
        self.changed_inds = []
        self.saved_changed_inds = []
        
    @field_has_changed(FloatVecIn)
    def float_vec_changed(self):
        for i in range(self.FloatVecIn.get_size()):
            if self.FloatVecIn.value_has_changed(i):
                if i not in self.changed_inds:
                    self.changed_inds.append(i)
       
    def evaluate(self):
        self.saved_changed_inds = self.changed_inds
        self.changed_inds = []
        self.FloatVecIn.reset_changed_flags()


class MVFNodeOut(avango.script.Script):

    FloatVecOut = avango.utils.MVFFloat()

    def __init__(self):
        self.super(MVFNodeOut).__init__()
        self.FloatVecOut.reset_changed_flags()
        
    def compute_results(self):
        self.FloatVecOut.insert_1value(3.0, 3, False)
        self.FloatVecOut.insert_1value(5.0, 5, False)
        self.FloatVecOut.set_1value(0.0, 0, False)
        self.FloatVecOut.trigger_field_change(None)
        
    def evaluate(self):
        self.FloatVecOut.reset_changed_flags()


class MVFNodeTestCase(unittest.TestCase):

    def testMakeInstance(self):
        self.assert_(MVFNode())
        
    def testSetGetValues(self):
        node = MVFNode()
        node.FloatVec.set_all_values([3.0, 4.0, 5.0])
        node.FloatVec.insert_1value(1.0, 0, False)
        node.FloatVec.insert_1value(2.0, 1, False)
        node.FloatVec.set_some_values([-3.0, -4.0, -5.0], [0, 1, 2])
        node.FloatVec.erase_1value(2, False)
        node.FloatVec.add_1value(6.0)
        node.FloatVec.add_1value(7.0)
        node.FloatVec.remove_1value(-4.0)
        sz = node.FloatVec.get_size()
        node.FloatVec.set_1value( -1.0 * node.FloatVec.get_1value(sz-1), sz-1, False)
        is_empty = node.FloatVec.is_empty()
        val1 = node.FloatVec.get_1value(2)
        values = node.FloatVec.get_all_values()
        self.assertEqual(is_empty, False)
        self.assertEqual(val1, 5.0)
        self.assertEqual(list(values), [-3.0, 4.0, 5.0, 6.0, -7.0])
        node.FloatVec.clear_all_values()
        self.assertEqual(node.FloatVec.is_empty(), True)
        node.FloatVec.all_values = [7.0, 8.0, 9.0]
        self.assertEqual(list(node.FloatVec.all_values), [7.0, 8.0, 9.0])

    def testConnectedFields(self):
        node0 = MVFNodeOut()
        node1 = MVFNodeIn()
        node2 = MVFNodeIn()
        
        node1.FloatVecIn.connect_from(node0.FloatVecOut)
        node2.FloatVecIn.connect_from(node0.FloatVecOut)
        node0.FloatVecOut.all_values = [-1.0, 1.0, 2.0, 4.0]
        
        self.assertEqual(list(node0.FloatVecOut.all_values), list(node1.FloatVecIn.all_values))
        self.assertEqual(list(node0.FloatVecOut.all_values), list(node2.FloatVecIn.all_values))        
        
        avango.evaluate()
        
        self.assertEqual(node1.saved_changed_inds, [0, 1, 2, 3])
        self.assertEqual(node1.saved_changed_inds, node2.saved_changed_inds)
        
        node0.compute_results()
        avango.evaluate()
        
        self.assertEqual(node1.saved_changed_inds, [0, 3, 4, 5])
        self.assertEqual(node1.saved_changed_inds, node2.saved_changed_inds)
        self.assertEqual(list(node0.FloatVecOut.all_values), list(node1.FloatVecIn.all_values))
        self.assertEqual(list(node0.FloatVecOut.all_values), list(node2.FloatVecIn.all_values))

 
def Suite():
   suite = unittest.TestLoader().loadTestsFromTestCase(MVFNodeTestCase)
   return suite
