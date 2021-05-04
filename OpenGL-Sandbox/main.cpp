#include <vector>
#include <future>
#include <mutex>

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
#include "ResourceManager.h"

//https://google.github.io/filament/Filament.md.html#listing_glsldirectionallight

Random Random::s_Instance;

#pragma region Create Window & Camera
Window window("Window", true, 1460, 768);
Camera camera(&window, glm::vec3(0.f, 2.f, 5.f), glm::vec3(0, 1, 0), 0, 0, 5, 110, 90.f);
#pragma endregion

#pragma region Create Shader
Shader shader;
Shader dirShadowShader;
Shader omniShadowShader;
Shader quadShader;
#pragma endregion

RenderTarget renderTarget(window.GetBufferWidth(), window.GetBufferHeight());

#pragma region Create Texture & Material

Texture HDRI("Textures/HDRIs/pink_sunrise_4k.hdr");
Texture hdriCubemap;
Texture irradianceCubemap;
Texture prefilterMap;
Texture brdfLUTMap;

Texture Gold_Albedo	  ("Textures/PBR/Gold (Au)_schvfgwp_Metal/Albedo_4K__schvfgwp.jpg");
Texture Gold_Normal	  ("Textures/PBR/Gold (Au)_schvfgwp_Metal/Normal_4K__schvfgwp.jpg");
Texture Gold_Roughness("Textures/PBR/Gold (Au)_schvfgwp_Metal/Roughness_4K__schvfgwp.jpg");
Texture Gold_AO		  ("Textures/PBR/Gold (Au)_schvfgwp_Metal/Metalness_4K__schvfgwp.jpg");
Texture Gold_Metallic ("Textures/PBR/Gold (Au)_schvfgwp_Metal/Metalness_4K__schvfgwp.jpg");

Texture Marble_Albedo   ("Textures/PBR/marble_polished_vdkgdbpv/vdkgdbpv_4K_Albedo.jpg");
Texture Marble_Normal   ("Textures/PBR/marble_polished_vdkgdbpv/vdkgdbpv_4K_Normal.jpg");
Texture Marble_Roughness("Textures/PBR/marble_polished_vdkgdbpv/vdkgdbpv_4K_Roughness.jpg");
Texture Marble_AO		("Textures/PBR/marble_polished_vdkgdbpv/vdkgdbpv_4K_AO.jpg");
Texture Marble_Metallic ("Textures/PBR/marble_polished_vdkgdbpv/vdkgdbpv_4K_Metalness.jpg");

Texture Rock_Albedo("Textures/PBR/rock_rough_vdljfeglw/vdljfeglw_4K_Albedo.jpg");
Texture Rock_Normal("Textures/PBR/rock_rough_vdljfeglw/vdljfeglw_4K_Normal.jpg");
Texture Rock_Roughness("Textures/PBR/rock_rough_vdljfeglw/vdljfeglw_4K_Roughness.jpg");
Texture Rock_AO("Textures/PBR/rock_rough_vdljfeglw/vdljfeglw_4K_AO.jpg");
Texture Rock_Metallic("Textures/PBR/rock_rough_vdljfeglw/vdljfeglw_4K_Displacement.jpg");

Texture Brick_Albedo("Textures/PBR/Rough Brick Wall 2x2 M_th5mdawaw/Albedo_4K__th5mdawaw.png");
Texture Brick_Normal("Textures/PBR/Rough Brick Wall 2x2 M_th5mdawaw/Normal_4K__th5mdawaw.png");
Texture Brick_Roughness("Textures/PBR/Rough Brick Wall 2x2 M_th5mdawaw/Roughness_4K__th5mdawaw.png");
Texture Brick_AO("Textures/PBR/Rough Brick Wall 2x2 M_th5mdawaw/AO_4K__th5mdawaw.png");

Texture Stone_Tiles_Albedo("Textures/PBR/Stone Tile 2x2_ujzjdcao/Albedo_4K__ujzjdcao.png");
Texture Stone_Tiles_Normal("Textures/PBR/Stone Tile 2x2_ujzjdcao/Normal_4K__ujzjdcao.png");
Texture Stone_Tiles_Roughness("Textures/PBR/Stone Tile 2x2_ujzjdcao/Roughness_4K__ujzjdcao.png");
Texture Stone_Tiles_AO("Textures/PBR/Stone Tile 2x2_ujzjdcao/AO_4K__ujzjdcao.png");

Texture Stone_Pavement_Albedo	 ("Textures/PBR/Stone Pavement 2x2 M_ujwnafvew/Albedo_4K__ujwnafvew.png");
Texture Stone_Pavement_Normal	 ("Textures/PBR/Stone Pavement 2x2 M_ujwnafvew/Normal_4K__ujwnafvew.png");
Texture Stone_Pavement_Roughness("Textures/PBR/Stone Pavement 2x2 M_ujwnafvew/Roughness_4K__ujwnafvew.png");
Texture Stone_Pavement_AO       ("Textures/PBR/Stone Pavement 2x2 M_ujwnafvew/AO_4K__ujwnafvew.png");


