#include "CSword.h"
#include "CTaskManager.h"
#include "CPlayer.h"

#define OBJ "cube.obj"
#define MTL "cube.mtl"

CModel CSword::mSwordModel;	//���f���f�[�^�̍쐬

CSword::CSword(){
	if (mSwordModel.mTriangles.size() == 0){
		mSwordModel.Load(OBJ, MTL);
	}
	mpModel = &mSwordModel;
}

CSword::CSword(const CVector& position, const CVector& rotation, const CVector& scale)
:CSword()
{
	//�ʒu�A��]�A�g�k��ݒ肷��
	mPosition = position;	//�ʒu�̐ݒ�
	mRotation = rotation;	//��]�̐ݒ�
	mScale = scale;	//�g�k�̐ݒ�
	CTransform::Update();	//�s��̍X�V
	//�D��x��1�ɕύX����
	mPriority = 1;
	CTaskManager::Get()->Remove(this); //�폜����
	CTaskManager::Get()->Add(this); //�ǉ�����
}

void CSword::Update(){
	mPosition = CPlayer::spThis->mPosition;
	mRotation.mY = CPlayer::spThis->mRotation.mY;
	if (CPlayer::spThis->mAttack_Decision == true){
		mRotation.mX += 3;
	}
	else{
		mRotation.mX = 0;
	}
	CTransform::Update();
}