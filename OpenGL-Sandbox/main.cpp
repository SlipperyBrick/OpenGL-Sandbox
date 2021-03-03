#include <vector>
#include <future>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"	
#include "GuiLayer.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "Texture.h"
#include "PointLight.h"
#include "Skybox.h"
#include "Timer.hpp"
#include "DirectionalLight.h"

GLfloat quadVerts[] = {
	-1.f,   1.f, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
	-1.f,  -1.f, 0.0f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
	 1.0f, -1.f, 0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
	 1.0f,  1.f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f
};
unsigned int quadIndices[] = {
	0, 1, 2,
	0, 2, 3
};

GLfloat triVert[] = {
	  -1.0f,  0.f, 0.0f,   0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
	  0.0f,   1.f, 0.0f,   0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
	  1.0f,   0.f, 0.0f,   0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
};
unsigned int triIndices[] = {
	0, 1, 2
};

Window window("Window", true, 1460, 768);

Camera camera(glm::vec3(0.f, 0.f, 10.f), glm::vec3(0, 1, 0), 0, 0, 5, 110, 90.f);

Shader shader;
Shader DirShadowShader;

Texture HDRI("Textures/HDRIs/fireplace_8k.hdr");
Texture hdriCubemap;
Texture irradianceCubemap;
Texture prefilterMap;
Texture brdfLUTMap;

Texture Albedo   ("Textures/PBR/Icelandic Cracked Rock_vclnajuew_Surface/Albedo_4K__vclnajuew.jpg");
Texture Normal   ("Textures/PBR/Icelandic Cracked Rock_vclnajuew_Surface/Normal_4K__vclnajuew.jpg");
Texture Roughness("Textures/PBR/Icelandic Cracked Rock_vclnajuew_Surface/Roughness_4K__vclnajuew.jpg");
Texture AO       ("Textures/PBR/Icelandic Cracked Rock_vclnajuew_Surface/AO_4K__vclnajuew.jpg");
Texture Metallic ("Textures/PBR/Icelandic Cracked Rock_vclnajuew_Surface/AO_4K__vclnajuew.jpg");

DirectionalLight DirLight(2048, 2048, { 1, 1, 1 }, .01, { 0.5, -1, 0 });

Mesh obj;
Model shpere;

bool useTexture = true;

void RenderScene(Shader* shader) {

	shader->Set1i(useTexture, "u_usePRB");
	shader->SetMat4f(shpere.GetModel(), "u_Model", false);
	shpere.Render();

	shader->Set1i(0, "u_usePRB");
	obj.SetScale(glm::vec3(50.5f));
	obj.SetTranslate({ 0.f, -2.5f, 0.f });
	obj.SetRotation({ 90.f, 0.f , 0.f });
	shader->SetMat4f(obj.GetModel(), "u_Model", false);
	obj.Render();

}

