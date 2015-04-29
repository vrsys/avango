import bpy

DEFAULT_SOCKETS = [
    'NodeSocketFloat',
    'NodeSocketBool',
    'NodeSocketInt',
]


def export_values(obj):
    values = {}
    for socket_key in obj.inputs.keys():
        socket = obj.inputs[socket_key]
        if socket.bl_idname in DEFAULT_SOCKETS:
            values[socket_key] = socket.default_value
        else:
            values[socket_key] = socket.get_default_value()

    return values


def export_links(obj):
    field_connections = []
    for key in obj.outputs.keys():
        for link in obj.outputs[key].links:
            if link.to_node.bl_idname == 'FromObject':
                ref_obj = bpy.data.objects[link.to_node.referenced_object]
            else:
                ref_obj = link.to_node
                field_connections.append({
                    "from_field": key,
                    "to_node": ref_obj.name,
                    "to_field": link.to_socket.name,
                    })
    return field_connections
