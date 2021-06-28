//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
//�L�[���̓N���X�̃C���N���[�h
#include "CKey.h"
//
#include "CBullet.h"
//
#include "CTaskManager.h"
//
#include "CCollisionManager.h"
//
#include "CUtil.h"
//
#include "CEffect.h"
#include "CColliderMesh.h"
#include <Windows.h>
#include "CInput.h"

CPlayer *CPlayer::spThis = 0;

#define GRAVITY -0.5		//�d��
#define ACCELERATION 0.005	//�����l
#define VELOCITYMAX 0.2		//�ړ��X�s�[�h�ő�l
#define VELOCITYMIN -0.2	//�ړ��X�s�[�h�ŏ��l
#define MOVEADJUST 0.7		//�΂߈ړ����̈ړ��ʒ����l
#define RUNSPEED 1.5		//�_�b�V���X�s�[�h


CPlayer::CPlayer()
: mLine(this, &mMatrix, CVector(0.0f, 0.0f, -5.0f), CVector(0.0f, 0.0f, 5.0f))
, mLine2(this, &mMatrix, CVector(0.0f, 2.0f, 0.0f), CVector(0.0f, -2.0f, 0.0f))
, mLine3(this, &mMatrix, CVector(5.0f, 0.0f, 0.0f), CVector(-5.0f, 0.0f, 0.0f))
, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.5f)
, mAttackCollider(this,&mMatrix,CVector(-1.0f,3.0f,-2.0f),0.5f)
, mVelocityX(0.0)
, mVelocityZ(0.0)
, mXMoveRange(0.0)
, mZMoveRange(0.0)
, mMouseX(0)
, mMouseY(0)
{
	mTag = EPLAYER;	//�^�O�̐ݒ�
	spThis = this;
	//�e�N�X�`���t�@�C���̓ǂݍ��݁i1�s64��j
	mText.LoadTexture("FontWhite.tga", 1, 64);
	//�N�����̃}�E�X�J�[�\���̍��W���o����
	CInput::GetMousePos(&mMouseX, &mMouseY);
}

//�X�V����
void CPlayer::Update() {
	//�}�E�X�J�[�\���̍��W���擾
	int px, py;
	CInput::GetMousePos(&px, &py);
	if (px < mMouseX) {
		//�}�E�X�̈ړ��ʂ̕�������]
		mRotation.mY += (mMouseX - px) / 15.0;
	}
	if (mMouseX < px) {
		//�}�E�X�̈ړ��ʂ̕�������]
		mRotation.mY += (mMouseX - px) / 15.0;
	}
	////�����艺�A�ړ�����////
	//A�L�[���͂ō��ړ�
	if (CKey::Push('A')) {
		if (mVelocityX < VELOCITYMAX){
			mVelocityX += ACCELERATION;
		}
	}
	else {
		if (mVelocityX>0){
			mVelocityX -= ACCELERATION;
		}
	}
	//D�L�[���͂ŉE�ړ�
	if (CKey::Push('D')) {
		if (mVelocityX > VELOCITYMIN){
			mVelocityX -= ACCELERATION;
		}
	}
	else {
		if (mVelocityX<0){
			mVelocityX += ACCELERATION;
		}
	}
	//W�L�[���͂őO�ړ�
	if (CKey::Push('W')) {
		if (mVelocityZ < VELOCITYMAX){
			mVelocityZ += ACCELERATION;
		}
	}
	else {
		if (mVelocityZ>0){
			mVelocityZ -= ACCELERATION;
		}
	}
	//S�L�[���͂Ō��ړ�
	if (CKey::Push('S')) {
		if (mVelocityZ > VELOCITYMIN){
			mVelocityZ -= ACCELERATION;
		}
	}
	else {
		if (mVelocityZ<0){
			mVelocityZ += ACCELERATION;
		}
	}
	mXMoveRange = mVelocityX;
	mZMoveRange = mVelocityZ;
	//shift�L�[�������Ȃ���ړ��Ń_�b�V��
	if (CKey::Push(VK_SHIFT)){
		mPosition = CVector(mXMoveRange, 0.0f, mZMoveRange) * RUNSPEED * mMatrix;
	}
	else {
		mPosition = CVector(mXMoveRange, 0.0f, mZMoveRange) * mMatrix;
	}
	////�ړ������I���////



	//CTransform�̍X�V
	CTransform::Update();
}

void CPlayer::Collision(CCollider *m, CCollider *o) {
	//���肪�T�[�`�̎��͖߂�
	if (o->mTag == CCollider::ESEARCH)
	{
		return;
	}
	//���g�̃R���C�_�^�C�v�̔���
	switch (m->mType) {
	case CCollider::ELINE://�����R���C�_
		//����̃R���C�_���O�p�R���C�_�̎�
		if (o->mType == CCollider::ETRIANGLE) {
			CVector adjust;//�����p�x�N�g��
			//�O�p�`�Ɛ����̏Փ˔���
			CCollider::CollisionTriangleLine(o, m, &adjust);
			//�ʒu�̍X�V(mPosition + adjust)
			mPosition = mPosition - adjust * -1;
			//�s��̍X�V
			CTransform::Update();
		}
		break;
	case CCollider::ESPHERE:
		//����̃R���C�_�����R���C�_�̎�
		if (o->mType == CCollider::ESPHERE) {
			if (CCollider::Collision(m, o))
			{
				//�G�t�F�N�g����
				new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			}
		}
		break;
	}
}
//�Փˏ���
void CPlayer::TaskCollision()
{
	//�R���C�_�̗D��x�ύX
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	mCollider.ChangePriority();
	mAttackCollider.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Get()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine3, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mAttackCollider, COLLISIONRANGE);
}

void CPlayer::Render()
{
	//�e�̕`�揈��
	CCharacter::Render();

	//2D�̕`��J�n
	CUtil::Start2D(-400, 400, -300, 300);
	//�`��F�̐ݒ�i�ΐF�̔������j
	glColor4f(0.0f, 1.0f, 0.0f, 0.4f);
	//������ҏW�G���A�̍쐬
	char buf[64];

	//Y���W�̕\��
	//������̐ݒ�
	//sprintf(buf, "X_SPEED:%f",mXMoveRange );
	//������̕`��
	//mText.DrawString(buf, 100, 30, 8, 16);

	//X����]�l�̕\��
	//������̐ݒ�
	//sprintf(buf, "Z_SPEED:%f", mZMoveRange);
	//������̕`��
	//mText.DrawString(buf, 100, 0, 8, 16);
	//
	////Y����]�l�̕\��
	////������̐ݒ�
	//sprintf(buf, "RY:%7.2f", mRotation.mY);
	////������̕`��
	//mText.DrawString(buf, 100, -100, 8, 16);

	//2D�̕`��I��
	CUtil::End2D();

}