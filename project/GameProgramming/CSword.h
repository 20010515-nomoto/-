#ifndef CSWORD_H
#define CSWORD_H

#include "CCharacter.h"

class CSword :public CCharacter{
public:
	CSword();
	CSword(const CVector& position, const CVector& rotation, const CVector& scale);
	void Update();

	static CModel mSwordModel;
};

#endif