// SHADE
// Switch

void Switch_vertex_impl(Switch_SELF)
 {
  Switch_set_uv(self, gl_MultiTexCoord0.xy);
  Transform_transform(gl_Color.a);
 }

void Switch_fragment_impl(Switch_SELF)
 {
  gl_FragColor = texture2D(texture_unit, Switch_get_uv(self));
  //gl_FragColor = vec4(1., 1., 0., 1.);
 }


void Transform_transform_impl(Transform_SELF)
 {
  gl_Position = ftransform();
 }


void WaveTransform_transform_impl(WaveTransform_SELF)
 {
  gl_Vertex += vec4(0., value, 0., 0.);
  gl_Position = ftransform();
 }
