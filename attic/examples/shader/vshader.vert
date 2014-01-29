
/* A basic phong shader*/          


varying vec3 Position;
varying vec3 Normal;
   
void main( void )
{
   gl_Position    = ftransform();

   Position       = (gl_ModelViewMatrix * gl_Vertex).xyz;
   Normal         = gl_NormalMatrix * gl_Normal;
}
