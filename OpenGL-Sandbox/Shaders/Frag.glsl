#version 450 core                                  

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

uniform samplerCube u_irradianceMap;

//vec3  u_albedo;
uniform float u_metallic;  
uniform float u_roughness;
//uniform float u_ao;

uniform bool u_PBRToggle;

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

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {

    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}   

vec3 CalculatePBR(vec3 albedo, vec3 normal, float metallic, float roughness, float ao) {

    vec3 N = normal;
    vec3 V = normalize(u_cameraPosition - vs_position);
    vec3 R = reflect(-V, N); 

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 4; i++) 
    {
        // calculate per-light radiance
        vec3 L = normalize(pointLight[i].m_position - vs_position);
        vec3 H = normalize(V + L);
        float distance = length(pointLight[i].m_position - vs_position);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance =pointLight[i].m_colour * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);    
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);   //fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness); //     
        
        vec3 nominator    = NDF * G * F;
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = nominator / denominator;
        
         // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	                
            
        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
     // ambient lighting (we now use IBL as the ambient term)
     //vec3 kS = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness); 
     //vec3 kD = 1.0 - kS;
     //vec3 irradiance = texture(u_irradianceMap, N).rgb;
     //vec3 diffuse    = irradiance * albedo;
     //vec3 ambient    = (kD * diffuse) * ao; 
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	
    vec3 irradiance = texture(u_irradianceMap, N).rgb;
    vec3 diffuse      = irradiance * albedo;
    vec3 color = diffuse + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    return color;
} 

void main() {   

    vec3  l_albedo   ;
    vec3  l_normal   ;
    float l_metallic ;
    float l_roughness;
    float l_ao       ;
    
    if(u_PBRToggle){
         l_albedo    = pow(texture(u_AlbedoTexture, vs_texcoord).rgb, vec3(2.2));
         l_normal    = getNormalFromMap();
         l_metallic  = texture(u_MetallicTexture, vs_texcoord).r;
         l_roughness = texture(u_RoughnessTexture, vs_texcoord).r;
         l_ao        = texture(u_AOTexture, vs_texcoord).r;
    } else {
        l_albedo = vec3(.5, 0, 0);
        l_normal = vs_normal;
        l_metallic = u_metallic; 
        l_roughness = u_roughness; 
        l_ao = 0.; 
    }


    colour = vec4(CalculatePBR(l_albedo, l_normal, l_metallic, l_roughness, l_ao), 1.f);
    
}