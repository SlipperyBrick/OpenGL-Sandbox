#version 450 core  

out vec4 colour;                               

in vec3 vs_position;
in vec2 vs_texcoord;
in vec3 vs_normal; 
in ivec4 vs_boneIDs;
in vec4 vs_weights;
                   
void main() {
   
   colour = vec4(vs_weights);

}