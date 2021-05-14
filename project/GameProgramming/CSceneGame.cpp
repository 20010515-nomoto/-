#include "CSceneGame.h"
//
#include "CCamera.h"
//
#include "CUtil.h"
#include "CColliderMesh.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
CVector mEye;

//モデルからコライダを生成
CColliderMesh mColliderMesh;
CSceneGame::~CSceneGame() {

}

void CSceneGame::Init() {
	//テキストフォントの読み込みと設定
	mFont.LoadTexture("FontG.tga", 1, 4096 / 64);

	mEye = CVector(1.0f, 2.0f, 3.0f);
	//モデルファイルの入力
	mModel.Load("cube.obj", "cube.mtl");
	mBackGround.Load("sky.obj", "sky.mtl");
	mPlayer.mpModel = &mModel;
	mPlayer.mScale = CVector(1.0f, 1.0f, 1.0f);
	mPlayer.mPosition = CVector(0.0f, 0.0f, -3.0f);
	mPlayer.mRotation = CVector(0.0f, 180.0f, 0.0f);
	//背景モデルから三角コライダを生成
	//親インスタンスと親行列は無し
	mColliderMesh.Set(NULL, NULL, &mBackGround);
	mColliderMesh.Set(NULL, NULL, &mModel);
}


void CSceneGame::Update() {
	//タスクマネージャの更新
	CTaskManager::Get()->Update();
	CTaskManager::Get()->TaskCollision();
	//カメラのパラメータを作成する
	CVector e, c, u;//視点、注視点、上方向
	//視点を求める
	e = CVector(0.0f, 7.0f, -15.0f)*mPlayer.mMatrix;
	//注視点を求める
	c = mPlayer.mPosition;
	//上方向を求める
	u = CVector(0.0f, 1.0f, 0.0f)*mPlayer.mMatrixRotate;

	//カメラクラスの設定
	Camera.Set(e, c, u);
	Camera.Render();
	mBackGround.Render();
	//2D描画開始
	CUtil::Start2D(0, 800, 0, 600);

	mFont.DrawString("3D PROGRAMMING", 20, 20, 10, 12);

	//2Dの描画終了
	CUtil::End2D();

	//タスクリストから削除
	CTaskManager::Get()->Delete();
	CTaskManager::Get()->Render();

	CCollisionManager::Get()->Render();
}

