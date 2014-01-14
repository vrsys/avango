// SHADE glsl_version:120

void Displacement_init_vertex_impl(Displacement_SELF)
{
  Displacement_set_normal(self, gl_ModelViewProjectionMatrix * vec4(gl_Normal, 0.));
  //Displacement_set_light_normal(self, ICoordinateSystem_transform(coordinate_system, vec4(gl_Normal, 0.)));
  Displacement_set_light_normal(self, vec4(0., 0., 1., 0.));  // FIXME Assume we shade in tangent space ...
  Displacement_set_uvcoord(self, gl_MultiTexCoord0.xy);
}

vec3 Displacement_transform_inverse_impl(Displacement_SELF, vec3 value)
{
  return value - Displacement_get_interpolated_light_normal(self).xyz;
}

void Displacement_interpolate_impl(Displacement_SELF, vec3 factor)
{
  vec2 uvcoord_0 = Displacement_get_uvcoord(shade_get_object(self, 0));
  vec2 uvcoord_1 = Displacement_get_uvcoord(shade_get_object(self, 1));
  vec2 uvcoord_2 = Displacement_get_uvcoord(shade_get_object(self, 2));
  float offset = 0.05 * (texture2D(texture_unit, factor.x*uvcoord_0 + factor.y*uvcoord_1 + factor.z*uvcoord_2).a - 1.);

  vec4 position = factor.x*gl_PositionIn[0] + factor.y*gl_PositionIn[1] + factor.z*gl_PositionIn[2];
  vec4 normal_0 = Displacement_get_normal(shade_get_object(self, 0));
  vec4 normal_1 = Displacement_get_normal(shade_get_object(self, 1));
  vec4 normal_2 = Displacement_get_normal(shade_get_object(self, 2));
  vec4 normal = factor.x*normal_0 + factor.y*normal_1 + factor.z*normal_2;
  gl_Position = position + offset* normal;

  vec4 light_normal_0 = Displacement_get_light_normal(shade_get_object(self, 0));
  vec4 light_normal_1 = Displacement_get_light_normal(shade_get_object(self, 1));
  vec4 light_normal_2 = Displacement_get_light_normal(shade_get_object(self, 2));
  vec4 light_normal = factor.x*light_normal_0 + factor.y*light_normal_1 + factor.z*light_normal_2;
  light_normal *= offset;
  Displacement_set_interpolated_light_normal(self, light_normal);

  Geometry_set_uvw(self, factor);
  shade_emit_vertex();
}

void Displacement_enter_impl(Displacement_SELF)
{
  const int segments = 8;

  float interpolations[segments];
  for (int i = 0; i != segments; ++i)
  {
    interpolations[i] = float(i)/float(segments-1);
  }

  Displacement_interpolate(self, vec3(1., 0., 0.));
  Displacement_interpolate(self, vec3(interpolations[segments-2], interpolations[1], 0.));
  Displacement_interpolate(self, vec3(interpolations[segments-2], 0., interpolations[1]));
  for (int i = 2; i < segments-1; ++i)
  {
    float factor = float(i-1)/float(segments-2);
    Displacement_interpolate(self, vec3((1.-factor)*interpolations[segments-2],
          interpolations[1],
          factor*interpolations[segments-2]));
    Displacement_interpolate(self, vec3(interpolations[segments-i-1], 0., interpolations[i]));
  }
  Displacement_interpolate(self, vec3(0., interpolations[1], interpolations[segments-2]));
  Displacement_interpolate(self, vec3(0., 0., 1.));
  EndPrimitive();

  for (int j = 1; j < segments-2; ++j)
  {
    Displacement_interpolate(self, vec3(interpolations[segments-j-1], interpolations[j], 0.));
    Displacement_interpolate(self, vec3(interpolations[segments-j-2], interpolations[j+1], 0.));

    float factor = 1./float(segments-j-1);
    Displacement_interpolate(self, vec3((1.-factor)*interpolations[segments-j-1],
          interpolations[j],
          factor*interpolations[segments-j-1]));

    for (int i = 2; i < segments-j-1; ++i)
    {
      factor = float(i-1)/float(segments-j-2);
      Displacement_interpolate(self, vec3((1.-factor)*interpolations[segments-j-2],
            interpolations[j+1],
            factor*interpolations[segments-j-2]));

      factor = float(i)/float(segments-j-1);
      Displacement_interpolate(self, vec3((1.-factor)*interpolations[segments-j-1],
            interpolations[j],
            factor*interpolations[segments-j-1]));
    }

    Displacement_interpolate(self, vec3(0., interpolations[j+1], interpolations[segments-j-2]));
    Displacement_interpolate(self, vec3(0., interpolations[j], interpolations[segments-j-1]));

    EndPrimitive();
  }

  Displacement_interpolate(self, vec3(interpolations[1], interpolations[segments-2], 0.));
  Displacement_interpolate(self, vec3(0., 1., 0.));
  Displacement_interpolate(self, vec3(0., interpolations[segments-2], interpolations[1]));
  EndPrimitive();
}
