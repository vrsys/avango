from avango import display
import avango
import sys

argv = display.init(sys.argv)
view1 = display.make_view()
sphere = avango.osg.nodes.Sphere(Radius=0.1, Matrix=avango.osg.make_trans_mat(0, 1.7, -1.2))
view1.Root.value = sphere

display.run()
