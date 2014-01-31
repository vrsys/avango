
uniform sampler2D seismic;
uniform sampler2D lut;

varying vec2 texture_coordinate;

void main ()
{
	float i = texture2D(seismic, texture_coordinate).r;
    vec2 coord = vec2(i,0);
    gl_FragColor = texture2D(lut, coord);
    
}