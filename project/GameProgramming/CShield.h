#ifndef CSHIELD_H
#define CSHIELD_H

#include "CCharacter.h"
#include "CCollider.h"
class CPlayer;

class CShield :public CCharacter{
private:
	//親へのポインタ
	CCharacter *mpParent;
	static CModel mShieldModel;
	CCollider mCollider;
	void TaskCollision();
public:
	CShield(CCharacter *parent);
	void Update();

	void Collision(CCollider *m, CCollider *o);
};

#endif