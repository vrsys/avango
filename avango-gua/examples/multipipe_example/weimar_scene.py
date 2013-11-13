import avango
import avango.gua


def create(user_count):

  graph = avango.gua.nodes.SceneGraph(Name = "WeimarScene")

  loader = avango.gua.nodes.GeometryLoader()

  weimar = loader.create_geometry_from_file("weimar",
                                            "/opt/3d_models/architecture/weimar_geometry/weimar_stadtmodell_latest_version/weimar_stadtmodell_final.obj",
                                            "White",
                                             avango.gua.LoaderFlags.DEFAULTS)

  weimar.Transform.value = avango.gua.make_scale_mat(0.1, 0.1, 0.1)

  spot = avango.gua.nodes.SpotLightNode(Name = "sun",
                                        Color = avango.gua.Color(1.0, 1.0, 1.0),
                                        Falloff = 0.009,
                                        Softness = 0.003,
                                        EnableShadows = True,
                                        EnableGodrays = False,
                                        EnableDiffuseShading = True,
                                        EnableSpecularShading = True,
                                        ShadowMapSize = 2048,
                                        ShadowOffset = 0.001)

  spot.Transform.value = avango.gua.make_trans_mat(0.0, 40.0, 40.0) * \
                         avango.gua.make_rot_mat(-45.0, 1.0, 0.0, 0.0) * \
                         avango.gua.make_scale_mat(100.0, 100.0, 160.0)


  screen = avango.gua.nodes.ScreenNode(Name = "screen",
                                       Width = 1.6,
                                       Height = 0.9)

  screen.Transform.value = avango.gua.make_rot_mat(-90.0, 0.0, 1.0, 0.0) * \
                           avango.gua.make_trans_mat(0.0, 3.0, 0.0)

  for i in range(0, user_count):
    head = avango.gua.nodes.TransformNode(Name = "head" + str(i))

    mono_eye = avango.gua.nodes.TransformNode(Name = "mono_eye" + str(i))

    left_eye = avango.gua.nodes.TransformNode(Name = "left_eye" + str(i))
    left_eye.Transform.value = avango.gua.make_trans_mat(-0.05, 0.0, 0.0)

    right_eye = avango.gua.nodes.TransformNode(Name = "right_eye" + str(i))
    right_eye.Transform.value = avango.gua.make_trans_mat(0.05, 0.0, 0.0)

    head.Children.value = [mono_eye, left_eye, right_eye]
    screen.Children.value.append(head)


  graph.Root.value.Children.value = [weimar, screen, spot]
  return graph
