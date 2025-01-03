#pragma once
#include <memory>
#include "Renderer.h"

class Scene
{
public:
	virtual void Init() = 0;
	virtual void Update(Renderer* gRenderer) = 0;
	virtual void Close() = 0;
};

class SceneManager {
private:
	std::unique_ptr<Scene> currScene = nullptr;
public:

	inline bool Init() {
		if (currScene!=nullptr) { 
			currScene->Init(); 
			return true; }
		else {
			printf("Scene not initialized as pointer set to NULL\n");
			return false;
		}
	}
	inline void Update(Renderer* gRenderer) {
		if (currScene!=nullptr) currScene->Update(gRenderer);
	}
	void ChangeScene(std::unique_ptr<Scene> NewScene) {
		if(currScene!=nullptr) currScene->Close();
		currScene = std::move( NewScene);
		currScene->Init();
	}
};

