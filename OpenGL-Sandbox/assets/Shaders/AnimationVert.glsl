#version 450 core

layout (location = 0) in vec3 position; 
layout (location = 1) in vec2 textureCoords; 
layout (location = 2) in vec3 normals; 
layout (location = 3) in ivec4 boneIds; 
layout (location = 4) in vec4 weights;

out vec3 vs_position;
out vec2 vs_texcoord;
out vec3 vs_normal;   
out ivec4 vs_boneIDs;
out vec4 vs_weights;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 finalBonesMatrices[MAX_BONES];    
    
uniform mat4 u_Model;                         
uniform mat4 u_Projection;  
uniform mat4 u_View;
    
void main()                                   
{       

   // vec4 totalPosition = vec4(0.0f);
   // for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
   // {
   //     if(boneIds[i] == -1) 
   //         continue;
   //     if(boneIds[i] >= MAX_BONES) 
   //     {
   //         totalPosition = vec4(position,1.0f);
   //         break;
   //     }
   //     vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(position,1.0f);
   //     totalPosition += localPosition * weights[i];
   //     vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * normals;
   //}

   // Multiply each bone transformation by the particular weight
	// and combine them. 
   	mat4 BoneTransform = finalBonesMatrices[ boneIds[0] ] * weights[0];
	BoneTransform +=     finalBonesMatrices[ boneIds[1] ] * weights[1];
    BoneTransform +=     finalBonesMatrices[ boneIds[2] ] * weights[2];
    BoneTransform +=     finalBonesMatrices[ boneIds[3] ] * weights[3];

	// Transformed vertex position 
	vec4 tPos = BoneTransform * vec4(position, 1.0);
	vec4 tNormal = BoneTransform * vec4(normals, 0.0);

    vs_position = vec4(u_Model * tPos).xyz;
    vs_texcoord = textureCoords;
    vs_normal = mat3(transpose(inverse(u_Model))) * normals;
    vs_boneIDs = boneIds;
    vs_weights = weights;

    gl_Position = u_Projection * u_View * u_Model * tPos;

}