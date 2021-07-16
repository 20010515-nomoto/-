//プレイヤークラスのインクルード
#include "CPlayer.h"
//キー入力クラスのインクルード
#include "CKey.h"
//
#include "CBullet.h"
//
#include "CTaskManager.h"
//
#include "CCollisionManager.h"
//
#include "CUtil.h"
//
#include "CEffect.h"
#include "CColliderMesh.h"
#include <Windows.h>
#include "CInput.h"
#include "CEnemy2.h"

CPlayer *CPlayer::spThis = 0;
CModel CPlayer::sModel;

#define PLAYER_MODEL "cube.obj","cube.mtl"

#define GRAVITY -0.5		//重力
#define ACCELERATION 0.005	//加速値
#define VELOCITYMAX 0.2		//移動スピード最大値
#define VELOCITYMIN -0.2	//移動スピード最小値
#define MOVEADJUST 0.7		//斜め移動時の移動量調整値
#define RUNSPEED 1.5		//ダッシュスピード
#define DODGESPEED 2		//回避スピード
#define STAMINA 100			//スタミナ上限
#define ATTACKCOUNT 60		//攻撃モーション中
#define ATTACK_STAMINA 40	//攻撃消費スタミナ
#define DEFENSE_STAMINA 40	//防御消費スタミナ
#define DODGE_STAMINA 80	//回避消費スタミナ
#define INVINCIBLE_TIME 60	//無敵時間
#define PLAYERHP 100		//プレイヤーHP
#define KNOCK_BACK_MAX 7.0		//ノックバック距離
#define KNOCK_BACK_RANGE 0.5	//ノックバック


CPlayer::CPlayer()
: mLine(this, &mMatrix, CVector(0.0f, 0.0f, -5.0f), CVector(0.0f, 0.0f, 5.0f))
, mLine2(this, &mMatrix, CVector(0.0f, 2.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f))
, mLine3(this, &mMatrix, CVector(5.0f, 0.0f, 0.0f), CVector(-5.0f, 0.0f, 0.0f))
, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.5f)
//, mAttackCollider(this,&mMatrix,CVector(0.0f,0.0f,0.0f),3.0f)
, mVelocityX(0.0)
, mVelocityZ(0.0)
, mXMoveRange(0.0)
, mZMoveRange(0.0)
, mMouseX(0)
, mMouseY(0)
, mStamina(0)
, mRun_Decision(false)
, mAttackCount(0)
, mDefense_Decision(false)
, mDodge_Decision(false)
, mAttack_Decision(false)
, mInvincible_Time(0)
, mPlayerHp(PLAYERHP)
, mSword(this)
, mKnock_Back(0)
{
	if (sModel.mTriangles.size() == 0){
		sModel.Load(PLAYER_MODEL);
	}
	mpModel = &sModel;
	mScale = CVector(1.0f, 1.0f, 1.0f);
	mPosition = CVector(0.0f, 1.0f, -3.0f);
	mRotation = CVector(0.0f, 180.0f, 0.0f);
	CTransform::Update();
	mTag = EPLAYER;	//タグの設定
	spThis = this;
	//テクスチャファイルの読み込み（1行64列）
	mText.LoadTexture("FontWhite.tga", 1, 64);
	//起動時のマウスカーソルの座標を覚える
	CInput::GetMousePos(&mMouseX, &mMouseY);
	mStamina = STAMINA;
	//mAttackCollider.mTag = CCollider::EPLAYER_ATTACK;
}

