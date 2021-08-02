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
#include "CSword.h"
#include "CShield.h"
#include "CMaterial.h"
#include "CRes.h"
#include "CXCharacter.h"
#include "CXPlayer.h"
#include "CXEnemy.h"
CVector mEye;

CMatrix Matrix;
//キャラクタのインスタンス
CXPlayer mPlayer;
//敵のインスタンス
CXEnemy mEnemy;

//モデルからコライダを生成
CColliderMesh mColliderMesh;
CSceneGame::~CSceneGame() {

}

void CSceneGame::Init() {
	//テキストフォントの読み込みと設定
	mFont.LoadTexture("FontG.tga", 1, 4096 / 64);

	mEye = CVector(1.0f, 2.0f, 3.0f);
	//モデルファイルの入力
	mBackGround.Load("sky.obj", "sky.mtl");

	CRes::sModelX.Load(MODEL_FILE);
	CRes::sKnight.Load("knight\\knight_low.x");
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");	//1:移動
	CRes::sKnight.SeparateAnimationSet(0, 1530, 1830, "idle1");	//2:待機
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");	//3:ダミー
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");	//4:ダミー
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");	//5:ダミー
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");	//6:ダミー
	CRes::sKnight.SeparateAnimationSet(0, 440, 520, "attack1");	//7:Attack1
	CRes::sKnight.SeparateAnimationSet(0, 520, 615, "attack2");	//8:Attack2
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");	//9:ダミー
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");	//10:ダミー
	CRes::sKnight.SeparateAnimationSet(0, 1160, 1260, "death1");	//11:ダウン

	//キャラクターにモデルを設定
	mPlayer.Init(&CRes::sModelX);
	//敵の初期設定
	mEnemy.Init(&CRes::sKnight);
	mEnemy.mAnimationFrameSize = 1024;
	mEnemy.ChangeAnimation(2, true, 200);
	//背景モデルから三角コライダを生成
	//親インスタンスと親行列は無し
	mColliderMesh.Set(NULL, NULL, &mBackGround);

	//敵の位置
	mEnemy.mPosition = CVector(0.0f, 0.0f, 5.0f);
}


void CSceneGame::Update() {
	//キャラクタークラスの更新
	mPlayer.Update();
	//敵の更新
	mEnemy.Update();
	//マウスカーソルの座標を取得
	int px, py;
	CInput::GetMousePos(&px, &py);
	if (py < CXPlayer::spThis->mMouseY) {
		//マウスの移動量の分だけ回転
		mEye.mY += (CXPlayer::spThis->mMouseY - py) / 4.0;
	}
	if (CXPlayer::spThis->mMouseY < py) {
		//マウスの移動量の分だけ回転
		mEye.mY += (CXPlayer::spThis->mMouseY - py) / 4.0;
	}
	//タスクマネージャの更新
	CTaskManager::Get()->Update();
	CTaskManager::Get()->TaskCollision();
	//カメラのパラメータを作成する
	CVector e, c, u;//視点、注視点、上方向
	//視点を求める
	e = CVector(0.0f, 4.0f, -8.0f)*mPlayer.mMatrix;
	//注視点を求める
	c = mPlayer.mPosition;
	//上方向を求める
	u = CVector(0.0f, 1.0f, 0.0f)*mPlayer.mMatrixRotate;

	//カメラクラスの設定
	Camera.Set(e, c, u);
	Camera.Render();
	mBackGround.Render();

	//行列設定
	glMultMatrixf(Matrix.mF);

	mPlayer.Render();
	mEnemy.Render();
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
	CInput::SetMousePos(CXPlayer::spThis->mMouseX, CXPlayer::spThis->mMouseY);
}

