#include "CSword.h"
#include "CTaskManager.h"
#include "CPlayer.h"

#define OBJ "cube.obj"
#define MTL "cube.mtl"

CModel CSword::mSwordModel;	//ƒ‚ƒfƒ‹ƒf[ƒ^‚Ìì¬

CSword::CSword(){
	if (mSwordModel.mTriangles.size() == 0){
		mSwordModel.Load(OBJ, MTL);
	}
	mpModel = &mSwordModel;
}

CSword::CSword(const CVector& position, const CVector& rotation, const CVector& scale)
:CSword()
{
	//ˆÊ’uA‰ñ“]AŠgk‚ğİ’è‚·‚é
	mPosition = position;	//ˆÊ’u‚Ìİ’è
	mRotation = rotation;	//‰ñ“]‚Ìİ’è
	mScale = scale;	//Šgk‚Ìİ’è
	CTransform::Update();	//s—ñ‚ÌXV
	//—Dæ“x‚ğ1‚É•ÏX‚·‚é
	mPriority = 1;
	CTaskManager::Get()->Remove(this); //íœ‚µ‚Ä
	CTaskManager::Get()->Add(this); //’Ç‰Á‚·‚é
}

void CSword::Update(){
	mPosition = mPosition + CVector()*CPlayer::spThis->mMatrix;
}