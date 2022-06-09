#pragma once

#include "GameDefine.h"

class CEnemy{
private:
	CMeshContainer*		m_pMesh;
	Vector3				m_Pos;
	Vector3				m_Rot;
	bool				m_bShow;

	float				m_AnimTime;

	int					m_HP;

	int					m_ShotWait;
	int					m_ShotWaitSet;
	int					m_TargetPos;
public:
	CEnemy();
	~CEnemy();
	void Initialize();
	void Start(const Vector3& p);
	void Update();
	void Render();
	void RenderDebugText(int i);
	void SetMesh(CMeshContainer* pm){ m_pMesh = pm; }
	bool GetShow(){ return m_bShow; }

	void RenderDebug();
	CSphere GetSphere() { return CSphere(m_Pos,0.5f); }
	void Damage(int dmg);
	void SetTagetPos(const Vector3& t) { m_TargetPos = t; }
};