Material GoldMaterial("Gold Material", Gold_Albedo,   Gold_Normal,   Gold_Roughness,   Gold_AO,   Gold_Metallic);
Material MarbleMaterial("Marble Material", Marble_Albedo, Marble_Normal, Marble_Roughness, Marble_AO, 1.f);
Material RockMaterial("Rock Material", Rock_Albedo, Rock_Normal, Rock_Roughness, Rock_AO, Rock_Metallic);
Material BrickMaterial("Brick Material", Brick_Albedo, Brick_Normal, Brick_Roughness, Brick_AO, 0.f);
Material StoneTilesMaterial("Stone Trim Material", Stone_Tiles_Albedo, Stone_Tiles_Normal, Stone_Tiles_Roughness, Stone_Tiles_AO, 0.1f);
Material StonePavementMaterial("Stone Pavement Material", Stone_Pavement_Albedo, Stone_Pavement_Normal, Stone_Pavement_Roughness, Stone_Pavement_AO, 0.1f);


Skybox skybox(&hdriCubemap);
#pragma endregion

#pragma region Create Lights
DirectionalLight dirLight(2024, 2024, 1, { 1, 1, 1 }, 0.f, { 0.5, -1, 0 });

std::vector<PointLight> pointlights(1);

SpotLight spotLight({ 0, 5, 0 }, { 1, 1, 1, 0 }, { 0, -1, 0 }, 40, 10, 1024, 1024, 0.f, 100);
#pragma endregion

#pragma region Create Models
Model quad("Models/quad.fbx", "Quad");
Model monkey("Models/monkey.fbx", "Monkey");
Model sphere("Models/Sphere64.fbx", "Sphere");
Model sponza("Models/Sponza/sponza.obj", "Sponza");
Model knuckles("Models/Uganda_Knuckles/Uganda_Knuckles.obj", "Knuckles");
#pragma endregion

#pragma region Uniform Variables
static bool usePBR = true;

static bool spotlightFlickering = false;
static float spotlightFlickeringSpeed = 1.f;
   
static bool u_MonochromeToggle = false;
static bool u_WobbleToggle = false;
static bool u_BlurToggle = false;
static float u_BlurStrength = 1.f;
static float x = 0, y = 0;

static glm::vec3  albedo = glm::vec3(1.0, 0.f, 0.f);
static float metallic = 1.0f;
static float roughness = 1.0f;
static float ao = 1.0f;
static int rX = 0;
#pragma endregion

