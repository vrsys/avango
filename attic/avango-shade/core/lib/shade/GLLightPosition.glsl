// SHADE
// GLLightPosition

vec4 GLLightPosition_get_impl(GLLightPosition_SELF)
{
  return gl_LightSource[index].position;
}
