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
#include "RenderTarget.h"
#include "Random.hpp"
#include "Material.h"

Window window("Window", true, 1460, 768);

Random Random::s_Instance;

Camera camera(glm::vec3(0.f, 2.f, 10.f), glm::vec3(0, 1, 0), 0, 0, 5, 110, 90.f);

Shader shader;
Shader dirShadowShader;
Shader omniShadowShader;

Shader QuadShader;

RenderTarget renderTarget(window.GetBufferWidth(), window.GetBufferHeight());

Texture HDRI("Textures/HDRIs/herkulessaulen_4k.hdr");
Texture hdriCubemap;
Texture irradianceCubemap;
Texture prefilterMap;
Texture brdfLUTMap;

Texture HeightMap("Textures/parallax_mapping_height_map.png");

Texture Gold_Albedo("Textures/PBR/Gold (Au)_schvfgwp_Metal/Albedo_4K__schvfgwp.jpg");
Texture Gold_Normal("Textures/PBR/Gold (Au)_schvfgwp_Metal/Normal_4K__schvfgwp.jpg");
Texture Gold_Roughness("Textures/PBR/Gold (Au)_schvfgwp_Metal/Roughness_4K__schvfgwp.jpg");
Texture Gold_AO("Textures/PBR/Gold (Au)_schvfgwp_Metal/Metalness_4K__schvfgwp.jpg");
Texture Gold_Metallic("Textures/PBR/Gold (Au)_schvfgwp_Metal/Metalness_4K__schvfgwp.jpg");

Texture Marble_Albedo    ("Textures/PBR/marble_polished_vdkgdbpv/vdkgdbpv_4K_Albedo.jpg");
Texture Marble_Normal	 ("Textures/PBR/marble_polished_vdkgdbpv/vdkgdbpv_4K_Normal.jpg");
Texture Marble_Roughness ("Textures/PBR/marble_polished_vdkgdbpv/vdkgdbpv_4K_Roughness.jpg");
Texture Marble_AO		 ("Textures/PBR/marble_polished_vdkgdbpv/vdkgdbpv_4K_AO.jpg");
Texture Marble_Metallic  ("Textures/PBR/marble_polished_vdkgdbpv/vdkgdbpv_4K_Metalness.jpg");

Texture Rock_Albedo   ("Textures/PBR/rock_rough_vdljfeglw/vdljfeglw_4K_Albedo.jpg");
Texture Rock_Normal   ("Textures/PBR/rock_rough_vdljfeglw/vdljfeglw_4K_Normal.jpg");
Texture Rock_Roughness("Textures/PBR/rock_rough_vdljfeglw/vdljfeglw_4K_Roughness.jpg");
Texture Rock_AO	      ("Textures/PBR/rock_rough_vdljfeglw/vdljfeglw_4K_AO.jpg");


Material GoldMaterial(Gold_Albedo, Gold_Normal, Gold_Roughness, Gold_AO, Gold_Metallic);
Material MarbleMaterial(Marble_Albedo, Marble_Normal, Marble_Roughness, Marble_AO);
Material RockMaterial(Rock_Albedo, Rock_Normal, Rock_Roughness, Rock_AO);

DirectionalLight dirLight(1024, 1024, 1, { 1, 1, 1 }, 0.f, { 0.5, -1, 0 });
std::vector<PointLight> pointlights(1);

SpotLight spotLight({ 0, 5, 0 }, { 1, 1, 1, 1 }, { 0, -1, 0 }, 40, 10, 1024, 1024, 0.1, 100);

Model shpere;
Model quad;
Model cube;

//Uniforms 
bool useTexture = false;

bool spotlightFlickering = false;		
float spotlightFlickeringSpeed = 1.f;	

bool u_MonochromeToggle = false; 
bool u_WobbleToggle     = false; 
bool u_BlurToggle       = false; 
float u_BlurStrength = 1.f;		 
float x = 0, y = 0;

glm::vec3  albedo = glm::vec3(1.0, 0.f, 0.f);
float metallic = 1.0f;
float roughness = 1.0f;
float ao = 1.0f;

