#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CCollider.h"
#include "CShield.h"
class CXPlayer :public CXCharacter{
public:
	//更新処理
	void Update();
	void Init(CModelX *model);
	//デフォルトコンストラクタ
	CXPlayer();
	//コライダの宣言
	CCollider mColSphereBody;	//体
	CCollider mColSphereHead;	//頭
	CCollider mColSphereSword;	//剣

	CShield mShield;

	//起動時のマウスカーソルの座標を覚える
	int mMouseX;
	int mMouseY;
	static CXPlayer *spThis;

	float mVelocityX;		//速度
	float mVelocityZ;		//速度
	float mXMoveRange;	//X軸移動量
	float mZMoveRange;	//Z軸移動量
	int mStamina;		//スタミナ：各行動時に消費
	bool mRun_Decision;	//ダッシュ判定	true:走っている

	int mAttackCount;	//攻撃の判定時間
	bool mAttack_Decision;	//攻撃中判定 true:攻撃中
	bool mDefense_Decision;	//防御中判断 true:防御中

	bool mDodge_Decision;	//回避中判断 true:回避中
	int mInvincible_Time;	//無敵時間

	int mPlayerHp;	//HP
	float mKnock_Back;		//ノックバック距離

};


#endif