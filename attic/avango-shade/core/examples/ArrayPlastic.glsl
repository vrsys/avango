// SHADE glsl_version:120 GL_EXT_gpu_shader4:require
// ArrayPlastic

void ArrayPlastic_illuminance_impl(ArrayPlastic_SELF, vec3 color, vec3 direction)
{
  float index = IlluminatedMaterial_get_position(self).y * 7;
  vec4 array_color = texture2DArray(texture_unit, vec3(ArrayPlastic_get_uv(self), index));

  vec3 normal = IlluminatedMaterial_get_normal(self);
  vec3 eye = normalize(IlluminatedMaterial_get_eyepoint(self)-IlluminatedMaterial_get_position(self));

  float diffuse_intensity = max(0., dot(direction, normal));
  float specular_intensity = pow(max(0., dot(normal, normalize(direction+eye))), 16.);

  vec4 fragment_color = diffuse_intensity * diffuse * vec4(color, 1.) * array_color;
  fragment_color += specular_intensity * specular * vec4(color, 1.) * vec4(1.);
  IlluminatedMaterial_accum_color(self, fragment_color);
}
