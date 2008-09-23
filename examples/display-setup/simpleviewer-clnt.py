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

'''
A distributed viewer setup: This Python scripts starts an avango.osg.simpleviewer
and connects to the distribution group "testgroup". If there is another instance
loading some geometry under this node this model should also appear in the client.
(see also simpleviewer-srv.py)
'''

import avango.osg.simpleviewer
import avango.script
import time
import math
import sys

from avango import enable_logging

avango.osg.set_ensemble_option("-deering_port", "2346")
avango.osg.set_ensemble_option("-multiread",  "")
avango.osg.set_ensemble_option("-properties", "Gmp:Sync:Heal:Switch:Frag:Suspect:Flow:Total")
avango.osg.set_ensemble_option("-verbose", "")
avango.osg.set_ensemble_option("-socket_verb", "")

nettrans = avango.osg.nodes.NetMatrixTransform(Groupname = "testgroup")

avango.osg.simpleviewer.run(nettrans)
