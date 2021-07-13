#include "CSword.h"
#include "CTaskManager.h"
#include "CPlayer.h"

#define OBJ "cube.obj"
#define MTL "cube.mtl"

CModel CSword::mSwordModel;	//モデルデータの作成

CSword::CSword(){
	if (mSwordModel.mTriangles.size() == 0){
		mSwordModel.Load(OBJ, MTL);
	}
	mpModel = &mSwordModel;
}

CSword::CSword(const CVector& position, const CVector& rotation, const CVector& scale)
:CSword()
{
	//位置、回転、拡縮を設定する
	mPosition = position;	//位置の設定
	mRotation = rotation;	//回転の設定
	mScale = scale;	//拡縮の設定
	CTransform::Update();	//行列の更新
	//優先度を1に変更する
	mPriority = 1;
	CTaskManager::Get()->Remove(this); //削除して
	CTaskManager::Get()->Add(this); //追加する
}

void CSword::Update(){
	mPosition = CPlayer::spThis->mPosition;
	mRotation.mY = CPlayer::spThis->mRotation.mY;
	if (CPlayer::spThis->mAttack_Decision == true){
		mRotation.mX += 3;
	}
	else{
		mRotation.mX = 0;
	}
	CTransform::Update();
}