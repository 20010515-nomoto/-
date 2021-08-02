#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CCollider.h"
#include "CShield.h"
class CXPlayer :public CXCharacter{
public:
	//�X�V����
	void Update();
	void Init(CModelX *model);
	//�f�t�H���g�R���X�g���N�^
	CXPlayer();
	//�R���C�_�̐錾
	CCollider mColSphereBody;	//��
	CCollider mColSphereHead;	//��
	CCollider mColSphereSword;	//��

	CShield mShield;

	//�N�����̃}�E�X�J�[�\���̍��W���o����
	int mMouseX;
	int mMouseY;
	static CXPlayer *spThis;

	float mVelocityX;		//���x
	float mVelocityZ;		//���x
	float mXMoveRange;	//X���ړ���
	float mZMoveRange;	//Z���ړ���
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