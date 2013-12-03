#!/usr/bin/python

import avango
import avango.script
from avango.script import field_has_changed
import avango.gua

import time
import random

import examples_common.navigator
from examples_common.GuaVE import GuaVE
from SlideSwitcher import SlideSwitcher

SLIDE_OFFSET = 4.0

class TimedMaterialUniformUpdate(avango.script.Script):

  TimeIn = avango.SFFloat()
  MaterialName = avango.SFString()
  UniformName = avango.SFString()

  @field_has_changed(TimeIn)
  def update(self):
    avango.gua.set_material_uniform(self.MaterialName.value,
                                    self.UniformName.value,
                                    self.TimeIn.value)

# for avango example
class TimedRotate(avango.script.Script):
  TimeIn = avango.SFFloat()
  MatrixOut = avango.gua.SFMatrix4()

  @field_has_changed(TimeIn)
  def update(self):
    self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value*30.0, 0.0, 1.0, 0.0)

def setup_pipe():
  camera = avango.gua.nodes.Camera(
                                   #View = "/navigation/view",
                                   LeftEye = "/navigation/view",
                                   RightEye = "/navigation/view",
                                   #Screen = "/navigation/screen",
                                   LeftScreen = "/navigation/screen",
                                   RightScreen = "/navigation/screen",
                                   SceneGraph = "scenegraph")

  width = 1800
  height = 1180
  size = avango.gua.Vec2ui(width, height)

  window = avango.gua.nodes.Window(Size = size,
                                   Title = "final_presentation",
                                   LeftResolution = size)

  pipe = avango.gua.nodes.Pipeline(Camera = camera,
                                   Window = window,
                                   LeftResolution = size)


  avango.gua.create_texture("data/textures/sky.jpg")
  avango.gua.create_texture("data/textures/sky2.jpg")
  avango.gua.create_texture("data/textures/sky3.jpg")

  pipe.EnableFXAA.value = True
  pipe.EnableFrustumCulling.value = True
  pipe.EnableBackfaceCulling.value = True

  pipe.EnableHDR.value = False
  pipe.HDRKey.value = 3.0

  pipe.EnableFog.value = True
  pipe.FogTexture.connect_from(pipe.BackgroundTexture)
  pipe.FogStart.value = 250.0
  pipe.FogEnd.value = 580.0

  pipe.BackgroundTexture.value = "data/textures/sky.jpg"

  pipe.AmbientColor.value = avango.gua.Color(0.8, 0.8, 0.8)

  pipe.EnableSsao.value = True
  pipe.SsaoIntensity.value = 3.0
  pipe.SsaoRadius.value = 5.0

  pipe.EnableBloom.value = True
  pipe.BloomRadius.value = 10.0
  pipe.BloomThreshold.value = 0.9
  pipe.BloomIntensity.value = 0.3

  return pipe

