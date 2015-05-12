import bpy
from bpy.props import BoolProperty


class AvangoPanel(bpy.types.Panel):
    '''
    : intended to handle io of OSC messages
    '''
    bl_idname = "AvangoPanel"
    bl_label = "Avango panel"
    bl_space_type = 'NODE_EDITOR'
    bl_region_type = 'UI'
    bl_category = 'FLOW'
    bl_options = {'DEFAULT_CLOSED'}
    use_pin = True

    @classmethod
    def poll(cls, context):
        try:
            return context.space_data.node_tree.bl_idname ==\
                'AvangoCustomTreeType'
        except:
            return False

    def draw(self, context):
        pass
        # ntree = context.space_data.node_tree

        # layout = self.layout
        # col = layout.column()
        # tstr = 'start' if not ntree.avango_state else 'end'
        # col.operator('wm.spflow_osc_server', text=tstr).mode = tstr

        # # show some controls when server is started
        # if tstr == 'end':
        #     col.operator(
        #         'wm.spflow_eval_synthdef',
        #         text='send',
        #         ).mode = 'send'
        #     col.operator(
        #         'wm.spflow_eval_synthdef',
        #         text='trigger',
        #         ).mode = 'trigger'
        #     col.operator(
        #         'wm.spflow_eval_synthdef',
        #         text='free',
        #         ).mode = 'free'
        #     col.operator(
        #         'wm.spflow_eval_synthdef',
        #         text='freeAll',
        #         ).mode = 'freeAll'


def register():
    bpy.types.AvangoCustomTreeType.avango_state = BoolProperty(
        default=False,
        description=
        'toggle used to indicate state of osc client and hide buttons'
        )
    bpy.utils.register_class(AvangoPanel)


def unregister():
    bpy.utils.unregister_class(AvangoPanel)
    del bpy.types.AvangoCustomTreeType.avango_state
