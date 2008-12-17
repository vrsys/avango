import display
import avango
import avango.osg
import sys

wiimote = display.make_dominant_user_device()
sphere_transform = avango.osg.nodes.MatrixTransform()
sphere_transform.Matrix.connect_from(wiimote.Matrix)

sphere = avango.osg.nodes.Sphere(Radius=0.3, Matrix=avango.osg.make_trans_mat(0, 1.2, -3))
sphere_transform.Children.value = [sphere]

argv = display.init(sys.argv)
view1 = display.make_view()
view1.Root.value = sphere_transform

display.run()
