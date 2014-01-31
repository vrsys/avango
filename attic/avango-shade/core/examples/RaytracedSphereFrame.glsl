// SHADE
// RaytracedSphereFrame

vec4 RaytracedSphereFrame_transform_impl(RaytracedSphereFrame_SELF, vec4 v)
{
  return gl_ModelViewMatrixInverse * v;
}

void RaytracedSphereFrame_init_vertex_impl(RaytracedSphereFrame_SELF)
{
  RaytracedSphereFrame_set_position(self, gl_Vertex.xyz);
  RaytracedSphereFrame_set_normal(self, gl_Normal);

  vec4 eyepoint = RaytracedSphereFrame_transform_impl(self, vec4(0., 0., 0., 1.));
  RaytracedSphereFrame_set_eyepoint(self, eyepoint.xyz);
}

void RaytracedSphereFrame_init_fragment_impl(RaytracedSphereFrame_SELF)
{
  vec3 origin = RaytracedSphereFrame_get_eyepoint(self);
  vec3 direction = RaytracedSphereFrame_get_position(self) - origin;

  // Formula:
  // dot(direction, direction)*t^2 + 2*dot(origin, direction)*t + dot(origin, origin) - 1 = 0

  float scale = dot(direction, direction);
  float p = 2. * dot(origin, direction) / scale; 
  float q = (dot(origin, origin) - 1.) / scale;
  float discriminant = p*p/4. - q;

  if (discriminant < 0.)
    discard;

  float minus_half_p = -1.*p/2.;
  float sqrt_discriminant = sqrt(discriminant);

  float t_hit;
  float t_0 = minus_half_p - sqrt_discriminant;
  if (t_0 < 0.)
  {
    float t_1 = minus_half_p + sqrt_discriminant;
    if (t_1 < 0.)
      discard;
    else
      t_hit = t_1;
  }
  else
    t_hit = t_0;

  RaytracedSphereFrame_set_sphere_position(self, origin + t_hit * direction);
}

vec3 RaytracedSphereFrame_get_transformed_normal_impl(RaytracedSphereFrame_SELF)
{
  return normalize(RaytracedSphereFrame_get_sphere_position(self));
}

vec3 RaytracedSphereFrame_get_transformed_eyepoint_impl(RaytracedSphereFrame_SELF)
{
  return RaytracedSphereFrame_get_eyepoint(self);
}
