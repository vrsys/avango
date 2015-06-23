import avango
import avango.gua
import avango.script

from importlib import machinery


def create_new_script(json, app, filepath):
    module = json['module']
    classname = json['name']

    filepath = filepath + "tmp/" + module + ".py"

    loader = machinery.SourceFileLoader(module, filepath)
    loader.load_module()
    exec("from " + module + " import " + classname)

    new_script = eval(classname + "()", globals(), locals())
    new_script.Name.value = classname

    app.add_field_container(new_script)

    for connection in json["field_connections"]:
        app.plan_field_connection(
            new_script.Name.value,
            connection["from_field"],
            connection["to_node"],
            connection["to_field"]
        )

    for field_name in json["values"]:
        field = new_script.get_field(field_name)
        field.value = load_field_value(field, json["values"][field_name])


def load_field_value(field, json_value):
    if isinstance(field, avango.SFFloat):
        return json_value
    elif isinstance(field, avango.SFBool):
        return json_value
    elif isinstance(field, avango.SFInt):
        return json_value
    elif isinstance(field, avango.gua.SFVec3):
        return load_vec3_value(json_value)
    elif isinstance(field, avango.gua.SFMatrix4):
        return load_mat4_value(json_value)


def load_vec3_value(json_value):
    return avango.gua.Vec3(json_value[0], json_value[1], json_value[2])


def load_mat4_value(json_value):
    matrix = avango.gua.make_identity_mat()

    for element in range(15):
        matrix.set_element(
            int(element/4),
            element % 4,
            json_value[element],
        )

    return matrix
