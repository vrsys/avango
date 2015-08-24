import avango

def load_monkey_scene(node):
    loader = avango.gua.nodes.TriMeshLoader()
    monkey1 = loader.create_geometry_from_file(
        "monkey",
        "data/objects/monkey.obj",
        avango.gua.LoaderFlags.NORMALIZE_SCALE
    )
    node.Children.value.append(monkey1)

    light = avango.gua.nodes.LightNode(
        Type=avango.gua.LightType.POINT,
        Name="light",
        Color=avango.gua.Color(1.0, 1.0, 1.0),
        Brightness=100.0,
        Transform=(avango.gua.make_trans_mat(1, 1, 5) *
                   avango.gua.make_scale_mat(30, 30, 30))
        )
    node.Children.value.append(light)
    return avango.gua.Vec3(0.0, 0.0, 5.0)


def load_oilrig_scene(node):
    def load_oilrig(transform, node):
        loader = avango.gua.nodes.TriMeshLoader()
        oilrig = loader.create_geometry_from_file(
            "oilrig",
            "/opt/3d_models/OIL_RIG_GUACAMOLE/oilrig.obj",
            avango.gua.LoaderFlags.NORMALIZE_SCALE |
            avango.gua.LoaderFlags.NORMALIZE_POSITION |
            avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY |
            avango.gua.LoaderFlags.LOAD_MATERIALS |
            avango.gua.LoaderFlags.OPTIMIZE_MATERIALS
        )        
        oilrig.Transform.value = transform * oilrig.Transform.value
        node.Children.value.append(oilrig)

    load_oilrig(
        avango.gua.make_trans_mat(19.9998, 1.1111, 8.8888) * avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * avango.gua.make_scale_mat(0.001),
        node
    ) 
    load_oilrig(
        avango.gua.make_trans_mat(19.998, 1.111, 8.888) * avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * avango.gua.make_scale_mat(0.01),
        node
    ) 
    load_oilrig(
        avango.gua.make_trans_mat(19.98, 1.11, 8.88) * avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * avango.gua.make_scale_mat(0.1),
        node
    )        
    load_oilrig(
        avango.gua.make_trans_mat(19.8, 1.1, 8.8) * avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * avango.gua.make_scale_mat(1),
        node
    )
    load_oilrig(
        avango.gua.make_trans_mat(18.0, 1.0, 8.0) * avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * avango.gua.make_scale_mat(10),
        node
    )
    load_oilrig(
        avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * avango.gua.make_scale_mat(100),
        node
    )
    load_oilrig(
        avango.gua.make_trans_mat(-180.0, -10.0, -80.0) * avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0) * avango.gua.make_scale_mat(1000),
        node
    )

    light = avango.gua.nodes.LightNode(
        Type=avango.gua.LightType.SUN,
        Name="light",
        Color=avango.gua.Color(1.0, 1.0, 1.0),
        Brightness=2.5,
        Transform=avango.gua.make_rot_mat(-50.0, 1.0, -0.9, 0.0)
        )
    node.Children.value.append(light)

    node.Transform.value *= avango.gua.make_rot_mat(-90.0, 0.0, 1.0, 0.0)

    return avango.gua.Vec3(46.0, 45.0, 2718.0)

