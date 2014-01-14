// SHADE
// Diffuse

void Diffuse_illuminance_impl(Diffuse_SELF, vec3 color, vec3 direction)
{
  vec3 normal = IlluminatedMaterial_get_normal(self);
  float diffuse_intensity = max(0., dot(direction, normal));

  vec4 fragment_color = diffuse_intensity * vec4(color, 1.) * Diffuse_get_color(self);
  IlluminatedMaterial_accum_color(self, fragment_color);
}
