#ifndef CPLAYER_H
#define CPLAYER_H
//キャラクタクラスのインクルード
#include "CCharacter.h"
//
#include "CBullet.h"
//
#include "CColliderLine.h"
//
#include "CText.h"
#include <Windows.h>
/*
プレイヤークラス
キャラクタクラスを継承
*/
class CPlayer : public CCharacter {
public:
	CCollider mCollider;
	CText mText;
	static CPlayer *spThis;
	CColliderLine mLine; //線分コライダ
	CColliderLine mLine2;
	CColliderLine mLine3;
	//デフォルトコンストラクタ
	CPlayer();
//	CBullet bullet;
	//更新処理
	void Update();
	//衝突処理
	void Collision(CCollider *m, CCollider *o);
	//衝突処理
	void TaskCollision();
	//描画処理
	void Render();

	float mVelocityX;		//速度
	float mVelocityZ;		//速度
	float mXMoveRange;	//X軸移動量
	float mZMoveRange;	//Z軸移動量

};

#endif

