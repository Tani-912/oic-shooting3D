/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	基本ゲームアプリ。

															@author	濱田　享
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"
#include	"Player.h"
#include    "Stage.h"
#include    "Stage1.h"


CStage		gStage;

CCamera		gCamera;
CDirectionalLight	gLight;
CPlayer		gPlayer;
#define		ENEMY_COUNT	 (20)
CEnemy		gEnemyArray[ENEMY_COUNT];
bool	gbDebug = false;

CVector3	gCameraPos;
CVector3	gTargetPos;
CVector3	gUpVecter;


/*************************************************************************//*!
		@brief			アプリケーションの初期化
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Initialize(void){
	//リソース配置ディレクトリの設定
	CUtilities::SetCurrentDirectory("Resource");

	gCameraPos = Vector3(0, 6.0f, -17.0f);
	gTargetPos = Vector3(0, 0, -10);
	gUpVecter = Vector3(0, 1, 0);


	gCamera.SetViewPort();
	gCamera.LookAt(Vector3(0, 6.0f, -17.0f),//カメラポジション
		Vector3(0, 0, -10),//ターゲットポジション
		Vector3(0, 1, 0));//アップベクタ

	
	gCamera.PerspectiveFov(MOF_ToRadian(60.0f), 1024.0f / 768.0f, 0.01f, 1000.0f);
	CGraphicsUtilities::SetCamera(&gCamera);

	gLight.SetDirection(Vector3(-1, -2, 1.5f));
	gLight.SetDiffuse(MOF_COLOR_WHITE);
	gLight.SetAmbient(MOF_COLOR_HWHITE);
	gLight.SetSpeculer(MOF_COLOR_WHITE);
	CGraphicsUtilities::SetDirectionalLight(&gLight);

	gPlayer.Load();

	gStage.Load();

	gPlayer.Initialize();

	gStage.Initialize(&gStg1EnemyStart);
	for (int i = 0; i < ENEMY_COUNT; i++) {
		gEnemyArray[i].Initialize();
	}


	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの更新
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Update(void){
	//キーの更新
	g_pInput->RefreshKey();

	gStage.Update(gEnemyArray,ENEMY_COUNT);
	gPlayer.Update();
	for (int i = 0; i < ENEMY_COUNT; i++) {
		gEnemyArray[i].Update();
	}

	for (int i = 0; i < ENEMY_COUNT; i++) {
		gPlayer.CollisionEnemy(gEnemyArray[i]);
	}

	if (g_pInput->IsKeyPush(MOFKEY_F1))
		gbDebug = ((gbDebug) ? false : true);

	if (g_pInput->IsKeyPush(MOFKEY_RETURN) && gPlayer.IsDead()) {
		gPlayer.Initialize();
		gStage.Initialize(&gStg1EnemyStart);
		for (int i = 0; i < ENEMY_COUNT; i++) {
			gEnemyArray[i].Initialize();
		}
	}

	float posX = gPlayer.GetPosition().x * 0.4f;
	CVector3 cpos = gCamera.GetViewPosition();
	CVector3 tpos = gCamera.GetTargetPosition();
	CVector3 vup = CVector3(0, 1, 0);
	cpos.x = posX;
	tpos.x = posX;
	vup.RotationZ(gPlayer.GetPosition().x / FIELD_HALF_X * MOF_ToRadian(10));
	gCamera.LookAt(cpos, tpos, vup);
	gCamera.Update();

	return TRUE;
}

/*************************************************************************//*!
		@brief			アプリケーションの描画
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//描画処理
	g_pGraphics->RenderStart();
	// 画面のクリア
	g_pGraphics->ClearTarget(0.65f,0.65f,0.67f,0.0f,1.0f,0);

	g_pGraphics->SetDepthEnable(true);

	gStage.Render();

	gPlayer.Render();

	for (int i = 0; i < ENEMY_COUNT; i++) {
		gEnemyArray[i].Render();
	}

	if (gbDebug) {
		CMatrix44 matWorld;
		matWorld.Scaling(FIELD_HALF_X * 2, 1, FIELD_HALF_Z * 2);
		CGraphicsUtilities::RenderPlane(matWorld, Vector4(1, 1, 1, 0.4f));

		gPlayer.RenderDebug();
		for (int i = 0; i < ENEMY_COUNT; i++) {
			gEnemyArray[i].RenderDebug();
		}
	}

	g_pGraphics->SetDepthEnable(false);

	if (gbDebug) {
		gPlayer.RenderDebugText();
		gStage.RenderDebugText();
		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			gEnemyArray[i].RenderDebugText(i);
		}
	}

	if (gPlayer.IsDead()) {
		CGraphicsUtilities::RenderString(240, 350, MOF_COLOR_RED, "ゲームオーバー　：　Enterキーでもう一度最初から");
	}

	// 描画の終了
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの解放
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Release(void){
	gStage.Release();
	gPlayer.Release();
	return TRUE;
}