uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;
uniform mat3 normal_matrix;
const vec3 GroundColor=vec3(0.0,1.0,0.0);
const vec3 SkyColor=vec3(0.0,0.0,1.0);
const vec3 LightPosition=vec3(0.0,10.0,0.0);
void main()
{
    vec3 ecPosition = vec3(model_view_matrix * gl_Vertex);
    vec3 tnorm = normalize(normal_matrix * gl_Normal); 
    vec3 lightVec = normalize(LightPosition - ecPosition); 
    float costheta = dot(tnorm, lightVec);
    float a = 0.5 + 0.5 * costheta;
    gl_FrontColor = vec4(mix(GroundColor, SkyColor, a),1.0);
    gl_Position = projection_matrix*model_view_matrix * gl_Vertex;
}