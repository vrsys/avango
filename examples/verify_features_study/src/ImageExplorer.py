import avango
import avango.script
import avango.gua
from avango.script import field_has_changed

from src.DynamicQuad import DynamicQuad
from src.MyProjector import MyProjector


## print the subgraph under a given node to the console
def print_graph(root_node):
    stack = [(root_node, 0)]
    while stack:
        node, level = stack.pop()
        print("│   " * level + "├── {0} <{1}>".format(node.Name.value, node.__class__.__name__))
        stack.extend([(child, level + 1) for child in reversed(node.Children.value)])


class ImageExplorer(avango.script.Script):

    Button0 = avango.SFBool()
    Button1 = avango.SFBool()

    def __init__(self):
        self.super(ImageExplorer).__init__()
        self.always_evaluate(False)
        self.is_initialized = False

    def my_constructor(self, scenegraph, parent_node, atlas_path, images, width, height, rows=3, columns=4):
        self.parent_node = parent_node
        self.atlas_path = atlas_path
        self.width = width
        self.height = height
        self.rows = rows
        self.columns = columns
        self.views = rows* columns
        self.offset = avango.gua.make_identity_mat()
        self.button0_pressed = False

        self.images = images
        self.dynamic_quads = []
        self.projectors = []
        self.quad_transforms = [ avango.gua.nodes.TransformNode(Name='quad_trans_'+str(i)) for i in range(self.views)]
        for i in range(self.views):
            quad_trans_node = self.quad_transforms[i]
            
            x = (self.width/2) - (i // self.rows * (self.width/self.columns)) - (self.width/self.columns/2)
            y = self.height/2 - (i % self.rows * (self.height/self.rows)) - (self.height/self.rows/2)
            quad_trans_node.Transform.value = avango.gua.make_trans_mat(x, y, 0.0)
            print('QUAD SIZE',(self.width/self.columns) / 2)
            dq = DynamicQuad(quad_trans_node, (self.width/self.columns) / 2, (self.height/self.rows) / 2)
            dynamic_quad_node = dq.get_node()
            self.dynamic_quads.append(dynamic_quad_node)
            self.parent_node.Children.value.append(quad_trans_node)

            projector3 = MyProjector()
            projector3.my_constructor(parent_node,"p"+str(i))
            self.projectors.append(projector3)
            # parent_node.Children.value.append(projector3.group_node)
            projector3.set_localized_image_list(self.images)
            projector3.Graph.value = scenegraph
            projector3.Texture.value = self.atlas_path
            projector3.Transform.value = avango.gua.make_trans_mat(x, y, 1.0)

            projector3.set_projection_lense(dynamic_quad_node, quad_trans_node)
            dynamic_quad_node.Material.connect_from(projector3.Material)

            projector3.change_photo_projection(i)

            print('appended projector', i)

    def set_image_list(self, images):
        self.images = images

    def update_images(self, image_id_list, offset_list):
        if self.views == len(image_id_list) and self.views == len(offset_list):
            for idx in range(self.views):
                self.projectors[idx].set_offset(offset_list[idx])
                # self.projectors[idx].set_image_id(image_id_list[idx])
                self.projectors[idx].change_photo_projection(image_id_list[idx])
        else:
            print('Error in lengths of lists')


    @field_has_changed(Button0)
    def button0_changed(self):
        # pass
        if self.Button0.value:
            self.update_perspective()

            self.button0_pressed = True
        # print('TASTE 0')
        # self.projection_lense.Material.connect_from(self.Material)
        else:
            if self.button0_pressed:
                lense_mat = self.projection_lense.WorldTransform.value
                projector_mat = self.localized_image_list[self.old_closest_id].transform

                self.offset = avango.gua.make_inverse_mat(lense_mat) * projector_mat

        self.Transform.value = self.projection_lense.WorldTransform.value * self.offset

        self.button0_pressed = False