//Scenes
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
	
	shader->Bind();

	if (spotlightFlickering) {
		float v0 = glfwGetTime() * spotlightFlickeringSpeed;
		float v1 = (glm::fract(sin(v0)));
		spotLight.SetIntensity(v1 * 5000);
	}

	dirLight.Bind(shader);
	spotLight.Bind(shader, 0);
	pointlights[0].Bind(shader, 0);

	shader->Set1f(glfwGetTime(), "u_Time");
	shader->SetVec3f(camera.GetCameraPosition(), "u_cameraPosition");
	shader->SetMat4f(camera.CalculateProjectionMatrix(window.GetBufferWidth(), window.GetBufferHeight()), "u_Projection", false);
	shader->SetMat4f(camera.CalculateViewMatrix(), "u_View", false);

	shader->SetVec3f(albedo, "albedo");
	shader->Set1f(metallic, "metallic");
	shader->Set1f(roughness, "roughness");
	shader->Set1f(ao, "ao");

	GoldMaterial.Bind();

	shader->Set1i(useTexture, "u_usePRB");
	shader->SetMat4f(cube.GetModel(), "u_Model", false);
	
	cube.Render();

	RockMaterial.Bind();

	shader->Set1i(1, "u_usePRB");
	shader->SetMat4f(quad.GetModel(), "u_Model", false);
	
	quad.SetScale({ 5.f, 0.001, 5.f });
	quad.SetTranslation({ 0.f, -2.5f, 0.f });
	quad.SetRotation({ 180.f, 0.f, 0.f });
	quad.Render();

	x += 10 * window.GetDeltaTime();
	y += 10 * window.GetDeltaTime();

}

//Shadow Passes 
void DirectionalShadowMapPass(DirectionalLight* dirLight) {

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	dirShadowShader.Bind();
	dirShadowShader.SetMat4f(dirLight->CalculateLightTransform(), "u_DirectionLightTransform", false);

	glViewport(0, 0, dirLight->GetShadowMapPtr()->GetWidth(), dirLight->GetShadowMapPtr()->GetHeight());
	dirLight->GetShadowMapPtr()->BindFBO();
	glClear(GL_DEPTH_BUFFER_BIT);

	RenderScene(&dirShadowShader);
	renderTarget.Bind(window);
	glDisable(GL_CULL_FACE);
};

void OmniShadowMapPass(PointLight* light) {

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

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

	RenderScene(&omniShadowShader);

	renderTarget.Bind(window);
	glDisable(GL_CULL_FACE);

}

/* ~TODO~
* Add Async 
* Add System for swaping scene using GUI
* Add System for drop and dropping textures with GUI
* Add Displacement Mapping
* Add Skelatal Animation Class
*/

