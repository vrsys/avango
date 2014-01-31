
varying vec3 Position;
varying vec3 Normal;
varying vec2 texture_coordinate;

void main( void )
{
    gl_Position    = ftransform();
    
    Position       = (gl_ModelViewMatrix * gl_Vertex).xyz;
    Normal         = gl_NormalMatrix * gl_Normal;
    texture_coordinate = vec2(gl_MultiTexCoord0);
}
