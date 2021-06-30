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
	CColliderLine mLine; //�����R���C�_
	CColliderLine mLine2;
	CColliderLine mLine3;
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
	bool mAction_Decision;	//�A�N�V�������� true:�A�N�V������	false:�A�N�V�������Ă��Ȃ�

	int mAttackCount;	//�U�������f
	bool mDefense_Decision;	//�h�䒆���f true:�h�䒆 false:�h�䂵�Ă��Ȃ�

};

#endif

