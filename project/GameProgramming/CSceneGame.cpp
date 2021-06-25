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

//���f������R���C�_�𐶐�
CColliderMesh mColliderMesh;
CSceneGame::~CSceneGame() {

}

void CSceneGame::Init() {
	//�e�L�X�g�t�H���g�̓ǂݍ��݂Ɛݒ�
	mFont.LoadTexture("FontG.tga", 1, 4096 / 64);

	mEye = CVector(1.0f, 2.0f, 3.0f);
	//���f���t�@�C���̓���
	mModel.Load("cube.obj", "cube.mtl");
	mBackGround.Load("sky.obj", "sky.mtl");
	mPlayer.mpModel = &mModel;
	mPlayer.mScale = CVector(1.0f, 1.0f, 1.0f);
	mPlayer.mPosition = CVector(0.0f, 0.0f, -3.0f);
	mPlayer.mRotation = CVector(0.0f, 180.0f, 0.0f);

	//�GC5���f���̓ǂݍ���
	mModelC5.Load("c5.obj", "c5.mtl");
	new CEnemy2(CVector(-5.0f, 1.0f, -30.0f),
		CVector(), CVector(0.1f, 0.1f, 0.1f));

	//�w�i���f������O�p�R���C�_�𐶐�
	//�e�C���X�^���X�Ɛe�s��͖���
	mColliderMesh.Set(NULL, NULL, &mBackGround);
}


void CSceneGame::Update() {
	//�}�E�X�J�[�\���̍��W���擾
	int px, py;
	CInput::GetMousePos(&px, &py);
	if (py < CPlayer::spThis->mMouseY) {
		//�}�E�X�̈ړ��ʂ̕�������]
		mEye.mY += (CPlayer::spThis->mMouseY - py) / 4.0;
	}
	if (CPlayer::spThis->mMouseY < py) {
		//�}�E�X�̈ړ��ʂ̕�������]
		mEye.mY += (CPlayer::spThis->mMouseY - py) / 4.0;
	}
	//�^�X�N�}�l�[�W���̍X�V
	CTaskManager::Get()->Update();
	CTaskManager::Get()->TaskCollision();
	//�J�����̃p�����[�^���쐬����
	CVector e, c, u;//���_�A�����_�A�����
	//���_�����߂�
	e = CVector(0.0f, 7.0f, -15.0f)*mPlayer.mMatrix;
	//�����_�����߂�
	c = mPlayer.mPosition;
	//����������߂�
	u = CVector(0.0f, 1.0f, 0.0f)*mPlayer.mMatrixRotate;

	//�J�����N���X�̐ݒ�
	Camera.Set(e, c, u);
	Camera.Render();
	mBackGround.Render();
	//2D�`��J�n
	CUtil::Start2D(0, 800, 0, 600);

	mFont.DrawString("3D PROGRAMMING", 20, 20, 10, 12);

	//2D�̕`��I��
	CUtil::End2D();

	//�^�X�N���X�g����폜
	CTaskManager::Get()->Delete();
	CTaskManager::Get()->Render();

	CCollisionManager::Get()->Render();
	//�}�E�X�J�[�\�����N�����̍��W�Ɉړ�
	CInput::SetMousePos(CPlayer::spThis->mMouseX, CPlayer::spThis->mMouseY);
}

