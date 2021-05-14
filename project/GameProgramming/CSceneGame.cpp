#include "CSceneGame.h"
//
#include "CCamera.h"
//
#include "CUtil.h"
#include "CColliderMesh.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
CVector mEye;

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
	//�w�i���f������O�p�R���C�_�𐶐�
	//�e�C���X�^���X�Ɛe�s��͖���
	mColliderMesh.Set(NULL, NULL, &mBackGround);
	mColliderMesh.Set(NULL, NULL, &mModel);
}


void CSceneGame::Update() {
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
}

