#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"

class CXEnemy :public CXCharacter{
public:
	CXEnemy();
	void Init(CModelX *model);
	void Update();
	CCollider mEnemyColSphereBody;	//‘Ì
	CCollider mEnemyColSphereHead;	//“ª
	CCollider mEnemyColSphereSword0;	//Œ•
	CCollider mEnemyColSphereSword1;	//Œ•
	CCollider mEnemyColSphereSword2;	//Œ•

	//Õ“Ëˆ—
	void Collision(CCollider *m, CCollider *o);

};

#endif