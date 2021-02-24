#version 450                                  

layout (location = 0) in vec3 position; 
layout (location = 1) in vec2 textureCoords; 
layout (location = 2) in vec3 normals; 

out vec3 vs_position;
out vec2 vs_texcoord;
out vec3 vs_normal;            
                                              
uniform mat4 u_Model;                         
uniform mat4 u_Projection;  
uniform mat4 u_View;
                                          
void main()                                   
{       
    vs_position = vec4(u_Model * vec4(position, 1.f)).xyz;
    vs_texcoord = textureCoords;
    vs_normal = mat3(transpose(inverse(u_Model))) * normals;

     gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.f);

}