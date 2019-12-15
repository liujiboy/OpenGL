uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;
uniform mat3 normal_matrix;
const vec3 GroundColor=vec3(1.0,1.0,1.0);
const vec3 SkyColor=vec3(0.5,0.5,0.5);
const vec3 LightPosition=vec3(0.0,10.0,0.0);
attribute vec3 vertex_position;
attribute vec2 vertex_texture_coord;
attribute vec3 vertex_normal;
varying vec2 texture_coord;
void main()
{
    vec3 ecPosition = vec3(model_view_matrix * vec4(vertex_position,1.0));
    vec3 tnorm = normalize(normal_matrix * vertex_normal); 
    vec3 lightVec = normalize(LightPosition - ecPosition); 
    float costheta = dot(tnorm, lightVec);
    float a = 0.5 + 0.5 * costheta;
    gl_FrontColor = vec4(mix(GroundColor, SkyColor, a),1.0);
    gl_Position = projection_matrix*model_view_matrix * vec4(vertex_position,1.0);
    texture_coord=vertex_texture_coord;
}