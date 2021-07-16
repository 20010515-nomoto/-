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
#include "CEnemy2.h"

CPlayer *CPlayer::spThis = 0;
CModel CPlayer::sModel;

#define PLAYER_MODEL "cube.obj","cube.mtl"

#define GRAVITY -0.5		//�d��
#define ACCELERATION 0.005	//�����l
#define VELOCITYMAX 0.2		//�ړ��X�s�[�h�ő�l
#define VELOCITYMIN -0.2	//�ړ��X�s�[�h�ŏ��l
#define MOVEADJUST 0.7		//�΂߈ړ����̈ړ��ʒ����l
#define RUNSPEED 1.5		//�_�b�V���X�s�[�h
#define DODGESPEED 2		//����X�s�[�h
#define STAMINA 100			//�X�^�~�i���
#define ATTACKCOUNT 60		//�U�����[�V������
#define ATTACK_STAMINA 40	//�U������X�^�~�i
#define DEFENSE_STAMINA 40	//�h�����X�^�~�i
#define DODGE_STAMINA 80	//�������X�^�~�i
#define INVINCIBLE_TIME 60	//���G����
#define PLAYERHP 100		//�v���C���[HP
#define KNOCK_BACK_MAX 7.0		//�m�b�N�o�b�N����
#define KNOCK_BACK_RANGE 0.5	//�m�b�N�o�b�N


CPlayer::CPlayer()
: mLine(this, &mMatrix, CVector(0.0f, 0.0f, -5.0f), CVector(0.0f, 0.0f, 5.0f))
, mLine2(this, &mMatrix, CVector(0.0f, 2.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f))
, mLine3(this, &mMatrix, CVector(5.0f, 0.0f, 0.0f), CVector(-5.0f, 0.0f, 0.0f))
, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.5f)
//, mAttackCollider(this,&mMatrix,CVector(0.0f,0.0f,0.0f),3.0f)
, mVelocityX(0.0)
, mVelocityZ(0.0)
, mXMoveRange(0.0)
, mZMoveRange(0.0)
, mMouseX(0)
, mMouseY(0)
, mStamina(0)
, mRun_Decision(false)
, mAttackCount(0)
, mDefense_Decision(false)
, mDodge_Decision(false)
, mAttack_Decision(false)
, mInvincible_Time(0)
, mPlayerHp(PLAYERHP)
, mSword(this)
, mKnock_Back(0)
{
	if (sModel.mTriangles.size() == 0){
		sModel.Load(PLAYER_MODEL);
	}
	mpModel = &sModel;
	mScale = CVector(1.0f, 1.0f, 1.0f);
	mPosition = CVector(0.0f, 1.0f, -3.0f);
	mRotation = CVector(0.0f, 180.0f, 0.0f);
	CTransform::Update();
	mTag = EPLAYER;	//�^�O�̐ݒ�
	spThis = this;
	//�e�N�X�`���t�@�C���̓ǂݍ��݁i1�s64��j
	mText.LoadTexture("FontWhite.tga", 1, 64);
	//�N�����̃}�E�X�J�[�\���̍��W���o����
	CInput::GetMousePos(&mMouseX, &mMouseY);
	mStamina = STAMINA;
	//mAttackCollider.mTag = CCollider::EPLAYER_ATTACK;
}

