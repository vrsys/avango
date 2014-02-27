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