def start():

  avango.gua.load_shading_models_from("data/materials")
  avango.gua.load_materials_from("data/materials")

  graph  = avango.gua.nodes.SceneGraph(Name = "scenegraph")
  loader = avango.gua.nodes.GeometryLoader()

  oilrig_cs = avango.gua.create_triangle_mesh_shape_from_geometry_file(
    "oilrig_cs",
    "data/objects/OIL_RIG_GUACAMOLE/oilrig.obj",
    True,
    False,
    avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY
  )
  oilrig_cs.Scaling.value = avango.gua.Vec3(0.1, 0.1, 0.1)


  oilrig_csn = avango.gua.nodes.CollisionShapeNode(
    Name = "oilrig_csn",
    ShapeName = "oilrig_cs"
  )

  oilrig = loader.create_geometry_from_file("town",
          "data/objects/OIL_RIG_GUACAMOLE/oilrig.obj",
          "White",
          avango.gua.LoaderFlags.LOAD_MATERIALS |
          avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY )
  oilrig.Transform.value = avango.gua.make_scale_mat(0.1, 0.1, 0.1)

  oilrig_rb = avango.gua.nodes.RigidBodyNode(
    Name = "oilrig_rb",
    Mass = 0.0,
    Restitution = 10.0,
    Transform = avango.gua.make_rot_mat(-90, 1, 0, 0),
    Children = [oilrig_csn]
  )

  oilrig_csn.Children.value = [oilrig]



  water = loader.create_geometry_from_file("water_geometry",
          "data/objects/plane.obj",
          "Water",
          avango.gua.LoaderFlags.DEFAULTS)

  water.Transform.value = avango.gua.make_scale_mat(1000, 1, 1000) * avango.gua.make_trans_mat(0, -10, 0)

  slides = loader.create_geometry_from_file("slides",
           "data/objects/test_slides.obj",
           "White",
           # avango.gua.LoaderFlags.DEFAULTS)
           avango.gua.LoaderFlags.LOAD_MATERIALS)


  sun = avango.gua.nodes.SpotLightNode(Name = "sun",
                                       Color = avango.gua.Color(1.0, 0.8, 0.6),
                                       Falloff = 0.009,
                                       Softness = 0.003,
                                       EnableDiffuseShading = True,
                                       EnableSpecularShading = False,
                                       EnableShadows = True,
                                       ShadowMapSize = 4096,
                                       ShadowOffset = 0.0005)

  sun.Transform.value = avango.gua.make_trans_mat(-10.0, 20.0, 40.0) * \
                        avango.gua.make_scale_mat(120, 120, 80) * \
                        avango.gua.make_rot_mat(-25.0, 1.0, 0.0, 0.0)

  head_light = avango.gua.nodes.PointLightNode(Name = "head",
                                       Color = avango.gua.Color(1.0, 0.8, 0.6),
                                       Falloff = 1,
                                       EnableDiffuseShading = True,
                                       EnableSpecularShading = False,
                                       Transform = avango.gua.make_scale_mat(5, 5, 5))

  fake_sun = avango.gua.nodes.SpotLightNode(Name = "fake_sun",
                                            Color = avango.gua.Color(1.2, 0.5, 0.3),
                                            Falloff = 1.0,
                                            Softness = 1.0,
                                            EnableShadows = False,
                                            EnableSpecularShading = False,
                                            EnableDiffuseShading = False,
                                            EnableGodrays = True)

  fake_sun.Transform.value = avango.gua.make_trans_mat(-100, 6, 200) * avango.gua.make_scale_mat(100, 100, 100)

  navigation = avango.gua.nodes.TransformNode(Name = "navigation")
  navigation.Transform.value = avango.gua.make_trans_mat(14.501, 7.142, -9.199)*avango.gua.make_rot_mat(155, 0, 1, 0)

  screen = avango.gua.nodes.ScreenNode(Name = "screen",
                                       Width = 3.0,
                                       Height = 1.97)
  slide_transform = avango.gua.nodes.TransformNode(Name = "slideTransform")
  slide_transform.Transform.value = avango.gua.make_trans_mat(0.0, 1.38, -1.57)*avango.gua.make_trans_mat(0, -1.05, -0.1) * avango.gua.make_rot_mat(-90, 0, 1, 0)

  screen.Transform.value = avango.gua.make_trans_mat(0.0, 1.38, -1.57)

  #view = avango.gua.nodes.ViewNode(Name = "view", StereoWidth = 0.3)
  view = avango.gua.nodes.TransformNode(Name = "view")
  view.Transform.value = avango.gua.make_trans_mat(0.0, 1.57, 0.0)

  ##############################################################################
  ## examples
  ##############################################################################

  # avango example
  avango_example = avango.gua.nodes.TransformNode(Name = "avango_example")
  #avango_example.Transform.value = avango.gua.make_trans_mat(0, 1, -4.2*SLIDE_OFFSET) * avango.gua.make_scale_mat(0.5, 0.5, 0.5)
  avango_example.Transform.value = avango.gua.make_trans_mat(0, 1, -12.2*SLIDE_OFFSET) * avango.gua.make_scale_mat(0.5, 0.5, 0.5)
  monkey = loader.create_geometry_from_file("monkey",
           "data/objects/monkey.obj",
           "CarPaintOrange", avango.gua.LoaderFlags.DEFAULTS)
  avango_example.Children.value = [monkey]

  timer = avango.nodes.TimeSensor()
  timed_rotate = TimedRotate();
  timed_rotate.TimeIn.connect_from(timer.Time)

  # python example
  def make_noon():
    pipe.BackgroundTexture.value = "data/textures/sky2.jpg"
    pipe.AmbientColor.value = avango.gua.Color(0.8, 0.8, 1.0)
    sun.Color.value = avango.gua.Color(2.0, 2.0, 2.5)
    sun.Transform.value = sun.Transform.value = avango.gua.make_trans_mat(-10.0, 40.0, 40.0) * avango.gua.make_scale_mat(120, 120, 80) * avango.gua.make_rot_mat(-45.0, 1.0, 0.0, 0.0)
    fake_sun.Color.value = avango.gua.Color(0, 0, 0)
    avango.gua.set_material_uniform("Water", "skymap", pipe.BackgroundTexture.value)

  def make_evening():
    pipe.BackgroundTexture.value = "data/textures/sky.jpg"
    pipe.AmbientColor.value = avango.gua.Color(0.6, 0.6, 1.0)
    sun.Color.value = avango.gua.Color(1.0, 0.8, 0.6)
    sun.Transform.value = avango.gua.make_trans_mat(-10.0, 20.0, 40.0) * \
                          avango.gua.make_scale_mat(120, 120, 120) * \
                          avango.gua.make_rot_mat(-25.0, 1.0, 0.0, 0.0)
    fake_sun.Color.value = avango.gua.Color(1.2, 0.5, 0.3)
    avango.gua.set_material_uniform("Water", "skymap", pipe.BackgroundTexture.value)

  def make_night():
    pipe.BackgroundTexture.value = "data/textures/sky3.jpg"
    pipe.AmbientColor.value = avango.gua.Color(0.0, 0.0, 0.0)
    sun.Color.value = avango.gua.Color(1.0, 1.0, 3.5)
    sun.Transform.value = avango.gua.make_trans_mat(-10.0, 20.0, 40.0) * avango.gua.make_scale_mat(120, 120, 80) *avango.gua.make_rot_mat(-25.0, 1.0, 0.0, 0.0)
    fake_sun.Color.value = avango.gua.Color(0, 0, 0)
    avango.gua.set_material_uniform("Water", "skymap", pipe.BackgroundTexture.value)

  # material example
  material_example = avango.gua.nodes.TransformNode(Name = "material_example")
  #material_example.Transform.value = avango.gua.make_trans_mat(0, 1, -15*SLIDE_OFFSET) * avango.gua.make_scale_mat(0.5, 0.5, 0.5)
  material_example.Transform.value = avango.gua.make_trans_mat(0, 1, -7*SLIDE_OFFSET) * avango.gua.make_scale_mat(0.5, 0.5, 0.5)

  material_example_group = avango.gua.nodes.TransformNode(Name = "material_example_group")
  material_example_group.Transform.connect_from(timed_rotate.MatrixOut)

  material_example.Children.value = [material_example_group]

  def _display_material(parent, row, column, name):
    column_width = 2.0
    row_width = 2.0

    sphere = loader.create_geometry_from_file(
      name, "data/objects/sphere.obj", name,
      avango.gua.LoaderFlags.DEFAULTS
    )

    plane = loader.create_geometry_from_file(
      name, "data/objects/plane.obj", name,
      avango.gua.LoaderFlags.DEFAULTS
    )

    monkey = loader.create_geometry_from_file(
      name, "data/objects/monkey.obj", name,
      avango.gua.LoaderFlags.DEFAULTS
    )

    sphere.Transform.value = avango.gua.make_trans_mat(row*row_width, 0.0, column*column_width) * \
                             avango.gua.make_scale_mat(0.4, 0.4, 0.4)
    plane.Transform.value =  avango.gua.make_trans_mat(row*row_width, -0.4, column*column_width) * \
                             avango.gua.make_scale_mat(2, 1, 2)
    monkey.Transform.value = avango.gua.make_trans_mat(row*row_width, 0.8, column*column_width) * \
                             avango.gua.make_scale_mat(0.3, 0.3, 0.3)

    parent.Children.value.append(sphere)
    parent.Children.value.append(plane)
    parent.Children.value.append(monkey)

  # physics / lights example
  physics = avango.gua.nodes.Physics()
  physics.add_rigid_body(oilrig_rb)

  light_proxy_size = 0.2
  light_size = 2.0

  avango.gua.create_sphere_shape("sphere", light_proxy_size*0.5)

  def add_lights():

    physics.State.value = avango.gua.PhysicsRunningState.STOPPED

    physics.Gravity.value = avango.gua.Vec3(0, 0, 0)

    loader = avango.gua.nodes.GeometryLoader()

    count = 200

    for x in range(0, count):

      color = avango.gua.Color(1.0, 0.5, 0.5)
      material = "Light3"

      if x%3 == 0:
        color = avango.gua.Color(0.5, 1.0, 0.5)
        material = "Light2"
      elif x%3 == 1:
        color = avango.gua.Color(0.5, 0.5, 1.0)
        material = "Light1"

      light_csn = avango.gua.nodes.CollisionShapeNode(
        Name = "light_csn",
        ShapeName = "sphere",
        Transform = avango.gua.make_identity_mat()
      )

      randdir = avango.gua.Vec3(
        5*(random.random() * 2.0 - 1.0) + 8,
        3*(random.random() * 2.0 - 1.0) + 6,
        5*(random.random() * 2.0 - 1.0) + 0
      )

      sphere_geometry = loader.create_geometry_from_file(
        "sphere",
        "data/objects/light_sphere.obj",
        material,
        avango.gua.LoaderFlags.DEFAULTS
      )
      sphere_geometry.Transform.value = avango.gua.make_scale_mat(light_proxy_size, light_proxy_size, light_proxy_size)

      point_light = avango.gua.nodes.PointLightNode(
        Name = "light",
        Color = color,
        Transform = avango.gua.make_scale_mat(light_size, light_size, light_size)
      )

      light_rb = avango.gua.nodes.RigidBodyNode(
        Name = "light_rb" + str(x),
        Mass = 0.5,
        Friction = 0.6,
        RollingFriction = 0.03,
        AngularDamping = 0.1,
        LinearDamping = 0.1,
        Restitution = 0.08,
        Transform = avango.gua.make_trans_mat(randdir),
        Children = [light_csn, point_light]
      )

      light_csn.Children.value = [sphere_geometry]


      graph.Root.value.Children.value.append(light_rb)
      physics.add_rigid_body(light_rb)

    physics.State.value = avango.gua.PhysicsRunningState.RUNNING


  def drop_lights():
    physics.Gravity.value = avango.gua.Vec3(0, -1.7, 0)


  def _display_materials(parent):

    mats = {
      "SimplePhong" : ["SimplePhongGreen", "SimplePhongRed", "SimplePhongWhite"],
      "Planet" : ["PlanetPink", "PlanetYellow", "PlanetGray"],
      "ComplexPhong" : ["ComplexPhongStone", "ComplexPhongWood", "Tiles"]
    }

    x = 0;
    y = 0;

    for materials in mats.values():
      for material in materials:
        _display_material(parent, x-1, y-1, material);
        y += 1
      x += 1
      y = 0

  _display_materials(material_example_group);

  ##############################################################################
  ## navigation
  ##############################################################################

  slide_transform.Children.value = [slides]
  navigation.Children.value = [screen, view, slide_transform]

  graph.Root.value.Children.value = [oilrig_rb, water, sun, fake_sun, navigation]

  pipe = setup_pipe()

  navigator = examples_common.navigator.Navigator()
  #navigator.Keyboard.set_device_number(0)
  navigator.StartLocation.value = navigation.Transform.value.get_translate()
  navigator.StartRotation.value = avango.gua.Vec2(0, 790)
  navigator.OutTransform.connect_from(navigation.Transform)

  navigator.RotationSpeed.value = 0.2
  navigator.MotionSpeed.value = 0.04

  # enable navigation
  # navigation.Transform.connect_from(navigator.OutTransform)

  slide_switcher = SlideSwitcher()
  slide_switcher.LastSlide.value = 15
  slide_switcher.NextSlide.connect_from(navigator.Keyboard.KeyUp)
  slide_switcher.PreviousSlide.connect_from(navigator.Keyboard.KeyDown)
  slide_switcher.LastSlide.value = 18
  slide_switcher.SlideLocation.value = slide_transform.Transform.value.get_translate()
  slide_switcher.SlideYRotation.value = -90
  slide_switcher.SlideOffset.value = SLIDE_OFFSET

  slide_switcher.OutTransform.connect_from(slide_transform.Transform)
  slide_transform.Transform.connect_from(slide_switcher.OutTransform)

  slide_transform.Children.value.append(avango_example)
  slide_transform.Children.value.append(material_example)

  ##############################################################################
  ## fancy scene setup
  ##############################################################################
  def make_presentation():
    slide_switcher.TransitionSmoothness.value = 0
    slide_switcher.increase_current_slide()

  def make_awesome():
    slide_switcher.SlideLocation.value = avango.gua.Vec3(0, 0, -2.37)
    slide_switcher.TransitionSmoothness.value = 0.99
    pipe.AmbientColor.value = avango.gua.Color(0.6, 0.6, 1.0)
    view.Children.value = [head_light]
    slide_switcher.CurrentSlide.value = 2

  def disable_effects():
    pipe.EnableSsao.value = False
    pipe.EnableBloom.value = False
    sun.EnableShadows.value = False
    fake_sun.EnableGodrays.value = False

  def enable_effects():
    pipe.EnableSsao.value = True
    pipe.EnableBloom.value = True
    sun.EnableShadows.value = True
    fake_sun.EnableGodrays.value = True

  def toggle_ssao():
    pipe.EnableSsao.value = not (pipe.EnableSsao.value)

  def toggle_bloom():
    pipe.EnableBloom.value = not (pipe.EnableBloom.value)

  def toggle_sun_shadows():
    sun.EnableShadows.value = not (sun.EnableShadows.value)

  def toggle_godrays():
    fake_sun.EnableGodrays.value = not (fake_sun.EnableGodrays.value)

  def show_buffers():
    pipe.EnablePreviewDisplay.value = True

  def hide_buffers():
    pipe.EnablePreviewDisplay.value = False

  def next_slide():
    slide_switcher.TransitionSmoothness.value = 0.9
    slide_switcher.increase_current_slide()

  def previous_slide():
    slide_switcher.TransitionSmoothness.value = 0.9
    slide_switcher.decrease_current_slide()

  def goto_slide(i):
    slide_switcher.goto_slide(i)


  water_updater = TimedMaterialUniformUpdate()
  water_updater.MaterialName.value = "Water"
  water_updater.UniformName.value = "time"
  water_updater.TimeIn.connect_from(timer.Time)

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer = avango.gua.nodes.Viewer()
  viewer.Pipelines.value = [pipe]
  viewer.SceneGraphs.value = [graph]
  viewer.Physics.value = physics

  viewer.run()


if __name__ == '__main__':
  start()