//�X�V����
void CPlayer::Update() {
	//�U���A�h��m�F�̂��ߐF�𔒂ɐݒ�
	mpModel->mpMaterials[0]->mDiffuse[0] = 1.0f;
	mpModel->mpMaterials[0]->mDiffuse[1] = 1.0f;
	mpModel->mpMaterials[0]->mDiffuse[2] = 1.0f;
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
	if (mAttack_Decision == false){
		//A�L�[���͂ō��ړ�
		if (CKey::Push('A')) {
			if (mVelocityX < VELOCITYMAX){
				mVelocityX += ACCELERATION;
			}
		}
		else {
			if (mVelocityX>0.0f){
				mVelocityX -= ACCELERATION;
				if (mVelocityX<0.0f){
					mVelocityX = 0.0f;
				}
			}
		}
		//D�L�[���͂ŉE�ړ�
		if (CKey::Push('D')) {
			if (mVelocityX > VELOCITYMIN){
				mVelocityX -= ACCELERATION;
			}
		}
		else {
			if (mVelocityX < 0.0f){
				mVelocityX += ACCELERATION;
				if (mVelocityX>0.0f){
					mVelocityX = 0.0f;
				}
			}
		}
		//W�L�[���͂őO�ړ�
		if (CKey::Push('W')) {
			if (mVelocityZ < VELOCITYMAX){
				mVelocityZ += ACCELERATION;
			}
		}
		else {
			if (mVelocityZ>0.0f){
				mVelocityZ -= ACCELERATION;
				if (mVelocityZ<0.0f){
					mVelocityZ = 0.0f;
				}
			}
		}
		//S�L�[���͂Ō��ړ�
		if (CKey::Push('S')) {
			if (mVelocityZ > VELOCITYMIN){
				mVelocityZ -= ACCELERATION;
			}
		}
		else {
			if (mVelocityZ<0.0f){
				mVelocityZ += ACCELERATION;
				if (mVelocityZ>0.0f){
					mVelocityZ = 0.0f;
				}
			}
		}
		mXMoveRange = mVelocityX;
		mZMoveRange = mVelocityZ;
		if (mKnock_Back>0){
			mZMoveRange -= KNOCK_BACK_RANGE;
			mKnock_Back -= KNOCK_BACK_RANGE;
		}
		else{
			mKnock_Back = 0;
		}
		if (CKey::Push('A') && CKey::Push('W') || CKey::Push('A') && CKey::Push('S')
			|| CKey::Push('D') && CKey::Push('W') || CKey::Push('D') && CKey::Push('S')){
			mPosition = CVector(mXMoveRange, 0.0f, mZMoveRange) * MOVEADJUST * mMatrix;
		}
		//shift�L�[�������Ȃ���ړ��Ń_�b�V��
		if (CKey::Push(VK_SHIFT)){
			mRun_Decision = true;
			if (mStamina>0){
				mPosition = CVector(mXMoveRange, 0.0f, mZMoveRange) * RUNSPEED * mMatrix;
				mStamina--;
			}
			else{
				mPosition = CVector(mXMoveRange, 0.0f, mZMoveRange) * mMatrix;
			}
		}
		//�X�y�[�X�L�[�������ꂽ�Ƃ��A���
		else if (mDodge_Decision == true){
			mPosition = CVector(mXMoveRange, 0.0f, mZMoveRange) *DODGESPEED* mMatrix;
		}
		else {
			mPosition = CVector(mXMoveRange, 0.0f, mZMoveRange) * mMatrix;
			mRun_Decision = false;
		}
	}
	////�ړ������I���////

	////�U������////
	if (CKey::Once(VK_LBUTTON) && mStamina >= ATTACK_STAMINA
		&&mAttack_Decision == false&&mDefense_Decision==false){
		mAttackCount = ATTACKCOUNT;
		mStamina -= ATTACK_STAMINA;
		mAttack_Decision = true;
	}
	if (mAttackCount > 0){
		mAttackCount--;
	}
	else{
		mAttack_Decision = false;
	}
	////�U�������I��////

	////�h�䏈��(���f��������Ȃ��߁A�h�䒆�͐F��ς��Ă܂����f�o�b�O�̂�)////
	if (CKey::Push(VK_RBUTTON)&&mAttack_Decision==false){
		mDefense_Decision = true;
	}
	else{
		mDefense_Decision = false;
	}
	if (mDefense_Decision == true){
		mpModel->mpMaterials[0]->mDiffuse[0] = 1.0f;
		mpModel->mpMaterials[0]->mDiffuse[1] = 0.3f;
		mpModel->mpMaterials[0]->mDiffuse[2] = 0.0f;
	}
	////�h�䏈���I��////

	////�������(��𒆂̖��G���Ԃ͕�����₷���悤�ɐF��ς��Ă܂����f�o�b�O�̂�)////
	if (CKey::Once(' ')&&mStamina>DODGE_STAMINA){
		mDodge_Decision = true;
		mStamina -= DODGE_STAMINA;
		mInvincible_Time = INVINCIBLE_TIME / 2;
	}
	if (mInvincible_Time > 0){
		mInvincible_Time--;
	}
	else {
		mDodge_Decision = false;
	}
	if (mDodge_Decision == true){
		mpModel->mpMaterials[0]->mDiffuse[0] = 0.0f;
		mpModel->mpMaterials[0]->mDiffuse[1] = 0.0f;
		mpModel->mpMaterials[0]->mDiffuse[2] = 1.0f;
	}

	if (mAttack_Decision == false && mDefense_Decision == false &&
		mDodge_Decision == false && mRun_Decision == false){
		if (mStamina < STAMINA){
			mStamina++;
		}
	}


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
		if (m->mTag == CCollider::EBODY){
			//����̃R���C�_�����R���C�_�̎�
			if (mDodge_Decision == false){
				if (o->mType == CCollider::ESPHERE) {
					if (o->mTag == CCollider::EENEMY_ATTACK){
						if (CEnemy2::spThis->mAcquisitionFlg==true){
							if (mInvincible_Time <= 0){
								if (CCollider::Collision(m, o))
								{
									mInvincible_Time = INVINCIBLE_TIME;
									if (mDefense_Decision == true && mStamina >= DEFENSE_STAMINA){
										mStamina -= DEFENSE_STAMINA;
										mKnock_Back = KNOCK_BACK_MAX;
									}
									else{
										//�G�t�F�N�g����
										new CEffect(o->mpParent->mPosition, 3.0f, 3.0f, "exp.tga", 4, 4, 2);
										mPlayerHp -= 20;
										mKnock_Back = KNOCK_BACK_MAX;
									}
								}
							}
						}
					}
				}
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
	//mAttackCollider.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Get()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine3, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
	//CCollisionManager::Get()->Collision(&mAttackCollider, COLLISIONRANGE);
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
	sprintf(buf, "STAMINA%d", mStamina);
	mText.DrawString(buf, 100, -100, 8, 16);

	sprintf(buf, "HP%d", mPlayerHp);
	mText.DrawString(buf, 100, -120, 8, 16);

	//2D�̕`��I��
	CUtil::End2D();

}