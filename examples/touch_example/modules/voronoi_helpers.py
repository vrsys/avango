# import guacamole libraries
import avango
import avango.gua

# returns index of centroid which is closest to the given target
def get_closest(target, centroids):
  closest = 0
  dist = target.distance_to(centroids[0])

  for i in range(1, len(centroids)):
    new_dist = target.distance_to(centroids[i])

    if (new_dist < dist):
      dist = new_dist
      closest = i

  return closest

# returns min and max points of an axis aligned bounding box of the voronoi cell
# around the given centroid in regard to the other centroids clipped to the
# given maximum bounding box
def get_cell_bbox(min, max, centroid, other_centroids):

  #def intersect_rays(a_origin, delta_a, b, delta_b)

  min = avango.gua.Vec2(min)
  max = avango.gua.Vec2(max)

  for other_centroid in other_centroids:
    mid = (other_centroid + centroid)/2
    spilt_direction = avango.gua.Vec2(
      other_centroid.y - centroid.y,
      centroid.x - other_centroid.x
    )

    #y_min = mid.x +
    #y_max = mid.x
    #y = mid + x*spilt_direction

  return min, max



