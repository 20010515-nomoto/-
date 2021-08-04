#include "CShield.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CXPlayer.h"

#define SHIELDMODEL "resource\\plane.obj","resource\\plane.mtl"
#define SHIELDROTATION 5

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
	CTransform::Update();
	CTaskManager::Get()->Remove(this);
	CTaskManager::Get()->Add(this);
	mCollider.mTag = CCollider::ESHIELD;
}
void CShield::Update(){
	if (CXPlayer::spThis->mDefense_Decision == true){
		mRotation.mY -= SHIELDROTATION;
		if (mRotation.mY <= -90){
			mRotation.mY = -90;
		}
	}
	else{
		mRotation.mY += SHIELDROTATION;
		if (mRotation.mY >= 0){
			mRotation.mY = 0;
		}
	}
	mMatrix = CMatrix().RotateZ(-90.0f)*CMatrix().Translate(2.0f, 2.0f, 0.0f)*CMatrix().RotateY(mRotation.mY);
	mMatrix = mMatrix*mpParent->mMatrix;
}
void CShield::TaskCollision(){
	mCollider.ChangePriority();
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}

void CShield::Collision(CCollider *m, CCollider *o){
	if (m->mTag == CCollider::ESHIELD){
		if (o->mType == CCollider::EENEMY_ATTACK){
			if (CXPlayer::spThis->mDefense_Decision == true){
				if (mRotation.mY == -90.0f){
					if (CCollider::Collision(m, o)){
						CXPlayer::spThis->mDefense_Success = true;
					}
				}
			}
		}
	}
}