int main() {

	GuiLayer GuiLayer(window.GetWindow());
	Skybox skybox(&hdriCubemap);
	pointlights[0] = PointLight({ 1, 1, 1, 0.f }, { 2.f, 5.0f, 0.0f }, 1024, 1024, 0.1, 100);

	shader.CreateFromFile("Shaders/Vertex.glsl", "Shaders/Frag.glsl");
	dirShadowShader.CreateFromFile("Shaders/DirectionShadowMapVert.glsl", "Shaders/DirectionShadowMapFrag.glsl");
	omniShadowShader.CreateFromFile("Shaders/OmniShadowMapShaderVert.glsl", "Shaders/OmniShadowMapShaderGeom.glsl", "Shaders/OmniShadowMapShaderFrag.glsl");
	QuadShader.CreateFromFile("Shaders/QuadShaderVert.glsl", "Shaders/QuadShaderFrag.glsl");

	HDRI.CreateHDRI();
	hdriCubemap.CreateCubemapFromHDRI(HDRI);
	irradianceCubemap.CreateIrradianceTexture(&hdriCubemap);
	prefilterMap.CreatePrefilterMap(&hdriCubemap);
	brdfLUTMap.CreateBRDFLookUpTable();

	HeightMap.CreateTexture2D();
	
	Gold_Albedo.CreateTexture2D();
	Gold_Normal.CreateTexture2D();
	Gold_Roughness.CreateTexture2D();
	Gold_AO.CreateTexture2D();
	Gold_Metallic.CreateTexture2D();

	Marble_Albedo.CreateTexture2D();
	Marble_Normal.CreateTexture2D();
	Marble_Roughness.CreateTexture2D();
	Marble_AO.CreateTexture2D();
	Marble_Metallic.CreateTexture2D();

	Rock_Albedo.CreateTexture2D();
	Rock_Normal.CreateTexture2D();
	Rock_Roughness.CreateTexture2D();
	Rock_AO.CreateTexture2D();

	quad.Load("Models/quad.fbx");
	quad.Create();

	shpere.Load("Models/Sphere64.fbx");
	shpere.Create();

	cube.Load("Models/monkey.fbx");
	cube.Create();
	cube.SetRotation({260, 0, 0});

	shader.Bind();

	irradianceCubemap.Bind(TU_IRRADIANCE);
	prefilterMap.Bind(TU_PREFILTER);
	brdfLUTMap.Bind(TU_BRDF);

	// Init Textures
	shader.Set1i(TU_IRRADIANCE, "u_irradianceMap");
	shader.Set1i(TU_PREFILTER, "u_prefilterMap");
	shader.Set1i(TU_BRDF, "u_brdfLUT");

	shader.Set1i(TU_ALBEDO, "u_AlbedoTexture");
	shader.Set1i(TU_NORMAL, "u_NormalTexture");
	shader.Set1i(TU_ROUGHNESS, "u_RoughnessTexture");
	shader.Set1i(TU_AO, "u_AOTexture");
	shader.Set1i(TU_METALLIC, "u_MetallicTexture");
	shader.Set1i(TU_DISPLACEMENT, "u_DisplacementTexture");

	shader.Unbind();

	while (window.IsOpen()) {
		

		renderTarget.Bind(window);

		//Shadow Passes
		DirectionalShadowMapPass(&dirLight);
		OmniShadowMapPass(&pointlights[0]);
		OmniShadowMapPass(&spotLight);

		glDisable(GL_DEPTH);
		skybox.Render(camera.CalculateViewMatrix(), camera.CalculateProjectionMatrix(window.GetBufferWidth(), window.GetBufferHeight()));
		glEnable(GL_DEPTH);
		RenderScene(&shader);

		renderTarget.Unbind();
	
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

			if (ImGui::CollapsingHeader("Render Options")) {
				ImGui::Checkbox("Blur", &u_BlurToggle);
				ImGui::SliderFloat("Blur Strength", &u_BlurStrength, 1, 10);
				ImGui::Checkbox("Woggle", &u_WobbleToggle);
				ImGui::Checkbox("Monochrome", &u_MonochromeToggle);
			}

			if (ImGui::CollapsingHeader("Texture Options")) {
				ImGui::Checkbox("Use PBR Textures", &useTexture);
				if (!useTexture) {
					ImGui::ColorEdit3("Object Colour", (float*)&albedo);
					ImGui::DragFloat("Metallic", &metallic, 0.01, 0.f, 1.f);
					ImGui::DragFloat("AO", &ao, 0.01, 0.f, 1.f);
					ImGui::DragFloat("Roughness", &roughness, 0.01, 0.f, 1.f);
				}
			}

			if (ImGui::CollapsingHeader("PointLight Options")) {
				ImGui::DragFloat3("PL Position", (float*)pointlights[0].GetPositionPtr(), 0.01, -25, 25);
				ImGui::ColorEdit3("PL Colour", (float*)pointlights[0].GetColourPtr());
				ImGui::SliderFloat("PL Intensity", pointlights[0].GetIntensityPtr(), 0.f, 5000.f);
			}

			if (ImGui::CollapsingHeader("DirectionLight Options")) {
				ImGui::ColorEdit3("DL Intensity", (float*)dirLight.GetColourPtr());
				ImGui::SliderFloat3("DL Direction", (float*)(dirLight.GetDirectionPtr()), -1.f, 1.f);
				ImGui::DragInt("DL Shadow Filter Level", dirLight.GetFilterLevelPtr(), 1.f, 0.f, 10);
				ImGui::SliderFloat("DL Intensity", dirLight.GetIntensityPtr(), 0.f, 1.f);
			}

			if (ImGui::CollapsingHeader("SpotLight Options")) {
				ImGui::ColorEdit3("SL Colour", (float*)spotLight.GetColourPtr());
				ImGui::DragFloat3("SL Position", (float*)spotLight.GetPositionPtr(), 0.01, -25, 25);
				ImGui::DragFloat3("SL Direction", (float*)spotLight.GetDirectionPtr(), 0.01, -1, 1);
				ImGui::DragFloat("SL InnerCutOff", spotLight.GetInnerCutOffPtr(), 0.01, 0, 1);
				ImGui::DragFloat("SL OutterCutOff", spotLight.GetOutterCutOffPtr(), 0.01, 0, 1);
				ImGui::Checkbox("Enable Flickering", &spotlightFlickering);
				if (spotlightFlickering) {
					ImGui::SliderFloat("SL Flickering Speed", &spotlightFlickeringSpeed, 0.f, 10);
				}
				else {
					ImGui::SliderFloat("SL Intensity", spotLight.GetIntensityPtr(), 0.f, 5000);
				}
			};

			ImGui::End();
		}

		QuadShader.Bind();
		
		QuadShader.Set1i(0, "u_Frame");
		renderTarget.GetTexture()->Bind(0);

		QuadShader.Set1i(u_BlurToggle, "u_BlurToggle");
		QuadShader.Set1f(u_BlurStrength, "u_BlurStrength");
		QuadShader.Set1i(u_MonochromeToggle, "u_MonochromeToggle");
		QuadShader.Set1i(u_WobbleToggle, "u_WobbleToggle");

		QuadShader.SetVec2f(glm::vec2(window.GetBufferWidth(), window.GetBufferHeight()), "u_Resolution");
		QuadShader.Set1f(glfwGetTime(), "u_Offset");

		//Draw quad
		renderQuad();

		GuiLayer.End();
		window.Clear();

	}


	return 0;

}