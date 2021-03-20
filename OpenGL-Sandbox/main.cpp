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
#include "SpotLight.h"

Window window("Window", true, 1460, 768);

Camera camera(glm::vec3(0.f, 2.f, 10.f), glm::vec3(0, 1, 0), 0, 0, 5, 110, 90.f);

Shader shader;
Shader dirShadowShader;
Shader omniShadowShader;

Texture HDRI("Textures/HDRIs/herkulessaulen_4k.hdr");
Texture hdriCubemap;
Texture irradianceCubemap;
Texture prefilterMap;
Texture brdfLUTMap;

Texture Gold_Albedo		 ("Textures/PBR/Gold (Au)_schvfgwp_Metal/Albedo_4K__schvfgwp.jpg");
Texture Gold_Normal      ("Textures/PBR/Gold (Au)_schvfgwp_Metal/Normal_4K__schvfgwp.jpg");
Texture Gold_Roughness   ("Textures/PBR/Gold (Au)_schvfgwp_Metal/Roughness_4K__schvfgwp.jpg");
Texture Gold_AO          ("Textures/PBR/Gold (Au)_schvfgwp_Metal/Metalness_4K__schvfgwp.jpg");
Texture Gold_Metallic    ("Textures/PBR/Gold (Au)_schvfgwp_Metal/Metalness_4K__schvfgwp.jpg");
Texture Gold_Displacement("Textures/parallax_mapping_height_map.png");

Texture Rock_Albedo      ("Textures/PBR/Icelandic Cracked Rock_vclnajuew_Surface/Albedo_4K__vclnajuew.jpg");
Texture Rock_Normal      ("Textures/PBR/Icelandic Cracked Rock_vclnajuew_Surface/Normal_4K__vclnajuew.jpg");
Texture Rock_Roughness   ("Textures/PBR/Icelandic Cracked Rock_vclnajuew_Surface/Roughness_4K__vclnajuew.jpg");
Texture Rock_AO			 ("Textures/PBR/Icelandic Cracked Rock_vclnajuew_Surface/AO_4K__vclnajuew.jpg");
Texture Rock_Metallic    ("Textures/PBR/Icelandic Cracked Rock_vclnajuew_Surface/Albedo_4K__vclnajuew.jpg");
Texture Rock_Displacement("Textures/parallax_mapping_height_map.png");

DirectionalLight DirLight(1024, 1024, { 1, 1, 1 }, 0.f, { 0.5, -1, 0 });
std::vector<PointLight> pointlights(1);
SpotLight spotLight({ 0, 5, 0 }, { 1, 1, 1, 1 }, { 0, -1, 0 }, 40, 10, 1024, 1024, 0.1, 100);

Model shpere;
Model quad;
Model cube;

//Uniforms 
bool useTexture = true;
bool toggleShadowMapTexture = true;
static float u_FilterLevel = 3;

void SetSpotLightUniforms(Shader* shader) {

	shader->SetVec3f(spotLight.GetPosition(), "spotlight.base.m_position");
	shader->SetVec3f(spotLight.GetDirection(), "spotlight.m_direction");
	shader->SetVec4f(glm::vec4(spotLight.GetColour(), spotLight.GetIntensity()), "spotlight.base.m_colour");

	shader->Set1f(spotLight.GetConstant(), "spotlight.base.m_constant");
	shader->Set1f(spotLight.GetLinear(), "spotlight.base.m_linear");
	shader->Set1f(spotLight.GetQuadratic(), "spotlight.base.m_quadratic");

	shader->Set1f(spotLight.GetOutterCutOff(), "spotlight.m_outterEdge");
	shader->Set1f(spotLight.GetInnerCutOff(), "spotlight.m_innerEdge");

}

void PBRScene(Shader* shader) {
	
	static int nrRows = 5;
	static int nrColumns = 5;
	static float spacing = 2.5;

	for (int row = 0; row < nrRows; row++) {
		
		shader->Set1f((float)row / (float)nrRows, "u_metallic");
		
		for (int col = 0; col < nrColumns; col++) {

		// we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
		// on direct lighting.
		shader->Set1f(glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f), "u_roughness");

		shpere.ResetModel();
		shpere.SetRotation({ 90.f, 0.f, 0.f });
		shpere.SetTranslation(glm::vec3(
			(col - (nrColumns / 2)) * spacing,
			(row - (nrRows / 2)) * spacing,
			0.0f));

		shader->SetMat4f(shpere.GetModel(), "u_Model", false);
		shpere.Render();
	    }
    }

}

void RenderScene(Shader* shader) {

	Gold_Albedo.Bind(0);
	Gold_Normal.Bind(1);
	Gold_Roughness.Bind(2);
	Gold_AO.Bind(3);
	Gold_Metallic.Bind(4);
	Gold_Displacement.Bind(5);
	
	shader->Set1i(0, "u_test");
	shader->Set1i(useTexture, "u_usePRB");
	shader->SetMat4f(shpere.GetModel(), "u_Model", false);
	shpere.Render();

	Rock_Albedo.Bind(0);
	Rock_Normal.Bind(1);
	Rock_Roughness.Bind(2);
	Rock_AO.Bind(3);
	Rock_Metallic.Bind(4);
	Rock_Displacement.Bind(5);

	shader->Set1i(1, "u_usePRB");
	quad.SetScale({5.f, 0.001, 5.f});
	quad.SetTranslation({ 0.f, -2.5f, 0.f });
	quad.SetRotation({ 180.f, 0.f, 0.f });
	shader->SetMat4f(quad.GetModel(), "u_Model", false);
	quad.Render();
}

