#!/usr/bin/python

import avango
import avango.gua
import avango.script
from avango.script import field_has_changed
import avango.oculus

from SimpleViewer import SimpleViewer

class TimedRotate(avango.script.Script):
  TimeIn = avango.SFFloat()
  MatrixOut = avango.gua.SFMatrix4()
  RotationSpeed = avango.SFFloat()

  def __init__(self):
    self.super(TimedRotate).__init__()
    self.RotationSpeed.value = 2.0

  @field_has_changed(TimeIn)
  def update(self):
    self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value*self.RotationSpeed.value,
                                                   0.0, 2.0, 0.0)

def start():
  # initialize oculus rift
  avango.oculus.initOVR()  

  # initialize materials
  avango.gua.load_shading_models_from("data/materials")
  avango.gua.load_materials_from("data/materials")

  # create loader for geometries
  loader = avango.gua.nodes.GeometryLoader()

  # create scenegraph
  graph = avango.gua.nodes.SceneGraph(
    Name = 'scenegraph'
  )

  # create big monkey
  monkey_transform = avango.gua.nodes.TransformNode(Name = 'MonkeyTransform')
  monkey_transform.Transform.value = avango.gua.make_trans_mat(0, 6, 0) * avango.gua.make_scale_mat(3, 3, 3)

  monkey_node = loader.create_geometry_from_file('monkey', 'data/objects/monkey.obj', 'Stones', avango.gua.LoaderFlags.DEFAULTS)
  monkey_transform.Children.value.append(monkey_node)  
  graph.Root.value.Children.value.append(monkey_transform)

  # create airplane
  airplane_transform = avango.gua.nodes.TransformNode(Name = 'AirplaneTransform')
  airplane_transform.Transform.value = avango.gua.make_trans_mat(40, 6, -80) * avango.gua.make_rot_mat(55, 0, 1, 0.1)

  airplane_node = loader.create_geometry_from_file('airplane', 'data/objects/cessna.obj', 'Stones', avango.gua.LoaderFlags.DEFAULTS)
  airplane_transform.Children.value.append(airplane_node)
  graph.Root.value.Children.value.append(airplane_transform)

  # create frog 1
  frog1_transform = avango.gua.nodes.TransformNode(Name = 'Frog1Transform')
  frog1_transform.Transform.value = avango.gua.make_trans_mat(-12, 1, -12) * avango.gua.make_rot_mat(45, 0, 1, 0) * avango.gua.make_scale_mat(3, 3, 3)

  frog1_node = loader.create_geometry_from_file('frog1', 'data/objects/frog.obj', 'Stones', avango.gua.LoaderFlags.DEFAULTS)
  frog1_transform.Children.value.append(frog1_node)
  graph.Root.value.Children.value.append(frog1_transform)

  # create frog 2
  frog2_transform = avango.gua.nodes.TransformNode(Name = 'Frog2Transform')
  frog2_transform.Transform.value = avango.gua.make_trans_mat(12, 1, -12) * avango.gua.make_rot_mat(-45, 0, 1, 0) * avango.gua.make_scale_mat(3, 3, 3)

  frog2_node = loader.create_geometry_from_file('frog2', 'data/objects/frog.obj', 'Stones', avango.gua.LoaderFlags.DEFAULTS)
  frog2_transform.Children.value.append(frog2_node)
  graph.Root.value.Children.value.append(frog2_transform)

  # create frog 3
  frog3_transform = avango.gua.nodes.TransformNode(Name = 'Frog3Transform')
  frog3_transform.Transform.value = avango.gua.make_trans_mat(12, 1, 12) * avango.gua.make_rot_mat(-135, 0, 1, 0) * avango.gua.make_scale_mat(3, 3, 3)

  frog3_node = loader.create_geometry_from_file('frog3', 'data/objects/frog.obj', 'Stones', avango.gua.LoaderFlags.DEFAULTS)
  frog3_transform.Children.value.append(frog3_node)
  graph.Root.value.Children.value.append(frog3_transform)

  # create frog 4
  frog4_transform = avango.gua.nodes.TransformNode(Name = 'Frog4Transform')
  frog4_transform.Transform.value = avango.gua.make_trans_mat(-12, 1, 12) * avango.gua.make_rot_mat(-225, 0, 1, 0) * avango.gua.make_scale_mat(3, 3, 3)

  frog4_node = loader.create_geometry_from_file('frog4', 'data/objects/frog.obj', 'Stones', avango.gua.LoaderFlags.DEFAULTS)
  frog4_transform.Children.value.append(frog4_node)
  graph.Root.value.Children.value.append(frog4_transform)

  # create lamp
  lamp_node = avango.gua.nodes.SpotLightNode(
    Name = 'lamp',
    EnableShadows = True,
    EnableGodrays = True,
    Transform = avango.gua.make_trans_mat(0, 20, 0) * \
                avango.gua.make_rot_mat(-90, 1, 0, 0) * \
                avango.gua.make_scale_mat(55, 55, 30)
  )
  graph.Root.value.Children.value.append(lamp_node)

  # create floor
  plane_node = loader.create_geometry_from_file('floor', 'data/objects/plane.obj', 'Tiles', avango.gua.LoaderFlags.DEFAULTS)
  plane_node.Transform.value = avango.gua.make_scale_mat(15, 1, 15)
  graph.Root.value.Children.value.append(plane_node)

  # create teapot group
  tea_move_node = avango.gua.nodes.TransformNode(
    Name = 'tea_move',
    Transform = avango.gua.make_trans_mat(0, 0.55, 0) * avango.gua.make_scale_mat(5, 5, 5)
  )
  tea_rot_node = avango.gua.nodes.TransformNode(
    Name = 'tea_rot'
  )
  graph.Root.value.Children.value.append(tea_move_node)
  tea_move_node.Children.value.append(tea_rot_node)

  pot1 = loader.create_geometry_from_file('pot1', 'data/objects/teapot.obj', 'Shiny', avango.gua.LoaderFlags.DEFAULTS)
  pot2 = loader.create_geometry_from_file('pot2', 'data/objects/teapot.obj', 'Shiny', avango.gua.LoaderFlags.DEFAULTS)
  pot3 = loader.create_geometry_from_file('pot3', 'data/objects/teapot.obj', 'Shiny', avango.gua.LoaderFlags.DEFAULTS)

  tea_rot_node.Children.value = [pot1, pot2, pot3]

  pot1.Transform.value = avango.gua.make_trans_mat(2,  0, 0) * avango.gua.make_scale_mat(0.2, 0.2, 0.2)
  pot2.Transform.value = avango.gua.make_trans_mat(0,  0, 2) * avango.gua.make_scale_mat(0.2, 0.2, 0.2)
  pot3.Transform.value = avango.gua.make_trans_mat(-2,  0, 0) * avango.gua.make_scale_mat(0.2, 0.2, 0.2)

  timed_rotate = TimedRotate(
    RotationSpeed = 50.0
  )
  timer = avango.nodes.TimeSensor()
  timed_rotate.TimeIn.connect_from(timer.Time)

  monkey_node.Transform.connect_from(timed_rotate.MatrixOut)
  tea_rot_node.Transform.connect_from(timed_rotate.MatrixOut)

  # create and launch viewer
  viewer = SimpleViewer()
  viewer.SceneGraph.value = graph
  viewer.run(locals(), globals())

if __name__ == '__main__':
  start()