def load_pitoti_scene(node):
    def load_pitoti(path, node, transform, loader):
        plod = loader.create_geometry_from_file(
            path,
            avango.gua.PLODLoaderFlags.DEFAULTS
        )
        plod.Transform.value = transform
        plod.DisplayBoundingBox.value = True
        node.Children.value.append(plod)

    loader = avango.gua.nodes.PLODLoader()
    loader.UploadBudget.value = 128
    loader.RenderBudget.value = 4096
    loader.OutOfCoreBudget.value = 4096

   # rotate whole scene upright
    _rot_mat = avango.gua.make_identity_mat()
    _rot_mat.set_element(0,0, 0.088)
    _rot_mat.set_element(1,0, 0.016)
    _rot_mat.set_element(2,0, 0.996)
    _rot_mat.set_element(0,1, 0.996)
    _rot_mat.set_element(1,1, 0.005)
    _rot_mat.set_element(2,1, -0.088)
    _rot_mat.set_element(0,2, -0.007)
    _rot_mat.set_element(1,2, 1.0)
    _rot_mat.set_element(2,2, -0.016)

    _offset_mat = _rot_mat * avango.gua.make_trans_mat(-604050.0, -5098490.0, -400.0) 

    # seradina flyover
    _scale = 31.261682663898622
   
    _rot_mat = avango.gua.make_identity_mat()
    _rot_mat.set_element(0,0, -0.57732352)
    _rot_mat.set_element(1,0, 0.816437476)
    _rot_mat.set_element(2,0, 0.0112872)
    _rot_mat.set_element(0,1, 0.040792)
    _rot_mat.set_element(1,1, 0.042645956)
    _rot_mat.set_element(2,1, -0.998257147)
    _rot_mat.set_element(0,2, -0.8154959)
    _rot_mat.set_element(1,2, -0.5758569)
    _rot_mat.set_element(2,2, -0.057924671)
   
    _pos = avango.gua.Vec3(603956.727956973, 5098223.502562742, 819.626837676)
    
    _mat = avango.gua.make_trans_mat(_pos) * _rot_mat * avango.gua.make_scale_mat(_scale)
    _mat = _offset_mat * _mat

    paths = [
        "/mnt/pitoti/3d_pitoti/grundtruth_data/valley/seradina_flyover/seradina_full_las_0_part_00010_knobi_cutout_flagged.kdn",
        "/mnt/pitoti/3d_pitoti/grundtruth_data/valley/seradina_flyover/seradina_full_las_0_part_00011_knobi_cutout_flagged.kdn",

        "/mnt/pitoti/3d_pitoti/grundtruth_data/valley/seradina_flyover/seradina_full_las_0_part_00001_knobi.kdn",
        "/mnt/pitoti/3d_pitoti/grundtruth_data/valley/seradina_flyover/seradina_full_las_0_part_00002_knobi.kdn",
        "/mnt/pitoti/3d_pitoti/grundtruth_data/valley/seradina_flyover/seradina_full_las_0_part_00003_knobi.kdn",
        "/mnt/pitoti/3d_pitoti/grundtruth_data/valley/seradina_flyover/seradina_full_las_0_part_00004_knobi.kdn",
        "/mnt/pitoti/3d_pitoti/grundtruth_data/valley/seradina_flyover/seradina_full_las_0_part_00005_knobi.kdn",
        "/mnt/pitoti/3d_pitoti/grundtruth_data/valley/seradina_flyover/seradina_full_las_0_part_00006_knobi.kdn",
        "/mnt/pitoti/3d_pitoti/grundtruth_data/valley/seradina_flyover/seradina_full_las_0_part_00007_knobi.kdn",
        "/mnt/pitoti/3d_pitoti/grundtruth_data/valley/seradina_flyover/seradina_full_las_0_part_00008_knobi.kdn",
        "/mnt/pitoti/3d_pitoti/grundtruth_data/valley/seradina_flyover/seradina_full_las_0_part_00009_knobi.kdn",
        "/mnt/pitoti/3d_pitoti/grundtruth_data/valley/seradina_flyover/seradina_full_las_0_part_00012_knobi.kdn",
        "/mnt/pitoti/3d_pitoti/grundtruth_data/valley/seradina_flyover/seradina_full_las_0_part_00013_knobi.kdn",
        "/mnt/pitoti/3d_pitoti/grundtruth_data/valley/seradina_flyover/seradina_full_las_0_part_00014_knobi.kdn",
        "/mnt/pitoti/3d_pitoti/grundtruth_data/valley/seradina_flyover/seradina_full_las_0_part_00015_knobi.kdn",
        "/mnt/pitoti/3d_pitoti/grundtruth_data/valley/seradina_flyover/seradina_full_las_0_part_00016_knobi.kdn",
    ]

    for path in paths:
        load_pitoti(path, node, _mat, loader)

    ## seradina 12C
    _mat = avango.gua.make_trans_mat(604050.0, 5098490.0, 400.0)
    _mat = _offset_mat * _mat
    # load_pitoti("/mnt/pitoti/3d_pitoti/grundtruth_data/rocks/seradina12c/TLS_Seradina_Rock-12C_knn_cutout_flagged.kdn", node, _mat, loader)

    ## seradina 12C areas: 1, 2, 3, 4, 5, 6, 10
    paths = [
        "/mnt/pitoti/3d_pitoti/grundtruth_data/rocks/seradina12c/TLS_Seradina_Rock-12C_knn_cutout_flagged.kdn",

        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-1_Warrior-scene_P01-1_knn.kdn",
        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-1_Warrior-scene_P01-2_knn.kdn",
        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-1_Warrior-scene_P01-3_knn.kdn",
        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-1_Warrior-scene_P01-4_knn.kdn",

        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-1_Warrior-scene_P02-1_knn.kdn",
        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-1_Warrior-scene_P02-2_knn.kdn",
        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-1_Warrior-scene_P02-3_knn.kdn",
        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-1_Warrior-scene_P02-4_knn.kdn",

        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-1_Warrior-scene_P03-1_knn.kdn",
        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-1_Warrior-scene_P03-2_knn.kdn",
        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-1_Warrior-scene_P03-3_knn.kdn",
        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-1_Warrior-scene_P03-4_knn.kdn",

        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-2_Plowing-scene_P01-1_knn.kdn",
        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-2_Plowing-scene_P01-2_knn.kdn",
        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-2_Plowing-scene_P01-3_knn.kdn",
        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-2_Plowing-scene_P01-4_knn.kdn",

        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-2_Plowing-scene_P02-1_knn.kdn",
        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-2_Plowing-scene_P02-2_knn.kdn",
        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-2_Plowing-scene_P02-3_knn.kdn",
        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-2_Plowing-scene_P02-4_knn.kdn",

        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area_4_hunter_with_bow_knn.kdn",

        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-5_hunter_with_speer_P01_knn.kdn",
        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-5_hunter_with_speer_P02_knn.kdn",

        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-6_house_P01_knn.kdn",
        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-6_house_P02_knn.kdn",

        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-10_Hunting_Scene_P01_knn.kdn",
        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-10_Hunting_Scene_P02_knn.kdn",
        "/mnt/pitoti/3d_pitoti/seradina_12c/areas/Area-10_Hunting_Scene_P03_knn.kdn",
    ]
    
    for path in paths:
        load_pitoti(path, node, _mat, loader)
    return avango.gua.Vec3(385.0, 59.0, -27.0)

def load_scene(scene_name, node):
    if scene_name == "monkey":
        return load_monkey_scene(node)
    elif scene_name == "oilrigs":
        return load_oilrig_scene(node)
    elif scene_name == "pitoti":
        return load_pitoti_scene(node)
    else:
        print("there is no scene with this name, loading monkey scene..")
        return load_monkey_scene(node)

def get_scene_names():
    return ["monkey","oilrigs","pitoti"]
