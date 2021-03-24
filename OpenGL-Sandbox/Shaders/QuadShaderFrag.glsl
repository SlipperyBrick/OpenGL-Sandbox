#version 450 core  

out vec4 colour;

in vec3 vs_position;
in vec2 vs_texcoord;

uniform sampler2D u_Frame;
uniform vec2 u_Resolution;
uniform float u_Offset;

uniform bool  u_MonochromeToggle;
uniform bool  u_WobbleToggle;
uniform bool  u_BlurToggle;

uniform float u_BlurStrength;

uniform mat3 u_blur;

vec4 ImageKernal(vec2 texcoords) {

	vec4 sum = vec4(0.f);
	vec2 stepSize = u_BlurStrength / (u_Resolution);

	sum += texture2D(u_Frame, vec2(texcoords.x - stepSize.x, texcoords.y - stepSize.y))   * u_blur[0][0];
    sum += texture2D(u_Frame, vec2(texcoords.x, texcoords.y - stepSize.y))                * u_blur[0][1];
    sum += texture2D(u_Frame, vec2(texcoords.x + stepSize.x, texcoords.y - stepSize.y))   * u_blur[0][2];

	sum += texture2D(u_Frame, vec2(texcoords.x - stepSize.x, texcoords.y))                * u_blur[1][0];
    sum += texture2D(u_Frame, vec2(texcoords.x, texcoords.y))                             * u_blur[1][1];
    sum += texture2D(u_Frame, vec2(texcoords.x + stepSize.x, texcoords.y))                * u_blur[1][2];

    sum += texture2D(u_Frame, vec2(texcoords.x - stepSize.x, texcoords.y + stepSize.y))   * u_blur[2][0];
    sum += texture2D(u_Frame, vec2(texcoords.x, texcoords.y + stepSize.y))                * u_blur[2][1];
    sum += texture2D(u_Frame, vec2(texcoords.x + stepSize.x, texcoords.y + stepSize.y))   * u_blur[2][2];

	sum.a = 1.0;

	return sum;
}

vec4 Monochrome() {

	vec3 avg = vec3((colour.x + colour.y + colour.z) / 3);
	return vec4(avg, 1.f);

}

vec2 Wobble() {
	vec2 texcoord = vs_texcoord;
    texcoord.x += sin(texcoord.y * 6*2*3.14159 + u_Offset) / 100;
	//texcoord.y += cos(texcoord.x * 6*2*3.14159 + u_Offset) / 100;
	return texcoord;
}

void main() {

	
	vec2 texCoords;

	if(u_WobbleToggle) {
		texCoords = Wobble();
	} else {
		texCoords = vs_texcoord;
	}

	colour = texture(u_Frame, texCoords);

	if(u_BlurToggle){
		colour = ImageKernal(texCoords);
	}

	if(u_MonochromeToggle){
		colour = Monochrome();
	}




}