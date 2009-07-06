uniform vec4 diffuse;

varying vec3 position_es;
varying vec3 normal_es;




void main()
{

  vec3 vectoeye = normalize(-position_es);
  vec3 normal   = normalize(normal_es);

  float eye_contribution = max(0.0, dot(vectoeye,normal));


  vec4 color = eye_contribution * diffuse;


  gl_FragColor = color;
}
