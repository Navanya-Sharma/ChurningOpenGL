#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "Scene.h"

GLFWwindow* Init(int Width, int Height);

void InitImGui(GLFWwindow* Window);

void Close();

int UpdateImGui(float* color, char SceneName[][32], int TotalScenes, SceneManager* gSceneManager);