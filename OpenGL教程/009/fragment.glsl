varying vec2 texture_coord;
uniform sampler2D texture;
void main()
{
    gl_FragColor = gl_Color*0.2+0.8*texture2D(texture,texture_coord.xy);
}

