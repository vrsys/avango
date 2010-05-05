// SHADE
// UVPointLight

void UVPointLight_transform_impl(UVPointLight_SELF, OBJREF coordinate_system)
{
  vec4 pos = ICoordinateSystem_transform(coordinate_system, vec4(position, 1.));
  UVPointLight_set_transformed_position(self, pos.xyz);
}

void UVPointLight_illuminate_impl(UVPointLight_SELF, OBJREF surface, vec3 point_position)
{
  vec3 pos = UVPointLight_get_transformed_position(self);
  vec3 direction = normalize(pos - point_position);
  UVPlastic_illuminance_uv(surface, color, direction); // FIXME use proper interface
}
