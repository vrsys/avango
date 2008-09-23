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

import avango.osg.simpleviewer
import avango.script
import time
import math
import sys

print """Note:
Even though this example works, it does not show the recommended way to implement
the shown behaviour.

Please look at simpleviewer2.py."""

if len(sys.argv) != 2:
    print "Usage: "+sys.argv[0]+" <modelname>"
    sys.exit(1)

obj = avango.osg.nodes.LoadFile(Filename=sys.argv[1])
trans = avango.osg.nodes.MatrixTransform()
trans.Children.value.append(obj)

class RotateTransform(object):
    def __init__(self, trans):
        self.trans = trans
        self.time = time.time()
    def callback(self):
        cur_rot = math.sin((time.time()-self.time)/2.)
        cur_mat = avango.osg.make_rot_mat(cur_rot, 1., 0., 0.)
        self.trans.Matrix.value = cur_mat

rt = RotateTransform(trans)
update = avango.script.nodes.Update(Callback = rt.callback, Active = True)

avango.osg.simpleviewer.run(trans)
