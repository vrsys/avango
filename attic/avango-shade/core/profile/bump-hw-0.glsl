uniform vec3 light_pos_0;
varying vec3 trans_light_pos_0;
uniform vec3 light_pos_1;
varying vec3 trans_light_pos_1;
varying vec3 obj_position;
varying vec3 obj_normal;
varying vec3 obj_eyepoint;
varying vec2 obj_texco;

void main()
{
  obj_position = gl_Vertex.xyz;
  obj_texco = gl_MultiTexCoord0.xy;
  vec3 obj_tangent = gl_MultiTexCoord1.xyz;
  vec3 obj_bitangent = gl_MultiTexCoord2.xyz;
  vec3 obj_normal = gl_Normal;

  vec4 v_object = gl_ModelViewMatrixInverse * vec4(0., 0., 0., 1.) - gl_Vertex;
  obj_eyepoint.x = dot(v_object.xyz, obj_tangent);
  obj_eyepoint.y = dot(v_object.xyz, obj_bitangent);
  obj_eyepoint.z = dot(v_object.xyz, obj_normal);

  v_object = gl_ModelViewMatrixInverse * vec4(light_pos_0, 1.) - gl_Vertex;
  trans_light_pos_0.x = dot(v_object.xyz, obj_tangent);
  trans_light_pos_0.y = dot(v_object.xyz, obj_bitangent);
  trans_light_pos_0.z = dot(v_object.xyz, obj_normal);

  v_object = gl_ModelViewMatrixInverse * vec4(light_pos_1, 1.) - gl_Vertex;
  trans_light_pos_1.x = dot(v_object.xyz, obj_tangent);
  trans_light_pos_1.y = dot(v_object.xyz, obj_bitangent);
  trans_light_pos_1.z = dot(v_object.xyz, obj_normal);

  gl_Position = ftransform();
} // main end
