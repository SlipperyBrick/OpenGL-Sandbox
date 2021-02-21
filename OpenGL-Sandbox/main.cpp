
#include <vector>

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

int main() {

	Window window("Window", true, 1460, 768);

	Shader shader;
	shader.CreateFromFile("Shaders/Vertex.glsl", "Shaders/Frag.glsl");

	GuiLayer GuiLayer(window.GetWindow());

	std::vector<std::string> faces = {
		"Textures/Cubemaps/BlueSky/bluecloud_rt.jpg",
		"Textures/Cubemaps/BlueSky/bluecloud_lf.jpg",
		"Textures/Cubemaps/BlueSky/bluecloud_up.jpg",
		"Textures/Cubemaps/BlueSky/bluecloud_dn.jpg",
		"Textures/Cubemaps/BlueSky/bluecloud_bk.jpg",
		"Textures/Cubemaps/BlueSky/bluecloud_ft.jpg",
	};

	Skybox skybox(faces);

	Camera camera(glm::vec3(0.f, 0.f, 10.f), glm::vec3(0, 1, 0), 0, 0, 1, 1, 90.f);

	std::vector<PointLight> pointlights(4);
	{
	   pointlights[0].SetPosition(glm::vec3(-10.0f,  10.0f, 10.0f));
	   pointlights[1].SetPosition(glm::vec3( 10.0f,  10.0f, 10.0f));
	   pointlights[2].SetPosition(glm::vec3(-10.0f, -10.0f, 10.0f));
	   pointlights[3].SetPosition(glm::vec3( 10.0f, -10.0f, 10.0f));
	   
	   pointlights[0].SetColour(glm::vec3(300.0f, 300.0f, 300.0f));
	   pointlights[1].SetColour(glm::vec3(300.0f, 300.0f, 300.0f));
	   pointlights[2].SetColour(glm::vec3(300.0f, 300.0f, 300.0f));
	   pointlights[3].SetColour(glm::vec3(300.0f, 300.0f, 300.0f));
	}

	Texture Albedo   ("Textures/rustediron1-alt2-bl/rustediron2_basecolor.png");
	Texture Normal   ("Textures/rustediron1-alt2-bl/rustediron2_normal.png");
	Texture Roughness("Textures/rustediron1-alt2-bl/rustediron2_roughness.png");
	Texture AO       ("Textures/rustediron1-alt2-bl/rustediron2_ao.png");
	Texture Metallic ("Textures/rustediron1-alt2-bl/rustediron2_metallic.png");

	Mesh obj;
	obj.Create(quadVerts, quadIndices, 32, 6);

	Model shpere;
	shpere.Load("Models/shpere.fbx");

	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	while (window.IsOpen()) {

		window.Update();
		if (window.UpdateOnFocus()) {
			camera.mouseControl(window.GetXChange(), window.GetYChange());
			camera.keyControl(window.GetsKeys(), window.GetDeltaTime());
		}

		skybox.Render(camera.CalculateViewMatrix(), camera.CalculateProjectionMatrix(window.GetBufferWidth(), window.GetBufferHeight()));

		// GUI //
		{
			GuiLayer.Begin();

			ImGui::Begin("GUI");
			ImGui::Text("Application average %.2f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		} 

		shader.Bind();

		Albedo.Bind(0);
		Normal.Bind(1);
		Roughness.Bind(2);
		//AO.Bind(3);
		Metallic.Bind(4);

		shader.SetVec3f(camera.GetCameraPosition(), "u_cameraPosition");
		shader.SetMat4f(camera.CalculateProjectionMatrix(window.GetBufferWidth(), window.GetBufferHeight()), "u_Projection", false);
		shader.SetMat4f(camera.CalculateViewMatrix(), "u_View", false);
		
		shader.Set1i(0, "u_AlbedoTexture");
		shader.Set1i(1, "u_NormalTexture");
		shader.Set1i(2, "u_RoughnessTexture");
		shader.Set1i(3, "u_AOTexture");
		shader.Set1i(4, "u_MetallicTexture");

		shpere.ResetModel();
		for (int row = 0; row < nrRows; row++) {

			shader.Set1f((float)row / (float)nrRows, "u_metallic");
			for (int col = 0; col < nrColumns; col++) {

				// we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
				// on direct lighting.
				shader.Set1f(glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f), "u_roughness");

				shpere.ResetModel();
				shpere.SetRotate({ 90.f, 0.f, 0.f });
				//shpere.SetScale({ 0.01f, 0.01f, 0.01f });
				shpere.SetPosition(glm::vec3(
					(col - (nrColumns / 2)) * spacing,
					(row - (nrRows / 2)) * spacing,
					0.0f
				));

				shader.SetMat4f(shpere.GetModel(), "u_Model", false);
				shpere.Render();
			}
		}
	
		// render light source (simply re-render sphere at light positions)
		// this looks a bit off as we use the same shader, but it'll make their positions obvious and 
		// keeps the codeprint small.
		for (unsigned int i = 0; i < pointlights.size(); i++)
		{
			glm::vec3 newPos = pointlights[i].GetPosition() + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = pointlights[i].GetPosition();
			std::string pos = "pointLight[" + std::to_string(i) + "].m_position";
			shader.SetVec3f(newPos, pos.c_str());
			std::string col = "pointLight[" + std::to_string(i) + "].m_colour";
			shader.SetVec3f(pointlights[i].GetColour(), col.c_str());

			//shpere.ResetModel();
			//shpere.SetPosition(newPos);
			//shpere.SetScale(glm::vec3(0.4f));
			//shader.SetMat4f(shpere.GetModel(), "u_Model", false);
			//shpere.Render();

		}

		shader.Unbind();
		
		
		GuiLayer.End();
		window.Clear();
	}


	return 0;

}