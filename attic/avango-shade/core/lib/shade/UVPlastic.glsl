// SHADE
// UVPlastic

void UVPlastic_illuminance_impl(UVPlastic_SELF, vec3 color, vec3 direction)
{
  vec3 normal = IlluminatedMaterial_get_normal(self);
  vec3 eye = normalize(IlluminatedMaterial_get_eyepoint(self)-IlluminatedMaterial_get_position(self));
  
  float diffuse_intensity = max(0., dot(direction, normal));
  float specular_intensity = pow(max(0., dot(normal, normalize(direction+eye))), 16.);

  vec4 fragment_color = diffuse_intensity * diffuse * vec4(color, 1.) * UVPlastic_get_color(self);
  fragment_color += specular_intensity * specular * vec4(color, 1.) * vec4(1.);
  IlluminatedMaterial_accum_color(self, fragment_color);
}

void UVPlastic_illuminance_uv_impl(UVPlastic_SELF, vec3 color, vec3 direction)
{
  vec3 normal = IlluminatedMaterial_get_normal(self);
  vec3 eye = normalize(IlluminatedMaterial_get_eyepoint(self)-IlluminatedMaterial_get_position(self));
  
  float diffuse_intensity = max(0., dot(direction, normal));

  vec4 fragment_color = diffuse_intensity * diffuse * vec4(color, 1.) * UVPlastic_get_uv_color(self);
  IlluminatedMaterial_accum_color(self, fragment_color);
}
