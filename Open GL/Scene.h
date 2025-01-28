#pragma once
#include <memory>
#include "Renderer.h"

class Scene
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void UpdateImGui() { };
	virtual void Close() = 0;
};

class SceneManager {
private:
	std::unique_ptr<Scene> currScene = nullptr;
	SceneManager() {};
	static SceneManager a;

public:
	inline static SceneManager& GetManager(){ return a; }

	inline bool Init() {
		if (currScene!=nullptr) { 
			currScene->Init(); 
			return true; }
		else {
			printf("Scene not initialized as pointer set to NULL\n");
			return false;
		}
	}
	inline void Update() {
		if (currScene != nullptr) currScene->Update();
	}
	inline void UpdateImGui() {
		if (currScene != nullptr) currScene->UpdateImGui();
	}
	void ChangeScene(std::unique_ptr<Scene> NewScene) {
		if(currScene!=nullptr) currScene->Close();
		currScene = std::move( NewScene);
		currScene->Init();
	}
};

