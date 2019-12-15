uniform mat4 m;
uniform mat4 p;
void main()
{
    gl_Position =p*m*gl_Vertex;
}