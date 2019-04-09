import avango
import avango.script
import avango.gua
from avango.script import field_has_changed

from src.DynamicQuad import DynamicQuad
from src.PhotoProjector import PhotoProjector

class MultiViewExplorer(avango.script.Script):
    def __init__(self):
        self.super(MultiViewExplorer).__init__()
        self.always_evaluate(False)
        self.is_initialized = False

    def my_constructor(self, scenegraph, parent_node, atlas_path, images, width, height, rows=2, columns=1):

        self.parent_node = parent_node
        self.atlas_path = atlas_path
        self.width = width
        self.height = height
        self.rows = rows
        self.columns = columns
        self.views = rows* columns

        self.images = images
        self.dynamic_quads = []
        self.projectors = []
        self.quad_transforms = [ avango.gua.nodes.TransformNode(Name='quad_trans_'+str(i)) for i in range(self.views)]
        for i in range(self.views):
            quad_trans_node = self.quad_transforms[i]
            
            x = (self.width/2) - (i // self.rows * (self.width/self.columns)) - (self.width/self.columns/2)
            y = self.height/2 - (i % self.rows * (self.height/self.rows)) - (self.height/self.rows/2)
            quad_trans_node.Transform.value = avango.gua.make_trans_mat(x, y, 0.0)
            
            dq = DynamicQuad(quad_trans_node, (self.width/self.columns) / 2, (self.height/self.rows) / 2)
            dynamic_quad_node = dq.get_node()
            self.dynamic_quads.append(dynamic_quad_node)
            self.parent_node.Children.value.append(quad_trans_node)

            projector = PhotoProjector()
            projector.my_constructor(quad_trans_node, dynamic_quad_node, quad_trans_node, self.images)
            projector.Graph.value = scenegraph
            projector.Texture.value = self.atlas_path

            projector.update_images(i)
            dynamic_quad_node.Material.connect_from(projector.Material)
            projector.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 3.0) 
                                        # *\
                                        # avango.gua.make_rot_mat(90, 1.0, 0.0, 0.0)
            # projector.Transform.connect_from(quad_trans_node.Transform)
            self.projectors.append(projector)
            # photo_projection = PhotoProjection()
            # photo_projection.my_constructor()
            # projector.set_localized_image_list(localized_images)
            projector.set_projection_quad(dynamic_quad_node, quad_trans_node)
            print('####', projector.Material)

    def set_image_list(self, images):
        self.images = images

    def update_images(self):
        pass

            

