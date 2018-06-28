/* A basic phong shader*/          

varying vec3 Position;
varying vec3 Normal;
varying vec2 texcoord;

void main(void)
{
  gl_Position = ftransform();

  Position = (gl_ModelViewMatrix * gl_Vertex).xyz;
  Normal = gl_NormalMatrix * gl_Normal;
  texcoord = gl_MultiTexCoord0.xy;
}
