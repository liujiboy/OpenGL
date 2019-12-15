uniform mat4 m;
uniform mat4 p;
varying float x;
void main()
{
    x=gl_Vertex.x;
    gl_Position =p*m*gl_Vertex;
}