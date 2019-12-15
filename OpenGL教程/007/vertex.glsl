uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;
varying vec4 texture_coord;
void main()
{
    texture_coord=gl_MultiTexCoord0;
    gl_Position = projection_matrix*model_view_matrix * gl_Vertex;
}