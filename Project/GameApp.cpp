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
#include	"Boss.h"


CStage		gStage;

CCamera		gCamera;
CDirectionalLight	gLight;
CPlayer		gPlayer;
#define		ENEMY_COUNT	 (20)
CEnemy		gEnemyArray[ENEMY_COUNT];
bool	gbDebug = false;

#define ENEMYSHOT_COUNT (200)
CEnemyShot	gShotArray[ENEMYSHOT_COUNT];
CMeshContainer	gEnemyShotMesh;

CVector3	gCameraPos;
CVector3	gTargetPos;
CVector3	gUpVecter;

// ボス
CBoss				gBoss;
// 雑魚全滅フラグ
bool				gbEnemyDestroyed = false;
// ステージクリアフラグ
bool				gbClear = false;

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

	if (!gEnemyShotMesh.Load("eshot.mom"))
		return false;

	// ボスの素材読み込み
	gBoss.Load();


	gPlayer.Initialize();

	gStage.Initialize(&gStg1EnemyStart);
	for (int i = 0; i < ENEMY_COUNT; i++) {
		gEnemyArray[i].Initialize();
	}

	for (int i = 0; i < ENEMYSHOT_COUNT; i++) {
		gShotArray[i].Initialize();
		gShotArray[i].SetMesh(&gEnemyShotMesh);
	}

	// ボスの状態初期化
	gBoss.Initialize();

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
		gEnemyArray[i].SetTagetPos(gPlayer.GetPosition());
		gEnemyArray[i].Update(gShotArray,ENEMYSHOT_COUNT);
	}

	// ボスの更新
	gBoss.SetTargetPos(gPlayer.GetPosition());
	gBoss.Update(gShotArray, ENEMYSHOT_COUNT);

	for (int i = 0; i < ENEMYSHOT_COUNT; i++) {
		gShotArray[i].Update();
	}

	for (int i = 0; i < ENEMY_COUNT; i++) {
		gPlayer.CollisionEnemy(gEnemyArray[i]);
	}

	for (int i = 0; i < ENEMYSHOT_COUNT; i++) {
		gPlayer.CollisionEnemyShot(gShotArray[i]);
	}

	// ボスとの当たり判定
	gPlayer.CollisionBoss(gBoss);

	if (g_pInput->IsKeyPush(MOFKEY_F1))
		gbDebug = ((gbDebug) ? false : true);

	// 雑魚全滅判定
	if (!gbEnemyDestroyed && gStage.IsAllEnemy())
	{
		bool btmp = true;
		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			if (gEnemyArray[i].GetShow())
			{
				btmp = false;
			}
		}
		gbEnemyDestroyed = btmp;
		// 全滅していればボス出現
		if (gbEnemyDestroyed)
		{
			gBoss.Start();
			gBoss.SetTargetPos(gPlayer.GetPosition());
			gBoss.Update(gShotArray, ENEMYSHOT_COUNT);
		}
	}
	// ゲームクリア判定
	if (!gbClear && gbEnemyDestroyed && !gBoss.GetShow())
	{
		gbClear = true;
	}

	if (g_pInput->IsKeyPush(MOFKEY_RETURN) && (gPlayer.IsDead() || gbClear))
	{
		// クリアフラグを戻す
		gbEnemyDestroyed = false;
		gbClear = false;
		gPlayer.Initialize();
		gStage.Initialize(&gStg1EnemyStart);
		for (int i = 0; i < ENEMY_COUNT; i++) {
			gEnemyArray[i].Initialize();
		}

		for (int i = 0; i < ENEMY_COUNT; i++) {
			gShotArray[i].Initialize();
		}
		gBoss.Initialize();
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

	// ボスの描画
	gBoss.Render();

	for (int i = 0; i < ENEMY_COUNT; i++) {
		gShotArray[i].Render();
	}

	if (gbDebug) {
		CMatrix44 matWorld;
		matWorld.Scaling(FIELD_HALF_X * 2, 1, FIELD_HALF_Z * 2);
		CGraphicsUtilities::RenderPlane(matWorld, Vector4(1, 1, 1, 0.4f));

		gPlayer.RenderDebug();
		for (int i = 0; i < ENEMY_COUNT; i++) {
			gEnemyArray[i].RenderDebug();
		}

		// ボスのデバッグ描画
		gBoss.RenderDebug();

		for (int i = 0; i < ENEMY_COUNT; i++) {
			gShotArray[i].RenderDebug();
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
	else if (gbClear)
	{
		CGraphicsUtilities::RenderString(240, 350, MOF_COLOR_RED, "ゲームクリア　：　Enterキーでもう一度最初から");
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
	gEnemyShotMesh.Release();
	gBoss.Release();
	return TRUE;
}