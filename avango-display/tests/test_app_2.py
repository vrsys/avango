import display
import avango
import sys

argv = display.init(sys.argv)
view1 = display.make_view()
scene1 = avango.osg.nodes.Sphere(Radius=0.3, Matrix=avango.osg.make_trans_mat(0, 1.2, -3))
view1.Root.value = scene1
view1.Viewport.value = avango.osg.Vec4(0, 0, 0.48, 1)

view2 = display.make_view()
scene2 = avango.osg.nodes.Sphere(Radius=0.3, Matrix=avango.osg.make_trans_mat(0, 1.2, -3), Color=avango.osg.Vec4(1, 0, 0, 0))
view2.Root.value = scene2
view2.Viewport.value = avango.osg.Vec4(0.48, 0, 0.52, 1)

display.run()