#ifndef CPLAYER_H
#define CPLAYER_H
//�L�����N�^�N���X�̃C���N���[�h
#include "CCharacter.h"
//
#include "CBullet.h"
//
#include "CColliderLine.h"
//
#include "CText.h"
#include <Windows.h>
#include "CSword.h"
class CEnemy2;
/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/
class CPlayer : public CCharacter {
public:
	CCollider mCollider;
	//CCollider mAttackCollider;
	CText mText;
	static CPlayer *spThis;
	static CModel sModel;
	CColliderLine mLine; //�����R���C�_
	CColliderLine mLine2;
	CColliderLine mLine3;
	//CCollider mAttackCollider;	//�U������

	//�q�̃C���X�^���X
	CSword mSword;

	//�f�t�H���g�R���X�g���N�^
	CPlayer();
//	CBullet bullet;
	//�X�V����
	void Update();
	//�Փˏ���
	void Collision(CCollider *m, CCollider *o);
	//�Փˏ���
	void TaskCollision();
	//�`�揈��
	void Render();

	float mVelocityX;		//���x
	float mVelocityZ;		//���x
	float mXMoveRange;	//X���ړ���
	float mZMoveRange;	//Z���ړ���
	//�N�����̃}�E�X�J�[�\���̍��W���o����
	int mMouseX;
	int mMouseY;

	int mStamina;		//�X�^�~�i�F�e�s�����ɏ���
	bool mRun_Decision;	//�_�b�V������	true:�����Ă���

	int mAttackCount;	//�U���̔��莞��
	bool mAttack_Decision;	//�U�������� true:�U����
	bool mDefense_Decision;	//�h�䒆���f true:�h�䒆

	bool mDodge_Decision;	//��𒆔��f true:���
	int mInvincible_Time;	//���G����

	int mPlayerHp;	//HP
	float mKnock_Back;		//�m�b�N�o�b�N����
};

#endif

