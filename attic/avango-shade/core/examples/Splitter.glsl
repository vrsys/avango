// SHADE glsl_version:120

void Splitter_enter_impl(Splitter_SELF)
{
  float offset = 1.2;

  gl_Position = gl_PositionIn[0] + vec4(-offset, 0., 0., 0.);
  Geometry_set_uvw(self, vec3(1., 0., 0.));
  shade_emit_vertex();

  gl_Position = gl_PositionIn[1] + vec4(-offset, 0., 0., 0.);
  Geometry_set_uvw(self, vec3(0., 1., 0.));
  shade_emit_vertex();

  gl_Position = gl_PositionIn[2] + vec4(-offset, 0., 0., 0.);
  Geometry_set_uvw(self, vec3(0., 0., 1.));
  shade_emit_vertex();

  EndPrimitive();

  gl_Position = gl_PositionIn[0] + vec4(offset, 0., 0., 0.);
  Geometry_set_uvw(self, vec3(1., 0., 0.));
  shade_emit_vertex();

  gl_Position = gl_PositionIn[1] + vec4(offset, 0., 0., 0.);
  Geometry_set_uvw(self, vec3(0., 1., 0.));
  shade_emit_vertex();

  gl_Position = gl_PositionIn[2] + vec4(offset, 0., 0., 0.);
  Geometry_set_uvw(self, vec3(0., 0., 1.));
  shade_emit_vertex();

  EndPrimitive();

  gl_Position = gl_PositionIn[0] + vec4(0., -offset, 0., 0.);
  Geometry_set_uvw(self, vec3(1., 0., 0.));
  shade_emit_vertex();

  gl_Position = gl_PositionIn[1] + vec4(0., -offset, 0., 0.);
  Geometry_set_uvw(self, vec3(0., 1., 0.));
  shade_emit_vertex();

  gl_Position = gl_PositionIn[2] + vec4(0., -offset, 0., 0.);
  Geometry_set_uvw(self, vec3(0., 0., 1.));
  shade_emit_vertex();

  EndPrimitive();

  gl_Position = gl_PositionIn[0] + vec4(0., offset, 0., 0.);
  Geometry_set_uvw(self, vec3(1., 0., 0.));
  shade_emit_vertex();

  gl_Position = gl_PositionIn[1] + vec4(0., offset, 0., 0.);
  Geometry_set_uvw(self, vec3(0., 1., 0.));
  shade_emit_vertex();

  gl_Position = gl_PositionIn[2] + vec4(0., offset, 0., 0.);
  Geometry_set_uvw(self, vec3(0., 0., 1.));
  shade_emit_vertex();

  return;
}
