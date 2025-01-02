#pragma once
#include "Scene.h"
#include "Renderer.h"

class ModelViewProjection : public Scene {
	void Init() override;
	void Update(Renderer* gRenderer) override;
	void Close() override;
};