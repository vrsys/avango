import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import avango.gua.lod
from examples_common.GuaVE import GuaVE
import examples_common.navigator
from examples_common.GuaVE import GuaVE

def start():

  graph  = avango.gua.nodes.SceneGraph(Name = "scene")
  mesh_loader = avango.gua.nodes.TriMeshLoader()
  
  #configure lod backend
  lod_loader = avango.gua.lod.nodes.LodLoader()
  lod_loader.UploadBudget.value = 64
  lod_loader.RenderBudget.value = 2048
  lod_loader.OutOfCoreBudget.value = 4096

  aux_loader = avango.gua.lod.nodes.Aux()
  aux_loader.load_aux_file("/home/senu8384/Desktop/master-thesis/data/salem.aux");
  aux_loader.test_wrapping()
  aux_loader.get_filename()
  v_num = aux_loader.get_num_views()
  sp_num = aux_loader.get_num_sparse_points()
  num = aux_loader.get_num_atlas_tiles()
  atlas = aux_loader.get_atlas()
  number_octree_nodes = aux_loader.get_num_nodes()

  print(' number of octree nodes: ', number_octree_nodes)
  

  for i in range(number_octree_nodes):
    octree_node = aux_loader.get_octree_node(i)
    ci = octree_node.get_child_idx()
    cm = octree_node.get_child_mask()
    gmin = octree_node.get_min()
    gmax = octree_node.get_max()
    num_fot = octree_node.get_number_fotos()
    print("octree node ", ci , cm, gmin, gmax, num_fot)
    
  octree_node = aux_loader.get_octree_node(number_octree_nodes-1)
  num_fot = octree_node.get_number_fotos()
  for i in range(num_fot):
    print('foto ', octree_node.get_foto_by_id(i))
    gmin = octree_node.get_min()

    
  aa = aux_loader.get_octree_query(avango.gua.Vec3(1.0, 1.0, 1.0))
  print('some octree info ?:', aa)


  print(atlas)
  print(atlas.get_tiles() )
  print(atlas.get_width() )
  print(atlas.get_height() )
  print(atlas.get_rotated() )


  #### TEST get atlas tile worked !
  # for i in range(num):
  #   atlas_tile = aux_loader.get_atlas_tile(i)
  #   tile_id = atlas_tile.get_tile_id()
  #   tile_x = atlas_tile.get_x()
  #   tile_y = atlas_tile.get_y()
  #   tile_w = atlas_tile.get_width()
  #   tile_h = atlas_tile.get_height()
  #   print("out ", i , tile_id, tile_x, tile_y, tile_w, tile_h)
 
  #### TEST get view worked !
  # for i in range(v_num):
  #   view = aux_loader.get_view(i)
  #   cam_id = view.get_camera_id()
  #   pos = view.get_position()
  #   trans = view.get_transform()
  #   focal = view.get_focal_length()
  #   dist = view.get_distortion()
  #   imag_w = view.get_image_width()
  #   imag_h = view.get_image_height()
  #   atlas_t_id = view.get_atlas_tile_id()
  #   image_file = view.get_image_file()
  #   # print("out ", i , cam_id, pos, trans,focal,dist,imag_w, imag_h, atlas_t_id, image_file)
  #   print("out ", i , cam_id,  image_file)

  #### TEST get sparse points worked !
  # for i in range(sp_num):
  #   sp = aux_loader.get_sparse_point(i)
  #   pos = sp.get_pos()
  #   r = sp.r()
  #   g = sp.g()
  #   b = sp.b()
  #   a = sp.a()
    

  # sp = aux_loader.get_sparse_point(1)
  # feature_number = sp.get_num_features()

  # feature_list = []
  # for x in range(feature_number):
  #   feature_list.append(sp.get_feature_by_id(x))

  # for f in feature_list:
  #   c = f.get_camera_id()
  #   coords = f.get_coords()
  #   f_error = f.get_error()
  #   print('feature', c, coords, f_error)
    
   
    #print("out ", i , pos, r, g ,b ,a)
  print("Number of sparse points", sp_num)
  
  fallback_mat = avango.gua.create_material(avango.gua.MaterialCapabilities.COLOR_VALUE)


  
  plod_node = lod_loader.load_lod_pointcloud(
                  #"/opt/3d_models/point_based/plod/pig_pr.bvh",
                  "/home/senu8384/Desktop/master-thesis/data/salem_02.bvh",
                  avango.gua.lod.LoaderFlags.NORMALIZE_SCALE |
                  avango.gua.lod.LoaderFlags.NORMALIZE_POSITION
                  )

  new_cube = mesh_loader.create_geometry_from_file(
    "cube",
    "data/objects/monkey.obj",
    fallback_mat,
    avango.gua.LoaderFlags.DEFAULTS
  )
  
  #plod_node.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 1.0, 1.0, 1.0))
  plod_node.Material.value.set_uniform("Emissivity", 1.0)

  new_cube.Transform.value = avango.gua.make_trans_mat(-1, 0.3, 0) * \
                             avango.gua.make_scale_mat(0.3, 0.3, 0.3)
  
  
  
  plod_node.Transform.value = avango.gua.make_trans_mat(0, 0.3, 0) * \
                              avango.gua.make_scale_mat(0.3, 0.3, 0.3)
  
  graph.Root.value.Children.value.append(plod_node)
  plod_node.ShadowMode.value = 1
  #graph.Root.value.Children.value.append(new_cube)
  #new_cube.ShadowMode.value = 1


  spot_light_1 = avango.gua.nodes.LightNode(Name = "spot_light_1",
                                         Type = avango.gua.LightType.SPOT,
                                         Color = avango.gua.Color(1.0, 1.0, 1.0),
                                         EnableShadows = True,
                                         ShadowMapSize = 1024,
                                         ShadowOffset = 0.002,
                                         ShadowMaxDistance = 10,
                                         Falloff = 1.5,
                                         ShadowNearClippingInSunDirection = 0.1,
                                         ShadowFarClippingInSunDirection = 10.0,
                                         Softness = 2,
                                         Brightness = 20)
  spot_light_1.Transform.value = avango.gua.make_trans_mat(0.0, 3.0, 0.0) * avango.gua.make_rot_mat(-90, 1, 0, 0) * avango.gua.make_scale_mat(4)
  graph.Root.value.Children.value.append(spot_light_1)

  point_light1 = avango.gua.nodes.LightNode(
                                         Type = avango.gua.LightType.POINT,
                                         Name = "point_light1",
                                         Color = avango.gua.Color(0.2, 1.0, 1.7),
                                         EnableShadows = True,
                                         ShadowMapSize = 1024,
                                         ShadowMaxDistance = 10,
                                         ShadowOffset = 0.03,
                                         Falloff = 0.5,
                                         ShadowNearClippingInSunDirection = 0.1,
                                         ShadowFarClippingInSunDirection = 10.0,
                                         Brightness = 20)
                                         
  point_light1.Transform.value = avango.gua.make_trans_mat(1.5, 1.0, 1.5) * avango.gua.make_scale_mat(4)
  graph.Root.value.Children.value.append(point_light1)

  sun_light = avango.gua.nodes.LightNode(Name = "sun_light",
                                         Type = avango.gua.LightType.SUN,
                                         Color = avango.gua.Color(1.0, 1.0, 0.7),
                                         EnableShadows = True,
                                         ShadowMapSize = 1024,
                                         ShadowOffset = 0.0005,
                                         ShadowCascadedSplits = [0.1, 4, 7, 20],
                                         ShadowMaxDistance = 30,
                                         ShadowNearClippingInSunDirection = 100,
                                         ShadowFarClippingInSunDirection = 100,
                                         Brightness = 2
                                         )
  sun_light.Transform.value = avango.gua.make_rot_mat(210, 0, 1, 0) * avango.gua.make_rot_mat(-50.0, 1.0, 0.0, 0.0)
  #graph.Root.value.Children.value.append(sun_light)

  floor = mesh_loader.create_geometry_from_file(
    "floor",
    "data/objects/plane.obj",
    avango.gua.LoaderFlags.DEFAULTS
  )
  
  floor.Transform.value = avango.gua.make_scale_mat(4, 1, 4) * avango.gua.make_trans_mat(0, -0.2, 0)
  floor.ShadowMode.value = 1
  graph.Root.value.Children.value.append(floor)

  width = 1920;
  height = int(width * 9.0 / 16.0)
  size = avango.gua.Vec2ui(width, height)

  screen = avango.gua.nodes.ScreenNode(Name = "screen",
                                       Width = 4.8,
                                       Height = 2.7)

  screen.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -2.5)

  camera = avango.gua.nodes.CameraNode(
    Name = "cam",
    LeftScreenPath = "/cam/screen",
    RightScreenPath = "/cam/screen",
    SceneGraph = "scene",
    Resolution = size,
    OutputWindowName = "window",
    EyeDistance = 0.2,
    EnableStereo = False,
    Children = [screen],
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 2.0)
  )

  res_pass = avango.gua.nodes.ResolvePassDescription()
  res_pass.EnableSSAO.value = False
  res_pass.SSAOIntensity.value = 3.0
  res_pass.SSAOFalloff.value = 20.0
  res_pass.SSAORadius.value = 10.0
  res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1,0.1,0.1)
  res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
  res_pass.Exposure.value = 1.0
  res_pass.BackgroundMode.value = avango.gua.BackgroundMode.CUBEMAP_TEXTURE
  res_pass.BackgroundTexture.value = "awesome_skymap"
  # res_pass.BackgroundColor.value = avango.gua.Color(0.45, 0.5, 0.6)
  res_pass.VignetteColor.value = avango.gua.Vec4(0, 0, 0, 1)

  plod_pass = avango.gua.lod.nodes.PLodPassDescription()
  plod_pass.SurfelRenderMode.value = avango.gua.lod.RenderFlags.HQ_TWO_PASS
  #plod_pass.SurfelRenderMode.value = avango.gua.lod.RenderFlags.LQ_ONE_PASS

  pipeline_description = avango.gua.nodes.PipelineDescription(
    Passes = [
      avango.gua.nodes.TriMeshPassDescription(),
      plod_pass,
      avango.gua.nodes.SkyMapPassDescription(
        OutputTextureName="awesome_skymap"
      ),
      avango.gua.nodes.LightVisibilityPassDescription(),
      res_pass,
      avango.gua.nodes.DebugViewPassDescription()
    ],
    EnableABuffer = False
  )

  camera.PipelineDescription.value = pipeline_description

  graph.Root.value.Children.value.append(camera)

  window = avango.gua.nodes.Window(
    Size = size,
    Title = "shadows",
    LeftResolution = size,
    RightResolution = size,
    EnableVsync = False,
    StereoMode = avango.gua.StereoMode.MONO
  )

  avango.gua.register_window("window", window)

  navigator = examples_common.navigator.Navigator()
  navigator.StartLocation.value = camera.Transform.value.get_translate()
  navigator.OutTransform.connect_from(camera.Transform)

  navigator.RotationSpeed.value = 0.2
  navigator.MotionSpeed.value = 0.04

  camera.Transform.connect_from(navigator.OutTransform)

  viewer = avango.gua.nodes.Viewer()
  # viewer.DesiredFPS.value = 200
  viewer.SceneGraphs.value = [graph]
  viewer.Windows.value = [window]

  timer = avango.nodes.TimeSensor()

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()

