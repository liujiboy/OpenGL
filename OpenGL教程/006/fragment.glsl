uniform sampler2D texture;
varying vec4 texture_coord;
void main()
{
    vec3 color = vec3(texture2D(texture, texture_coord.xy));
    gl_FragColor = vec4(color.x,color.y,color.z,1.0);
}

