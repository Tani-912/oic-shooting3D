#pragma once

#include "GameDefine.h"

// スクロール速度
#define		SCROLL_SPEED	(1.0f)

class CStage{
private:
	CSprite3D			m_BG;
	float				m_Scroll;
public:
	CStage();
	~CStage();
	bool Load();
	void Initialize();
	void Update();
	void Render();
	void RenderDebugText();
	void Release();
};