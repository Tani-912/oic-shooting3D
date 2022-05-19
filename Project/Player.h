#pragma once

#include "GameDefine.h"

// �ړ����x
#define PLAYER_SPEED		(0.1f)
#define MAX_SKILL           (100.0)

class CPlayer{
private:
	CMeshContainer	m_Mesh;
	CVector3		m_Pos;
	float			m_RotZ;
	float           m_Speed;
	float			SkillTimer;
public:
	CPlayer();
	~CPlayer();
	bool Load();
	void Initialize();
	void Update();
	void Render();
	void RenderDebugText();
	const CVector3 GetPosition(){ return m_Pos; }
	void Release();
};