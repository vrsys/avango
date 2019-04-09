import avango
import avango.gua
import avango.script

class DynamicQuad:

    number_of_instances = 0

    def __init__(self, parent_node, width=0.5, height=0.5):

        ## @var id
        # Identification number of this DynamicQuad.
        self.id = DynamicQuad.number_of_instances
        DynamicQuad.number_of_instances += 1

        self.width = width
        self.height = height

        dynamic_loader = avango.gua.nodes.DynamicTriangleLoader()
        self.dynamic_quad = dynamic_loader.create_empty_geometry(
            "dynamic_quad_" + str(self.id), 
            "dynamic_" + str(self.id) + ".lob", 
            avango.gua.LoaderFlags.DEFAULTS)

        quad_transform = avango.gua.make_trans_mat(0.0, 0.0, 0.0)

        pos = quad_transform * avango.gua.Vec3( width, height, 0.0)
        uv  = avango.gua.Vec2(1.0, 0.0)
        self.dynamic_quad.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
        
        pos = quad_transform * avango.gua.Vec3(-width, -height, 0.0)
        uv  = avango.gua.Vec2(0.0, 1.0)
        self.dynamic_quad.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
        
        pos = quad_transform * avango.gua.Vec3( width, -height, 0.0)
        uv  = avango.gua.Vec2(1.0, 1.0)
        self.dynamic_quad.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)

        pos = quad_transform * avango.gua.Vec3( width, height, 0.0)
        uv  = avango.gua.Vec2(1.0, 0.0)
        self.dynamic_quad.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)
        
        pos = quad_transform * avango.gua.Vec3(-width, height, 0.0)
        uv  = avango.gua.Vec2(0.0, 0.0)
        self.dynamic_quad.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)

        pos = quad_transform * avango.gua.Vec3(-width, -height, 0.0)
        uv  = avango.gua.Vec2(0.0, 1.0)
        self.dynamic_quad.push_vertex(pos.x, pos.y, pos.z, 1.0, 1.0, 1.0, 1.0, uv.x, uv.y)

        parent_node.Children.value.append(self.dynamic_quad)
        return self.dynamic_quad

    def get_node(self):
        return self.dynamic_quad