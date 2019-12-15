uniform mat4 m;
void main()
{
    gl_Position =m*gl_Vertex;
}