#pragma once
#include "Scene.h"

class CGameScene : public CScene
{
public:
	CGameScene();
	virtual ~CGameScene();

	virtual void Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
};