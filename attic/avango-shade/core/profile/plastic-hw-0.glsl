uniform vec3 light_pos_0;
varying vec3 trans_light_pos_0;
uniform vec3 light_pos_1;
varying vec3 trans_light_pos_1;
varying vec3 obj_position;
varying vec3 obj_normal;
varying vec3 obj_eyepoint;

void main()
{
  obj_position = gl_Vertex.xyz;
  obj_normal = gl_Normal;
  obj_eyepoint = (gl_ModelViewMatrixInverse * vec4(0., 0., 0., 1.)).xyz;

  trans_light_pos_0 = (gl_ModelViewMatrixInverse * vec4(light_pos_0, 1.)).xyz;
  trans_light_pos_1 = (gl_ModelViewMatrixInverse * vec4(light_pos_1, 1.)).xyz;

  gl_Position = ftransform();
} // main end
