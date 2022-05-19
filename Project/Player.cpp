#include "Player.h"

/**
 * コンストラクタ
 */
CPlayer::CPlayer() :
m_Mesh(),
m_Pos(0.0f,0.0f,0.0f),
m_RotZ(0.0f),
m_Speed(0.0f),
SkillTimer(0.0f){
}

/**
 * デストラクタ
 */
CPlayer::~CPlayer(){
}

/**
 * 読み込み
 */
bool CPlayer::Load(void){
	// メッシュの読み込み
	if (!m_Mesh.Load("player.mom"))
		return false;

	return true;
}

/**
 * 初期化
 */
void CPlayer::Initialize(void){
	m_Pos = Vector3(0.0f, 0.0f, -FIELD_HALF_Z + 2.0f);
	m_RotZ = 0;
	m_Speed = 0.1f;

	SkillTimer = 100.0;
}

/**
 * 更新
 */
void CPlayer::Update(void){
	float Roll = 0;

	float RotSpeed;// = MOF_ToRadian(10);

	

	if (g_pInput->IsKeyHold(MOFKEY_LSHIFT)&&SkillTimer>0) {
		m_Speed = 0.3f;
		RotSpeed = MOF_ToRadian(20);
		SkillTimer -= 1.0;
	}
	else 
	{
		m_Speed = PLAYER_SPEED;
		RotSpeed = MOF_ToRadian(10);
		
		if(SkillTimer<MAX_SKILL)
		SkillTimer += 0.5;
	}


	if (g_pInput->IsKeyHold(MOFKEY_LEFT)) {
		m_Pos.x = max(m_Pos.x - m_Speed, -FIELD_HALF_X);
		Roll -= MOF_MATH_PI;
	}

	if (g_pInput->IsKeyHold(MOFKEY_RIGHT)) {
		m_Pos.x = min(m_Pos.x + m_Speed, FIELD_HALF_X);
		Roll += MOF_MATH_PI;
	}

	if (g_pInput->IsKeyHold(MOFKEY_UP)) {
		m_Pos.z = min(m_Pos.z + m_Speed, FIELD_HALF_Z);
	}

	if (g_pInput->IsKeyHold(MOFKEY_DOWN)) {
		m_Pos.z = max(m_Pos.z - m_Speed, -FIELD_HALF_Z);
	}

	//float RotSpeed = MOF_ToRadian(10);
	if (Roll == 0)
		RotSpeed = min(abs(m_RotZ) * 0.1f, RotSpeed);

	if (abs(m_RotZ) <= RotSpeed || signbit(m_RotZ) != signbit(Roll))
		m_RotZ += Roll;

	m_RotZ -= copysignf(min(RotSpeed, abs(m_RotZ)), m_RotZ);
}

/**
 * 描画
 */
void CPlayer::Render(void){
	CMatrix44 matWorld;
	matWorld.RotationZ(m_RotZ);
	matWorld.SetTranslation(m_Pos);
	m_Mesh.Render(matWorld);

	for (int i = 0; i < SkillTimer; i++) {
		CGraphicsUtilities::RenderString(800+i, 0, MOF_XRGB(0, 0, 0),
			"|");
	}
}

/**
 * デバッグ文字描画
 */
void CPlayer::RenderDebugText(void){
	// 位置の描画
	CGraphicsUtilities::RenderString(10,40,MOF_XRGB(0,0,0),
			"プレイヤー位置 X : %.1f , Y : %.1f , Z : %.1f",m_Pos.x,m_Pos.y,m_Pos.z);

	
}

/**
 * 解放
 */
void CPlayer::Release(void){
	m_Mesh.Release();
}