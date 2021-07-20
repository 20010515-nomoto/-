#include "CShield.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CPlayer.h"

#define SHIELDMODEL "cube.obj","cube.mtl"

CModel CShield::mShieldModel;

CShield::CShield(CCharacter *parent)
:mCollider(this, &mMatrix, CVector(0.0f,0.0f,0.0f),0.5f)
{
	mpParent = parent;
	if (mShieldModel.mTriangles.size() == 0){
		mShieldModel.Load(SHIELDMODEL);
	}
	mpModel = &mShieldModel;
	mScale = CVector(0.5f, 0.5f, 0.5f);
	mPosition = CVector(3.0f, 0.0f, 0.0f);
	CTransform::Update();
	CTaskManager::Get()->Remove(this);
	CTaskManager::Get()->Add(this);
	mCollider.mTag = CCollider::ESEARCH;
}
void CShield::Update(){
	if (CPlayer::spThis->mDefense_Decision == true){
		if (mRotation.mY <= 20.0f){
			mRotation.mY += 2.0f;
		}
	}
	else{
		mRotation.mY = 0;
	}
	CTransform::Update();
	mMatrix = mMatrix * mpParent->mMatrix;
}
void CShield::TaskCollision(){
	mCollider.ChangePriority();
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}