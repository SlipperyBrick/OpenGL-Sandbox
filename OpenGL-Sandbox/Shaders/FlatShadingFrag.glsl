#version 450

out vec4 colour;                               

in vec3 vs_position;
in vec2 vs_texcoord;
in vec3 vs_normal;

void main() {

   colour = vec4(vs_position, 1.f);

}