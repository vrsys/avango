import bpy
from bpy.types import NodeTree
from nodeitems_utils import NodeCategory


class AvangoCustomTree(NodeTree):
    ''' FieldContainer Nodes, (Avango) '''
    bl_idname = 'AvangoCustomTreeType'  # bl_idname = 'CustomTreeType'
    bl_label = 'Avango Custom Tree'
    bl_icon = 'SEQ_CHROMA_SCOPE'

    def update(self):
        try:
            pass
            # is_ready = bpy.data.node_groups[self.id_data.name]
        except:
            return


# Mix-in class for all custom nodes in this tree type.
# Defines a poll function to enable instantiation.
class AvangoCustomTreeNode(object):
    @classmethod
    def poll(cls, ntree):
        return ntree.bl_idname == 'AvangoCustomTreeNode'

    def draw_buttons(self, context, layout):
        # this helps some nodes which don't have buttons but do have sliders
        # to not be visually weird.(else the effect is cropping in y direction)
        # this somehow adds a few pixels.
        pass


class AvangoNodeCategory(NodeCategory):
    @classmethod
    def poll(cls, context):
        return context.space_data.tree_type == 'AvangoCustomTreeType'
        #return context.space_data.tree_type == 'CustomTreeType'


tree_classes = [
    AvangoCustomTree
    ]


def register():
    print("node_tree.register.")
    for c in tree_classes:
        bpy.utils.register_class(c)


def unregister():
    print("node_tree.unregister.")
    for c in tree_classes:
        bpy.utils.unregister_class(c)
