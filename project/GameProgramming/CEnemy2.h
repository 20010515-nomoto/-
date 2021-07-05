#ifndef CENEMY2_H
#define CENEMY2_H
//キャラクタクラスのインクルード
#include "CCharacter.h"
//コライダクラスのインクルード
#include "CCollider.h"
/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemy2 : public CCharacter {
public:
	int mFireCount;

	CVector mPoint;	//目標地点

	int mHp;	//ヒットポイント

	CCharacter *mpPlayer;	//プレイヤーのポインタ

	//モデルデータ
	static CModel mModel;

	static CEnemy2 *spThis;

	//コライダ
	CCollider mCollider;
	CCollider mColSearch;	//サーチ用コライダ
	CCollider mColAttackRange;	//攻撃範囲用コライダ
	CCollider mColAttack;	//攻撃判定用コライダ

	//コンストラクタ
	CEnemy2();
	//CEnemy(位置, 回転, 拡縮)
	CEnemy2(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ1, コライダ2)
	void Collision(CCollider *m, CCollider *o);
	void TaskCollision();

	bool mAcquisitionFlg;	//捕捉フラグ:敵がプレイヤーを捕捉しているかどうか true:している
	bool mAttackFlg;	//攻撃フラグ:true 攻撃する
};

#endif
