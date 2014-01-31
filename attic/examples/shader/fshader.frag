
/* A basic phong shader*/


uniform float NumLights;


uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float shininess;


varying vec3 Position;
varying vec3 Normal;

void main( void )
{

   vec3  fvNormal         = normalize( Normal );
   vec3  fvViewDirection  = normalize( -Position );


   vec4  fvTotalAmbient = vec4(0.0);
   vec4  fvTotalDiffuse = vec4(0.0);
   vec4  fvTotalSpecular = vec4(0.0);

   
   for(int lightNum = 0; lightNum  < int(NumLights); ++lightNum)
   {

	fvTotalAmbient         += /*gl_FrontMaterial.*/ambient * gl_LightSource[lightNum].ambient;

	vec3  fvLightDirection = normalize( gl_LightSource[lightNum].position.xyz - Position );
   	float fNDotL           = dot( fvNormal, fvLightDirection );


	if(fNDotL > 0.0){

   	fvTotalDiffuse         += /*gl_FrontMaterial.*/diffuse * gl_LightSource[lightNum].diffuse * fNDotL;



   	vec3  fvReflection     = normalize( ( ( 2.0 * fvNormal ) * fNDotL ) - fvLightDirection );
   	float fRDotV           = max( 0.0, dot( fvReflection, fvViewDirection ) );

   	fvTotalSpecular        += /*gl_FrontMaterial.*/specular
                                    * gl_LightSource[int(lightNum)].specular
                                    * pow(fRDotV, /*gl_FrontMaterial.*/shininess);

        }

   }
   gl_FragColor = ( fvTotalAmbient + fvTotalDiffuse + fvTotalSpecular );

}
