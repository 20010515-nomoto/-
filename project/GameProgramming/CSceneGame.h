#ifndef CSCENEGAME_H
#define CSCENEGAME_H
#include "CScene.h"
#include "CModel.h"
#include "CPlayer.h"

/*
�Q�[���̃V�[��
*/
class CSceneGame : public CScene {
public:

	~CSceneGame();
	//�����������̃I�[�o�[���C�h
	void Init();
	//�X�V�����̃I�[�o�[���C�h
	void Update();

	CModel mBackGround;
	CModel mModel;
	CPlayer mPlayer;

};

#endif
