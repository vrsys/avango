import avango
import avango.gua


def create(user_count):

  graph = avango.gua.nodes.SceneGraph(Name = "OilrigScene")

  loader = avango.gua.nodes.GeometryLoader()

  water = loader.create_geometry_from_file("water_geometry",
                                           "data/objects/plane.obj",
                                           "Water",
                                            avango.gua.LoaderFlags.DEFAULTS)

  water.Transform.value = avango.gua.make_trans_mat(0.0, -10.0, 0.0) * \
                          avango.gua.make_scale_mat(1500.0, 1.0, 1500.0)

  water.GroupNames.value = ["no_picking"]

  oilrig = loader.create_geometry_from_file("oilrig_geometry",
                                           "/opt/3d_models/OIL_RIG_GUACAMOLE/oilrig.obj",
                                           "White",
                                            avango.gua.LoaderFlags.LOAD_MATERIALS | \
                                            avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY)

  oilrig.Transform.value = avango.gua.make_trans_mat(1.0, 1.0, 2.0) * \
                           avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * \
                           avango.gua.make_scale_mat(0.05, 0.05, 0.05)

  screen = avango.gua.nodes.ScreenNode(Name = "screen",
                                       Width = 3.0,
                                       Height = 1.97)

  for i in range(0, user_count):
    head = avango.gua.nodes.TransformNode(Name = "head" + str(i))
    head.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 1.7)

    mono_eye = avango.gua.nodes.TransformNode(Name = "mono_eye" + str(i))

    left_eye = avango.gua.nodes.TransformNode(Name = "left_eye" + str(i))
    left_eye.Transform.value = avango.gua.make_trans_mat(-0.05, 0.0, 0.0)

    right_eye = avango.gua.nodes.TransformNode(Name = "right_eye" + str(i))
    right_eye.Transform.value = avango.gua.make_trans_mat(0.05, 0.0, 0.0)

    head.Children.value = [mono_eye, left_eye, right_eye]
    screen.Children.value.append(head)

  spot = avango.gua.nodes.SpotLightNode(Name = "sun",
                                        Color = avango.gua.Color(1.4, 1.4, 1.4),
                                        Falloff = 0.001,
                                        Softness = 0.001,
                                        EnableShadows = True,
                                        EnableGodrays = True,
                                        EnableDiffuseShading = True,
                                        EnableSpecularShading = True,
                                        ShadowMapSize = 2048,
                                        ShadowOffset = 0.0001)

  spot.Transform.value = avango.gua.make_rot_mat(-45.0, 1.0, 0.0, 0.0) * \
                         avango.gua.make_trans_mat(0.0, 0.0, 60.0) * \
                         avango.gua.make_scale_mat(35.0, 35.0, 80.0)

  graph.Root.value.Children.value = [water, oilrig, screen, spot]
  return graph


