#pragma once

#include "GameDefine.h"

class CEnemyShot{
private:
	CMeshContainer*		m_pMesh;	//ショットのメッシュポインタ
	Vector3				m_Pos;		//座標
	Vector3				m_Spd;		//移動ベクトル
	bool				m_bShow;	//表示状態
	float				m_RotY;		//Y回転角度（ラジアン）
public:
	CEnemyShot();
	~CEnemyShot();
	
	
	/**
	 * 初期化
	 *
	 */
	void Initialize();
	
	/**
	 * 発射
	 *
	 */
	void Fire(const Vector3& p,const Vector3& s);
	
	/**
	 * 更新
	 *
	 */
	void Update();
	
	/**
	 * 描画
	 *
	 */
	void Render();
	
	/**
	 * デバッグ表示
	 *
	 */
	void RenderDebug();
	
	/**
	 * ショットのメッシュを設定する
	 *
	 */
	void SetMesh(CMeshContainer* pm){ m_pMesh = pm; }
	
	/**
	 * 表示状態取得
	 *  戻り値がtrueなら表示中、falseなら非表示
	 */
	bool GetShow(){ return m_bShow; }
	
	/**
	 * 表示状態切り替え
	 *
	 */
	void SetShow(bool bs){ m_bShow = bs; }
	
	/**
	 * 衝突球を返す
	 *
	 */
	CSphere GetSphere(){ return CSphere(m_Pos,0.2f); }
	
	
	/**
	 * 弾の配列から非表示のものを検索する
	 *
	 */
	static CEnemyShot* FindAvailableShot(CEnemyShot* shot,int smax);
};