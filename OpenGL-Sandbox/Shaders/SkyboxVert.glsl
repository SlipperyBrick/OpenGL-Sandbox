#version 450                                  

layout (location = 0) in vec3 position; 

out vec3 vs_texcoord;
                                                                       
uniform mat4 u_projectionMatrix;  
uniform mat4 u_viewMatrix;
                                          
void main()                                   
{       

  

    vs_texcoord = position;
    gl_Position =   vec4(u_projectionMatrix * u_viewMatrix * vec4(position, 1.0)).xyww;
}