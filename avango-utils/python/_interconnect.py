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

import socket
import errno
import datetime
import avango.script
import platform
import re


class InterconnectOutputNode(avango.script.Script):
    def __init__(self):
        self.super(InterconnectOutputNode).__init__()
        
        self.always_evaluate(True)
        self._old_output = None
        self._stream = None
        self._nodes = {}
        self._node_fields = {}
        self._fields_to_remove = []     # TODO: This is just a hack

    def set_stream(self, stream):
        self._stream = stream

    def __remove_field(self, name):
        field = self._get_field(name)
        
        # disconnect field from node's field
        field.disconnect_auditors()
        field.disconnect()
        
        # send remove field notification
        self.write_field(field, name, True)
        
        # remove field
        self._fields_to_remove.append(name) # TODO: This is just a hack (remove_field() should work directly)
        #self.remove_field(name)

    @avango.script.field_has_changed
    def field_has_changed(self, name, field):
        # send field notification
        self.write_field(field, name)

    def evaluate(self):
        if not self._stream:
            self._old_output = None
            return
        
        if self._stream.output != self._old_output:
            # Full update
            for i in xrange(self._get_num_fields()):
                field = self._get_field(i)
                self.write_field(field, field._get_name())
        
        # TODO: This is just a hack
        for field_name in self._fields_to_remove:
            self.remove_field(field_name)
        self._fields_to_remove = []
        
        self._old_output = self._stream.output
        self._stream.flush()

    def write_field(self, field, name, remove = False):
        #print "write_field: " + field._get_type() + " " + name
        self._stream.output.write(field._get_type() + " " + name)
        if not remove:
            self._stream.output.write(" ")
            outstream = avango._avango.StringOutputStream()
            outstream.buffer = self._stream.output._content;
            field.write(outstream)
            self._stream.output._content = outstream.buffer;
        self._stream.output.write("\n")

    def add_node(self, node):
        node_name = node.Name.value
        #print "add_node: " + str(node_name) + " " + node.Name.value
        
        # add node field
        nodefieldname = "__" + node.Name.value
        self.add_and_init_field(avango.SFString(), nodefieldname, node._get_type())
        self._nodes[node_name] = nodefieldname
        self._node_fields[node_name] = []
        
        for i in xrange(node._get_num_fields()):
            field = node._get_field(i)
            
            # create and add field with same type and mangled name
            new_field = avango._avango._make_field_by_name(field._get_type())
            name = "__" + node.Name.value + "_" + field._get_name()
            self.add_field(new_field, name)
            self._node_fields[node_name].append(name)
            
            # connect new field from node's field
            new_field = self._get_field(name)
            new_field.connect_from(field)

    def remove_node(self, node):
        node_name = node.Name.value
        #print "remove_node: " + str(node_name) + " " + node.Name.value
        
        if node_name not in self._nodes:
            print "WARNING: trying to remove node that was not added!"
            return
        
        for fieldname in self._node_fields[node_name]:
            self.__remove_field(fieldname)
        
        # remove node field
        self.__remove_field(self._nodes[node_name])
        del self._nodes[node_name]
        del self._node_fields[node_name]


