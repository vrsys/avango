import avango.osg
import avango.inspector
import avango.osg.simpleviewer
import math

time_sensor = avango.nodes.TimeSensor()

root = avango.osg.nodes.Group(Name="Root")
sphere1 = avango.osg.nodes.Sphere(Name="RedSphere", Color=avango.osg.Vec4(1., 0., 0., 1), DetailRatio = 0.8)
sphere1.Matrix.value = avango.osg.make_trans_mat(1, 0, 0)
sphere2 = avango.osg.nodes.Sphere(Name="GreenSphere", Color=avango.osg.Vec4(0., 1., 0., 1), DetailRatio = 0.8)
sphere2.Matrix.value = avango.osg.make_trans_mat(-1, 0, 0)
root.Children.value = [ sphere1, sphere2 ]

class Wobble(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.osg.SFMatrix()

    def evaluate(self):
        self.MatrixOut.value = avango.osg.make_trans_mat(math.sin(self.TimeIn.value), 0., 0.)

wobble = Wobble(Name='Wobble')
sphere1.Matrix.connect_from(wobble.MatrixOut)
wobble.TimeIn.connect_from(time_sensor.Time)

inspector = avango.inspector.nodes.Inspector(Children=[root])
avango.osg.simpleviewer.run(root)
