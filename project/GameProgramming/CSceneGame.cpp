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
//�L�����N�^�̃C���X�^���X
CXPlayer mPlayer;
//�G�̃C���X�^���X
CXEnemy mEnemy;

//���f������R���C�_�𐶐�
CColliderMesh mColliderMesh;
CSceneGame::~CSceneGame() {

}

void CSceneGame::Init() {
	//�e�L�X�g�t�H���g�̓ǂݍ��݂Ɛݒ�
	mFont.LoadTexture("FontG.tga", 1, 4096 / 64);

	mEye = CVector(1.0f, 2.0f, 3.0f);
	//���f���t�@�C���̓���
	mBackGround.Load("sky.obj", "sky.mtl");

	CRes::sModelX.Load(MODEL_FILE);
	CRes::sKnight.Load("knight\\knight_low.x");
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");	//1:�ړ�
	CRes::sKnight.SeparateAnimationSet(0, 1530, 1830, "idle1");	//2:�ҋ@
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");	//3:�_�~�[
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");	//4:�_�~�[
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");	//5:�_�~�[
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");	//6:�_�~�[
	CRes::sKnight.SeparateAnimationSet(0, 440, 520, "attack1");	//7:Attack1
	CRes::sKnight.SeparateAnimationSet(0, 520, 615, "attack2");	//8:Attack2
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");	//9:�_�~�[
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");	//10:�_�~�[
	CRes::sKnight.SeparateAnimationSet(0, 1160, 1260, "death1");	//11:�_�E��

	//�L�����N�^�[�Ƀ��f����ݒ�
	mPlayer.Init(&CRes::sModelX);
	//�G�̏����ݒ�
	mEnemy.Init(&CRes::sKnight);
	mEnemy.mAnimationFrameSize = 1024;
	mEnemy.ChangeAnimation(2, true, 200);
	//�w�i���f������O�p�R���C�_�𐶐�
	//�e�C���X�^���X�Ɛe�s��͖���
	mColliderMesh.Set(NULL, NULL, &mBackGround);

	//�G�̈ʒu
	mEnemy.mPosition = CVector(0.0f, 0.0f, 5.0f);
}


void CSceneGame::Update() {
	//�L�����N�^�[�N���X�̍X�V
	mPlayer.Update();
	//�G�̍X�V
	mEnemy.Update();
	//�}�E�X�J�[�\���̍��W���擾
	int px, py;
	CInput::GetMousePos(&px, &py);
	if (py < CXPlayer::spThis->mMouseY) {
		//�}�E�X�̈ړ��ʂ̕�������]
		mEye.mY += (CXPlayer::spThis->mMouseY - py) / 4.0;
	}
	if (CXPlayer::spThis->mMouseY < py) {
		//�}�E�X�̈ړ��ʂ̕�������]
		mEye.mY += (CXPlayer::spThis->mMouseY - py) / 4.0;
	}
	//�^�X�N�}�l�[�W���̍X�V
	CTaskManager::Get()->Update();
	CTaskManager::Get()->TaskCollision();
	//�J�����̃p�����[�^���쐬����
	CVector e, c, u;//���_�A�����_�A�����
	//���_�����߂�
	e = CVector(0.0f, 4.0f, -8.0f)*mPlayer.mMatrix;
	//�����_�����߂�
	c = mPlayer.mPosition;
	//����������߂�
	u = CVector(0.0f, 1.0f, 0.0f)*mPlayer.mMatrixRotate;

	//�J�����N���X�̐ݒ�
	Camera.Set(e, c, u);
	Camera.Render();
	mBackGround.Render();

	//�s��ݒ�
	glMultMatrixf(Matrix.mF);

	mPlayer.Render();
	mEnemy.Render();
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
	CInput::SetMousePos(CXPlayer::spThis->mMouseX, CXPlayer::spThis->mMouseY);
}

