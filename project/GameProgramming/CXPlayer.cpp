#include "CXPlayer.h"
#include "CKey.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "CXEnemy.h"
#include "CRes.h"

CXPlayer *CXPlayer::spThis = 0;

#define ACCELERATION 0.003	//�����l
#define VELOCITYMAX 0.15		//�ړ��X�s�[�h�ő�l
#define VELOCITYMIN -0.15	//�ړ��X�s�[�h�ŏ��l
#define MOVEADJUST 0.5		//�΂߈ړ����̈ړ��ʒ����l
#define RUNSPEED 1.1		//�_�b�V���X�s�[�h
#define DODGESPEED 2		//����X�s�[�h
#define STAMINA 100			//�X�^�~�i���
#define ATTACKCOUNT 20		//�U�����[�V������
#define ATTACK_STAMINA 40	//�U������X�^�~�i
#define DEFENSE_STAMINA 40	//�h�����X�^�~�i
#define DODGE_STAMINA 80	//�������X�^�~�i
#define INVINCIBLE_TIME 60	//���G����
#define PLAYERHP 100		//�v���C���[HP
#define KNOCK_BACK_MAX 7.0		//�m�b�N�o�b�N����
#define KNOCK_BACK_RANGE 0.5	//�m�b�N�o�b�N

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
	//�^�O�ݒ�
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
	//�����s��̐ݒ�
	mColSphereBody.mpMatrix = &mpCombinedMatrix[9];
	mColSphereHead.mpMatrix = &mpCombinedMatrix[12];
	mColSphereSword.mpMatrix = &mpCombinedMatrix[22];
	//�N�����̃}�E�X�J�[�\���̍��W���o����
	CInput::GetMousePos(&mMouseX, &mMouseY);
}

//�X�V����
void CXPlayer::Update(){
	//�}�E�X�J�[�\���̍��W���擾
	int px, py;
	CInput::GetMousePos(&px, &py);
	if (px < mMouseX) {
		//�}�E�X�̈ړ��ʂ̕�������]
		mRotation.mY += (mMouseX - px) / 15.0;
	}
	if (mMouseX < px) {
		//�}�E�X�̈ړ��ʂ̕�������]
		mRotation.mY += (mMouseX - px) / 15.0;
	}
	if (mAnimationIndex == 3 && mAnimationFrame >= mAnimationFrameSize){
		ChangeAnimation(4, false, 30);
	}
	else if (mAnimationIndex == 4 && mAnimationFrame >= mAnimationFrameSize){
		ChangeAnimation(0, true, 60);
	}
	else if (mAnimationIndex != 3 && mAnimationIndex != 4){
		////�����艺�A�ړ�����////
		if (mAttack_Decision == false){
			//A�L�[���͂ō��ړ�
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
			//D�L�[���͂ŉE�ړ�
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
			//W�L�[���͂őO�ړ�
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
			//S�L�[���͂Ō��ړ�
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
			//shift�L�[�������Ȃ���ړ��Ń_�b�V��
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
			//�X�y�[�X�L�[�������ꂽ�Ƃ��A���
			else if (mDodge_Decision == true){
				mPosition = CVector(mXMoveRange, 0.0f, mZMoveRange) *DODGESPEED* mMatrix;
			}
			else {
				mPosition = CVector(mXMoveRange, 0.0f, mZMoveRange) * mMatrix;
				mRun_Decision = false;
			}
		}
		////�ړ������I���////

		////�U������////
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
		////�U�������I��////

		////�h�䏈��(���f��������Ȃ��߁A�h�䒆�͐F��ς��Ă܂����f�o�b�O�̂�)////
		if (CKey::Push(VK_RBUTTON) && mAttack_Decision == false){
			mDefense_Decision = true;
		}
		else{
			mDefense_Decision = false;
		}
		////�h�䏈���I��////

		////�������(��𒆂̖��G���Ԃ͕�����₷���悤�ɐF��ς��Ă܂����f�o�b�O�̂�)////
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
	//���肪�T�[�`�̎��͖߂�
	if (o->mTag == CCollider::ESEARCH)
	{
		return;
	}
	switch (m->mType)
	{
	case CCollider::ESPHERE:
		if (m->mTag == CCollider::EBODY){
			if (mDodge_Decision == false){
				//����̃R���C�_�����R���C�_�̎�
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