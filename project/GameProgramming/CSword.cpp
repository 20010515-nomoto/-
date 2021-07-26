#include "CSword.h"
#include "CTaskManager.h"
#include "CPlayer.h"
#include "CCollisionManager.h"

#define SWORD_MODEL "sword.obj","sword.mtl"

CModel CSword::mSwordModel;	//���f���f�[�^�̍쐬

CSword::CSword(CCharacter *parent)
:mCollider(this, &mMatrix, CVector(0.0f,15.0f,0.0f),0.5f)
{
	//�e�̃C���X�^���X�̑ޔ�
	mpParent = parent;
	if (mSwordModel.mTriangles.size() == 0){
		mSwordModel.Load(SWORD_MODEL);
	}
	mpModel = &mSwordModel;
	mScale = CVector(0.3f, 0.3f, 0.3f);
	mPosition = CVector(0.0f, 1.0f, 0.0f);
	CTransform::Update();	//�s��̍X�V
	CTaskManager::Get()->Remove(this); //�폜����
	CTaskManager::Get()->Add(this); //�ǉ�����
	mCollider.mTag = CCollider::EPLAYER_ATTACK;
}

void CSword::Update(){
	if (CPlayer::spThis->mAttack_Decision == true){
		mRotation.mX += 10;
	}
	else{
		mRotation.mX = 0;
	}
	CTransform::Update();
	mMatrix = mMatrix * mpParent->mMatrix;
}

void CSword::TaskCollision(){
	mCollider.ChangePriority();
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}