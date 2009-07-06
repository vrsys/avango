



varying vec3 position_es;
varying vec3 normal_es;



void main()
{
  gl_Position = ftransform();


  position_es = (gl_ModelViewMatrix * gl_Vertex).xyz;
  normal_es = gl_NormalMatrix * gl_Normal;

}