void RenderCubeScene(Shader* shader) {
	shader->Set1i(toggleShadowMapTexture, "u_test");
	shader->Set1i(0, "u_usePRB");
	cube.SetTranslation({ 0, 5, 0 });
	shader->SetMat4f(cube.GetModel(), "u_Model", false);
	cube.Render();
}

void DirectionalShadowMapPass() {
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	dirShadowShader.Bind();
	dirShadowShader.SetMat4f(DirLight.CalculateLightTransform(), "u_DirectionLightTransform", false);

	glViewport(0, 0, DirLight.GetShadowMapPtr()->GetWidth(), DirLight.GetShadowMapPtr()->GetHeight());
	DirLight.GetShadowMapPtr()->BindFBO();	
	glClear(GL_DEPTH_BUFFER_BIT);

	RenderScene(&dirShadowShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_CULL_FACE);
};

void OmniShadowMapPass(PointLight *light) {

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	
	omniShadowShader.Bind();
	
	omniShadowShader.SetVec3f(light->GetPosition(), "u_lightPos");
	omniShadowShader.Set1f(light->GetFarPlane(), "u_farPlane");

	auto lightMatrices = light->CalculateLightTransform();

	omniShadowShader.SetMat4f(lightMatrices[0], "u_lightMatrices[0]", false);
	omniShadowShader.SetMat4f(lightMatrices[1], "u_lightMatrices[1]", false);
	omniShadowShader.SetMat4f(lightMatrices[2], "u_lightMatrices[2]", false);
	omniShadowShader.SetMat4f(lightMatrices[3], "u_lightMatrices[3]", false);
	omniShadowShader.SetMat4f(lightMatrices[4], "u_lightMatrices[4]", false);
	omniShadowShader.SetMat4f(lightMatrices[5], "u_lightMatrices[5]", false);

	glViewport(0, 0, light->GetShadowMapPtr()->GetWidth(), light->GetShadowMapPtr()->GetHeight());

	light->GetShadowMapPtr()->BindFBO();
	glClear(GL_DEPTH_BUFFER_BIT);

	omniShadowShader.Validate();
	RenderScene(&omniShadowShader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_CULL_FACE);

}

/* ~TODO~
* Add Base Light Calculations for Point & Spot lights in Shader
* Add System for passing shadow maps into shader
* Add System for swaping sense using GUI
* Add System for drop and dropping textures with GUI
* Add Displacement Mapping
* Add Skelatal Animation Class
*/

