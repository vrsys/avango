// SHADE
// DirectionalLight

void DirectionalLight_transform_impl(DirectionalLight_SELF, OBJREF coordinate_system)
{
  vec4 direction = ICoordinateSystem_transform(coordinate_system, vec4(DirectionalLight_get_direction(self), 0.));
  DirectionalLight_set_transformed_direction(self, normalize(direction.xyz));
}

void DirectionalLight_illuminate_impl(DirectionalLight_SELF, OBJREF surface, vec3 point_position)
{
  vec3 direction = DirectionalLight_get_transformed_direction(self);
  IlluminatedMaterial_illuminance(surface, DirectionalLight_get_color(self), direction);
}
