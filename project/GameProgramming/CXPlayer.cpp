#include "CXPlayer.h"
#include "CKey.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "CXEnemy.h"
#include "CRes.h"

CXPlayer *CXPlayer::spThis = 0;

#define ACCELERATION 0.003	//加速値
#define VELOCITYMAX 0.15		//移動スピード最大値
#define VELOCITYMIN -0.15	//移動スピード最小値
#define MOVEADJUST 0.5		//斜め移動時の移動量調整値
#define RUNSPEED 1.1		//ダッシュスピード
#define DODGESPEED 2		//回避スピード
#define STAMINA 100			//スタミナ上限
#define ATTACKCOUNT 20		//攻撃モーション中
#define ATTACK_STAMINA 40	//攻撃消費スタミナ
#define DEFENSE_STAMINA 40	//防御消費スタミナ
#define DODGE_STAMINA 80	//回避消費スタミナ
#define INVINCIBLE_TIME 60	//無敵時間
#define PLAYERHP 100		//プレイヤーHP
#define KNOCK_BACK_MAX 7.0		//ノックバック距離
#define KNOCK_BACK_RANGE 0.5	//ノックバック

CXPlayer::CXPlayer()
:mColSphereBody(this, nullptr, CVector(), 0.5f)
, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f)
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
, mShield(this)
, mKnock_Back(0)
, mDefense_Success(false)
{
	//タグ設定
	mTag = EPLAYER;
	mColSphereSword.mTag = CCollider::ESWORD;
	mColSphereBody.mTag = mColSphereHead.mTag = CCollider::EBODY;
	spThis = this;
	mpModel = &CRes::sModelX;
	mPosition = CVector(0.0f, 0.0f, -3.0f);
	mRotation = CVector(0.0f, 180.0f, 0.0f);
}

void CXPlayer::Init(CModelX *model){
	CXCharacter::Init(model);
	//合成行列の設定
	mColSphereBody.mpMatrix = &mpCombinedMatrix[9];
	mColSphereHead.mpMatrix = &mpCombinedMatrix[12];
	mColSphereSword.mpMatrix = &mpCombinedMatrix[22];
	//起動時のマウスカーソルの座標を覚える
	CInput::GetMousePos(&mMouseX, &mMouseY);
}

//更新処理
void CXPlayer::Update(){
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
	if (mAnimationIndex == 3 && mAnimationFrame >= mAnimationFrameSize){
		ChangeAnimation(4, false, 30);
	}
	else if (mAnimationIndex == 4 && mAnimationFrame >= mAnimationFrameSize){
		ChangeAnimation(0, true, 60);
	}
	else if (mAnimationIndex != 3 && mAnimationIndex != 4){
		////これより下、移動処理////
		if (mAttack_Decision == false){
			//Aキー入力で左移動
			if (CKey::Push('A')) {
				if (mVelocityX < VELOCITYMAX){
					mVelocityX += ACCELERATION;
					ChangeAnimation(1, true, 60);
				}
			}
			else {
				if (mVelocityX>0.0f){
					mVelocityX -= ACCELERATION;
					if (mVelocityX<0.0f){
						mVelocityX = 0.0f;
						ChangeAnimation(0, true, 60);
					}
				}
			}
			//Dキー入力で右移動
			if (CKey::Push('D')) {
				if (mVelocityX > VELOCITYMIN){
					mVelocityX -= ACCELERATION;
					ChangeAnimation(1, true, 60);
				}
			}
			else {
				if (mVelocityX < 0.0f){
					mVelocityX += ACCELERATION;
					if (mVelocityX>0.0f){
						mVelocityX = 0.0f;
						ChangeAnimation(0, true, 60);
					}
				}
			}
			//Wキー入力で前移動
			if (CKey::Push('W')) {
				if (mVelocityZ < VELOCITYMAX){
					mVelocityZ += ACCELERATION;
					ChangeAnimation(1, true, 60);
				}
			}
			else {
				if (mVelocityZ>0.0f){
					mVelocityZ -= ACCELERATION;
					if (mVelocityZ<0.0f){
						mVelocityZ = 0.0f;
						ChangeAnimation(0, true, 60);
					}
				}
			}
			//Sキー入力で後ろ移動
			if (CKey::Push('S')) {
				if (mVelocityZ > VELOCITYMIN){
					mVelocityZ -= ACCELERATION;
					ChangeAnimation(1, true, 60);
				}
			}
			else {
				if (mVelocityZ<0.0f){
					mVelocityZ += ACCELERATION;
					if (mVelocityZ>0.0f){
						mVelocityZ = 0.0f;
						ChangeAnimation(0, true, 60);
					}
				}
			}
			mXMoveRange = mVelocityX;
			mZMoveRange = mVelocityZ;
			if (mKnock_Back > 0){
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
				if (mStamina > 0){
					mPosition = CVector(mXMoveRange, 0.0f, mZMoveRange) * RUNSPEED * mMatrix;
					mStamina--;
					ChangeAnimation(1, true, 60);
				}
				else{
					mPosition = CVector(mXMoveRange, 0.0f, mZMoveRange) * mMatrix;
					ChangeAnimation(1, true, 60);
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
			&&mAttack_Decision == false && mDefense_Decision == false){
			mAttackCount = ATTACKCOUNT;
			mStamina -= ATTACK_STAMINA;
			mAttack_Decision = true;
			ChangeAnimation(3, false, 30);
		}
		if (mAttackCount > 0){
			mAttackCount--;
		}
		else{
			mAttack_Decision = false;
		}
		////攻撃処理終了////

		////防御処理(モデルが未定なため、防御中は色を変えてます※デバッグのみ)////
		if (CKey::Push(VK_RBUTTON) && mAttack_Decision == false){
			mDefense_Decision = true;
		}
		else{
			mDefense_Decision = false;
		}
		////防御処理終了////

		////回避処理(回避中の無敵時間は分かりやすいように色を変えてます※デバッグのみ)////
		if (CKey::Once(' ') && mStamina > DODGE_STAMINA){
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

		if (mAttack_Decision == false && mDefense_Decision == false &&
			mDodge_Decision == false && mRun_Decision == false){
			if (mStamina < STAMINA){
				mStamina++;
			}
		}
	}
	CXCharacter::Update();
}

void CXPlayer::Collision(CCollider *m, CCollider *o){
	//相手がサーチの時は戻る
	if (o->mTag == CCollider::ESEARCH)
	{
		return;
	}
	switch (m->mType)
	{
	case CCollider::ESPHERE:
		if (m->mTag == CCollider::EBODY){
			if (mDodge_Decision == false){
				//相手のコライダが球コライダの時
				if (o->mType == CCollider::ESPHERE) {
					if (o->mTag == CCollider::EENEMY_ATTACK){
						if (CXEnemy::spThis->mAcquisitionFlg == true){
							if (mInvincible_Time <= 0){
								if (CCollider::Collision(m, o))
								{
									mInvincible_Time = INVINCIBLE_TIME;
									if (mStamina >= DEFENSE_STAMINA&&mDefense_Success==true){
										mStamina -= DEFENSE_STAMINA;
										mKnock_Back = KNOCK_BACK_MAX;
									}
									else{
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

void CXPlayer::TaskCollision(){
	mColSphereBody.ChangePriority();
	mColSphereHead.ChangePriority();
	mColSphereSword.ChangePriority();

	CCollisionManager::Get()->Collision(&mColSphereBody, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereHead, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereSword, COLLISIONRANGE);
}