int main() {

	GuiLayer GuiLayer(window.GetWindow());

	shader.CreateFromFile("Shaders/Vertex.glsl", "Shaders/Frag.glsl");
	dirShadowShader.CreateFromFile("Shaders/DirectionShadowMapVert.glsl", "Shaders/DirectionShadowMapFrag.glsl");
	omniShadowShader.CreateFromFile("Shaders/OmniShadowMapShaderVert.glsl", "Shaders/OmniShadowMapShaderGeom.glsl", "Shaders/OmniShadowMapShaderFrag.glsl");

	HDRI.CreateHDRI();
	hdriCubemap.CreateCubemapFromHDRI(HDRI);
	irradianceCubemap.CreateIrradianceTexture(&hdriCubemap);
	prefilterMap.CreatePrefilterMap(&hdriCubemap);
	brdfLUTMap.CreateBRDFLookUpTable();

	Rock_Albedo.CreateTexture2D();
	Rock_Normal.CreateTexture2D();
	Rock_Roughness.CreateTexture2D();
	Rock_AO.CreateTexture2D();
	Rock_Metallic.CreateTexture2D();
	Rock_Displacement.CreateTexture2D();

	Gold_Albedo.CreateTexture2D();
	Gold_Normal.CreateTexture2D();
	Gold_Roughness.CreateTexture2D();
	Gold_AO.CreateTexture2D();
	Gold_Metallic.CreateTexture2D();
	Gold_Displacement.CreateTexture2D();
	
	pointlights[0] = PointLight({ 1, 1, 1, 0.f }, { 2.f, 5.0f, 0.0f }, 1024, 1024, 0.1, 100);

	Skybox skybox(&hdriCubemap);

	quad.Load("Models/quad.fbx");
	quad.Create();
	
	shpere.Load("Models/Sphere64.fbx");
	shpere.Create();

	cube.Load("Models/Cube.dae");
	cube.Create();

	shader.Bind();

	Gold_Albedo.Bind(0);
	Gold_Normal.Bind(1);
	Gold_Roughness.Bind(2);
	Gold_AO.Bind(3);
	Gold_Metallic.Bind(4);
	Gold_Displacement.Bind(5);
	irradianceCubemap.Bind(6);
	prefilterMap.Bind(7);
	brdfLUTMap.Bind(8);

	//Shadow Textures
	DirLight.GetShadowMapPtr()->BindTexture(9);
	pointlights[0].GetShadowMapPtr()->BindTexture(10);
	spotLight.GetShadowMapPtr()->BindTexture(11);

	shader.Set1i(0, "u_AlbedoTexture");
	shader.Set1i(1, "u_NormalTexture");
	shader.Set1i(2, "u_RoughnessTexture");
	shader.Set1i(3, "u_AOTexture");
	shader.Set1i(4, "u_MetallicTexture");
	shader.Set1i(5, "u_DisplacementTexture");
	shader.Set1i(6, "u_irradianceMap");
	shader.Set1i(7, "u_prefilterMap");
	shader.Set1i(8, "u_brdfLUT");
	shader.Set1i(9, "u_directionalShadowMap");
	shader.Set1i(10, "u_omniShadowMap");
	shader.Set1i(11, "u_SpotLightShadowMap");


	shader.Set1f(pointlights[0].GetFarPlane(), "u_omniFarPlane");
	shader.Set1f(spotLight.GetFarPlane(), "u_SpotLightFarPlane");

	for (unsigned int i = 0; i < 1; i++) {
		glm::vec3 newPos = pointlights[i].GetPosition() + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
		newPos = pointlights[i].GetPosition();
		std::string pos = "pointLight[" + std::to_string(i) + "].m_position";
		shader.SetVec3f(newPos, pos.c_str());
		std::string col = "pointLight[" + std::to_string(i) + "].m_colour";
		shader.SetVec4f(glm::vec4(pointlights[i].GetColour(), pointlights[i].GetIntensity()), col.c_str());
	}
	
	shader.Unbind();

	while (window.IsOpen()) {

		//Shadow Passes
		DirectionalShadowMapPass();
		OmniShadowMapPass(&pointlights[0]);
		OmniShadowMapPass(&spotLight);

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

			if (ImGui::CollapsingHeader("Texture Options")) {
				ImGui::Checkbox("Use PBR Textures", &useTexture);
			}
			
			if (ImGui::CollapsingHeader("PointLight Options")) {
				ImGui::DragFloat3("PL Position", (float*)pointlights[0].GetPositionPtr(), 0.01, -25, 25);
				ImGui::ColorEdit3 ("PL Colour", (float*)pointlights[0].GetColourPtr());
				ImGui::SliderFloat("PL Intensity", pointlights[0].GetIntensityPtr(), 0.f, 5000.f);
			}

			if (ImGui::CollapsingHeader("DirectionLight Options")) {
				ImGui::SliderFloat ("DL Intensity", DirLight.GetIntensityPtr(), 0.f, 1.f);
				ImGui::SliderFloat3("DL Direction", (float*)(DirLight.GetDirectionPtr()), -1.f, 1.f);
				ImGui::DragFloat   ("DL Shadow Filter Level", &u_FilterLevel, 1.f, 0.f, 10);
			}

			if(ImGui::CollapsingHeader("SpotLight Options")) {
				ImGui::ColorEdit3("SL Colour", (float*)spotLight.GetColourPtr());
				ImGui::DragFloat3("SL Position", (float*)spotLight.GetPositionPtr(), 0.01, -25, 25);
				ImGui::DragFloat3("SL Direction", (float*)spotLight.GetDirectionPtr(), 0.01, -1, 1);
				ImGui::DragFloat ("SL InnerCutOff", spotLight.GetInnerCutOffPtr(), 0.01, 0, 1);
				ImGui::DragFloat ("SL OutterCutOff", spotLight.GetOutterCutOffPtr(), 0.01, 0, 1);
				ImGui::SliderFloat("SL Intensity", spotLight.GetIntensityPtr(), 0.f, 1000);
			};

			
		}

		skybox.Render(camera.CalculateViewMatrix(), camera.CalculateProjectionMatrix(window.GetBufferWidth(), window.GetBufferHeight()));

		shader.Bind();
		
		SetSpotLightUniforms(&shader);
		
		shader.SetVec4f(glm::vec4(pointlights[0].GetColour(), pointlights[0].GetIntensity()), "pointLight[0].m_colour");
		shader.SetVec3f(pointlights[0].GetPosition(), "pointLight[0].m_position");
		shader.SetVec3f(camera.GetCameraPosition(), "u_cameraPosition");
		shader.SetMat4f(camera.CalculateProjectionMatrix(window.GetBufferWidth(), window.GetBufferHeight()), "u_Projection", false);
		shader.SetMat4f(camera.CalculateViewMatrix(), "u_View", false);
		shader.SetVec4f(glm::vec4(DirLight.GetColour(), DirLight.GetIntensity()), "DirLight.m_colour");
		shader.SetVec3f(DirLight.GetDirection(), "DirLight.m_direction");
		shader.SetMat4f(DirLight.CalculateLightTransform(), "u_DirectionLightTransform", false);
		shader.Set1i(u_FilterLevel, "u_FilterLevel");

		RenderScene(&shader);

		GuiLayer.End();
		window.Clear();
	}


	return 0;

}