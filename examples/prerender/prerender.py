from avango import display
import avango
import avango.osg
import avango.script
import math
import sys

class RotateMatrix(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.osg.SFMatrix()

    def evaluate(self):
        self.MatrixOut.value = avango.osg.make_rot_mat(self.TimeIn.value, 0., 1., 0.)

class SwingMatrix(avango.script.Script):
    TimeIn = avango.SFFloat()
    Scale = avango.SFFloat()
    MatrixOut = avango.osg.SFMatrix()

    def evaluate(self):
        self.MatrixOut.value = avango.osg.make_trans_mat(0., self.Scale.value*math.sin(self.TimeIn.value), 0.)


def build_texture(time_sensor, root):
    texture2d = avango.osg.nodes.Texture2D()

    camera_attachment = avango.osg.nodes.CameraAttachment(Texture=texture2d)

    camera = avango.osg.nodes.Camera(Attachment=[camera_attachment],
                                     RenderOrder=avango.osg.RenderOrder.PRE_RENDER,
                                     ClearColor=avango.osg.Vec4(0., 1., 0.,  1.),
                                     Viewport=avango.osg.Vec4(0., 0., 256., 256.),
                                    )

    sphere = avango.osg.nodes.Sphere(Radius=0.1, Matrix=avango.osg.make_trans_mat(0., 0., 0.))

    swing = SwingMatrix(Scale=0.1)
    swing.TimeIn.connect_from(time_sensor.Time)

    transform = avango.osg.nodes.MatrixTransform()
    transform.Matrix.connect_from(swing.MatrixOut)
    transform.Children.value = [sphere]

    camera.Children.value.append(transform)
    root.Children.value.append(camera)

    return texture2d, [swing]


def build_quad(time_sensor):
    geometry = avango.osg.nodes.Geometry(Mode = 7, NormalBinding = 1, ColorBinding = 1)

    vertex_coords = []
    tex_coords = []
    uvs = [ (1., 1.), (0., 1.), (0., 0.), (1., 0.) ]
    for u, v in uvs:
        vertex_coords.append(avango.osg.Vec3(0.1*(2*u-1.), 0.1*(2.*v-1.), 0.))
        tex_coords.append(avango.osg.Vec2(u, v))

    geometry.VertexArray.value = vertex_coords
    geometry.TexCoordArray.value = tex_coords
    geometry.PrimitiveIndices.value = [0,1,2,3]
    geometry.PrimitiveLengths.value = [4]

    root = avango.osg.nodes.Group()

    texture2d, keep_alive = build_texture(time_sensor, root)
    state_set = avango.osg.nodes.StateSet(Texture0 = texture2d)

    rotation = RotateMatrix()
    rotation.TimeIn.connect_from(time_sensor.Time)

    transform = avango.osg.nodes.MatrixTransform()
    transform.Matrix.connect_from(rotation.MatrixOut)
    transform.Children.value = [avango.osg.nodes.Geode(Drawables=[geometry], StateSet=state_set)]

    root.Children.value.append(transform)
    return root, keep_alive+[rotation]


argv = display.init(sys.argv)
view1 = display.make_view()
view1.BackgroundColor.value = avango.osg.Vec4(0.5, 0.5, 0.5, 1.)

time_sensor = avango.nodes.TimeSensor()

sphere, keep_alive = build_quad(time_sensor)

transform = avango.osg.nodes.MatrixTransform(Children=[sphere], Matrix=avango.osg.make_trans_mat(0, 1.7, -1.2))
view1.Root.value = transform

display.run()
