// SHADE
// ShadowPointLight

void ShadowPointLight_transform_impl(ShadowPointLight_SELF, OBJREF coordinate_system)
{
  vec4 pos = ICoordinateSystem_transform(coordinate_system, vec4(ShadowPointLight_get_position(self), 1.));
  ShadowPointLight_set_transformed_position(self, pos.xyz);

  vec4 surface_pos = modelview * gl_Vertex;
  ShadowPointLight_set_transformed_surface_position(self, surface_pos);
}

void ShadowPointLight_illuminate_impl(ShadowPointLight_SELF, OBJREF surface, vec3 point_position)
{
  vec3 pos = ShadowPointLight_get_transformed_position(self);
  vec3 direction = normalize(pos - point_position);

  // TODO transformation vom [-1,1] range to [0,1] range should done with the modelview matrix
  vec4 surf = ShadowPointLight_get_transformed_surface_position(self);
  vec4 surf_proj = surf * vec4(0.5, 0.5, 0.5, 1.) + surf.w * vec4(0.5, 0.5, 0.5, 0.);
  vec3 shadowed_color = ShadowPointLight_get_color(self) * shadow2DProj(shadow_map, surf_proj).a;

  IlluminatedMaterial_illuminance(surface, shadowed_color, direction);
}
