#include "CShield.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CPlayer.h"

#define SHIELDMODEL "plane.obj","plane.mtl"

CModel CShield::mShieldModel;

CShield::CShield(CCharacter *parent)
:mCollider(this, &mMatrix, CVector(0.0f,0.0f,0.0f),0.5f)
{
	mpParent = parent;
	if (mShieldModel.mTriangles.size() == 0){
		mShieldModel.Load(SHIELDMODEL);
	}
	mpModel = &mShieldModel;
	mScale = CVector(1.5f, 1.5f, 1.5f);
	mPosition = CVector(2.0f, 0.0f, 0.0f);
	mRotation = CVector(0.0f, 0.0f, 90.0f);
	CTransform::Update();
	CTaskManager::Get()->Remove(this);
	CTaskManager::Get()->Add(this);
	mCollider.mTag = CCollider::ESEARCH;
}
void CShield::Update(){
	CVector vp = mPosition - mpParent->mPosition;
	if (CPlayer::spThis->mDefense_Decision == true){
		mPosition = vp*mMatrixRotate.RotateY(-5)*mpParent->mMatrix;
	}
	else{
		mMatrixRotate.RotateY(0);
	}
	CTransform::Update();
}
void CShield::TaskCollision(){
	mCollider.ChangePriority();
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}