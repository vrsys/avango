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

import socket
import _pipe

class _NetworkStream(object):
    def __init__(self):
        self._input = _pipe.Pipe()
        self._output = _pipe.Pipe()
        self._pending_output = ''
        self._connection = None

    def get_input(self):
        self._update()
        return self._input
    input = property(get_input)

    def get_output(self):
        self._update()
        return self._output
    output = property(get_output)

    def flush(self):
        if not self._update():
            return

        try:
            # FIXME use of small constant
            data = self._connection.recv(8192)
            if not len(data):
                self._connection = None
                return
            self._input.write(data)
        except socket.error, err:
            pass

        while True:
            line = self._output.readline()
            if not line:
                break
            self._pending_output += line
        if self._pending_output:
            try:
                n = self._connection.send(self._pending_output)
                self._pending_output = self._pending_output[n:]
            except socket.error, err:
                pass

    def close(self):
        self._connection.close()
        self._connection = None


class Server(_NetworkStream):
    def __init__(self, host='', port=0):
        super(Server, self).__init__()
        self._sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self._sock.bind((host, port))
        self.port = self._sock.getsockname()[1]
        self._sock.listen(1)
        self._sock.setblocking(0)

    def _update(self):
        if self._connection:
            return True
        try:
            self._connection = self._sock.accept()[0]
            self._connection.setblocking(0)
        except socket.error, err:
            return False
        if not self._connection:
            return False
        self._input = _pipe.Pipe()
        self._output = _pipe.Pipe()
        return True


class Client(_NetworkStream):
    def __init__(self, host='localhost', port=0):
        super(Client, self).__init__()
        self._connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._connection.connect((host, port))
        self._connection.setblocking(0)

    def _update(self):
        return self._connection