//Scenes
static void PBRScene(Shader* shader) {


	skybox.Render(&camera);

	static int nrRows = 5;
	static int nrColumns = 5;
	static float spacing = 2.5;

	shader->Bind();

	dirLight.Bind(shader);
	spotLight.Bind(shader, 0);
	pointlights[0].Bind(shader, 0);

	shader->Set1f("u_Time", glfwGetTime());
	shader->SetVec3f("u_cameraPosition", (glm::vec3&)camera.GetCameraPosition());
	shader->SetMat4f("u_View", camera.GetViewMatrix(), false);
	shader->SetMat4f("u_Projection", camera.GetProjectionMatrix(),  false);
	shader->SetVec3f("u_albedo", albedo);
	shader->Set1f("u_ao", ao);
	shader->Set1i("u_usePRB", usePBR);

	for (int row = 0; row < nrRows; row++) {

		shader->Set1f("u_metallic", (float)row / (float)nrRows);

		for (int col = 0; col < nrColumns; col++) {

			// we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
			// on direct lighting.
			shader->Set1f("u_roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

			sphere.ResetModel();
			sphere.SetRotation({ 90.f, 0.f, 0.f });
			sphere.SetTranslation(glm::vec3(
				(col - (nrColumns / 2)) * spacing,
				(row - (nrRows / 2)) * spacing,
				0.0f));

			shader->SetMat4f("u_Model", sphere.GetModel(), false);
			sphere.Render(shader);
		}
	}

}

static void RenderScene(Shader* shader) {
	
	shader->Bind();
	sponza.Render(shader);
}

//Shadow Passes 
static void DirectionalShadowMapPass(DirectionalLight* dirLight) {

	dirShadowShader.Bind();
	dirShadowShader.SetMat4f("u_DirectionLightTransform", dirLight->CalculateLightTransform(), false);

	glViewport(0, 0, dirLight->GetShadowMapPtr()->GetWidth(), dirLight->GetShadowMapPtr()->GetHeight());
	dirLight->GetShadowMapPtr()->BindFBO();
	glClear(GL_DEPTH_BUFFER_BIT);

	RenderScene(&dirShadowShader);

};

static void OmniShadowMapPass(PointLight* light) {

	omniShadowShader.Bind();

	omniShadowShader.SetVec3f("u_lightPos", (glm::vec3&)light->GetPosition());
	omniShadowShader.Set1f("u_farPlane", light->GetFarPlane());

	static std::vector<glm::mat4> lightMatrices = light->CalculateLightTransform();

	omniShadowShader.SetMat4f("u_lightMatrices[0]", lightMatrices[0], false);
	omniShadowShader.SetMat4f("u_lightMatrices[1]", lightMatrices[1], false);
	omniShadowShader.SetMat4f("u_lightMatrices[2]", lightMatrices[2], false);
	omniShadowShader.SetMat4f("u_lightMatrices[3]", lightMatrices[3], false);
	omniShadowShader.SetMat4f("u_lightMatrices[4]", lightMatrices[4], false);
	omniShadowShader.SetMat4f("u_lightMatrices[5]", lightMatrices[5],  false);

	glViewport(0, 0, light->GetShadowMapPtr()->GetWidth(), light->GetShadowMapPtr()->GetHeight());

	light->GetShadowMapPtr()->BindFBO();
	glClear(GL_DEPTH_BUFFER_BIT);

	RenderScene(&omniShadowShader);

}

/* ~TODO~ 
* Add System for drop and dropping textures with GUI
* Add System for swaping scene using GUI
* Add Displacement Mapping
* Add Skelatal Animation Class
*/

int main() {
	std::cout << "[MAIN]: " << std::this_thread::get_id() << "\n";

	std::vector<Material*> materialList;

	materialList.push_back(&MarbleMaterial);
	materialList.push_back(&GoldMaterial);
	materialList.push_back(&RockMaterial);
	materialList.push_back(&BrickMaterial);
	materialList.push_back(&StoneTilesMaterial);
	materialList.push_back(&StonePavementMaterial);
	GuiLayer GuiLayer(window.GetWindow());

	ResourceManager rManager;
	pointlights[0] = PointLight({ 1, 1, 1, 0.f }, { 0.f, 5.0f, 0.0f }, 1024, 1024, 0.0, 100);

	shader.CreateFromFile("Shaders/Vertex.glsl", "Shaders/Frag.glsl");
	dirShadowShader.CreateFromFile("Shaders/DirectionShadowMapVert.glsl", "Shaders/DirectionShadowMapFrag.glsl");
	omniShadowShader.CreateFromFile("Shaders/OmniShadowMapShaderVert.glsl", "Shaders/OmniShadowMapShaderGeom.glsl", "Shaders/OmniShadowMapShaderFrag.glsl");
	quadShader.CreateFromFile("Shaders/QuadShaderVert.glsl", "Shaders/QuadShaderFrag.glsl");

#pragma region Load Textures

	HDRI.CreateHDRI();
	hdriCubemap.CreateCubemapFromHDRI(HDRI);
	irradianceCubemap.CreateIrradianceTexture(&hdriCubemap);
	prefilterMap.CreatePrefilterMap(&hdriCubemap);
	brdfLUTMap.CreateBRDFLookUpTable();

	for (const auto& m : materialList)
	{
		rManager.Load(m);
	}

#pragma endregion

#pragma region Load Models


	quad.SetScale({ 5, 0.01, 5 });
	quad.SetTranslation({ 0, -2, 0 });
	monkey.SetRotation({260, 0, 0});
	knuckles.SetScale(glm::vec3(0.01));
	sponza.SetScale(glm::vec3(0.01f));

	rManager.Load(&sponza);
	rManager.Load(&knuckles);
	rManager.Load(&monkey);
	rManager.Load(&sphere);
	rManager.Load(&quad);

#pragma endregion
	
	quadShader.Bind();
	quadShader.Set1i("u_Frame", 0);

	
#pragma region Shader Texture Setup
	shader.Bind();

	irradianceCubemap.Bind(TU_IRRADIANCE);
	prefilterMap.Bind(TU_PREFILTER);
	brdfLUTMap.Bind(TU_BRDF);

	// Init Textures
	shader.Set1i("u_irradianceMap", TU_IRRADIANCE);
	shader.Set1i("u_prefilterMap", TU_PREFILTER);
	shader.Set1i("u_brdfLUT", TU_BRDF);

	shader.Set1i("material.m_AlbedoTexture", TU_ALBEDO);
	shader.Set1i("material.m_NormalTexture", TU_NORMAL);
	shader.Set1i("material.m_RoughnessTexture", TU_ROUGHNESS);
	shader.Set1i("material.m_AOTexture", TU_AO);
	shader.Set1i("material.m_MetallicTexture", TU_METALLIC);
	shader.Set1i("material.m_DisplacementTexture", TU_DISPLACEMENT);

	shader.Unbind();

#pragma endregion
	sponza.UpdateModel();
	glEnable(GL_CULL_FACE);
	while (window.IsOpen()) {

		window.Update();
		rManager.Update(materialList);
		camera.Update();

		renderTarget.Bind(window);

		shader.Bind();
		dirLight.Bind(&shader);
		spotLight.Bind(&shader, 0);
		pointlights[0].Bind(&shader, 0);

		shader.SetVec3f("u_albedo", albedo);
		shader.Set1f("u_metallic", metallic);
		shader.Set1f("u_roughness", roughness);
		shader.Set1f("u_ao", ao);

		shader.SetVec3f("u_cameraPosition", camera.GetCameraPosition());
		shader.SetMat4f("u_View", camera.GetViewMatrix(), false);
		shader.SetMat4f("u_Projection", camera.GetProjectionMatrix(), false);
		shader.Set1i("u_usePRB", usePBR);

		//Shadow Passes

		glCullFace(GL_FRONT);
		DirectionalShadowMapPass(&dirLight); 
		OmniShadowMapPass(&pointlights[0]);
		OmniShadowMapPass(&spotLight);
		glCullFace(GL_BACK);

		renderTarget.Bind(window);

		skybox.Render(&camera);
		RenderScene(&shader);
		renderTarget.Unbind();
	
		if (window.UpdateOnFocus()) {
			camera.MouseControl();
			camera.KeyControl();
		}

		// GUI //
		{
			GuiLayer.Begin();

			ImGui::Begin("Models");
			sphere.Update(materialList);
			quad.Update(materialList);
			sponza.Update(materialList);
			ImGui::End();

			ImGui::Begin("GUI");
			ImGui::Text("Application average %.2f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			if (ImGui::CollapsingHeader("Render Options")) {
				ImGui::Checkbox("Blur", &u_BlurToggle);
				ImGui::SliderFloat("Blur Strength", &u_BlurStrength, 1, 10);
				ImGui::Checkbox("Woggle", &u_WobbleToggle);
				ImGui::Checkbox("Monochrome", &u_MonochromeToggle);
			}

			if (ImGui::CollapsingHeader("Camera Options")) {
				glm::vec3 pos = camera.GetCameraPosition();
				glm::vec3 dir = camera.GetCameraDirection();
				std::string posStr = "Position: ";
				std::string dirStr = "Direction: ";
				ImGui::Text(posStr.append(std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z)).c_str());
				ImGui::Text(dirStr.append(std::to_string(dir.x) + ", " + std::to_string(dir.y) + ", " + std::to_string(dir.z)).c_str());
			}

			if (ImGui::CollapsingHeader("Texture Options")) {
				ImGui::Checkbox("Use PBR Textures", &usePBR);
		
				ImGui::ColorEdit3("Object Colour", (float*)&albedo);
				ImGui::DragFloat("Metallic", &metallic, 0.01, 0.f, 1.f);
				ImGui::DragFloat("AO", &ao, 0.01, 0.05f, 1.f);
				ImGui::DragFloat("Roughness", &roughness, 0.01, 0.f, 1.f);
				
			}

			if (ImGui::CollapsingHeader("DirectionLight Options")) {
				ImGui::ColorEdit3("DL Intensity", (float*)dirLight.GetColourPtr());
				ImGui::SliderFloat3("DL Direction", (float*)(dirLight.GetDirectionPtr()), -1.f, 1.f);
				ImGui::DragInt("DL Shadow Filter Level", dirLight.GetFilterLevelPtr(), 1.f, 0.f, 10);
				ImGui::SliderFloat("DL Intensity", dirLight.GetIntensityPtr(), 0.f, 10.f);
			}

			if (ImGui::CollapsingHeader("PointLight Options")) {
				ImGui::DragFloat3("PL Position", (float*)pointlights[0].GetPositionPtr(), 0.01, -25, 25);
				ImGui::ColorEdit3("PL Colour", (float*)pointlights[0].GetColourPtr());
				ImGui::DragFloat("PL Intensity", pointlights[0].GetIntensityPtr(), 1.f, 0, 5000);
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


		quadShader.Bind();
		renderTarget.GetTexture()->Bind(0);

		//quadShader.Set1i("u_BlurToggle", u_BlurToggle);
		//quadShader.Set1f("u_BlurStrength", u_BlurStrength);
		//quadShader.Set1i("u_MonochromeToggle", u_MonochromeToggle);
		//quadShader.Set1i("u_WobbleToggle", u_WobbleToggle);
		//quadShader.SetVec2f("u_Resolution", glm::vec2(window.GetBufferWidth(), window.GetBufferHeight()));
		//quadShader.Set1f("u_Offset", glfwGetTime());

		//Draw quad
		renderQuad();

		GuiLayer.End();
		window.Clear();
	
	}

	return 0;

}