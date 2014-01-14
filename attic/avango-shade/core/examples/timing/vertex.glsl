varying vec3 obj_0x7d2060_position;
varying vec3 obj_0x7d2060_normal;
varying vec2 obj_0x7d2060_uv;

void main(void)
{
  obj_0x7d2060_position = (gl_ModelViewMatrix * gl_Vertex).xyz;
  obj_0x7d2060_normal = gl_NormalMatrix * gl_Normal;
  obj_0x7d2060_uv = gl_MultiTexCoord0.xy;
  gl_Position = ftransform();
}
