#ifndef CSWORD_H
#define CSWORD_H

#include "CCharacter.h"
#include "CCollider.h"
class CPlayer;

class CSword :public CCharacter{
private:
	//親へのポインタ
	CCharacter *mpParent;
	static CModel mSwordModel;
	CCollider mCollider;
	void TaskCollision();

public:
	CSword(CCharacter *parent);
	void Update();

};

#endif