//更新処理
void CPlayer::Update() {
	//攻撃、防御確認のため色を白に設定
	mpModel->mpMaterials[0]->mDiffuse[0] = 1.0f;
	mpModel->mpMaterials[0]->mDiffuse[1] = 1.0f;
	mpModel->mpMaterials[0]->mDiffuse[2] = 1.0f;
	//マウスカーソルの座標を取得
	int px, py;
	CInput::GetMousePos(&px, &py);
	if (px < mMouseX) {
		//マウスの移動量の分だけ回転
		mRotation.mY += (mMouseX - px) / 15.0;
	}
	if (mMouseX < px) {
		//マウスの移動量の分だけ回転
		mRotation.mY += (mMouseX - px) / 15.0;
	}
	////これより下、移動処理////
	if (mAttack_Decision == false){
		//Aキー入力で左移動
		if (CKey::Push('A')) {
			if (mVelocityX < VELOCITYMAX){
				mVelocityX += ACCELERATION;
			}
		}
		else {
			if (mVelocityX>0.0f){
				mVelocityX -= ACCELERATION;
				if (mVelocityX<0.0f){
					mVelocityX = 0.0f;
				}
			}
		}
		//Dキー入力で右移動
		if (CKey::Push('D')) {
			if (mVelocityX > VELOCITYMIN){
				mVelocityX -= ACCELERATION;
			}
		}
		else {
			if (mVelocityX < 0.0f){
				mVelocityX += ACCELERATION;
				if (mVelocityX>0.0f){
					mVelocityX = 0.0f;
				}
			}
		}
		//Wキー入力で前移動
		if (CKey::Push('W')) {
			if (mVelocityZ < VELOCITYMAX){
				mVelocityZ += ACCELERATION;
			}
		}
		else {
			if (mVelocityZ>0.0f){
				mVelocityZ -= ACCELERATION;
				if (mVelocityZ<0.0f){
					mVelocityZ = 0.0f;
				}
			}
		}
		//Sキー入力で後ろ移動
		if (CKey::Push('S')) {
			if (mVelocityZ > VELOCITYMIN){
				mVelocityZ -= ACCELERATION;
			}
		}
		else {
			if (mVelocityZ<0.0f){
				mVelocityZ += ACCELERATION;
				if (mVelocityZ>0.0f){
					mVelocityZ = 0.0f;
				}
			}
		}
		mXMoveRange = mVelocityX;
		mZMoveRange = mVelocityZ;
		if (mKnock_Back>0){
			mZMoveRange -= KNOCK_BACK_RANGE;
			mKnock_Back -= KNOCK_BACK_RANGE;
		}
		else{
			mKnock_Back = 0;
		}
		if (CKey::Push('A') && CKey::Push('W') || CKey::Push('A') && CKey::Push('S')
			|| CKey::Push('D') && CKey::Push('W') || CKey::Push('D') && CKey::Push('S')){
			mPosition = CVector(mXMoveRange, 0.0f, mZMoveRange) * MOVEADJUST * mMatrix;
		}
		//shiftキーを押しながら移動でダッシュ
		if (CKey::Push(VK_SHIFT)){
			mRun_Decision = true;
			if (mStamina>0){
				mPosition = CVector(mXMoveRange, 0.0f, mZMoveRange) * RUNSPEED * mMatrix;
				mStamina--;
			}
			else{
				mPosition = CVector(mXMoveRange, 0.0f, mZMoveRange) * mMatrix;
			}
		}
		//スペースキーが押されたとき、回避
		else if (mDodge_Decision == true){
			mPosition = CVector(mXMoveRange, 0.0f, mZMoveRange) *DODGESPEED* mMatrix;
		}
		else {
			mPosition = CVector(mXMoveRange, 0.0f, mZMoveRange) * mMatrix;
			mRun_Decision = false;
		}
	}
	////移動処理終わり////

	////攻撃処理////
	if (CKey::Once(VK_LBUTTON) && mStamina >= ATTACK_STAMINA
		&&mAttack_Decision == false&&mDefense_Decision==false){
		mAttackCount = ATTACKCOUNT;
		mStamina -= ATTACK_STAMINA;
		mAttack_Decision = true;
	}
	if (mAttackCount > 0){
		mAttackCount--;
	}
	else{
		mAttack_Decision = false;
	}
	////攻撃処理終了////

	////防御処理(モデルが未定なため、防御中は色を変えてます※デバッグのみ)////
	if (CKey::Push(VK_RBUTTON)&&mAttack_Decision==false){
		mDefense_Decision = true;
	}
	else{
		mDefense_Decision = false;
	}
	if (mDefense_Decision == true){
		mpModel->mpMaterials[0]->mDiffuse[0] = 1.0f;
		mpModel->mpMaterials[0]->mDiffuse[1] = 0.3f;
		mpModel->mpMaterials[0]->mDiffuse[2] = 0.0f;
	}
	////防御処理終了////

	////回避処理(回避中の無敵時間は分かりやすいように色を変えてます※デバッグのみ)////
	if (CKey::Once(' ')&&mStamina>DODGE_STAMINA){
		mDodge_Decision = true;
		mStamina -= DODGE_STAMINA;
		mInvincible_Time = INVINCIBLE_TIME / 2;
	}
	if (mInvincible_Time > 0){
		mInvincible_Time--;
	}
	else {
		mDodge_Decision = false;
	}
	if (mDodge_Decision == true){
		mpModel->mpMaterials[0]->mDiffuse[0] = 0.0f;
		mpModel->mpMaterials[0]->mDiffuse[1] = 0.0f;
		mpModel->mpMaterials[0]->mDiffuse[2] = 1.0f;
	}

	if (mAttack_Decision == false && mDefense_Decision == false &&
		mDodge_Decision == false && mRun_Decision == false){
		if (mStamina < STAMINA){
			mStamina++;
		}
	}


	//CTransformの更新
	CTransform::Update();
}

