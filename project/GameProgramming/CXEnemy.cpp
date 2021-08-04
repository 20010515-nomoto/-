#include "CXEnemy.h"
#include "CCollisionManager.h"
#include "CXPlayer.h"
#include "CRes.h"

#define HP 10
#define VELOCITY 0.11f	//���x

CXEnemy *CXEnemy::spThis = 0;

CXEnemy::CXEnemy()
:mEnemyColSphereBody(this, nullptr, CVector(0.5f, -1.0f, 0.0f), 1.0f)
, mEnemyColSphereHead(this, nullptr, CVector(0.0f, 1.0f, 0.0f), 1.5f)
, mEnemyColSphereSword0(this, nullptr, CVector(0.7f, 3.5f, -0.2f), 0.5f)
, mEnemyColSphereSword1(this, nullptr, CVector(0.5f, 2.5f, -0.2f), 0.5f)
, mEnemyColSphereSword2(this, nullptr, CVector(0.3f, 1.5f, -0.2f), 0.5f)
, mColSearch(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 30.0f)
, mColAttack_Range(this,&mMatrix,CVector(0.0f,0.0f,0.0f),5.0f)
, mpPlayer(nullptr)
, mHp(HP)
, mAttackFlg(false)
, mAcquisitionFlg(false)
{
	mPoint = CXPlayer::spThis->mPosition;
	spThis = this;
	mpModel = &CRes::sKnight;
}

void CXEnemy::Init(CModelX *model){
	CXCharacter::Init(model);
	mColSearch.mTag = CCollider::ESEARCH;
	mEnemyColSphereSword0.mTag = mEnemyColSphereSword1.mTag =
		mEnemyColSphereSword2.mTag = CCollider::EENEMY_ATTACK;
	mColAttack_Range.mTag = CCollider::EENEMY_ATTACK_RANGE;
	//�����s��̐ݒ�
	mEnemyColSphereBody.mpMatrix = &mpCombinedMatrix[1];
	mEnemyColSphereHead.mpMatrix = &mpCombinedMatrix[1];
	mEnemyColSphereSword0.mpMatrix = &mpCombinedMatrix[26];
	mEnemyColSphereSword1.mpMatrix = &mpCombinedMatrix[26];
	mEnemyColSphereSword2.mpMatrix = &mpCombinedMatrix[26];
}

void CXEnemy::Update(){
	if (mHp <= 0){
		mHp--;
		ChangeAnimation(11, false, 30);
	}

	if (mAnimationIndex == 7 && mAnimationFrame >= mAnimationFrameSize){
		ChangeAnimation(8, false, 60);
	}
	else if (mAnimationIndex == 8 && mAnimationFrame >= mAnimationFrameSize){
		ChangeAnimation(2, true, 200);
		mAttackFlg = false;
	}
	else if (mAnimationIndex != 7 && mAnimationIndex != 8&&mAnimationIndex!=11){
		//�������iX���j�̃x�N�g�������߂�
		CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
		//������iY���j�̃x�N�g�������߂�
		CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
		//�O�����iZ���j�̃x�N�g�������߂�
		CVector vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;

		//�ڕW�n�_�܂ł̃x�N�g�������߂�
		CVector vp = mPoint - mPosition;
		float dx = vp.Dot(vx);	//���x�N�g���Ƃ̓��ς����߂�
		float dy = vp.Dot(vy);	//��x�N�g���Ƃ̓��ς����߂�
		float dz = vp.Dot(vz);
		float margin = 1.0f;
		//���E�����։�]
		if (dx > margin)
		{
			mRotation.mY += 1.0f;
		}
		else if (dx < -margin)
		{
			mRotation.mY -= 1.0f;
		}
		//�㉺�����։�]
		if (dy > margin)
		{
			mRotation.mX -= 1.0f;
		}
		else if (dy < -margin)
		{
			mRotation.mX += 1.0f;
		}
		if (mAttackFlg == false){
			if (dz>0.0f&&-3.0f < dx&&dx < 3.0f){
				mPosition = mPosition + vp.Normalize() * VELOCITY;
				mAcquisitionFlg = true;
				ChangeAnimation(1, true, 60);
			}
			else{
				mAcquisitionFlg = false;
				ChangeAnimation(2, true, 200);
			}
		}
		if (mAttackFlg == true){
			ChangeAnimation(7, false, 100);
		}
	}
	CXCharacter::Update();

	//���悻3�b���ɖڕW�n�_���X�V
	int r = rand() % 30;	//rand()�͐����̗�����Ԃ�
	//% 180 ��180�Ŋ������]������߂�
	if (r == 0)
	{
		if (mpPlayer)
		{
			mPoint = mpPlayer->mPosition;
		}
		else
		{
			mPoint = mPoint * CMatrix().RotateY(45);
		}
	}

	mpPlayer = 0;
}

void CXEnemy::Collision(CCollider *m, CCollider *o){

	//���肪�T�[�`�̎��͖߂�
	if (o->mTag == CCollider::ESEARCH)
	{
		return;
	}
	//�������T�[�`�p�̎�
	if (m->mTag == CCollider::ESEARCH)
	{
		//���肪���R���C�_�̎�
		if (o->mType == CCollider::ESPHERE)
		{
			//���肪�v���C���[�̎�
			if (o->mpParent->mTag == EPLAYER)
			{
				//�Փ˂��Ă��鎞
				if (CCollider::Collision(m, o))
				{
					//�v���C���[�̃|�C���^�ݒ�
					mpPlayer = o->mpParent;
				}
			}
		}
		return;
	}
	//�G�̍U�������Ƀv���C���[�����邩�ǂ����̔���
	if (m->mTag == CCollider::EENEMY_ATTACK_RANGE){
		if (o->mType == CCollider::ESPHERE){
			if (o->mpParent->mTag == EPLAYER){
				if (mHp > 0){
					if (CCollider::Collision(m, o)){
						mAttackFlg = true;
					}
					else{
						mAttackFlg = false;
					}
				}
			}
		}
		return;
	}
	switch (o->mType)
	{
	case CCollider::ESPHERE:
		if (o->mTag == CCollider::ESWORD){
			if (CXPlayer::spThis->mAttackCount > 0){
				if (CCollider::Collision(m, o)){
					mHp--;
				}
			}
		}
	}
}

void CXEnemy::TaskCollision(){
	mColSearch.ChangePriority();
	mEnemyColSphereBody.ChangePriority();
	mEnemyColSphereHead.ChangePriority();
	mEnemyColSphereSword0.ChangePriority();
	mEnemyColSphereSword1.ChangePriority();
	mEnemyColSphereSword2.ChangePriority();
	mColAttack_Range.ChangePriority();

	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mEnemyColSphereBody, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mEnemyColSphereHead, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mEnemyColSphereSword0, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mEnemyColSphereSword1, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mEnemyColSphereSword2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColAttack_Range, COLLISIONRANGE);
}