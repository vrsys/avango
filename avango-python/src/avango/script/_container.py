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

import _meta_script

class Container(_meta_script.Script):
    '''Base class for containers.

       A container is a field container and contains itself any number of field
       containers. It can create a subgraph with connections and appear as one
       field container to the outside world. Note that containers are
       instantiated the same way all other nodes and field containers are
       instantiated.'''

    def __init__(self):
        self._nodes = []

    def register_internal_node(self, node):
        '''Registers an internal node for correct evaluation'''

        node.allow_scheduling(False)
        self._nodes.append(node)
        return node

    def evaluate(self):
        for node in self._nodes:
            self.evaluate_dependency(node)
