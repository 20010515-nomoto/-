#include "CXEnemy.h"
#include "CCollisionManager.h"

CXEnemy::CXEnemy()
:mEnemyColSphereBody(this, nullptr, CVector(0.5f,-1.0f,0.0f), 1.0f)
, mEnemyColSphereHead(this, nullptr, CVector(0.0f, 1.0f, 0.0f), 1.5f)
, mEnemyColSphereSword0(this, nullptr, CVector(0.7f, 3.5f, -0.2f), 0.5f)
, mEnemyColSphereSword1(this, nullptr, CVector(0.5f, 2.5f, -0.2f), 0.5f)
, mEnemyColSphereSword2(this, nullptr, CVector(0.3f, 1.5f, -0.2f), 0.5f)
{

}

void CXEnemy::Init(CModelX *model){
	CXCharacter::Init(model);
	//‡¬s—ñ‚ÌÝ’è
	mEnemyColSphereBody.mpMatrix = &mpCombinedMatrix[1];
	mEnemyColSphereHead.mpMatrix = &mpCombinedMatrix[1];
	mEnemyColSphereSword0.mpMatrix = &mpCombinedMatrix[26];
	mEnemyColSphereSword1.mpMatrix = &mpCombinedMatrix[26];
	mEnemyColSphereSword2.mpMatrix = &mpCombinedMatrix[26];
}

void CXEnemy::Update(){
	CXCharacter::Update();
}

void CXEnemy::Collision(CCollider *m, CCollider *o){
	if (m->mType == CCollider::ESPHERE){
		if (o->mType == CCollider::ESPHERE){
			if (o->mpParent->mTag == EPLAYER){
				if (o->mTag == CCollider::ESWORD){
					if (CCollider::Collision(m, o)){
						ChangeAnimation(11, false, 30);
					}
				}
			}
		}
	}
}
