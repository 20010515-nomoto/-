#include "CSceneGame.h"
//
#include "CCamera.h"
//
#include "CUtil.h"
#include "CColliderMesh.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "CEnemy2.h"
CVector mEye;
CModel mModelC5;

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

	//敵C5モデルの読み込み
	mModelC5.Load("c5.obj", "c5.mtl");
	new CEnemy2(CVector(-5.0f, 1.0f, -30.0f),
		CVector(), CVector(0.1f, 0.1f, 0.1f));

	//背景モデルから三角コライダを生成
	//親インスタンスと親行列は無し
	mColliderMesh.Set(NULL, NULL, &mBackGround);
}


void CSceneGame::Update() {
	//マウスカーソルの座標を取得
	int px, py;
	CInput::GetMousePos(&px, &py);
	if (py < CPlayer::spThis->mMouseY) {
		//マウスの移動量の分だけ回転
		mEye.mY += (CPlayer::spThis->mMouseY - py) / 4.0;
	}
	if (CPlayer::spThis->mMouseY < py) {
		//マウスの移動量の分だけ回転
		mEye.mY += (CPlayer::spThis->mMouseY - py) / 4.0;
	}
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
	//マウスカーソルを起動時の座標に移動
	CInput::SetMousePos(CPlayer::spThis->mMouseX, CPlayer::spThis->mMouseY);
}

