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
from _io import *

class OutputNode(avango.script.Script):
    def __init__(self):
        self.always_evaluate(True)
        self._old_output = None
        self._stream = None
        self._changed_fields = {}

    def set_stream(self, stream):
        self._stream = stream

    @avango.script.field_has_changed
    def field_has_changed(self, name, field):
        self._changed_fields[name] = field

    def evaluate(self):
        if not self._stream:
            self._old_output = None
            return

        if self._stream.output == self._old_output:
            # Only delta information
            for name, field in self._changed_fields.iteritems():
                write(name, field, self._stream.output)
        else:
            # Full update
            for i in xrange(self._get_num_fields()):
                field = self._get_field(i)
                name = self._get_field_name(i)
                write(name, field, self._stream.output)

        self._changed_fields = {}
        self._old_output = self._stream.output
        self._stream.flush()


class InputNode(avango.script.Script):
    def __init__(self):
        self.always_evaluate(True)
        self._stream = None

    def set_stream(self, stream):
        self._stream = stream

    def evaluate(self):
        self._stream.flush()
        while True:
            try:
                name, field = read(self._stream.input)
                getattr(self, name).value = field.value
            except:
                break
