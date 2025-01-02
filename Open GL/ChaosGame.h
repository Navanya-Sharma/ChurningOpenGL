#pragma once
#include "Scene.h"
#include "Renderer.h"


class ChaosGame : public Scene {
	void Init() override;
	void Update(Renderer* gRenderer) override;
	void Close() override;
};