class InterconnectInputNode(avango.script.Script):
    
    def __init__(self):
        self.super(InterconnectInputNode).__init__()
        
        self.always_evaluate(True)
        self._stream = None
        self._nodes = {}
        self._node_fields = {}
        self._connected = False

    def set_stream(self, stream):
        self._stream = stream

    def __remove_field(self, name):
        field = self._get_field(name)
        
        # disconnect field from node's field
        field.disconnect_auditors()
        field.disconnect()
        
        # remove field
        self.remove_field(name)

    def __cleanup_received_nodes(self):
        for node_name in self._nodes:
            for field_name in self._node_fields[node_name]:
                self.__remove_field(self._node_fields[node_name][field_name])
            self.__remove_field("__" + node_name)
            self.node_was_deleted(self._nodes[node_name])
        self._nodes = {}
        self._node_fields = {}

    def evaluate(self):
        # cleanup received nodes, if connection was lost
        if self._connected and not self._stream.Connected.value:
            self.__cleanup_received_nodes()
        self._connected = self._stream.Connected.value
        
        # receive data, if connected
        if self._connected:
            self._stream.flush()
            success = True
            while success:
                success = self.read_field()

    def read_field(self):
        raw_line = self._stream.input.readline()
        if raw_line == "":
            return False
        
        pattern = re.compile("([^ ]+) ([^ ]+)(.*)$")
        result = pattern.match(raw_line[:-1])
        if not result:
            return
        
        fieldtype = result.group(1)
        fieldname = result.group(2)
        fieldvalue = result.group(3)
        
        field = self._get_field(fieldname)
        
        if fieldvalue == "":
            #print "read_field: " + fieldtype + " " + fieldname
            if field:
                self.remove_field(fieldname)
                self.field_was_removed(fieldname, field)
        else:
            fieldvalue = fieldvalue[1:]
            #print "read_field: " + fieldtype + " " + fieldname + " = <" + fieldvalue + ">"
            
            if field == None:
                field = avango._avango._make_field_by_name(fieldtype)
                field_is_new = True
            else:
                field_is_new = False
            
            instream = avango._avango.StringInputStream()
            instream.buffer = fieldvalue
            field.read(instream)
            
            if field_is_new:
                self.add_field(field, fieldname)
                field = self._get_field(fieldname)
                self.field_was_added(fieldname, field)
        
        return True

    def field_was_added(self, name, field):
        #print "field was added: " + name + " = " + str(field.value)
        
        # if this is a node field, handle it
        pattern = re.compile("__([^_]+)$")
        result = pattern.match(name)
        if result:
            node_name = result.group(1)
            
            node_type = field.value
            node = avango._make_instance_by_name(node_type)
            
            self._nodes[node_name] = node
            self._node_fields[node_name] = {}
            self.node_was_added(node)
            
            #print "node was added: " + node_name + " = new " + node_type + "()"

        # if this is a field, add it to a registered node
        pattern = re.compile("__([^_]+)_(.*)")
        result = pattern.match(name)
        if result:
            node_name = result.group(1)
            field_name = result.group(2)
            
            if node_name in self._nodes:
                node = self._nodes[node_name]
                if node_name in self._node_fields:
                    self._node_fields[node_name][field_name] = name
                    
                    f_input = self._get_field(name)
                    f_node = node._get_field(field_name)
                    f_node.connect_from(f_input)
                    
                    #print "field was added: " + node_name + "." + field_name #+ " = " + str(field.value)
                    
                    if node._get_num_fields() == len(self._node_fields[node_name]):
                        self.node_is_initialized(node)

    def field_was_removed(self, name, field):
        #print "field was removed: " + name
        
        # if this is a node field, handle it
        pattern = re.compile("__([^_]+)$")
        result = pattern.match(name)
        if result:
            node_name = result.group(1)
            
            node = self._nodes[node_name]
            del self._node_fields[node_name]
            del self._nodes[node_name]
            self.node_was_deleted(node)
            
            #print "node was removed: " + node_name

        # if this is a field, add it to a registered node
        pattern = re.compile("__([^_]+)_(.*)")
        result = pattern.match(name)
        if result:
            node_name = result.group(1)
            field_name = result.group(2)
            
            del self._node_fields[node_name][field_name]
            
            #print "field was removed: " + node_name + "." + field_name

    def node_was_added(self, node):
        pass

    def node_is_initialized(self, node):
        pass

    def node_was_deleted(self, node):
        pass
    


class InterconnectPipe(object):
    def __init__(self):
        self._content = ''

    def write(self, data):
        self._content += data

    def readline(self):
        lineend = self._content.find('\n')
        result = self._content[:lineend+1]
        self._content = self._content[lineend+1:]
        return result
    


class _InterconnectNetworkStream(avango.script.Script):
    Connected = avango.SFBool()

    def __init__(self):
        self.super(_InterconnectNetworkStream).__init__()
        
        self.Connected.value = False
        self._input = InterconnectPipe()
        self._output = InterconnectPipe()
        self._pending_output = ""
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
        connected = self._update()
        if self.Connected.value != connected:
            self.Connected.value = connected
        
        if not connected:
            return

        try:
            # FIXME use of small constant
            data = self._connection.recv(8192)
            if not len(data):
                self._connection = None
                self.Connected.value = False
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
        self.Connected.value = False


class InterconnectServer(_InterconnectNetworkStream):
    Host = avango.SFString()
    Port = avango.SFInt()

    def __init__(self):
        self.super(InterconnectServer).__init__()
        
        self.Host.value = ""
        self.Port.value = 0
        self._socket = None

    def evaluate(self):
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self._socket.bind((self.Host.value, self.Port.value))
        self.port = self._socket.getsockname()[1]
        self._socket.listen(1)
        self._socket.setblocking(0)

    def _update(self):
        if not self._socket:
            return False
        if self._connection:
            return True
        try:
            self._connection = self._socket.accept()[0]
            self._connection.setblocking(0)
        except socket.error, err:
            return False
        if not self._connection:
            return False
        self._input = InterconnectPipe()
        self._output = InterconnectPipe()
        self._pending_output = ""
        return True


class InterconnectClient(_InterconnectNetworkStream):
    Host = avango.SFString()
    Port = avango.SFInt()

    def __init__(self):
        self.super(InterconnectClient).__init__()
        
        self.Host.value = ""
        self.Port.value = 0
        self._socket = None
        self._next_try = datetime.datetime.now()

    def _update(self):

        if self._connection:
            return True
        if datetime.datetime.now() < self._next_try:
            return False
        
        try:
            if not self._socket:
                self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self._socket.setblocking(0)
            self._socket.connect((self.Host.value, self.Port.value))
        except socket.error, err:
            #under windows ignore the error "The socket operation could not complete without blocking"
            #TODO: Most probably not the best way. Use other implementation
            if platform.system() == "Windows":                
                if err.args[0]!= 10035:
                    self._next_try = datetime.datetime.now() + datetime.timedelta(seconds=1)
                    return False
            else:
                self._next_try = datetime.datetime.now() + datetime.timedelta(seconds=1)
                return False
                
        self._connection = self._socket
        self._socket = None
        self._input = InterconnectPipe()
        self._output = InterconnectPipe()
        self._pending_output = ""
        return True
