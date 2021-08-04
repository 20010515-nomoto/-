#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"

class CXEnemy :public CXCharacter{
public:
	CXEnemy();
	void Init(CModelX *model);
	void Update();
	CCollider mEnemyColSphereBody;	//体
	CCollider mEnemyColSphereHead;	//頭
	CCollider mEnemyColSphereSword0;	//剣
	CCollider mEnemyColSphereSword1;	//剣
	CCollider mEnemyColSphereSword2;	//剣

	CCollider mColSearch;	//サーチ用コライダ
	CCollider mColAttack_Range;	//攻撃範囲

	//衝突処理
	void Collision(CCollider *m, CCollider *o);
	void TaskCollision();

	bool mAcquisitionFlg;	//捕捉フラグ:敵がプレイヤーを捕捉しているかどうか true:している
	bool mAttackFlg;	//攻撃フラグ:true 攻撃する

	int mHp;	//ヒットポイント
	CCharacter *mpPlayer;	//プレイヤーのポインタ
	CVector mPoint;	//目標地点

	static CXEnemy *spThis;
};

#endif