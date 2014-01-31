# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       #
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

import avango.display

class FakeTwoView(avango.display.Display):

    def __init__(self, inspector, options):
        super(FakeTwoView, self).__init__("FakeTwoView", inspector)

        # Viewer 1
        window1 = self.make_window(0, 0, 1280, 1024, 6, 2.4, False)
        window1.Name.value = "User 1"
        self.add_window(window1, avango.osg.make_trans_mat(0., 1.2, -2.4), 0)

        # Viewer 2
        window2 = self.make_window(1280, 0, 1280, 1024, 6, 2.4, False)
        window2.Name.value = "User 2"
        self.add_window(window2, avango.osg.make_trans_mat(0., 1.2, -2.4), 1)

        # Users
        user1 = avango.display.nodes.User()
        user1.Matrix.value = avango.osg.make_trans_mat(avango.osg.Vec3(0., 1.7, 0.))
        self.add_user(user1)
        user2 = avango.display.nodes.User()
        user2.Matrix.value = avango.osg.make_trans_mat(avango.osg.Vec3(0., 1.7, 0.))
        self.add_user(user2)
