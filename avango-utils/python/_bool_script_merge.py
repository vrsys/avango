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
import avango.script

import _bool_scripts


def merge_and_connect_bool_scripts(script1, script2):
    '''
    Merges the SFBool nodes a two avango.FieldContainers with an OR relation
    '''
    merged_script = avango.script.Script()
    
    for i in xrange(script1.get_num_fields()):
        #get field of script1
        script1_field = script1.get_field(i)
        field1_name = script1_field.get_name()
        field1_type = script1_field.get_type()
        
        #check if the type of the field is not SFBool or
        #the merged script already does not already contain a field with the given name
        if "SFBool" not in field1_type or merged_script.get_field(field1_name):
            continue 
        
        #get corresponding field of script2
        script2_field = script2.get_field(field1_name)
        field2_type = script1_field.get_type()
        
        #check if script2 also has a filed with this name and of the same type
        if script2_field == None and field1_type == field2_type:
            continue
        
        new_field = avango._avango._make_field_by_name(script1_field.get_type())
        merged_script.add_field(new_field, field1_name)

        # connect new field from node's field
        # use an OR relationship
        new_field = merged_script.get_field(field1_name)    
        new_field.connect_from(_bool_scripts.make_bool2_or(script1_field,script2_field))
        
    return merged_script