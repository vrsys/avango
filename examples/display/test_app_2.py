from avango import display
import avango
import sys

argv = display.init(sys.argv)

xsplit = 0.5
ysplit = 0.5

def setup_view(viewport, color, background):
    view = display.make_view()
    scene = avango.osg.nodes.Sphere(Radius=0.1, Matrix=avango.osg.make_trans_mat(0, 1.7, -3), Color=color)
    view.Root.value = scene
    view.Viewport.value = viewport
    view.BackgroundColor.value = background

setup_view(avango.osg.Vec4(0., 0., xsplit, ysplit), avango.osg.Vec4(1, 0, 0, 0), avango.osg.Vec4(0.2, 0.2, 0.2, 1.))
setup_view(avango.osg.Vec4(xsplit, ysplit, 1., 1.), avango.osg.Vec4(0, 1, 0, 0), avango.osg.Vec4(0.4, 0.4, 0.4, 1.))
setup_view(avango.osg.Vec4(xsplit, 0., 1, ysplit), avango.osg.Vec4(0, 0, 1, 0), avango.osg.Vec4(0.6, 0.6, 0.6, 1.))
setup_view(avango.osg.Vec4(0., ysplit, xsplit, 1.), avango.osg.Vec4(0, 1, 1, 0), avango.osg.Vec4(0.8, 0.8, 0.8, 1.))

display.run()
