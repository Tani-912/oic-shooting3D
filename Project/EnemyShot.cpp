#include "EnemyShot.h"

/**
 * コンストラクタ
 *
 */
CEnemyShot::CEnemyShot() :
m_pMesh(NULL),
m_Pos(0,0,0),
m_Spd(0,0,0),
m_bShow(false),
m_RotY(0){
}

/**
 * デストラクタ
 *
 */
CEnemyShot::~CEnemyShot(){
}

/**
 * 初期化
 *
 */
void CEnemyShot::Initialize(){
	m_Pos = Vector3(0,0,0);
	m_Spd = Vector3(0,0,0);
	m_bShow = false;
	m_RotY = 0;
}

/**
 * 発射
 *
 */
void CEnemyShot::Fire(const Vector3& p,const Vector3& s){
	m_Pos = p;
	m_Spd = s;
	m_bShow = true;
	m_RotY = 0;
}

/**
 * 更新
 *
 */
void CEnemyShot::Update(){
	// 非表示
	if(!m_bShow)
	{
		return;
	}
	// 速度を利用した移動
	m_Pos += m_Spd;
	// 画面外で消去
	float inflate = 1.0f;
	if (m_Pos.x < -FIELD_HALF_X - inflate || FIELD_HALF_X + inflate < m_Pos.x ||
		m_Pos.z < -FIELD_HALF_Z - inflate || FIELD_HALF_Z + inflate < m_Pos.z) {
		m_bShow = false;
	}
	// 弾を回転
	m_RotY += MOF_ToRadian(10);
}

/**
 * 描画
 *
 */
void CEnemyShot::Render(){
	if(!m_bShow)
	{
		return;
	}
	CMatrix44 matWorld;
	//回転を反映
	matWorld.RotationY(m_RotY);
	//座標を設定
	matWorld.SetTranslation(m_Pos);
	//描画
	m_pMesh->Render(matWorld);
}

/**
 * デバッグ表示
 *
 */
void CEnemyShot::RenderDebug(void){
	if(!m_bShow)
	{
		return;
	}
	
	CGraphicsUtilities::RenderSphere(GetSphere(),Vector4(1,0,0,0.3f));
}

/**
 * 弾の配列から非表示のものを検索する
 *
 */
CEnemyShot* CEnemyShot::FindAvailableShot(CEnemyShot* shot,int smax){
	for (int i = 0; i < smax; i++) {
		if (!shot[i].GetShow()) {
			return &shot[i];
		}
	}
	return NULL;
}