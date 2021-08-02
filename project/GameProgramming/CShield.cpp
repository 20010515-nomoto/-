#include "CShield.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CXPlayer.h"

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
	if (CXPlayer::spThis->mDefense_Decision == true){
		mRotation.mY -= 5;
		if (mRotation.mY <= -45){
			mRotation.mY = -45;
		}
	}
	else{
		mRotation.mY += 5;
		if (mRotation.mY >= 0){
			mRotation.mY = 0;
		}
	}
	mMatrixRotate.RotateZ(90);
	mMatrix = mMatrix*mpParent->mMatrix;
}
void CShield::TaskCollision(){
	mCollider.ChangePriority();
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}
