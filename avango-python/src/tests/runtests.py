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

import unittest
import TestFieldContainer
import TestField
import TestUpdate
import TestScript
import TestNodefactory
import TestContainer
import TestUtility
import sys

if __name__ == '__main__':
    suites = [
        TestFieldContainer.Suite(),
        TestField.Suite(),
        TestUpdate.Suite(),
        TestScript.Suite(),
        TestNodefactory.Suite(),
        TestContainer.Suite(),
        TestUtility.Suite(),
    ]
    alltests = unittest.TestSuite(suites)
    result = unittest.TextTestRunner(verbosity=2).run(alltests)
    if not result.wasSuccessful():
        sys.exit(1)
