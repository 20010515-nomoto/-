#ifndef CSCENEGAME_H
#define CSCENEGAME_H
#include "CScene.h"
#include "CModel.h"
#include "CPlayer.h"

/*
ゲームのシーン
*/
class CSceneGame : public CScene {
public:

	~CSceneGame();
	//初期化処理のオーバーライド
	void Init();
	//更新処理のオーバーライド
	void Update();

	CModel mBackGround;
	CModel mModel;
	CPlayer mPlayer;

};

#endif
