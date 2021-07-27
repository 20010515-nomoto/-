#ifndef CENEMYHP_H
#define CENEMYHP_H

#include "CBillBoard.h"

class CEnemyHp :public CBillBoard{
public:
	CEnemyHp(const CVector &pos, float w, float h, char *texture);
	static CMaterial sHpMaterial;
	void Update();
	void Render();
};

#endif