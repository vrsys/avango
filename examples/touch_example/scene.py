import avango
import avango.gua

CUBE_COUNT_X = 10
CUBE_COUNT_Y = 3
CUBE_COUNT_Z = 1

def create():
  # setup scenegraph
  graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")

  light = avango.gua.nodes.SunLightNode(
    Name = "light",
    Color = avango.gua.Color(1.0, 1.0, 1.0),
    Transform = avango.gua.make_rot_mat(-90, 1, 0, 0)
  )
  graph.Root.value.Children.value.append(light)

  loader = avango.gua.nodes.TriMeshLoader()
  for x in range(0, CUBE_COUNT_X):
    for y in range(0, CUBE_COUNT_Y):
      for z in range(0, CUBE_COUNT_Z):
        new_cube = loader.create_geometry_from_file("cube" + str(x) + str(y) + str(z),
                  "data/objects/monkey.obj",
                  "data/materials/White.gmd",
                  avango.gua.LoaderFlags.DEFAULTS)

        new_cube.Transform.value = avango.gua.make_trans_mat((x - CUBE_COUNT_X/2)*2, (y - CUBE_COUNT_Y/2)*2, z*2) * \
                                   avango.gua.make_scale_mat(0.3, 0.3, 0.3)
        graph.Root.value.Children.value.append(new_cube)

  return graph
