
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
#include "Texture.h"
#include "PointLight.h"

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

	Camera camera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0, 1, 0), 0, 0, 1, 1, 90.f);

	PointLight pointLight;
	pointLight.SetColour({ 0.5f, 0.5f, 0.5f });
	pointLight.SetIntensity(1.f);

	Mesh obj;
	obj.Create(quadVerts, quadIndices, 32, 6);

	Texture albedo("Textures/Charred Brick Wall/Albedo_4K__vcinbbafw.jpg");
	Texture ao("Textures/Charred Brick Wall/AO_4K__vcinbbafw.jpg");
	Texture normal("Textures/Charred Brick Wall/Normal_4K__vcinbbafw.jpg");
	Texture roughness("Textures/Charred Brick Wall/Roughness_4K__vcinbbafw.jpg");

	Texture albedo2      ("Textures/Dirty Metal Sheet/Albedo_4K__vbsieik.jpg");
	Texture ao2          ("Textures/Dirty Metal Sheet/AO_4K__vbsieik.jpg");
	Texture normal2      ("Textures/Dirty Metal Sheet/Normal_4K__vbsieik.jpg");
	Texture roughness2   ("Textures/Dirty Metal Sheet/Roughness_4K__vbsieik.jpg");

	
	glm::vec3 lightAmbient(1.f);
	glm::vec3 lightDiffuse(1.f);
	glm::vec3 lightSpecular(1.f);

	while (window.IsOpen()) {

		window.Update();
		if (window.UpdateOnFocus()) {
			camera.mouseControl(window.GetXChange(), window.GetYChange());
			camera.keyControl(window.GetsKeys(), window.GetDeltaTime());
		}

		// GUI //
		{
			GuiLayer.Begin();

			ImGui::Begin("GUI");
			ImGui::Text("Application average %.2f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			ImGui::DragFloat3("lightAmbient",  (float*)&lightAmbient,  0.01, 0.f, 1.f);
			ImGui::DragFloat3("lightDiffusec", (float*)&lightDiffuse,  0.01, 0.f, 1.f);
			ImGui::DragFloat3("lightSpecular", (float*)&lightSpecular, 0.01, 0.f, 1.f);

			ImGui::DragFloat("Constant", (float*)pointLight.GetConstantPtr(), 0.01);
			ImGui::DragFloat("Linear", (float*)pointLight.GetLinearPtr(), 0.01);
			ImGui::DragFloat("Quadratic", (float*)pointLight.GetQuadraticPtr(), 0.01);
			ImGui::DragFloat("Intensity", (float*)pointLight.GetIntensityPtr(), 0.01);

			ImGui::ColorEdit3("Colour", (float*)pointLight.GetColourPtr());
			ImGui::DragFloat3("light", (float*)pointLight.GetPositionPtr(), 0.01);

			ImGui::End();
		} 

		shader.Bind();

		pointLight.SetUniforms(&shader);

		albedo.Bind(0);
		ao.Bind(1);
		normal.Bind(2);
		roughness.Bind(3);

		shader.Set1i(0, "u_AlbedoTexture");
		shader.Set1i(1, "u_AOTexture");
		shader.Set1i(2, "u_NormalTexture");
		shader.Set1i(3, "u_RoughnessTexture");

		shader.SetVec3f(camera.GetCameraPosition(), "u_cameraPosition");

		shader.SetMat4f(obj.GetModel(), "u_Model", false);
		shader.SetMat4f(camera.CalculateProjectionMatrix(window.GetBufferWidth(), window.GetBufferHeight()), "u_Projection", false);
		shader.SetMat4f(camera.CalculateViewMatrix(), "u_View", false);

		shader.SetVec3f(lightAmbient,  "u_lightAmbient");
		shader.SetVec3f(lightDiffuse,  "u_lightDiffuse");
		shader.SetVec3f(lightSpecular, "u_lightSpecular");

		obj.Render();
	
		shader.Unbind();
		
		
		GuiLayer.End();
		window.Clear();
	}


	return 0;

}