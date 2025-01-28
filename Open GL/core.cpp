#include "core.h"
#include <iostream>
#include <stdio.h>

#include "IMGui/imgui.h"
#include "IMGui/imgui_impl_glfw.h"
#include "IMGui/imgui_impl_opengl3.h"


GLFWwindow* Init(int Width, int Height)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* win = NULL;
	win = glfwCreateWindow(Width, Height, "Tests Showcase", NULL, NULL);

	if (win == NULL) {
		printf("Null");
		return nullptr;
	}

	glfwMakeContextCurrent(win);
	glfwSwapInterval(2);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("GLAD not initialized");
		return nullptr;
	}
	return win;
}

void InitImGui(GLFWwindow* Window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	ImGui_ImplGlfw_InitForOpenGL(Window, true);
	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();

}

int UpdateImGui(float* color, char SceneName[][32], int TotalScenes) {
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Your Options");

	//ImGui::SliderFloat3("float A", &transA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//ImGui::SliderFloat3("float B", &transB.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("clear color", color, ImGuiColorEditFlags_NoInputs); // Edit 3 floats representing a color
	
	static int selected = -1;
	int changed = 0;
	/*for (int i = 0; i < TotalScenes; i++)
	{
		if (ImGui::Selectable(SceneName[i], selected == i)) {
			changed =1;
			selected = i;
		}
	}*/
	
	int isO = 0;
	static int lastOpened = -1;
	/*static bool once = true;
	int WorkingScene = 7;*/
	for (int i = 0; i < TotalScenes; i++)
	{
		int flagOpen = ImGui::CollapsingHeader(SceneName[i]);

		if (flagOpen && lastOpened != i) {
			//Close the last Opened
			ImGui::SetNextItemOpen(false);
			ImGui::CollapsingHeader(SceneName[lastOpened]);
			lastOpened = i;
		}

		if(flagOpen /* || (once and i == WorkingScene) */ )
		{
			SceneManager& gSceneManager = SceneManager::GetManager();
			gSceneManager.UpdateImGui();
			if (selected != i) {
				changed = 1;
				selected = i;
			}
			//once = false;
		}
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	
	

	ImGui::End();

	
	//ImGui::ShowDemoWindow();

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	if (changed) return selected;
	else return -1;
}

void Close() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}
