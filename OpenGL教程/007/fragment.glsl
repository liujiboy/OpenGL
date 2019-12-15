uniform sampler2D texture;
uniform sampler2D texture2;
varying vec4 texture_coord;
void main()
{
    vec3 color1 = vec3(texture2D(texture, texture_coord.xy));
    vec3 color2 = vec3(texture2D(texture2, texture_coord.xy));
    vec3 color  = color1*0.6+color2*0.4;
    gl_FragColor = vec4(color.x,color.y,color.z,1.0);
}

