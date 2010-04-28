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