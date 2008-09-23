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

def connect_fields(dst_node, *connections):
    """Connects multiple fields from possibly multiple field containers.

    Here is a simple example::

        connect_fields(dst_node, (src_node, [("dst_field", "src_field")])

    This creates a field connection from src_node.src_field to
    dst_node.dst_field.

    A more complex example::

        connect_fields(dst_node, (src_node1, [("dst_field", "src_field"),
                                               "same_field"
                                             ]
                                 ),
                                 (src_node2, [ "same_field2" ])
                      )

    This create field the following field connections:
        - dst_node.dst_field <- src_node1.src_field
        - dst_node.same_field <- src_node1.same_field
        - dst_node.same_field2 <- src_node2.same_field2
    """
    for src_node, fields in connections:
        for field in fields:
            if isinstance(field, str):
                dst_field = field
                src_field = field
            else:
                dst_field = field[0]
                src_field = field[1]
            getattr(dst_node, dst_field).connect_from(getattr(src_node, src_field))
