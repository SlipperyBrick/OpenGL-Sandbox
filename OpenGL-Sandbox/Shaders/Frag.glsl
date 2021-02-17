#version 450                                   

struct PointLight{
    vec3 m_position;
    vec3 m_colour;
    float m_intensity;
    float m_constant;
    float m_linear;
    float m_quadratic;
};

out vec4 colour;                               

in vec3 vs_position;
in vec2 vs_texcoord;
in vec3 vs_normal;      

uniform vec3 u_cameraPosition;

uniform PointLight pointLight;

uniform vec3 u_lightAmbient;
uniform vec3 u_lightDiffuse;
uniform vec3 u_lightSpecular;
   
uniform sampler2D u_AlbedoTexture;
uniform sampler2D u_AOTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_RoughnessTexture;
   
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {

    vec3 lightDir = normalize(light.m_position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), light.m_linear);
     
    // attenuation
    float distance    = length(light.m_position - fragPos);
    float attenuation = 1.f / (distance * distance);   
    
    // combine results
    vec3 ambient  = light.m_colour *        vec3(texture(u_AlbedoTexture, vs_texcoord));
    vec3 diffuse  = light.m_colour * diff * vec3(texture(u_AlbedoTexture, vs_texcoord)) * vec3(texture(u_AOTexture, vs_texcoord));
    vec3 specular = light.m_colour * spec * vec3(texture(u_AlbedoTexture, vs_texcoord)) * vec3(texture(u_RoughnessTexture, vs_texcoord));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular) * pointLight.m_intensity;
} 

void main() {   
    // properties
    vec3 normal = vs_normal + texture(u_NormalTexture, vs_texcoord).rgb;
    vec3 norm = normalize(normal); 
    vec3 viewDir = normalize(u_cameraPosition - vs_position);

    colour = vec4(CalcPointLight(pointLight, norm, vs_position, viewDir), 1.f);
}