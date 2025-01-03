#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"

//#include "IMGui/imgui.h"
//#include "IMGui/imgui_impl_glfw.h"
//#include "IMGui/imgui_impl_opengl3.h"

//#include "VertexBuffer.h"
//#include "BufferLayout.h"
//#include "IndexBuffer.h"
//#include "VertexArray.h"
//#include "Shader.h"
//#include "Texture.h"

#include "Renderer.h"
#include "core.h"
#include "Scene.h"
#include "RainbowSquare.h"
#include "ModelViewProjection.h"
#include "ChaosGame.h"

int main() {
	const int SCREEN_WIDTH = 1200;
	const int SCREEN_HEIGHT = 800;

	GLFWwindow* Window = Init(SCREEN_WIDTH, SCREEN_HEIGHT);

	InitImGui(Window);

	Renderer gRenderer;
	SceneManager gSceneManager;

	float color[4] = { 0.45f, 0.55f, 0.60f, 1.00f };
	const int TotalScenes = 3;
	char SceneNames[TotalScenes][32] = { "Rainbow Square", "Model View Projection", "Chaos Game" };
	//gSceneManager.ChangeScene(std::make_unique<ChaosGame>()); 

	while (!glfwWindowShouldClose(Window))
	{
		gRenderer.SetClearColor(color);
		gRenderer.Clear();

		gSceneManager.Update(&gRenderer);

		static int SceneNo;
		SceneNo = UpdateImGui(color, SceneNames, TotalScenes);

		if (SceneNo != -1) {
			switch (SceneNo)
			{
			case 0: gSceneManager.ChangeScene(std::make_unique<RainbowSquare>()); break; //Since RainbowSquare inherits from Scene, this can be implicitly converted to a unique_ptr<Scene> Polymorphism
			case 1: gSceneManager.ChangeScene(std::make_unique<ModelViewProjection>()); break;
			case 2: gSceneManager.ChangeScene(std::make_unique<ChaosGame>()); break;
			}
		}

		glfwSwapBuffers(Window);
		glfwPollEvents();
	}

	Close();
}