#include "CRandom.h"

CRandom::CRandom()
:mRandom(0)
, mRan1(0)
{

}

void CRandom::Update(){
	unsigned int now = (unsigned int)time(0);
	srand(now);
	mRandom = rand();

	rand(); rand(); rand(); rand(); rand(); rand();

	mRan1 = rand() % 4;
}