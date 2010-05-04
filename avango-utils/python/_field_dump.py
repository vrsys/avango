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

import avango

def dump_fields(script):
    
    buffer = ""
    
    num_fields = script.get_num_fields()
    buffer += "ScriptName <" + str(script.Name.value) +">\n"
    
    for i in xrange(num_fields):
        field = script._get_field(i)
        field_name = field._get_name()
        field_type = field._get_type()
        buffer += "Field: <"+ str(field_name) + "> of type: <" + str(field_type) + "> = <" + str(field.value) + ">\n" 
        
    return buffer


class ScriptFieldPrinter(avango.script.Script):
    
    Script = avango.script.SFObject()
    
    def __init__(self):
        self.super(ScriptFieldPrinter).__init__()
        self.always_evaluate(True)
    
    def evaluate(self):
        if self.Script.value:
            buffer = dump_fields(self.Script.value)
            print buffer