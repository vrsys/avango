#!/usr/bin/python

import avango
import avango.gua
import avango.script
from avango.script import field_has_changed
from examples_common.GuaVE import GuaVE

from Device import TouchDevice

import subprocess # for starting the touch driver


class TouchVisualization(avango.script.Script):
  PosX = avango.SFFloat()
  PosY = avango.SFFloat()
  ScreenWidth = avango.SFFloat()
  ScreenHeight = avango.SFFloat()
  MatrixOut = avango.gua.SFMatrix4()

  scale = 0.02

  def evaluate(self):
    # map touch input values to screen dimensions and create matrix
    _x = (self.PosX.value - 0.5) * self.ScreenWidth.value
    _y = (self.PosY.value - 0.5) * self.ScreenHeight.value
    self.MatrixOut.value =  avango.gua.make_trans_mat(_x, 0, _y) * \
                            avango.gua.make_scale_mat(self.scale, self.scale, self.scale)


def create_touch_visualizations(LOADER, PARENT_NODE, SCREEN_NODE, TOUCH_DEVICE):
  _touch_visualizations = []

  # iterate over possible touch positions and create visualization
  for i, _touch_cursor in enumerate(TOUCH_DEVICE.TouchCursors):
    _sphere = LOADER.create_geometry_from_file(
        "touch_vis" + str(i)
      , "data/objects/light_sphere.obj"
      , "data/materials/White.gmd"
      , avango.gua.LoaderFlags.DEFAULTS
      )

    # create field container and set up field connections
    _touch_vis = TouchVisualization()
    _touch_vis.PosX.connect_from(_touch_cursor.PosX)
    _touch_vis.PosY.connect_from(_touch_cursor.PosY)
    _touch_vis.ScreenWidth.connect_from(SCREEN_NODE.Width)
    _touch_vis.ScreenHeight.connect_from(SCREEN_NODE.Height)

    # connect sphere's transformation to field container output matrix
    _sphere.Transform.connect_from(_touch_vis.MatrixOut)

    # add sphere to scene graph
    PARENT_NODE.Children.value.append(_sphere)

    # store field container in list for access via guaVE
    _touch_visualizations.append(_touch_vis)

  return _touch_visualizations


def main():

  # start touch driver and suppress its output
  _devnull = open('/dev/null', 'w')
  subprocess.Popen(["sudo", "/usr/sbin/citmuto03drv"], stderr = _devnull, stdout = _devnull)

  # setup scenegraph
  graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")

  loader = avango.gua.nodes.GeometryLoader()

  eye = avango.gua.nodes.TransformNode(Name = "eye")
  eye.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 3.5)

  screen = avango.gua.nodes.ScreenNode(Name = "screen", Width = 1.6, Height = 0.9)
  screen.Transform.value = avango.gua.make_trans_mat(0, 1, 0) * avango.gua.make_rot_mat(-90, 1, 0, 0)
  screen.Children.value = [eye]

  graph.Root.value.Children.value = [screen]

  # setup viewing
  _width = 1920 * 2   # for fullscreen on quad-hd-display
  size = avango.gua.Vec2ui(_width, _width * 9 / 16)
  camera = avango.gua.nodes.Camera(
      LeftEye = "/screen/eye"
    , RightEye = "/screen/eye"
    , LeftScreen = "/screen"
    , RightScreen = "/screen"
    , SceneGraph = "scenegraph"
    , Mode = 1    # orthogonal projection
    )
  window = avango.gua.nodes.Window(
      Size = size
    , LeftResolution = size
    )
  pipe = avango.gua.nodes.Pipeline(
      Camera = camera
    , Window = window
    , LeftResolution = size
    )

  #setup viewer
  viewer = avango.gua.nodes.Viewer()
  viewer.Pipelines.value = [pipe]
  viewer.SceneGraphs.value = [graph]

  # initialize touch input
  touch_device = TouchDevice()

  # create a sphere visualizing each touch input
  touch_vis_transform = avango.gua.nodes.TransformNode(Name = "touch_visualizations")
  graph.Root.value.Children.value.append(touch_vis_transform)
  touch_visualizations = create_touch_visualizations(loader, touch_vis_transform, screen, touch_device)

  # start interactive shell
  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  # run the application
  viewer.run()

if __name__ == '__main__':
  main()
