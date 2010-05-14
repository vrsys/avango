# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
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

import avango.osg
import unittest

class OSGTestCase(unittest.TestCase):

    # check instanciation
    def testCreateMatrix(self):
        self.failIfEqual(None, avango.osg.Matrix())
        self.failIfEqual(None, avango.osg.Matrix(avango.osg.Matrix()))
    def testCreateQuat(self):
        self.failIfEqual(None, avango.osg.Quat())
        self.failIfEqual(None, avango.osg.Quat(0,0,0,1))
        self.failIfEqual(None, avango.osg.Quat(avango.osg.Vec4(0,0,0,1)))
        self.failIfEqual(None, avango.osg.Quat(0,avango.osg.Vec3(1,1,1)))
    def testCreateVec2(self):
        self.failIfEqual(None, avango.osg.Vec2())
        self.failIfEqual(None, avango.osg.Vec2(1,1))
        self.failIfEqual(None, avango.osg.Vec2(avango.osg.Vec2(1,1)))
    def testCreateVec3(self):
        self.failIfEqual(None, avango.osg.Vec3())
        self.failIfEqual(None, avango.osg.Vec3(1,1,1))
        self.failIfEqual(None, avango.osg.Vec3(avango.osg.Vec3(1,1,1)))
    def testCreateVec4(self):
        self.failIfEqual(None, avango.osg.Vec4())
        self.failIfEqual(None, avango.osg.Vec4(1,1,1,1))
        self.failIfEqual(None, avango.osg.Vec4(avango.osg.Vec4(1,1,1,1)))

    # check Vec2, Vec3, Vec4 operators and functions
    def testVecEqualsVec(self):
        self.assert_(avango.osg.Vec2(1,1) == avango.osg.Vec2(1,1))
        self.assert_(avango.osg.Vec3(1,1,1) == avango.osg.Vec3(1,1,1))
        self.assert_(avango.osg.Vec4(1,1,1,1) == avango.osg.Vec4(1,1,1,1))
    def testVecNotEqualVec(self):
        self.assert_(avango.osg.Vec2(1,1) != avango.osg.Vec2(2,2))
        self.assert_(avango.osg.Vec3(1,1,1) != avango.osg.Vec3(2,2,2))
        self.assert_(avango.osg.Vec4(1,1,1,1) != avango.osg.Vec4(2,2,2,2))
    def testVecSmallerThanVec(self):
        self.assert_(avango.osg.Vec2(1,1) < avango.osg.Vec2(2,2))
        self.assert_(avango.osg.Vec3(1,1,1) < avango.osg.Vec3(2,2,2))
        self.assert_(avango.osg.Vec4(1,1,1,1) < avango.osg.Vec4(2,2,2,2))
    def testVecAddVec(self):
        x1 = avango.osg.Vec2(1,1)
        x2 = avango.osg.Vec3(1,1,1)
        x3 = avango.osg.Vec4(1,1,1,1)
        self.assert_((x1 + x1) == avango.osg.Vec2(2,2))
        self.assert_((x2 + x2) == avango.osg.Vec3(2,2,2))
        self.assert_((x3 + x3) == avango.osg.Vec4(2,2,2,2))
        x1 += x1
        x2 += x2
        x3 += x3
        self.failUnlessEqual(x1,avango.osg.Vec2(2,2))
        self.failUnlessEqual(x2,avango.osg.Vec3(2,2,2))
        self.failUnlessEqual(x3,avango.osg.Vec4(2,2,2,2))
    def testVecMinusVec(self):
        x1 = avango.osg.Vec2(2,2)
        x2 = avango.osg.Vec3(2,2,2)
        x3 = avango.osg.Vec4(2,2,2,2)
        self.assert_((x1 - x1) == avango.osg.Vec2(0,0))
        self.assert_((x2 - x2) == avango.osg.Vec3(0,0,0))
        self.assert_((x3 - x3) == avango.osg.Vec4(0,0,0,0))
        x1 -= x1
        x2 -= x2
        x3 -= x3
        self.failUnlessEqual(x1,avango.osg.Vec2(0,0))
        self.failUnlessEqual(x2,avango.osg.Vec3(0,0,0))
        self.failUnlessEqual(x3,avango.osg.Vec4(0,0,0,0))
    def testVecTimesAScalar(self):
        x1 = avango.osg.Vec2(1,1)
        x2 = avango.osg.Vec3(1,1,1)
        x3 = avango.osg.Vec4(1,1,1,1)
        self.failUnlessEqual((x1 * 2), avango.osg.Vec2(2,2))
        self.failUnlessEqual((x2 * 2), avango.osg.Vec3(2,2,2))
        self.failUnlessEqual((x3 * 2), avango.osg.Vec4(2,2,2,2))
    def testScalarTimesAVec(self):
        x1 = avango.osg.Vec2(1,1)
        x2 = avango.osg.Vec3(1,1,1)
        x3 = avango.osg.Vec4(1,1,1,1)
        self.failUnlessEqual((2 * x1), avango.osg.Vec2(2,2))
        self.failUnlessEqual((2 * x2), avango.osg.Vec3(2,2,2))
        self.failUnlessEqual((2 * x3), avango.osg.Vec4(2,2,2,2))
    def testVecInlineTimesAScalar(self):
        x1 = avango.osg.Vec2(1,1)
        x2 = avango.osg.Vec3(1,1,1)
        x3 = avango.osg.Vec4(1,1,1,1)
        x1 *= 2
        x2 *= 2
        x3 *= 2
        self.failUnlessEqual(x1, avango.osg.Vec2(2,2))
        self.failUnlessEqual(x2, avango.osg.Vec3(2,2,2))
        self.failUnlessEqual(x3, avango.osg.Vec4(2,2,2,2))
    def testVecTimesVec(self):
        self.failUnlessEqual((avango.osg.Vec2(1,1) * avango.osg.Vec2(2,3)), 5)
        self.failUnlessEqual((avango.osg.Vec3(1,1,1) * avango.osg.Vec3(2,3,4)), 9)
        self.failUnlessEqual((avango.osg.Vec4(1,1,1,1) * avango.osg.Vec4(2,3,4,5)), 14)
    def testVecDividedByScalar(self):
        x1 = avango.osg.Vec2(2,2)
        x2 = avango.osg.Vec3(2,2,2)
        x3 = avango.osg.Vec4(2,2,2,2)
        self.failUnlessEqual((x1 / 2), avango.osg.Vec2(1,1))
        self.failUnlessEqual((x2 / 2), avango.osg.Vec3(1,1,1))
        self.failUnlessEqual((x3 / 2), avango.osg.Vec4(1,1,1,1))
        x1 /= 2
        x2 /= 2
        x3 /= 2
        self.failUnlessEqual(x1, avango.osg.Vec2(1,1))
        self.failUnlessEqual(x2, avango.osg.Vec3(1,1,1))
        self.failUnlessEqual(x3, avango.osg.Vec4(1,1,1,1))
    def testVecNormalizeAndLength(self):
        x1 = avango.osg.Vec2(27,0)
        x1.normalize()
        self.failUnlessEqual(1.0, round(x1.length()))
        x2 = avango.osg.Vec3(27,52,0)
        x2.normalize()
        self.failUnlessEqual(1.0, round(x2.length()))
        x3 = avango.osg.Vec4(27,44,9,0)
        x3.normalize()
        self.failUnlessEqual(1.0, round(x3.length()))
    def testVecXYOperators(self):
        x1 = avango.osg.Vec2(1,2)
        x2 = avango.osg.Vec3(3,4,5)
        x3 = avango.osg.Vec4(6,7,8,9)
        self.failUnlessEqual(1, x1.x)
        self.failUnlessEqual(2, x1.y)
        self.failUnlessEqual(3, x2.x)
        self.failUnlessEqual(4, x2.y)
        self.failUnlessEqual(5, x2.z)
        self.failUnlessEqual(6, x3.x)
        self.failUnlessEqual(7, x3.y)
        self.failUnlessEqual(8, x3.z)
        self.failUnlessEqual(9, x3.w)
        x1.x = 9
        x1.y = 8
        x2.x = 7
        x2.y = 6
        x2.z = 5
        x3.x = 4
        x3.y = 3
        x3.z = 2
        x3.w = 1
        self.failUnlessEqual(9, x1.x)
        self.failUnlessEqual(8, x1.y)
        self.failUnlessEqual(7, x2.x)
        self.failUnlessEqual(6, x2.y)
        self.failUnlessEqual(5, x2.z)
        self.failUnlessEqual(4, x3.x)
        self.failUnlessEqual(3, x3.y)
        self.failUnlessEqual(2, x3.z)
        self.failUnlessEqual(1, x3.w)

    # check Matrix operators and functions
    def testMatrixEqualsMatrix(self):
        self.assert_(avango.osg.Matrix() == avango.osg.Matrix())
    def testMatrixNotEqualsMatrix(self):
        x = avango.osg.Matrix()
        x.make_translate(1,2,3)
        y = avango.osg.Matrix()
        y.make_translate(4,5,6)
        self.assert_(x != y)
    def testMatrixTranslationRotationScaling(self):
        x = avango.osg.Matrix()
        y = avango.osg.Matrix()
        x.make_translate(1,2,3)
        y.make_translate(avango.osg.Vec3(1,2,3))
        self.failUnlessEqual(x,y)
        x.make_scale(5,5,5)
        y.make_scale(avango.osg.Vec3(5,5,5))
        self.failUnlessEqual(x,y)
        x.make_rotate(1, avango.osg.Vec3(1,1,1))
        y.make_rotate(1, 1,1,1)
        self.failUnlessEqual(x,y)
        x.set_translate(1,1,1)
        y.set_translate(avango.osg.Vec3(1,1,1))
        self.failUnlessEqual(x,y)
    def testMatrixIdentity(self):
        x = avango.osg.Matrix()
        x.make_translate(1,2,2)
        x.make_identity()
        self.assert_(x.is_identity())
    def testMatrixGlobalFunctions(self):
        self.failUnlessEqual(avango.osg.make_trans_mat(avango.osg.Vec3(1,2,3)), avango.osg.make_trans_mat(1,2,3))
        self.failUnlessEqual(avango.osg.make_rot_mat(1, avango.osg.Vec3(1,1,1)), avango.osg.make_rot_mat(1,1,1,1))
        self.failUnlessEqual(avango.osg.make_scale_mat(avango.osg.Vec3(5,5,5)), avango.osg.make_scale_mat(5,5,5))
        self.failUnlessEqual(avango.osg.make_ident_mat(), avango.osg.Matrix())
    def testMatrixTimesMatrix(self):
        x1 = avango.osg.make_trans_mat(0,1,0)
        x2 = avango.osg.make_trans_mat(1,0,0)
        self.failUnlessEqual((avango.osg.make_trans_mat(0,1,0) * avango.osg.make_trans_mat(1,0,0)), avango.osg.make_trans_mat(1,1,0))
        x1 *= x2
        self.failUnlessEqual(x1, avango.osg.make_trans_mat(1,1,0))
    def testMatrixTimesVec(self):
        self.failUnlessEqual((avango.osg.make_ident_mat() * avango.osg.Vec3(1,0,0)), avango.osg.Vec3(1,0,0))
        self.failUnlessEqual((avango.osg.make_ident_mat() * avango.osg.Vec4(1,0,0,0)), avango.osg.Vec4(1,0,0,0))
        self.failUnlessEqual((avango.osg.transform3x3(avango.osg.make_ident_mat(), avango.osg.Vec3(1,0,0))), avango.osg.Vec3(1,0,0))
        self.failUnlessEqual((avango.osg.transform3x3(avango.osg.Vec3(1,0,0), avango.osg.make_ident_mat())), avango.osg.Vec3(1,0,0))

    def testMatrixGetTranslate(self):
        translation = avango.osg.Vec3(1., 2., 3.)
        identity = avango.osg.make_trans_mat(translation)
        self.assertEqual(identity.get_translate(), translation)

def Suite():
    suite = unittest.TestLoader().loadTestsFromTestCase(OSGTestCase)
    return suite
