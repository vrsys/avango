bl_info = {
    "name": "Avango",
    "author": "Avango Development Team",
    "version": (14, 11, 0),
    "blender": (2, 72, 0),
    "location": "File > Import-Export",
    "description": "Avango is a 3D framework",
    "warning": "",
    "wiki_url": "github.com/vrsys/avango-blender",
    "category": "Import-Export"
}

if "bpy" in locals():
    import imp
    imp.reload(preferences)
    imp.reload(node_tree)
    imp.reload(exporter)
    imp.reload(sockets)
    imp.reload(engine)
else:
    import bpy
    import nodeitems_utils as nu
    from . import (
        preferences,
        node_tree,
        exporter,
        starter, nodes,
        sockets,
        engine,
        )

node_categories = [
    node_tree.AvangoNodeCategory("SOMENODES", "scene",
                                 items=[
                                     nu.NodeItem("Window"),
                                     nu.NodeItem("Camera"),
                                     nu.NodeItem("Light"),
                                     nu.NodeItem("Screen"),
                                     # nu.NodeItem("Transform"),
                                     nu.NodeItem("Mesh"),
                                     nu.NodeItem("FromObject"),
                                 ]),
    node_tree.AvangoNodeCategory(
        "UTILNODES", "utils",
        items=[
            nu.NodeItem("FloatInputNode"), nu.NodeItem("FloatMath"),
            nu.NodeItem("TranslationMatrix"), nu.NodeItem(
                "RotationMatrix"), nu.NodeItem("Vec3"), nu.NodeItem("Script")
        ]),
    node_tree.AvangoNodeCategory("CORENODES", "core",
                                 items=[
                                     nu.NodeItem("TimeSensor"),
                                     nu.NodeItem("Keyboard"),
                                 ])
]


def register():
    from . import ui
    from . import properties
    preferences.register()
    properties.register()
    ui.register()
    node_tree.register()
    sockets.register()
    exporter.register()
    starter.register()
    nodes.register()

    bpy.utils.register_module(__name__)

    # Idee: root am SceneGraph
    # 1.)
    # alle objects haben CollectionProperty,
    # sollte beim exportieren des scene graphs ein die CollectionProperty eines
    # Hierarchieknotens leer sein, wird ein default FieldContainer des
    # passenden typs erstellt
    # 2.) wenn ein Node gelöscht wird, soll auf referenzierten blender objecten
    #   vorher die Referenz auf diesen Node entfernt werden

    #bpy.types.Object.avango_nodes = bpy.props.CollectionProperty(
    #  type=bpy.types.PropertyGroup)

    if 'AVANGO_NODES' in nu._node_categories:
        nu.unregister_node_categories('AVANGO_NODES')
    nu.register_node_categories('AVANGO_NODES', node_categories)


def unregister():
    from . import ui
    from . import properties
    node_tree.unregister()
    exporter.unregister()
    starter.unregister()
    if 'AVANGO_NODES' in nu._node_categories:
        nu.unregister_node_categories('AVANGO_NODES')
    preferences.unregister()
    properties.unregister()
    ui.unregister()
    bpy.utils.unregister_module(__name__)


if __name__ == "__main__":
    register()