void DirectionalShadowMapPass() {
	
	DirShadowShader.Bind();
	DirShadowShader.SetMat4f(DirLight.CalculateLightTransform(), "u_DirectionLightTransform", false);

	glViewport(0, 0, DirLight.GetShadowMapPtr()->GetWidth(), DirLight.GetShadowMapPtr()->GetHeight());
	DirLight.GetShadowMapPtr()->BindFBO();	
	glClear(GL_DEPTH_BUFFER_BIT);

	RenderScene(&DirShadowShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
};

int main() {

	GuiLayer GuiLayer(window.GetWindow());

	shader.CreateFromFile("Shaders/Vertex.glsl", "Shaders/Frag.glsl");
	DirShadowShader.CreateFromFile("Shaders/DirectionShadowMapVert.glsl", "Shaders/DirectionShadowMapFrag.glsl");

	HDRI.CreateHDRI();
	hdriCubemap.CreateCubemapFromHDRI(HDRI);
	irradianceCubemap.CreateIrradianceTexture(&hdriCubemap);
	prefilterMap.CreatePrefilterMap(&hdriCubemap);
	brdfLUTMap.CreateBRDFLookUpTable();

	Albedo.CreateTexture2D();
	Normal.CreateTexture2D();
	Roughness.CreateTexture2D();
	AO.CreateTexture2D();
	Metallic.CreateTexture2D();
	
	Skybox skybox(&hdriCubemap);

	std::vector<PointLight> pointlights(1); {
		pointlights[0].SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));
		//pointlights[0].SetPosition(glm::vec3(-10.0f, 10.0f, 10.0f));
		//pointlights[1].SetPosition(glm::vec3(10.0f, 10.0f, 10.0f));
		//pointlights[2].SetPosition(glm::vec3(-10.0f, -10.0f, 10.0f));
		//pointlights[3].SetPosition(glm::vec3(10.0f, -10.0f, 10.0f));

		pointlights[0].SetColour(glm::vec3(1.f, 1.f, 1.f));
		//pointlights[1].SetColour(glm::vec3(1.f, 1.f, 1.f));
		//pointlights[2].SetColour(glm::vec3(1.f, 1.f, 1.f));
		//pointlights[3].SetColour(glm::vec3(1.f, 1.f, 1.f));

		pointlights[0].SetIntensity(300.f);
		//pointlights[1].SetIntensity(300.f);
		//pointlights[2].SetIntensity(300.f);
		//pointlights[3].SetIntensity(300.f);
	}

	obj.Create(quadVerts, quadIndices, 32, 6);
	shpere.Load("Models/Sphere256.fbx");
	shpere.Create();

	shader.Bind();

	Albedo.Bind(0);
	Normal.Bind(1);
	Roughness.Bind(2);
	AO.Bind(3);
	Metallic.Bind(4);
	irradianceCubemap.Bind(5);
	prefilterMap.Bind(6);
	brdfLUTMap.Bind(7);
	DirLight.GetShadowMapPtr()->BindTexture(8);

	shader.Set1i(0, "u_AlbedoTexture");
	shader.Set1i(1, "u_NormalTexture");
	shader.Set1i(2, "u_RoughnessTexture");
	shader.Set1i(3, "u_AOTexture");
	shader.Set1i(4, "u_MetallicTexture");
	shader.Set1i(5, "u_irradianceMap");
	shader.Set1i(6, "u_prefilterMap");
	shader.Set1i(7, "u_brdfLUT");
	shader.Set1i(8, "u_directionalShadowMap");

	// render light source (simply re-render sphere at light positions)
	// this looks a bit off as we use the same shader, but it'll make their positions obvious and 
	// keeps the codeprint small.
	
	for (unsigned int i = 0; i < 1; i++)
	{
		glm::vec3 newPos = pointlights[i].GetPosition() + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
		newPos = pointlights[i].GetPosition();
		std::string pos = "pointLight[" + std::to_string(i) + "].m_position";
		shader.SetVec3f(newPos, pos.c_str());
		std::string col = "pointLight[" + std::to_string(i) + "].m_colour";
		shader.SetVec4f(glm::vec4(pointlights[i].GetColour(), pointlights[i].GetIntensity()), col.c_str());
	}
	
	shader.Unbind();

	int nrRows = 5;
	int nrColumns = 5;
	float spacing = 2.5;
	

	while (window.IsOpen()) {

		DirectionalShadowMapPass();
		
		window.Update();
		if (window.UpdateOnFocus()) {
			camera.mouseControl(window.GetXChange(), window.GetYChange(), window.GetDeltaTime());
			camera.keyControl(window.GetsKeys(), window.GetDeltaTime());
		}

		// GUI //
		{
			GuiLayer.Begin();

			ImGui::Begin("GUI");
			ImGui::Text("Application average %.2f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Checkbox("Use PBR Textures", &useTexture);
			ImGui::ColorEdit3("", (float*)pointlights[0].GetColourPtr());
			ImGui::SliderFloat("Light Intensity", pointlights[0].GetIntensityPtr(), 0.f, 100000.f);
			ImGui::SliderFloat("Direction Light Intensity", DirLight.GetIntensityPtr(), 0.f, 1.f);
			ImGui::DragFloat3("Direction Light Direction", (float*)DirLight.GetDirectionPtr(), 0.01f);
			ImGui::End();
		}

		skybox.Render(camera.CalculateViewMatrix(), camera.CalculateProjectionMatrix(window.GetBufferWidth(), window.GetBufferHeight()));
	
		shader.Bind();

		shader.SetVec4f(glm::vec4(pointlights[0].GetColour(), pointlights[0].GetIntensity()), "pointLight[0].m_colour");
		shader.SetVec3f(camera.GetCameraPosition(), "u_cameraPosition");
		shader.SetMat4f(camera.CalculateProjectionMatrix(window.GetBufferWidth(), window.GetBufferHeight()), "u_Projection", false);
		shader.SetMat4f(camera.CalculateViewMatrix(), "u_View", false);

		shader.SetVec4f(glm::vec4(DirLight.GetColour(), DirLight.GetIntensity()), "DirLight.m_colour");
		shader.SetVec3f(DirLight.GetDirection(), "DirLight.m_direction");
		shader.SetMat4f(DirLight.CalculateLightTransform(), "u_DirectionLightTransform", false);
		
		RenderScene(&shader);

		/*
		for (int row = 0; row < nrRows; row++) {

			shader.Set1f((float)row / (float)nrRows, "u_metallic");
			for (int col = 0; col < nrColumns; col++) {

				// we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
				// on direct lighting.
				shader.Set1f(glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f), "u_roughness");

				shpere.ResetModel();
				shpere.SetRotate({ 90.f, 0.f, 0.f });
				shpere.SetPosition(glm::vec3(
					(col - (nrColumns / 2)) * spacing,
					(row - (nrRows / 2)) * spacing,
					0.0f
				));
				shader.SetMat4f(shpere.GetModel(), "u_Model", false);
				shpere.Render();
			}
		}
		*/


		GuiLayer.End();
		window.Clear();
	}


	return 0;

}