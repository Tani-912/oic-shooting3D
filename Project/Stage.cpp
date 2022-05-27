#include "Stage.h"

/**
 * コンストラクタ
 *
 */
CStage::CStage():
m_BG(),
m_Scroll(0.0f),
m_pEnemyStart(NULL),
m_EnemyMesh(),
m_EnemyNo(0){
}

/**
 * デストラクタ
 *
 */
CStage::~CStage(){
}

/**
 * 読み込み
 *
 */
bool CStage::Load(){
	if (!m_BG.CreateSprite("road.png"))
		return false;

	if (!m_EnemyMesh.Load("Enemy.mom"))
		return false;

	return true;
}

/**
 * 初期化
 *
 */
void CStage::Initialize(ENEMYSTART *pSt){
	m_BG.m_Angle.x = MOF_MATH_HALFPI;
	m_BG.m_Angle.y = MOF_MATH_HALFPI;
	m_BG.m_Scale = Vector3(200, 200, 200);
	m_BG.m_Position.y = -15;
	m_BG.SetImageRect(0, 0, m_BG.GetTexture()->GetWidth() * 4, m_BG.GetTexture()->GetHeight() * 4);
	m_Scroll = 0;
	m_EnemyNo = 0;
	m_pEnemyStart = pSt;
}

/**
 * 更新
 *
 */
void CStage::Update(CEnemy* ene,int ecnt){
	m_Scroll += SCROLL_SPEED;

	if (m_EnemyNo < m_pEnemyStart->Count && m_Scroll >= m_pEnemyStart->Scroll[m_EnemyNo]) {
		for (int i = 0; i < ecnt; i++) {
			if (ene[i].GetShow())
				continue;

			ene[i].SetMesh(&m_EnemyMesh);
			ene[i].Start(Vector3(m_pEnemyStart->PosX[m_EnemyNo], 0, 0));
			break;
		}
		m_EnemyNo++;
	}

	if (m_Scroll > 1100) {
		m_Scroll = 0;
		m_EnemyNo = 0;
	}
}

/**
 * 描画
 *
 */
void CStage::Render(){
	for (int i = 0; i < 2; i++) {
		m_BG.m_Position.z = 50.0f + fmodf(-m_Scroll, m_BG.m_Scale.z) + i * m_BG.m_Scale.z;
		m_BG.Update();
		m_BG.Render();
	}
}

/**
 * デバッグ文字描画
 *
 */
void CStage::RenderDebugText(){
	//スクロール値の描画
	CGraphicsUtilities::RenderString(10,10,MOF_XRGB(0,0,0),
		"スクロール : %.0f","敵の出現番号：%d",m_Scroll,m_EnemyNo);
}

/**
 * 解放
 *
 */
void CStage::Release(){
	m_BG.Release();
	m_EnemyMesh.Release();
}
