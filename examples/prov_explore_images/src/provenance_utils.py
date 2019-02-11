import math

def distance(pos1, pos2):
	d = pos2 - pos1
	return d.length()

    # if contains_flag:
    #   mat = avango.gua.nodes.Material()

    #   col = avango.gua.Vec4(0.2, 1.0, 0.0, 1.0)
    #   mat.set_uniform("Color", col)

    #   mat.set_uniform("Roughness", 1.0)
    #   mat.set_uniform("Emissivity", 1.0)
    #   mat.set_uniform("Metalness", 0.0)
      
    #   if self.indicator == None:
    #     loader = avango.gua.nodes.TriMeshLoader()
    #     self.indicator = loader.create_geometry_from_file("boob_", "data/objects/sphere.obj")
    #     self.indicator.Material.value = mat
    #     self.indicator.Transform.value = avango.gua.make_scale_mat(0.05, 0.05, 0.05)

    #     self.tracked_node.Children.value.append(self.indicator)
    #   else:
    #     self.indicator.Material.value = mat
    # if contains_flag is False:
    #   if self.indicator:
    #     mat = avango.gua.nodes.Material()
    #     col = avango.gua.Vec4(1.0, 0.0, 0.0, 1.0)
    #     mat.set_uniform("Color", col)
    #     self.indicator.Material.value = mat

         
    # distance = 10000.0
    # angle = 1000.0
    # closest_id = None
    # image_pos = None
    # best_size = 3
    # best_distance_ids = []
    # best_angle_ids = []
    # best_mix_ids = []

    # go through all views and find localized image
    # for i_id, img in enumerate(self.localized_image_list):
    #     new_distance = pu.distance(img.position, pos)
    #     # new_angle = math.cos(_abs_dir.x*img.direction.x + _abs_dir.y*img.direction.y + _abs_dir.z*img.direction.z)
    #     # print(new_distance)
    #     if new_distance < distance:
    #         distance = new_distance
    #         if len(best_distance_ids) > 3:
    #             del best_distance_ids[-1]
    #             e = (i_id, new_distance)
    #             best_distance_ids.append(e)
    #             best_distance_ids.sort(key=lambda tup: tup[1])
    #             distance = best_distance_ids[-1][1]
    #         else:
    #             best_distance_ids.append((i_id, new_distance))
    #             best_distance_ids.sort(key=lambda tup: tup[1])
    #             distance = best_distance_ids[-1][1]

    # for tu in best_distance_ids:
    #     img_id = tu[0]
    #     img = self.localized_image_list[img_id]
    #     new_angle = math.cos(_abs_dir.x*img.direction.x + _abs_dir.y*img.direction.y + _abs_dir.z*img.direction.z)
    #     if new_angle < angle:
    #         angle = new_angle
    #         closest_id = img_id
    # closest_id = 0

    # if distance > new_distance and new_angle < angle:
    #     best_mix_ids.append(i_id)
    #     closest_id = i_id

    # if new_distance < distance:
    #     distance = new_distance
    #     best_distance_ids.append((i_id, new_distance))

    # if new_angle < angle:
    #     angle = new_angle
    #     best_angle_ids.append((i_id, new_angle))

    # print('@@@@@@@@@@@')
    # print('mix len', best_mix_ids)
    # print('dist len', best_distance_ids)
    # print('angl len', best_angle_ids)

    # closest_id = i_id

    # for i_id, img in enumerate(self.localized_image_list):
    #     # print(img.direction)
    #     # print(_abs_dir)
    #     # new_angle = math.acos(_abs_dir*img.direction)
    #     new_angle = math.acos(_abs_dir.x*img.direction.x + 
    #       _abs_dir.y*img.direction.y +
    #       _abs_dir.z*img.direction.z)

    #     if new_angle < angle:
    #         angle = new_angle
    #         print(angle)
    # _vec1.normalize()
    # _vec2.normalize()
    # axis = vec1.cross(vec2)
    # angle = math.acos(vec1*vec2)