void CPlayer::Collision(CCollider *m, CCollider *o) {
	//相手がサーチの時は戻る
	if (o->mTag == CCollider::ESEARCH)
	{
		return;
	}
	//自身のコライダタイプの判定
	switch (m->mType) {
	case CCollider::ELINE://線分コライダ
		//相手のコライダが三角コライダの時
		if (o->mType == CCollider::ETRIANGLE) {
			CVector adjust;//調整用ベクトル
			//三角形と線分の衝突判定
			CCollider::CollisionTriangleLine(o, m, &adjust);
			//位置の更新(mPosition + adjust)
			mPosition = mPosition - adjust * -1;
			//行列の更新
			CTransform::Update();
		}
		break;
	case CCollider::ESPHERE:
		if (m->mTag == CCollider::EBODY){
			//相手のコライダが球コライダの時
			if (mDodge_Decision == false){
				if (o->mType == CCollider::ESPHERE) {
					if (o->mTag == CCollider::EENEMY_ATTACK){
						if (CEnemy2::spThis->mAcquisitionFlg==true){
							if (mInvincible_Time <= 0){
								if (CCollider::Collision(m, o))
								{
									mInvincible_Time = INVINCIBLE_TIME;
									if (mDefense_Decision == true && mStamina >= DEFENSE_STAMINA){
										mStamina -= DEFENSE_STAMINA;
										mKnock_Back = KNOCK_BACK_MAX;
									}
									else{
										//エフェクト生成
										new CEffect(o->mpParent->mPosition, 3.0f, 3.0f, "exp.tga", 4, 4, 2);
										mPlayerHp -= 20;
										mKnock_Back = KNOCK_BACK_MAX;
									}
								}
							}
						}
					}
				}
			}
		}
		break;
	}
}
//衝突処理
void CPlayer::TaskCollision()
{
	//コライダの優先度変更
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	mCollider.ChangePriority();
	//mAttackCollider.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine3, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
	//CCollisionManager::Get()->Collision(&mAttackCollider, COLLISIONRANGE);
}

void CPlayer::Render()
{
	//親の描画処理
	CCharacter::Render();

	//2Dの描画開始
	CUtil::Start2D(-400, 400, -300, 300);
	//描画色の設定（緑色の半透明）
	glColor4f(0.0f, 1.0f, 0.0f, 0.4f);
	//文字列編集エリアの作成
	char buf[64];

	//Y座標の表示
	//文字列の設定
	//sprintf(buf, "X_SPEED:%f",mXMoveRange );
	//文字列の描画
	//mText.DrawString(buf, 100, 30, 8, 16);

	//X軸回転値の表示
	//文字列の設定
	//sprintf(buf, "Z_SPEED:%f", mZMoveRange);
	//文字列の描画
	//mText.DrawString(buf, 100, 0, 8, 16);
	//
	////Y軸回転値の表示
	////文字列の設定
	//sprintf(buf, "RY:%7.2f", mRotation.mY);
	////文字列の描画
	//mText.DrawString(buf, 100, -100, 8, 16);
	sprintf(buf, "STAMINA%d", mStamina);
	mText.DrawString(buf, 100, -100, 8, 16);

	sprintf(buf, "HP%d", mPlayerHp);
	mText.DrawString(buf, 100, -120, 8, 16);

	//2Dの描画終了
	CUtil::End2D();

}