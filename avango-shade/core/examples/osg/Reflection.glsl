// SHADE
// Reflection

vec4 Reflection_shade_impl(Reflection_SELF)
{
  vec3 normal = ICoordinateSystem_get_transformed_normal(coordinate_system);

  vec3 position = Gettable_vec3_get(coordinate_system);
  vec3 eyepoint = ICoordinateSystem_get_transformed_eyepoint(coordinate_system);

  return textureCube(cube_map, reflect(position-eyepoint, normal));
}
