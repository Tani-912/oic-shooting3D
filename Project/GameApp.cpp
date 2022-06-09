/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	��{�Q�[���A�v���B

															@author	�_�c�@��
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
		@brief			�A�v���P�[�V�����̏�����
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Initialize(void){
	//���\�[�X�z�u�f�B���N�g���̐ݒ�
	CUtilities::SetCurrentDirectory("Resource");

	gCameraPos = Vector3(0, 6.0f, -17.0f);
	gTargetPos = Vector3(0, 0, -10);
	gUpVecter = Vector3(0, 1, 0);


	gCamera.SetViewPort();
	gCamera.LookAt(Vector3(0, 6.0f, -17.0f),//�J�����|�W�V����
		Vector3(0, 0, -10),//�^�[�Q�b�g�|�W�V����
		Vector3(0, 1, 0));//�A�b�v�x�N�^

	
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
		@brief			�A�v���P�[�V�����̍X�V
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Update(void){
	//�L�[�̍X�V
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
		@brief			�A�v���P�[�V�����̕`��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//�`�揈��
	g_pGraphics->RenderStart();
	// ��ʂ̃N���A
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
		CGraphicsUtilities::RenderString(240, 350, MOF_COLOR_RED, "�Q�[���I�[�o�[�@�F�@Enter�L�[�ł�����x�ŏ�����");
	}

	// �`��̏I��
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̉��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Release(void){
	gStage.Release();
	gPlayer.Release();
	return TRUE;
}