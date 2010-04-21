import avango

def dump_fields(script):
    
    buffer = ""
    
    num_fields = script.get_num_fields()
    buffer += "ScriptName <" + str(script.Name.value) +">\n"
    
    for i in xrange(num_fields):
        field = script.get_field(i)
        name = script.get_field_name(i)
        buffer += " " + str(name) +" <"+ str(field.value) +">\n" 
        
    return buffer