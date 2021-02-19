#version 450                                   

const float PI = 3.14159265359;
  
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

uniform PointLight pointLight[4];

uniform sampler2D u_AlbedoTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_RoughnessTexture;
uniform sampler2D u_AOTexture;
uniform sampler2D u_MetallicTexture;

vec3 u_albedo;
float u_metallic;  
float u_roughness;
float u_ao;

vec3 getNormalFromMap() {
    vec3 tangentNormal = texture(u_NormalTexture, vs_texcoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(vs_position);
    vec3 Q2  = dFdy(vs_position);
    vec2 st1 = dFdx(vs_texcoord);
    vec2 st2 = dFdy(vs_texcoord);

    vec3 N   = normalize(vs_normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {

    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {

    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
} 

void main() {   

    u_albedo = pow(texture(u_AlbedoTexture, vs_texcoord).rgb, vec3(2.2));
    vec3 normal = getNormalFromMap();
    u_metallic  = texture(u_MetallicTexture, vs_texcoord).r;
    u_roughness = texture(u_RoughnessTexture, vs_texcoord).r;
    u_ao = texture(u_AOTexture, vs_texcoord).r;

    vec3 N = normalize(vs_normal);
    vec3 V = normalize(u_cameraPosition - vs_position);

     vec3 F0 = vec3(0.04); 
    F0 = mix(F0, u_albedo, u_metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);

    for(int i = 0; i < 4; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(pointLight[i].m_position - vs_position);
        vec3 H = normalize(V + L);
        float distance    = length(pointLight[i].m_position - vs_position);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     = pointLight[i].m_colour * attenuation;        
        
        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, u_roughness);        
        float G   = GeometrySmith(N, V, L, u_roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - u_metallic;	  
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular     = numerator / max(denominator, 0.001);  
            
        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * u_albedo / PI + specular) * radiance * NdotL; 
    }   

    vec3 ambient = vec3(0.03) * u_albedo * u_ao;
    vec3 color = ambient + Lo;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 

    colour = vec4(color, 1.f);
    
}