// SHADE
// Surface

void Surface_vertex_impl(Surface_SELF)
{
//  Surface_set_position(self, (gl_ModelViewMatrix * gl_Vertex).xyz);
//  Surface_set_normal(self, gl_NormalMatrix * gl_Normal);
//  Surface_set_uv(self, gl_MultiTexCoord0.xy);
  gl_Position = ftransform();
}

void Surface_geometry_impl(Surface_SELF)
{
  Geometry_enter(geometry);
}

void Surface_fragment_impl(Surface_SELF)
{
  gl_FragColor = IMaterial_shade(material);
}
