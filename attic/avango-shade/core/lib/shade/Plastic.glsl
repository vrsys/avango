// SHADE
// Plastic

void Plastic_illuminance_impl(Plastic_SELF, vec3 color, vec3 direction)
{
  vec3 normal = IlluminatedMaterial_get_normal(self);
  vec3 eye = normalize(IlluminatedMaterial_get_eyepoint(self)-IlluminatedMaterial_get_position(self));
  
  float diffuse_intensity = dot(direction, normal);
  //if (diffuse_intensity <= 0.)
  //  return;
  if (diffuse_intensity > 0.)
  {
    float specular_intensity = pow(max(0., dot(normal, normalize(direction+eye))), Plastic_get_roughness(self));

    vec4 fragment_color = diffuse_intensity * Plastic_get_diffuse(self) * vec4(color, 1.) * Plastic_get_color(self);
    fragment_color += specular_intensity * Plastic_get_specular(self) * vec4(color, 1.) * vec4(1.);
    IlluminatedMaterial_accum_color(self, fragment_color);
  }
}
