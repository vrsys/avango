/* A basic phong shader*/

uniform sampler2D color_map;
uniform float NumLights;
varying vec3 Position;
varying vec3 Normal;
varying vec2 texcoord;

void 
main(void)
{
  vec3 fvNormal         = normalize( Normal );
  vec3 fvViewDirection  = normalize( -Position );

  vec4 fvTotalAmbient = vec4(0.0);
  vec4 fvTotalDiffuse = vec4(0.0);
  vec4 fvTotalSpecular = vec4(0.0);

  // texturing
  int texture_available = 0;
  vec4 texcol            = texture2D( color_map, texcoord );
  float texlightness     = texcol.r*0.3 + texcol.g*0.59 + texcol.b*0.11;
  if(texlightness > 0.0)
  texture_available = 1;

  for(int lightNum = 0; lightNum < int(NumLights); ++lightNum) {

    fvTotalAmbient += gl_FrontMaterial.ambient * gl_LightSource[lightNum].ambient;

    vec3  fvLightDirection = normalize( gl_LightSource[lightNum].position.xyz - Position );
    float fNDotL = dot( fvNormal, fvLightDirection );

    if (fNDotL > 0.0) {
      if (texture_available) {
        fvTotalDiffuse += texcol * gl_LightSource[lightNum].diffuse * fNDotL;
    } else {
      fvTotalDiffuse   += gl_FrontMaterial.diffuse * gl_LightSource[lightNum].diffuse * fNDotL;
    }

    vec3  fvReflection = normalize( ( ( 2.0 * fvNormal ) * fNDotL ) - fvLightDirection );
    float fRDotV       = max( 0.0, dot( fvReflection, fvViewDirection ) );

    fvTotalSpecular    += gl_FrontMaterial.specular
                        * gl_LightSource[int(lightNum)].specular
                        * pow(fRDotV, gl_FrontMaterial.shininess);
    }
  }
  gl_FragColor = ( fvTotalAmbient + fvTotalDiffuse + fvTotalSpecular );
}
