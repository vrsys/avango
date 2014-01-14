// SHADE
// PointLight

void PointLight_transform_impl(PointLight_SELF, OBJREF coordinate_system)
{
  vec4 pos = ICoordinateSystem_transform(coordinate_system, vec4(PointLight_get_position(self), 1.));
  PointLight_set_transformed_position(self, pos.xyz);
}

void PointLight_illuminate_impl(PointLight_SELF, OBJREF surface, vec3 point_position)
{
  vec3 pos = PointLight_get_transformed_position(self);
  vec3 direction = normalize(pos - point_position);
  IlluminatedMaterial_illuminance(surface, PointLight_get_color(self), direction);
}
